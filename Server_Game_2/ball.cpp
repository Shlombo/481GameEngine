#include "ball.h"

Ball::Ball(std::string type, float length, float width, float xPos, float yPos, float boundUL, float boundDR, int color, bool *win, bool *lose) : GameObject(false, true, true, true, true, length, width) {
    this->type = type;
    setPosition(xPos, yPos);
    if(color == 0) {
        setFillColor(sf::Color::Blue);
    } else if(color == 1) {
        setFillColor(sf::Color::Green);
    } else if(color == 2) {
        setFillColor(sf::Color::Yellow);
    } else {
        setFillColor(sf::Color::White);
    }
    this->boundUL = boundUL;
    this->boundDR = boundDR;
    this->velocityX = 0;
    this->velocityY = 0;
    this->speed = .2;
    this->win = win;
    this->lose = lose;
    this->score = 0;
    
}

Ball::Ball(std::string type, float length, float width, float xPos, float yPos, float boundUL, float boundDR, std::string textureFile) : GameObject(false, true, true, true, true, length, width) {
    this->type = type;
    setPosition(xPos, yPos);
    if(!this->tex.loadFromFile(textureFile)) {
        setFillColor(sf::Color::Blue);
    } else {
        setTexture(&this->tex);
    }
    this->boundUL = boundUL;
    this->boundDR = boundDR;
    this->velocityX = 1.f;
    this->velocityY = 1.f;
    this->speed = .2;

}


void Ball::movement() {
    std::cout << "moving ball" << std::endl;
    this->updateTime();
    if(this->getPosition().x > this->boundDR ) {
        this->velocityX = -1.f;
    }
    if(this->getPosition().x < this->boundUL) {
        this->velocityX = 1.f;
    }
    if(this->getPosition().y > this->boundDR ) {
        this->velocityY = -1.f;
        *(this->lose) = true;
    }
    if(this->getPosition().y < this->boundUL) {
        this->velocityY = 1.f;
    }
    std::cout << "moving ball " << this->getPosition().x << std::endl;
    this->move((this->velocityX * this->deltaT * this->speed), (this->velocityY * this->deltaT * this->speed));
    // this->move(this->velocityX, this->velocityY);

}

void Ball::render(sf::RenderWindow *window) {
    if(window->hasFocus()) {
        window->setActive(true);
        window->draw(*this);
    }
    window->setActive(false);
}

void Ball::collision(std::list<GameObject *> *worldList) {
    sf::FloatRect ballBounds = this->getGlobalBounds();
    sf::FloatRect nextPos = this->getGlobalBounds();
    //Iterate through game objects
    for (std::list<GameObject *>::iterator it = worldList->begin(); it != worldList->end(); ++it) {
        //Check if object has collision mechanics
        if((*it)->hasCollision) {
            //Get its bounds
            sf::FloatRect objectBounds = (*it)->getGlobalBounds();
            //Get what the player's next position would be
            nextPos.left += this->velocityX;
            nextPos.top += this->velocityY;

            if((*it)->UUID == this->UUID) {
                //Do nothing, its your own collider
            } else {
                //Check if object collides
                if(objectBounds.intersects(nextPos)) {
                    //CREATE COLLISION EVENT
                    //Bottom Collision
                    if(ballBounds.top < objectBounds.top
                        && ballBounds.top + ballBounds.height < objectBounds.top + objectBounds.height
                        && ballBounds.left < objectBounds.left + objectBounds.width
                        && ballBounds.left + ballBounds.width > objectBounds.left) 
                    {
                        this->velocityY = -1.f;
                        // this->setPosition(ballBounds.left, objectBounds.top - ballBounds.height);
                    } 
                    //Top Collision
                    else if(ballBounds.top > objectBounds.top
                        && ballBounds.top + ballBounds.height > objectBounds.top + objectBounds.height
                        && ballBounds.left < objectBounds.left + objectBounds.width
                        && ballBounds.left + ballBounds.width > objectBounds.left)
                    {
                        this->velocityY = 1.f;
                        // this->setPosition(ballBounds.left, objectBounds.top + objectBounds.height);
                    }
                    //Right collision
                    if(ballBounds.left < objectBounds.left
                        && ballBounds.left + ballBounds.width < objectBounds.left + objectBounds.width
                        && ballBounds.top < objectBounds.top + objectBounds.height
                        && ballBounds.top + ballBounds.height < objectBounds.top) 
                    {
                        this->velocityX = -1.f;
                        // this->setPosition(objectBounds.left - ballBounds.width, ballBounds.top);
                    } 
                    //Left Collision
                    else if(ballBounds.left > objectBounds.left
                        && ballBounds.left + ballBounds.width > objectBounds.left + objectBounds.width
                        && ballBounds.top < objectBounds.top + objectBounds.height
                        && ballBounds.top + ballBounds.height > objectBounds.top)
                    {
                        this->velocityX = 1.f;
                        // this->setPosition(objectBounds.left + objectBounds.width, ballBounds.top);
                    }
                    if((*it)->objectType == "static") {
                        it = worldList->erase(it);
                        this->score += 1;
                        if(this->score >= 16) {
                            *(this->win) = true;
                        }
                    }
                }

            }
            
        }
    }
}

void Ball::setTimeline(GameTime *timelineObj) {
    this->timeline = timelineObj;
    this->current_game_time = this->timeline->getTime();
    this->last_game_time = this->current_game_time;
}

void Ball::updateTime() {
    this->current_game_time = this->timeline->getTime();
    this->deltaT = this->current_game_time - this->last_game_time;
    this->last_game_time = this->current_game_time;
}

std::string Ball::getType() {
    return this->type;
}