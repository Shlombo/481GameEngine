#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
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



    MovingPlatform(std::string type, float length, float width, float xPos, float yPos, float boundUL, float boundDR, int color);

    MovingPlatform(std::string type, float length, float width, float xPos, float yPos, float boundUL, float boundDR, std::string textureFile);
    
    std::string getType();

    void movement();
    void render(sf::RenderWindow *window);

    void setTimeline(GameTime *timelineObj);
    void updateTime();



};