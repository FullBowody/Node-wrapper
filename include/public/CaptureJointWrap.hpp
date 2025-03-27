#pragma once
#include <napi.h>
#include <memory>
#include "Structs/CaptureJoint.hpp"

class CaptureJointWrap: public Napi::ObjectWrap<CaptureJointWrap>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Value Create(const Napi::CallbackInfo& info);
    static Napi::Value Wrap(Napi::Env env, std::shared_ptr<CaptureJoint> captureJoint);

    CaptureJointWrap(const Napi::CallbackInfo& info);
    ~CaptureJointWrap();

    Napi::Value setPosition(const Napi::CallbackInfo& info);
    Napi::Value setConfidence(const Napi::CallbackInfo& info);
    Napi::Value getPosition(const Napi::CallbackInfo& info);
    Napi::Value getConfidence(const Napi::CallbackInfo& info);

    const std::shared_ptr<CaptureJoint>& getCaptureJoint() { return captureJoint; }

private:
    inline static Napi::FunctionReference* constructor = nullptr;
    std::shared_ptr<CaptureJoint> captureJoint;
};
