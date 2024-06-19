#pragma once
#include <napi.h>
#include "Camera/Camera.hpp"

class CameraWrap: public Napi::ObjectWrap<CameraWrap>
{
private:
    static Napi::FunctionReference* constructor;
    Camera* camera = nullptr;

    std::vector<Napi::FunctionReference*> frameListeners;

public:
    static Napi::Value NewInstance(Napi::Env env, Camera* camera);
    static CameraWrap* FromObject(Napi::Object obj);
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    CameraWrap(const Napi::CallbackInfo& info);
    ~CameraWrap();

    Napi::Value SetParam(const Napi::CallbackInfo& info);
    Napi::Value GetParam(const Napi::CallbackInfo& info);
    Napi::Value StartTracking(const Napi::CallbackInfo& info);
    Napi::Value StopTracking(const Napi::CallbackInfo& info);

    Napi::Value GetId(const Napi::CallbackInfo& info);
    Napi::Value GetWidth(const Napi::CallbackInfo& info);
    Napi::Value GetHeight(const Napi::CallbackInfo& info);
    
    Napi::Value GetPose(const Napi::CallbackInfo& info);

    Napi::Value AddEventListener(const Napi::CallbackInfo& info);

    const Camera& getCamera();
};
