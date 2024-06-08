#include "EventEmitter.hpp"

EventEmitter::EventEmitter()
{
}

EventEmitter::~EventEmitter()
{
}

void EventEmitter::emitEvent(const Napi::CallbackInfo& info, std::string name, Napi::Value data)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (events.find(name) == events.end())
        return;

    Napi::Function func = events[name]->Value();
    func.Call({data});
}

Napi::Value EventEmitter::on(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() < 2)
        return env.Null();

    std::string name = info[0].As<Napi::String>().Utf8Value();
    Napi::Function func = info[1].As<Napi::Function>();

    if (events.find(name) == events.end())
        events[name] = new Napi::FunctionReference();
    *events[name] = Napi::Persistent(func);

    return env.Null();
}