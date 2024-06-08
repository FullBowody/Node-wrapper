#pragma once
#include <napi.h>
#include "Struct/Quaternion.hpp"

class QuaternionWrap: public Napi::ObjectWrap<QuaternionWrap>
{
private:
    static Napi::FunctionReference* constructor;
    Quaternion* quat = nullptr;

public:
    static Napi::Value NewInstance(Napi::Env env, Quaternion* vec);
    static QuaternionWrap* FromObject(Napi::Object obj);
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    QuaternionWrap(const Napi::CallbackInfo& info);
    ~QuaternionWrap();

    Napi::Value setX(const Napi::CallbackInfo& info);
    Napi::Value setY(const Napi::CallbackInfo& info);
    Napi::Value setZ(const Napi::CallbackInfo& info);
    Napi::Value setW(const Napi::CallbackInfo& info);
    Napi::Value getX(const Napi::CallbackInfo& info);
    Napi::Value getY(const Napi::CallbackInfo& info);
    Napi::Value getZ(const Napi::CallbackInfo& info);
    Napi::Value getW(const Napi::CallbackInfo& info);

    Quaternion* getQuaternion();
};
