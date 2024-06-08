#pragma once
#include <napi.h>
#include "Struct/Marker.hpp"

class MarkerWrap: public Napi::ObjectWrap<MarkerWrap>
{
private:
    static Napi::FunctionReference* constructor;
    Marker* marker = nullptr;

public:
    static Napi::Value NewInstance(Napi::Env env, Marker* marker);
    static MarkerWrap* FromObject(Napi::Object obj);
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    MarkerWrap(const Napi::CallbackInfo& info);
    ~MarkerWrap();

    Napi::Value setId(const Napi::CallbackInfo& info);
    Napi::Value setPose(const Napi::CallbackInfo& info);
    Napi::Value getId(const Napi::CallbackInfo& info);
    Napi::Value getPose(const Napi::CallbackInfo& info);

    Marker* getMarker();
};
