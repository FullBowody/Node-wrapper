#pragma once
#include <napi.h>
#include <memory>
#include "Camera/Camera.hpp"

class CameraWrap: public Napi::ObjectWrap<CameraWrap>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Value Create(const Napi::CallbackInfo& info);
    static Napi::Value Wrap(Napi::Env env, std::shared_ptr<Camera> camera);

    CameraWrap(const Napi::CallbackInfo& info);
    ~CameraWrap();

    Napi::Value getCapture(const Napi::CallbackInfo& info);
    Napi::Value getName(const Napi::CallbackInfo& info);

    Napi::Value useCapturePlugin(const Napi::CallbackInfo& info);
    Napi::Value setName(const Napi::CallbackInfo& info);

    Napi::Value startPreview(const Napi::CallbackInfo& info);
    Napi::Value stopPreview(const Napi::CallbackInfo& info);

    Napi::Value startTracking(const Napi::CallbackInfo& info);
    Napi::Value stopTracking(const Napi::CallbackInfo& info);

    const std::shared_ptr<Camera>& getCamera() { return camera; }

private:
    inline static Napi::FunctionReference* constructor = nullptr;
    std::shared_ptr<Camera> camera;
};
