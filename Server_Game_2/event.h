#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "gameTime.h"
#include "character.h"
#include "gameObject.h"

class Event {
public:
    std::string type;
    float timestamp;
    Character *player;
    GameObject *object1;

    Event(std::string typeString);
    Event(std::string typeString, float time);
    Event(std::string typeString, float time, Character *obj);
    Event(std::string typeString, float time, GameObject *obj1);
    Event(std::string typeString, float time, Character *obj, GameObject *obj1);

};