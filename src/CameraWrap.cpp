#include <memory.h>
#include "CameraWrap.hpp"
#include "PoseWrap.hpp"
#include "Struct/Callback.hpp"
#include "b64.h"

Napi::FunctionReference* CameraWrap::constructor = nullptr;

Napi::Value CameraWrap::NewInstance(Napi::Env env, Camera* camera)
{
    Napi::HandleScope scope(env);

    if (constructor == nullptr || camera == nullptr)
        return env.Null();

    return constructor->New({Napi::External<Camera>::New(env, camera)});
}

CameraWrap* CameraWrap::FromObject(Napi::Object obj)
{
    return Napi::ObjectWrap<CameraWrap>::Unwrap(obj);
}

Napi::Object CameraWrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Camera", {
        InstanceMethod("setParam", &CameraWrap::SetParam),
        InstanceMethod("getParam", &CameraWrap::GetParam),
        InstanceMethod("startTracking", &CameraWrap::StartTracking),
        InstanceMethod("stopTracking", &CameraWrap::StopTracking),
        InstanceMethod("addEventListener", &CameraWrap::AddEventListener),
        InstanceMethod("getWidth", &CameraWrap::GetWidth),
        InstanceMethod("getHeight", &CameraWrap::GetHeight),
        InstanceMethod("getPose", &CameraWrap::GetPose)
    });

    constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("Camera", func);

    return exports;
}

CameraWrap::CameraWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<CameraWrap>(info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    Napi::External<Camera> ext = info[0].As<Napi::External<Camera>>();
    camera = ext.Data();

    camera->onPreview(new CallbackFunction<Frame>([this](const Frame& frame) {
        int size;
        unsigned char* data = frame.encodeJPG(25, &size);
        if (data == nullptr)
        {
            std::cout << "=>  data is null" << std::endl;
            return;
        }
        char* b64 = b64_encode(data, size);
        delete[] data;

        std::string imgPrefix = "data:image/jpeg;base64,";

        Napi::Env env = this->Env();
        Napi::Object obj = Napi::Object::New(env);
        obj.Set("width", Napi::Number::New(env, frame.getWidth()));
        obj.Set("height", Napi::Number::New(env, frame.getHeight()));
        obj.Set("data", Napi::String::New(env, imgPrefix + b64));

        for (Napi::FunctionReference* ref : this->frameListeners)
        {
            ref->Call({obj});
        }
    }));
}

CameraWrap::~CameraWrap()
{
    
}

Napi::Value CameraWrap::SetParam(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    std::string paramName = info[0].As<Napi::String>().Utf8Value();
    Napi::Value paramValue = info[1];
    Param* param = camera->getParameter(paramName);
    if (param == nullptr)
        return Napi::Boolean::New(env, false);
    
    switch (param->getType())
    {
    case ParamType::INT:
        param->setValue(paramValue.As<Napi::Number>().Int32Value());
        break;
    case ParamType::FLOAT: 
        param->setValue(paramValue.As<Napi::Number>().FloatValue());
        break;
    case ParamType::BOOL:
        param->setValue(paramValue.As<Napi::Boolean>().Value());
        break;
    case ParamType::STRING:
        param->setValue(paramValue.As<Napi::String>().Utf8Value());
        break;
    case ParamType::ENUM:
        param->setValue(paramValue.As<Napi::Number>().Int32Value());
        break;
    default: return Napi::Boolean::New(env, false);
    }
    return Napi::Boolean::New(env, true);
}

Napi::Value CameraWrap::GetParam(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    std::string paramName = info[0].As<Napi::String>().Utf8Value();
    Param* param = camera->getParameter(paramName);
    if (param == nullptr)
        return Napi::Boolean::New(env, false);
    
    switch (param->getType())
    {
    case ParamType::INT:
        return Napi::Number::New(env, param->asInt());
    case ParamType::FLOAT: 
        return Napi::Number::New(env, param->asFloat());
    case ParamType::BOOL:
        return Napi::Boolean::New(env, param->asBool());
    case ParamType::STRING:
        return Napi::String::New(env, param->asString());
    case ParamType::ENUM:
        return Napi::Number::New(env, param->asInt());
    default: return env.Null();
    }
}

Napi::Value CameraWrap::StartTracking(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    int res = camera->startTracking();
    return Napi::Number::New(env, res);
}

Napi::Value CameraWrap::StopTracking(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    int res = camera->stopTracking();
    return Napi::Number::New(env, res);
}

Napi::Value CameraWrap::GetId(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return Napi::Number::New(env, (int) camera);
}

Napi::Value CameraWrap::GetWidth(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    int width = camera->getWidth();
    return Napi::Number::New(env, width);
}

Napi::Value CameraWrap::GetHeight(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    int height = camera->getHeight();
    return Napi::Number::New(env, height);
}

Napi::Value CameraWrap::GetPose(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return PoseWrap::NewInstance(env, nullptr); // TODO : Change
}

Napi::Value CameraWrap::AddEventListener(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    Napi::String eventName = info[0].As<Napi::String>();
    Napi::Function callback = info[1].As<Napi::Function>();
    std::string eventNameStr = eventName.Utf8Value();
    if (eventNameStr != "frame")
        return Napi::Boolean::New(env, false);

    Napi::FunctionReference* ref = new Napi::FunctionReference();
    *ref = Napi::Persistent(callback);
    this->frameListeners.push_back(ref);
    return Napi::Boolean::New(env, true);
}

const Camera& CameraWrap::getCamera()
{
    return *camera;
}
