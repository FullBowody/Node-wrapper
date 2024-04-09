#pragma once
#include <napi.h>
#include "Camera/Camera.hpp"

class CameraWrap: public Napi::ObjectWrap<CameraWrap>
{
private:
    static Napi::FunctionReference* constructor;
    Camera* camera;

public:
    static Napi::Value NewInstance(Napi::Env env, Camera* camera);
    static CameraWrap* FromObject(Napi::Object obj);
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    CameraWrap(const Napi::CallbackInfo& info);
    ~CameraWrap();

    Napi::Value ReadDevice(const Napi::CallbackInfo& info);
    Napi::Value ReadStream(const Napi::CallbackInfo& info);

    Camera* getCamera();
};
