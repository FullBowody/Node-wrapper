#pragma once
#include <napi.h>
#include <memory>
#include "Plugins/PluginHandle.hpp"
#include "Camera/Capture.hpp"

class PluginHandleWrap: public Napi::ObjectWrap<PluginHandleWrap>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Value Create(const Napi::CallbackInfo& info);
    static Napi::Value Wrap(Napi::Env env, PluginHandle<Capture>* pluginHandle);

    PluginHandleWrap(const Napi::CallbackInfo& info);
    ~PluginHandleWrap();

    PluginHandle<Capture>* getPluginHandle() { return pluginHandle; }

private:
    inline static Napi::FunctionReference* constructor = nullptr;
    PluginHandle<Capture>* pluginHandle;
};
