#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "gameTime.h"
#include "gameObject.h"

class Spawnpoint : public GameObject {
public:
    
    Spawnpoint(float xPos, float yPos);

};