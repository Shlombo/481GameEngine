#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "gameObject.h"

class Platform : public GameObject {

public:

    std::string type;
    sf::Texture tex;

    Platform(std::string type, float length, float width, float xPos, float yPos, int color);

    Platform(std::string type, float length, float width, float xPos, float yPos, std::string textureFile);

    std::string getType();

    void render(sf::RenderWindow *window);


};