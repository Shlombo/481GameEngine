#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <queue>
#include <v8.h>
#include "v8helpers.h"
#include "eventHandler.h"
#include "event.h"
#include "compareEvent.h"


class EventManager {
public:
    
    
    std::map<std::string, EventHandler*> registeredEvents;
    std::priority_queue<Event, std::vector<Event>, CompareEvent> eventList;
    // std::list<Event> eventList;
    
    int id;
    int scriptEventLeft;
    int scriptEventRight;

    std::string guid;


    EventManager(int num);

    void registerType(std::string type, EventHandler *handler);
    void raise(Event e);
    void handleEvents(float time);

    //Function to expose to V8
    v8::Local<v8::Object> exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name="default");

    //Function to be called when script raises fly event
    void raiseScriptedEvent(float time);

    static void setScriptEventLeft(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
	static void getScriptEventLeft(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info); // note return type
    static void setScriptEventRight(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
	static void getScriptEventRight(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info); // note return type

};