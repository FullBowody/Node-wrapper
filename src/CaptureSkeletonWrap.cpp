#include "CaptureSkeletonWrap.hpp"
#include "CaptureJointWrap.hpp"
#include "Vec3Wrap.hpp"
#include <iostream>

Napi::Object CaptureSkeletonWrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "CaptureSkeleton", {
        InstanceMethod("setJoint", &CaptureSkeletonWrap::setJoint),
        InstanceMethod("getJoints", &CaptureSkeletonWrap::getJoints),
        InstanceMethod("getJoint", &CaptureSkeletonWrap::getJoint),
        InstanceMethod("getJointCount", &CaptureSkeletonWrap::getJointCount)
    });

    constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("CaptureSkeleton", func);

    return exports;
}

Napi::Value CaptureSkeletonWrap::Create(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return constructor->New({});
}

Napi::Value CaptureSkeletonWrap::Wrap(Napi::Env env, std::shared_ptr<CaptureSkeleton> captureSkeleton)
{
    Napi::Object obj = constructor->New({});
    CaptureSkeletonWrap* wrapper = Napi::ObjectWrap<CaptureSkeletonWrap>::Unwrap(obj);
    wrapper->captureSkeleton = captureSkeleton;
    return obj;
}

CaptureSkeletonWrap::CaptureSkeletonWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<CaptureSkeletonWrap>(info)
{
    Napi::Env env = info.Env();
    // Napi::TypeError::New(env, "Use the engine.createCaptureSkeleton method to create a CaptureSkeleton object").ThrowAsJavaScriptException();
}

CaptureSkeletonWrap::~CaptureSkeletonWrap()
{
    // nothing to delete : shared_ptr will take care of it
}

Napi::Value CaptureSkeletonWrap::setJoint(const Napi::CallbackInfo& info)
{
    if (this->captureSkeleton == nullptr)
        return info.Env().Null();

    Napi::Env env = info.Env();
    if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsObject())
    {
        Napi::TypeError::New(env, "setJoint expects a number and a CaptureJoint object").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    this->captureSkeleton->setJoint(
        info[0].As<Napi::Number>().Int32Value(),
        *CaptureJointWrap::Unwrap(info[1].As<Napi::Object>())->getCaptureJoint()
    );
    return env.Undefined();
}

Napi::Value CaptureSkeletonWrap::getJoints(const Napi::CallbackInfo& info)
{
    if (this->captureSkeleton == nullptr)
        return info.Env().Null();

    const auto& joints = this->captureSkeleton->getJoints();
    Napi::Array array = Napi::Array::New(info.Env(), joints.size());
    for (size_t i = 0; i < joints.size(); i++)
    {
        array.Set(i, CaptureJointWrap::Wrap(info.Env(), joints[i]));
    }
    return array;
}

Napi::Value CaptureSkeletonWrap::getJoint(const Napi::CallbackInfo& info)
{
    if (this->captureSkeleton == nullptr)
        return info.Env().Null();

    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsNumber())
    {
        Napi::TypeError::New(env, "getJoint expects a number").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    int index = info[0].As<Napi::Number>().Int32Value();
    return CaptureJointWrap::Wrap(env, this->captureSkeleton->getJoint(index));
}

Napi::Value CaptureSkeletonWrap::getJointCount(const Napi::CallbackInfo& info)
{
    if (this->captureSkeleton == nullptr)
        return info.Env().Null();

    return Napi::Number::New(info.Env(), this->captureSkeleton->getJointCount());
}
