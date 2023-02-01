#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "gameTime.h"
#include "gameObject.h"
#include "ScriptManager.h"

class Event;
class EventManager;

class Character : public GameObject {
public:

    float velocityX;
    float velocityY;
    float speed;
    GameTime *timeline;
    float current_game_time;
    float deltaT;
    float last_game_time;
    EventManager *manager;

    ScriptManager *scriptManager;

    //Scripted position and guid
    float scriptX;
    float scriptY;
    std::string guid;

    Character(float length, float width, float xPos, float yPos, int id, GameTime *timelineObj, EventManager *man, ScriptManager *scriptMan);

    void input();
    void render(sf::RenderWindow *window);
    // void collision(std::list<GameObject *> *worldList);
    void movement();

    void setTimeline(GameTime *timelineObj);

    void updateTime();

    void setManager(EventManager *man);

    void setScriptManager(ScriptManager *man);

    //Function to expose to V8
    v8::Local<v8::Object> exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name="default");


    //Getters and Setters for object position
    static void setGameObjectX(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
	static void getGameObjectX(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info); // note return type
	static void setGameObjectY(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
	static void getGameObjectY(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info); // note return type
    static void setGameObjectGUID(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
	static void getGameObjectGUID(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info); // note return type


};