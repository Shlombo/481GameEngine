#include "gameObject.h"

GameObject::GameObject(bool input, bool render, bool collision, bool move, bool time, float length, float width) : sf::RectangleShape(sf::Vector2f(length, width)) {
    this->hasInput = input;
    this->hasRender = render;
    this->hasCollision = collision;
    this->hasMovement = move;
    this->hasTimeline = time;
}

void GameObject::setObjectType(std::string type) {
    this->objectType = type;
}

void GameObject::setId(int id) {
    this->UUID = id;
}