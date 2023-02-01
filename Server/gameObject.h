#pragma once
#include <SFML/Window.hpp>
#include <stdlib.h>
#include <list>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "gameTime.h"

class GameObject : public sf::RectangleShape {
    public:
        int UUID;
        std::string objectType;
        bool hasInput;
        bool hasRender;
        bool hasCollision;
        bool hasMovement;
        bool hasTimeline;
        GameTime *timeline;
        float current_game_time;
        float deltaT;
        float last_game_time;
        bool isGrounded;

        //Bools for character networking
        bool left = false;
        bool right = false;
        bool space = false;

        GameObject(bool input, bool render, bool collision, bool move, bool time, float length, float width);

        virtual void input() {};
        virtual void render(sf::RenderWindow *window) {};
        virtual void collision(std::list<GameObject *> *worldList) {};
        virtual void collision() {};
        virtual void movement() {};
        virtual void setTimeline(GameTime *timelineObj) {};
        virtual void setObjectType(std::string type);
        void setId(int id);

};