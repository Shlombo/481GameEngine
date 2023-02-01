#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "gameTime.h"
#include "gameObject.h"
#include "spawnpoint.h"

class Character : public GameObject {
public:
    
    //Bools for networking
    bool left;
    bool right;
    bool space;

    sf::Vector2f spawnpoint;
    bool isGrounded;
    float velocityX;
    float velocityY;
    float speed;
    float jumpSpeed;
    GameTime *timeline;
    float current_game_time;
    float deltaT;
    float last_game_time;
    sf::View *viewPtr;

    Character(float length, float width, float xPos, float yPos, int id);

    void input();
    void render(sf::RenderWindow *window);
    void collision(std::list<GameObject *> *worldList);
    void movement();

    void setTimeline(GameTime *timelineObj);

    void setSpawn(sf::Vector2f spawn);

    void updateTime();

    void setViewPtr(sf::View *view);


};