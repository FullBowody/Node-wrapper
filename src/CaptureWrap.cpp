#include "CaptureWrap.hpp"
#include "ParamWrap.hpp"
#include "CaptureSkeletonWrap.hpp"
#include "image_utils.hpp"

Napi::Object CaptureWrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Capture", {
        InstanceMethod("getWidth", &CaptureWrap::getWidth),
        InstanceMethod("getHeight", &CaptureWrap::getHeight),
        InstanceMethod("getParams", &CaptureWrap::getParams),
        InstanceMethod("getPreviewImage", &CaptureWrap::getPreviewImage),
        InstanceMethod("getCaptureSkeleton", &CaptureWrap::getCaptureSkeleton)
    });

    constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("Capture", func);

    return exports;
}

Napi::Value CaptureWrap::Create(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return constructor->New({});
}

Napi::Value CaptureWrap::Wrap(Napi::Env env, Capture* capture)
{
    Napi::Object obj = constructor->New({});
    CaptureWrap* wrapper = Napi::ObjectWrap<CaptureWrap>::Unwrap(obj);
    wrapper->capture = capture;
    return obj;
}

CaptureWrap::CaptureWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<CaptureWrap>(info)
{
    Napi::Env env = info.Env();
    // Napi::TypeError::New(env, "Use a plugin to create a Capture object").ThrowAsJavaScriptException();
}

CaptureWrap::~CaptureWrap()
{
    // nothing to delete : shared_ptr will take care of it
}

Napi::Value CaptureWrap::getWidth(const Napi::CallbackInfo& info)
{
    if (this->capture == nullptr)
        return info.Env().Null();
    return Napi::Number::New(info.Env(), this->capture->getWidth());
}

Napi::Value CaptureWrap::getHeight(const Napi::CallbackInfo& info)
{
    if (this->capture == nullptr)
        return info.Env().Null();
    return Napi::Number::New(info.Env(), this->capture->getHeight());
}

Napi::Value CaptureWrap::getParams(const Napi::CallbackInfo& info)
{
    if (this->capture == nullptr)
        return info.Env().Null();

    auto& params = this->capture->getParameters();
    Napi::Array arr = Napi::Array::New(info.Env(), params.size());
    for (size_t i = 0; i < params.size(); i++)
    {
        arr.Set(i, ParamWrap::Wrap(info.Env(), params[i]));
    }
    return arr;
}

Napi::Value CaptureWrap::getPreviewImage(const Napi::CallbackInfo& info)
{
    if (this->capture == nullptr)
        return info.Env().Null();

    Napi::Env env = info.Env();
    const Image& image = this->capture->getImage();
    if (image.getWidth() == 0 || image.getHeight() == 0)
        return env.Undefined(); // not ready yet

    char* b64_jpg = base64_encodeImage(image);
    Napi::String str = Napi::String::New(env, b64_jpg);
    free(b64_jpg);
    return str;
}

Napi::Value CaptureWrap::getCaptureSkeleton(const Napi::CallbackInfo& info)
{
    if (this->capture == nullptr)
        return info.Env().Null();

    if (this->capture->getSkeleton() == nullptr)
        return info.Env().Null();

    return CaptureSkeletonWrap::Wrap(info.Env(), this->capture->getSkeleton());
}
