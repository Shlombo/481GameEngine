#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "gameTime.h"
#include "gameObject.h"

class DeathBarrier : public GameObject {
public:
    
    DeathBarrier(float length, float width, float xPos, float yPos);

};