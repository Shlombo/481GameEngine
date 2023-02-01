#include "movingPlatform.h"

MovingPlatform::MovingPlatform(std::string type, float length, float width, float xPos, float yPos, float boundUL, float boundDR, int color) : GameObject(false, true, true, true, true, length, width) {
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
    this->boundUL = boundUL;
    this->boundDR = boundDR;
    if(this->getType() == "vertical") {
        this->velocityY = 1;
    } else {
        this->velocityX = 1;
    }
    
}

MovingPlatform::MovingPlatform(std::string type, float length, float width, float xPos, float yPos, float boundUL, float boundDR, std::string textureFile) : GameObject(false, true, true, true, true, length, width) {
    this->type = type;
    setPosition(xPos, yPos);
    if(!this->tex.loadFromFile(textureFile)) {
        setFillColor(sf::Color::Blue);
    } else {
        setTexture(&this->tex);
    }
    this->boundUL = boundUL;
    this->boundDR = boundDR;
    if(this->getType() == "vertical") {
        this->velocityY = 1;
    } else {
        this->velocityX = 1;
    }

}


void MovingPlatform::movement() {
    
    this->updateTime();
    if(this->getType() == "vertical") {
        if(this->getPosition().y < this->boundUL ) {
            this->velocityY = 1;
        } else if(this->getPosition().y > this->boundDR ) {
            this->velocityY = -1;
        }
        // std::cout << "MOVEUP" << "\n";
        this->move(0.f, (this->velocityY * this->deltaT) / 10);
    } else if(this->getType() == "horizontal") {
        if(this->getPosition().x < this->boundUL ) {
            this->velocityX = 1;
        } else if(this->getPosition().x > this->boundDR ) {
            this->velocityX = -1;
        }
        // std::cout << "MOVESIDE" << "\n";
        this->move(((this->velocityX * this->deltaT) / 10), 0.f);
    }

}

void MovingPlatform::render(sf::RenderWindow *window) {
    if(window->hasFocus()) {
        window->setActive(true);
        window->draw(*this);
    }
    window->setActive(false);
}

void MovingPlatform::setTimeline(GameTime *timelineObj) {
    this->timeline = timelineObj;
    this->current_game_time = this->timeline->getTime();
    this->last_game_time = this->current_game_time;
}

void MovingPlatform::updateTime() {
    this->current_game_time = this->timeline->getTime();
    this->deltaT = this->current_game_time - this->last_game_time;
    this->last_game_time = this->current_game_time;
}

std::string MovingPlatform::getType() {
    return this->type;
}