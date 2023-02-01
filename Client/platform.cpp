#include "platform.h"

Platform::Platform(std::string type, float length, float width, float xPos, float yPos, int color) : GameObject(false, true, true, false, false, length, width) {
    this->type = type;
    setPosition(xPos, yPos);
    if(color == 0) {
        setFillColor(sf::Color::Blue);
    } else if(color == 1) {
        setFillColor(sf::Color::Green);
    } else if(color == 2) {
        setFillColor(sf::Color::Yellow);
    } else {
        setFillColor(sf::Color::Magenta);
    }
    
    
}

Platform::Platform(std::string type, float length, float width, float xPos, float yPos, std::string textureFile) : GameObject(false, true, true, false, false, length, width) {
    this->type = type;
    setPosition(xPos, yPos);
    if(!this->tex.loadFromFile(textureFile)) {
        setFillColor(sf::Color::Blue);
    } else {
        setTexture(&this->tex);
    }
}


std::string Platform::getType() {
    return this->type;
}

void Platform::render(sf::RenderWindow *window) {
    if(window->hasFocus()) {
        window->setActive(true);
        window->draw(*this);
    }
    window->setActive(false);
}

