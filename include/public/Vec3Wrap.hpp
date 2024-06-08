#pragma once
#include <napi.h>
#include "Struct/Vec3.hpp"

class Vec3Wrap: public Napi::ObjectWrap<Vec3Wrap>
{
private:
    static Napi::FunctionReference* constructor;
    Vec3f* vec = nullptr;

public:
    static Napi::Value NewInstance(Napi::Env env, Vec3f* vec);
    static Vec3Wrap* FromObject(Napi::Object obj);
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    Vec3Wrap(const Napi::CallbackInfo& info);
    ~Vec3Wrap();

    Napi::Value setX(const Napi::CallbackInfo& info);
    Napi::Value setY(const Napi::CallbackInfo& info);
    Napi::Value setZ(const Napi::CallbackInfo& info);
    Napi::Value getX(const Napi::CallbackInfo& info);
    Napi::Value getY(const Napi::CallbackInfo& info);
    Napi::Value getZ(const Napi::CallbackInfo& info);

    Vec3f* getVec3f();
};
