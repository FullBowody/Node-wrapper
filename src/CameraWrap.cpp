#include <memory.h>
#include "CameraWrap.hpp"
#include "base64.hpp"

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
        InstanceMethod("readDevice", &CameraWrap::ReadDevice),
        InstanceMethod("readStream", &CameraWrap::ReadStream),
        InstanceMethod("getWidth", &CameraWrap::GetWidth),
        InstanceMethod("getHeight", &CameraWrap::GetHeight),
        InstanceMethod("getFps", &CameraWrap::GetFps)
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
    this->camera = ext.Data();
}

CameraWrap::~CameraWrap()
{
    
}

Napi::Value CameraWrap::ReadDevice(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    int device = info[0].As<Napi::Number>().Int32Value();
    this->camera->readDevice(device);
    return Napi::Boolean::New(env, true);
}

Napi::Value CameraWrap::ReadStream(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    std::string url = info[0].As<Napi::String>().Utf8Value();
    this->camera->readStream(url);
    return Napi::Boolean::New(env, true);
}

Napi::Value CameraWrap::GetWidth(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    int width = this->camera->getWidth();
    return Napi::Number::New(env, width);
}

Napi::Value CameraWrap::GetHeight(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    int height = this->camera->getHeight();
    return Napi::Number::New(env, height);
}

Napi::Value CameraWrap::GetFps(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    int fps = this->camera->getFps();
    return Napi::Number::New(env, fps);
}

Camera* CameraWrap::getCamera()
{
    return this->camera;
}
