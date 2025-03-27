#include "CameraWrap.hpp"
#include "CaptureWrap.hpp"
#include "ParamWrap.hpp"
#include "image_utils.hpp"
#include "PluginHandleWrap.hpp"

Napi::Object CameraWrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Camera", {
        InstanceMethod("getCapture", &CameraWrap::getCapture),
        InstanceMethod("getName", &CameraWrap::getName),
        InstanceMethod("useCapturePlugin", &CameraWrap::useCapturePlugin),
        InstanceMethod("setName", &CameraWrap::setName),
        InstanceMethod("startPreview", &CameraWrap::startPreview),
        InstanceMethod("stopPreview", &CameraWrap::stopPreview),
        InstanceMethod("startTracking", &CameraWrap::startTracking),
        InstanceMethod("stopTracking", &CameraWrap::stopTracking)
    });

    constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("Camera", func);

    return exports;
}

Napi::Value CameraWrap::Create(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return constructor->New({});
}

Napi::Value CameraWrap::Wrap(Napi::Env env, std::shared_ptr<Camera> camera)
{
    Napi::Object obj = constructor->New({});
    CameraWrap* wrapper = Napi::ObjectWrap<CameraWrap>::Unwrap(obj);
    wrapper->camera = camera;
    return obj;
}

CameraWrap::CameraWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<CameraWrap>(info)
{
    Napi::Env env = info.Env();
    // Napi::TypeError::New(env, "Use the engine.createCamera method to create a Camera object").ThrowAsJavaScriptException();
}

CameraWrap::~CameraWrap()
{
    // nothing to delete : shared_ptr will take care of it
}

Napi::Value CameraWrap::getCapture(const Napi::CallbackInfo& info)
{
    if (this->camera->getCapture() == nullptr)
        return info.Env().Null();
    return CaptureWrap::Wrap(info.Env(), this->camera->getCapture());
}

Napi::Value CameraWrap::getName(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), this->camera->getName());
}

Napi::Value CameraWrap::useCapturePlugin(const Napi::CallbackInfo& info)
{
    if (info.Length() < 1 || !info[0].IsObject())
    {
        Napi::TypeError::New(info.Env(), "Expected a PluginHandle object").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }
    PluginHandleWrap* pluginHandleWrap = Napi::ObjectWrap<PluginHandleWrap>::Unwrap(info[0].As<Napi::Object>());
    this->camera->useCapturePlugin(pluginHandleWrap->getPluginHandle());
    return info.Env().Undefined();
}

Napi::Value CameraWrap::setName(const Napi::CallbackInfo& info)
{
    if (info.Length() < 1 || !info[0].IsString())
    {
        Napi::TypeError::New(info.Env(), "String expected").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }
    this->camera->setName(info[0].As<Napi::String>().Utf8Value());
    return info.Env().Undefined();
}

Napi::Value CameraWrap::startPreview(const Napi::CallbackInfo& info)
{
    FBError err = this->camera->startPreview();
    return Napi::Boolean::New(info.Env(), err == FBError::OK);
}

Napi::Value CameraWrap::stopPreview(const Napi::CallbackInfo& info)
{
    FBError err = this->camera->stopPreview();
    return Napi::Boolean::New(info.Env(), err == FBError::OK);
}

Napi::Value CameraWrap::startTracking(const Napi::CallbackInfo& info)
{
    FBError err = this->camera->startTracking();
    return Napi::Boolean::New(info.Env(), err == FBError::OK);
}

Napi::Value CameraWrap::stopTracking(const Napi::CallbackInfo& info)
{
    FBError err = this->camera->stopTracking();
    return Napi::Boolean::New(info.Env(), err == FBError::OK);
}
