#include "eventManager.h"

EventManager::EventManager(int num) {
    this->id = num;
    this->guid = "eventManager";
    this->scriptEvent = 0;
}

void EventManager::registerType(std::string type, EventHandler *handler) {
    this->registeredEvents.insert(std::make_pair(type, handler));
}

void EventManager::raise(Event e) {
    std::cout << "Event pushed" << std::endl;
    this->eventList.push(e);
    
    // std::cout << "Event pushed" << std::endl;
    // this->eventList.push_back(e);
}

void EventManager::handleEvents(float time) {
    while(!this->eventList.empty()) {
        std::cout << "Check event list" << std::endl;
        std::cout << "Event list " << this->eventList.top().type << std::endl;
        Event event = this->eventList.top();
        std::cout << "Event list 123: " << event.type << std::endl;
        if(!this->eventList.empty()) {
            this->eventList.pop();
        }
        
        // std::cout << "Event Player id: " << event.player->UUID << std::endl;
        std::cout << "Event type: " << event.type << std::endl;
        auto it = this->registeredEvents.find(event.type);
        if(it != this->registeredEvents.end()) {
            std::cout << "Call to handle event" << std::endl;
            it->second->handleEvent(event);
        }
    }
    
}

void EventManager::raiseScriptedEvent(float time) {
    if(scriptEvent == 1) {
        std::cout << "Script Event Raise" << std::endl;
        Event event("fly", time);
        this->eventList.push(event);
        scriptEvent = 0;
    }
}

v8::Local<v8::Object> EventManager::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name)
{
	std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
    v.push_back(v8helpers::ParamContainer("scriptEvent", getScriptEvent, setScriptEvent));
    return v8helpers::exposeToV8(guid , this, v, isolate, context, context_name);
}

void EventManager::setScriptEvent(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	static_cast<EventManager*>(ptr)->scriptEvent = value->Int32Value();
}

void EventManager::getScriptEvent(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	int x_val = static_cast<EventManager*>(ptr)->scriptEvent;
	info.GetReturnValue().Set(x_val);
}