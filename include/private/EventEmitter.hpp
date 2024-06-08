#include <napi.h>
#include <string>
#include <map>

class EventEmitter
{
private:
    std::map<std::string, Napi::FunctionReference*> events;

protected:
    void emitEvent(const Napi::CallbackInfo& info, std::string name, Napi::Value data);
    
public:
    EventEmitter();
    ~EventEmitter();

    Napi::Value on(const Napi::CallbackInfo& info);
};
