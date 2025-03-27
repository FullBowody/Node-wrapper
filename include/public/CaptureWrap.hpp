#pragma once
#include <napi.h>
#include <memory>
#include "Camera/Capture.hpp"

class CaptureWrap: public Napi::ObjectWrap<CaptureWrap>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Value Create(const Napi::CallbackInfo& info);
    static Napi::Value Wrap(Napi::Env env, Capture* capture);

    CaptureWrap(const Napi::CallbackInfo& info);
    ~CaptureWrap();

    Napi::Value getWidth(const Napi::CallbackInfo& info);
    Napi::Value getHeight(const Napi::CallbackInfo& info);
    Napi::Value getParams(const Napi::CallbackInfo& info);
    Napi::Value getPreviewImage(const Napi::CallbackInfo& info);
    Napi::Value getCaptureSkeleton(const Napi::CallbackInfo& info);

    Capture* getCapture() { return capture; }

private:
    inline static Napi::FunctionReference* constructor = nullptr;
    Capture* capture;
};
