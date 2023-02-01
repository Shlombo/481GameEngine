#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <v8.h>
#include "v8helpers.h"
#include "platform.h"
#include "gameTime.h"

class MovingPlatform : public GameObject {

public:
    std::string type;
    sf::Texture tex;
    float boundDR;
    float boundUL;
    float velocityX;
    float velocityY;
    GameTime *timeline;
    float current_game_time;
    float deltaT;
    float last_game_time;

    //Scripted position and guid
    float scriptX;
    float scriptY;
    std::string guid;




    MovingPlatform(std::string type, float length, float width, float xPos, float yPos, float boundUL, float boundDR, int color);

    MovingPlatform(std::string type, float length, float width, float xPos, float yPos, float boundUL, float boundDR, std::string textureFile);
    
    std::string getType();

    void movement();
    void render(sf::RenderWindow *window);

    void setTimeline(GameTime *timelineObj);
    void updateTime();

    //Function to expose to V8
    v8::Local<v8::Object> exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name="default");


    //Setter to make sure movement isn't handled twice
    // void setHasMovement(bool boolean);

    //Getters and Setters for object position
    static void setGameObjectX(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
	static void getGameObjectX(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info); // note return type
	static void setGameObjectY(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
	static void getGameObjectY(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info); // note return type
    static void setGameObjectGUID(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
	static void getGameObjectGUID(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info); // note return type

};