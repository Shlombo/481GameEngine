#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "gameTime.h"
#include "gameObject.h"

class ViewBoundary : public GameObject {
public:
    
    sf::View *viewPtr;
    sf::Vector2f center;
    
    ViewBoundary(float length, float width, float xPos, float yPos, sf::View *view);

    void setViewCenter(sf::Vector2f cent);
    void collision();

};