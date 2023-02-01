#include "deathBarrier.h"

DeathBarrier::DeathBarrier(float length, float width, float xPos, float yPos) : GameObject(false, false, true, false, false, length, width) {
    this->setPosition(xPos, yPos);
    this->setObjectType("death");
}