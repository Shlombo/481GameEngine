#include "spawnpoint.h"

Spawnpoint::Spawnpoint(float xPos, float yPos) : GameObject(false, false, true, false, false, 1, 100) {
    this->setPosition(xPos, yPos);
    this->setObjectType("spawn");
}

