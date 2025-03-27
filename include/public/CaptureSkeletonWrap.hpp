#pragma once
#include <napi.h>
#include <memory>
#include "Structs/CaptureSkeleton.hpp"

class CaptureSkeletonWrap: public Napi::ObjectWrap<CaptureSkeletonWrap>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Value Create(const Napi::CallbackInfo& info);
    static Napi::Value Wrap(Napi::Env env, std::shared_ptr<CaptureSkeleton> captureSkeleton);

    CaptureSkeletonWrap(const Napi::CallbackInfo& info);
    ~CaptureSkeletonWrap();

    Napi::Value setJoint(const Napi::CallbackInfo& info);
    Napi::Value getJoints(const Napi::CallbackInfo& info);
    Napi::Value getJoint(const Napi::CallbackInfo& info);
    Napi::Value getJointCount(const Napi::CallbackInfo& info);

    const std::shared_ptr<CaptureSkeleton>& getCaptureSkeleton() { return captureSkeleton; }

private:
    inline static Napi::FunctionReference* constructor = nullptr;
    std::shared_ptr<CaptureSkeleton> captureSkeleton;
};
