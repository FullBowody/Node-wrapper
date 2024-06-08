#pragma once
#include <napi.h>
#include "Plugins/PluginDescriptor.hpp"

class PluginWrap: public Napi::ObjectWrap<PluginWrap>
{
private:
    static Napi::FunctionReference* constructor;
    PluginDescriptor* descriptor;

public:
    static Napi::Value NewInstance(Napi::Env env, PluginDescriptor descriptor);
    static PluginWrap* FromObject(Napi::Object obj);
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    PluginWrap(const Napi::CallbackInfo& info);
    ~PluginWrap();

    Napi::Value getName(const Napi::CallbackInfo& info);
    Napi::Value getDescription(const Napi::CallbackInfo& info);
    Napi::Value getAuthor(const Napi::CallbackInfo& info);
    Napi::Value getVersion(const Napi::CallbackInfo& info);
    Napi::Value getType(const Napi::CallbackInfo& info);

    PluginDescriptor* getPluginDescriptor();
};
