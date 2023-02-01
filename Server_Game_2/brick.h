#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "gameObject.h"

class Brick : public GameObject {

public:

    std::string type;
    sf::Texture tex;

    // ~Brick() { };

    Brick(std::string type, float length, float width, float xPos, float yPos, int color);

    Brick(std::string type, float length, float width, float xPos, float yPos, std::string textureFile);

    std::string getType();

    void render(sf::RenderWindow *window);


};