#pragma once
#include <napi.h>
#include <memory>
#include "Plugins/PluginDescription.hpp"

class PluginDescriptionWrap: public Napi::ObjectWrap<PluginDescriptionWrap>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Value Create(const Napi::CallbackInfo& info);
    static Napi::Value Wrap(Napi::Env env, std::shared_ptr<PluginDescription> pluginDescription);

    PluginDescriptionWrap(const Napi::CallbackInfo& info);
    ~PluginDescriptionWrap();

    Napi::Value getName(const Napi::CallbackInfo& info);
    Napi::Value getDescription(const Napi::CallbackInfo& info);
    Napi::Value getType(const Napi::CallbackInfo& info);
    Napi::Value getAuthor(const Napi::CallbackInfo& info);
    Napi::Value getVersion(const Napi::CallbackInfo& info);

    Napi::Value createHandle(const Napi::CallbackInfo& info);

    const std::shared_ptr<PluginDescription>& getPluginDescription() { return pluginDescription; }

private:
    inline static Napi::FunctionReference* constructor = nullptr;
    std::shared_ptr<PluginDescription> pluginDescription;
};
