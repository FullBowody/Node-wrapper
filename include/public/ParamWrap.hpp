#pragma once
#include <napi.h>
#include <memory>
#include "Params/Param.hpp"

class ParamWrap: public Napi::ObjectWrap<ParamWrap>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Value Create(const Napi::CallbackInfo& info);
    static Napi::Value Wrap(Napi::Env env, std::shared_ptr<Param> param);

    ParamWrap(const Napi::CallbackInfo& info);
    ~ParamWrap();

    Napi::Value getName(const Napi::CallbackInfo& info);
    Napi::Value getType(const Napi::CallbackInfo& info);
    Napi::Value getValue(const Napi::CallbackInfo& info);
    void setName(const Napi::CallbackInfo& info, const Napi::Value& value);
    void setType(const Napi::CallbackInfo& info, const Napi::Value& value);
    void setValue(const Napi::CallbackInfo& info, const Napi::Value& value);

    const std::shared_ptr<Param>& getParam() { return param; }

private:
    inline static Napi::FunctionReference* constructor = nullptr;
    std::shared_ptr<Param> param;
};
