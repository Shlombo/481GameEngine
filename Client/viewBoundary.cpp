#include "viewBoundary.h"

ViewBoundary::ViewBoundary(float length, float width, float xPos, float yPos, sf::View *view) : GameObject(false, false, true, false, false, length, width) {
    this->setPosition(xPos, yPos);
    this->setObjectType("view");
    this->viewPtr = view;
}

void ViewBoundary::collision() {
    this->viewPtr->setCenter(this->center);
}

void ViewBoundary::setViewCenter(sf::Vector2f cent) {
    this->center = cent;
}
