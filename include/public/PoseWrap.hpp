#pragma once
#include <napi.h>
#include "Struct/Pose.hpp"

class PoseWrap: public Napi::ObjectWrap<PoseWrap>
{
private:
    static Napi::FunctionReference* constructor;
    Pose* pose = nullptr;

public:
    static Napi::Value NewInstance(Napi::Env env, Pose* pose);
    static PoseWrap* FromObject(Napi::Object obj);
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    PoseWrap(const Napi::CallbackInfo& info);
    ~PoseWrap();

    Napi::Value setPosition(const Napi::CallbackInfo& info);
    Napi::Value getPosition(const Napi::CallbackInfo& info);
    Napi::Value setRotation(const Napi::CallbackInfo& info);
    Napi::Value getRotation(const Napi::CallbackInfo& info);

    const Pose& getPose();
};
