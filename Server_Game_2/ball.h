#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <v8.h>
#include "v8helpers.h"
#include "gameTime.h"
#include "gameObject.h"

class Ball : public GameObject {

public:
    std::string type;
    sf::Texture tex;
    float boundDR;
    float boundUL;
    float velocityX;
    float velocityY;
    float speed;
    GameTime *timeline;
    float current_game_time;
    float deltaT;
    float last_game_time;
    bool *win;
    bool *lose;
    int score;


    Ball(std::string type, float length, float width, float xPos, float yPos, float boundUL, float boundDR, int color, bool *win, bool *lose);

    Ball(std::string type, float length, float width, float xPos, float yPos, float boundUL, float boundDR, std::string textureFile);
    
    std::string getType();

    void movement();
    void render(sf::RenderWindow *window);
    void collision(std::list<GameObject *> *worldList);

    void setTimeline(GameTime *timelineObj);
    void updateTime();

};