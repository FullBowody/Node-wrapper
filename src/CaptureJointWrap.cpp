#include "CaptureJointWrap.hpp"
#include "Vec3Wrap.hpp"

Napi::Object CaptureJointWrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "CaptureJoint", {
        InstanceMethod("setPosition", &CaptureJointWrap::setPosition),
        InstanceMethod("setConfidence", &CaptureJointWrap::setConfidence),
        InstanceMethod("getPosition", &CaptureJointWrap::getPosition),
        InstanceMethod("getConfidence", &CaptureJointWrap::getConfidence)
    });

    constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("CaptureJoint", func);

    return exports;
}

Napi::Value CaptureJointWrap::Create(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return constructor->New({});
}

Napi::Value CaptureJointWrap::Wrap(Napi::Env env, std::shared_ptr<CaptureJoint> captureJoint)
{
    Napi::Object obj = constructor->New({});
    CaptureJointWrap* wrapper = Napi::ObjectWrap<CaptureJointWrap>::Unwrap(obj);
    wrapper->captureJoint = captureJoint;
    return obj;
}

CaptureJointWrap::CaptureJointWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<CaptureJointWrap>(info)
{
    Napi::Env env = info.Env();
    // Napi::TypeError::New(env, "Use the engine.createCaptureJoint method to create a CaptureJoint object").ThrowAsJavaScriptException();
}

CaptureJointWrap::~CaptureJointWrap()
{
    // nothing to delete : shared_ptr will take care of it
}

Napi::Value CaptureJointWrap::setPosition(const Napi::CallbackInfo& info)
{
    if (info.Length() < 1 || !info[0].IsObject())
    {
        Napi::TypeError::New(info.Env(), "Vec3 expected").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }
    // this->captureJoint->setPosition(*Vec3Wrap::Unwrap(info[0].As<Napi::Object>())->getVec3());
    return info.Env().Undefined();
}

Napi::Value CaptureJointWrap::setConfidence(const Napi::CallbackInfo& info)
{
    if (info.Length() < 1 || !info[0].IsNumber())
    {
        Napi::TypeError::New(info.Env(), "Number expected").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }
    this->captureJoint->setConfidence(info[0].As<Napi::Number>().FloatValue());
    return info.Env().Undefined();
}

Napi::Value CaptureJointWrap::getPosition(const Napi::CallbackInfo& info)
{
    return Vec3Wrap::Wrap(info.Env(), std::make_shared<glm::vec3>(this->captureJoint->getPosition()));
}

Napi::Value CaptureJointWrap::getConfidence(const Napi::CallbackInfo& info)
{
    return Napi::Number::New(info.Env(), this->captureJoint->getConfidence());
}
