#include "ParamWrap.hpp"
#include "Params/ParamEnum.hpp"
#include <memory>

Napi::Object ParamWrap::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Param", {
        InstanceAccessor("name", &ParamWrap::getName, &ParamWrap::setName),
        InstanceAccessor("type", &ParamWrap::getType, &ParamWrap::setType),
        InstanceAccessor("value", &ParamWrap::getValue, &ParamWrap::setValue)
    });

    constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    exports.Set("Param", func);

    return exports;
}

Napi::Value ParamWrap::Create(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    return constructor->New({});
}

Napi::Value ParamWrap::Wrap(Napi::Env env, std::shared_ptr<Param> param)
{
    Napi::Object obj = constructor->New({});
    ParamWrap* wrapper = Napi::ObjectWrap<ParamWrap>::Unwrap(obj);
    wrapper->param = param;
    return obj;
}

ParamWrap::ParamWrap(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<ParamWrap>(info)
{
    Napi::Env env = info.Env();
    // Napi::TypeError::New(env, "Use a plugin to create a Param object").ThrowAsJavaScriptException();
}

ParamWrap::~ParamWrap()
{
    // nothing to delete : shared_ptr will take care of it
}

Napi::Value ParamWrap::getName(const Napi::CallbackInfo& info)
{
    return Napi::String::New(info.Env(), this->param->getName());
}

Napi::Value ParamWrap::getType(const Napi::CallbackInfo& info)
{
    std::string str;
    switch (this->param->getType())
    {
        case ParamType::BOOL: str = "BOOL"; break;
        case ParamType::INT: str = "INT"; break;
        case ParamType::FLOAT: str = "FLOAT"; break;
        case ParamType::STRING: str = "STRING"; break;
        case ParamType::ENUM: str = "ENUM"; break;
        default: str = "UNKNOWN"; break;
    }
    return Napi::String::New(info.Env(), str);
}

Napi::Value ParamWrap::getValue(const Napi::CallbackInfo& info)
{
    switch (this->param->getType())
    {
        case ParamType::BOOL:
            return Napi::Boolean::New(info.Env(), this->param->asBool());
            break;
        case ParamType::INT:
            return Napi::Number::New(info.Env(), this->param->asInt());
            break;
        case ParamType::FLOAT:
            return Napi::Number::New(info.Env(), this->param->asFloat());
            break;
        case ParamType::STRING:
            return Napi::String::New(info.Env(), this->param->asString());
            break;
        case ParamType::ENUM:
            return Napi::Number::New(info.Env(), this->param->asEnum());
            break;
        default:
            Napi::Error::New(info.Env(), "Internal error : Unknown ParamType").ThrowAsJavaScriptException();
            break;
    }
    return info.Env().Undefined();
}

void ParamWrap::setName(const Napi::CallbackInfo& info, const Napi::Value& value)
{
    Napi::Env env = info.Env();
    Napi::TypeError::New(env, "Cannot set name").ThrowAsJavaScriptException();
}

void ParamWrap::setType(const Napi::CallbackInfo& info, const Napi::Value& value)
{
    Napi::Env env = info.Env();
    Napi::TypeError::New(env, "Cannot set type").ThrowAsJavaScriptException();
}

void ParamWrap::setValue(const Napi::CallbackInfo& info, const Napi::Value& value)
{
    switch (this->param->getType())
    {
    case ParamType::FLOAT:
        this->param->setValue(value.As<Napi::Number>().FloatValue());
        break;
    case ParamType::INT:
        this->param->setValue(value.As<Napi::Number>().Int32Value());
        break;
    case ParamType::BOOL:
        this->param->setValue(value.As<Napi::Boolean>().Value());
        break;
    case ParamType::STRING:
        this->param->setValue(value.As<Napi::String>().Utf8Value());
        break;
    case ParamType::ENUM:
        if (value.IsNumber())
        {
            this->param->setValue(value.As<Napi::Number>().Int32Value());
        }
        else if (value.IsString())
        {
            // std::shared_ptr<ParamEnum> paramEnum = std::static_pointer_cast<ParamEnum>(this->param);
            // if (paramEnum == nullptr)
            // {
            //     Napi::Error::New(info.Env(), "Internal error : ParamEnum expected for ENUM type").ThrowAsJavaScriptException();
            //     break;
            // }
            // std::string str = value.As<Napi::String>().Utf8Value();
            // int index = 0;
            // const std::vector<std::string>& options = paramEnum->getOptions();
            // for (auto& option : options)
            // {
            //     if (option == str)
            //     {
            //         this->param->setValue(index);
            //         return;
            //     }
            //     index++;
            // }
            // Napi::Error::New(info.Env(), "Invalid ENUM value").ThrowAsJavaScriptException();
            Napi::Error::New(info.Env(), "Not implemented").ThrowAsJavaScriptException();
            // TODO : Implement
        }
        else
        {
            Napi::TypeError::New(info.Env(), "Invalid value type").ThrowAsJavaScriptException();
        }
        break;
    }
}
