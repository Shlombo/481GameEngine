#include "character.h"

Character::Character(float length, float width, float xPos, float yPos, int id) : GameObject(true, true, true, true, true, length, width) {
    setPosition(xPos, yPos);
    setFillColor(sf::Color::Red);
    this->velocityX = 0;
    this->velocityY = 0;
    this->speed = 0.25;
    this->jumpSpeed = .4;
    this->isGrounded = false;
    this->left = false;
    this->right = false;
    this->space = false;
    this->setId(id);
    
}

void Character::input() {
    this->velocityX = 0;
    if(this->isGrounded) {
        this->velocityY = 0;
    } else {
        //Gravi
        this->velocityY += .1;
    }
    
    updateTime();
    if (left) {
        this->velocityX += (-(this->speed) * this->deltaT);
    }
    if (right) {
        this->velocityX += ((this->speed) * this->deltaT);
    } 
    if (space && this->isGrounded) {
        this->velocityY = (-(this->jumpSpeed) * this->deltaT);
    } 
}

void Character::render(sf::RenderWindow *window) {
    if(window->hasFocus()) {
        window->setActive(true);
        window->draw(*this);
    }
    window->setActive(false);
}

void Character::collision(std::list<GameObject *> *worldList) {
    sf::FloatRect playerBounds = this->getGlobalBounds();
    sf::FloatRect nextPos = this->getGlobalBounds();
    this->isGrounded = false;
    //Iterate through game objects
    for (std::list<GameObject *>::iterator it = worldList->begin(); it != worldList->end(); ++it) {
        //Check if object has collision mechanics
        if((*it)->hasCollision) {
            //Check if is of type Spawnpoint, Death zone, or view boundary
            if((*it)->objectType == "spawn") {
                if(playerBounds.intersects((*it)->getGlobalBounds())) {
                    this->setSpawn((*it)->getPosition());
                    std::cout << "SPAWN SET" << "\n";
                }
            } else if((*it)->objectType == "view") {
                if(playerBounds.intersects((*it)->getGlobalBounds())) {
                    (*it)->collision();
                    std::cout << "VIEW SET" << "\n";
                }
            } else if((*it)->objectType == "death"){
                if(playerBounds.intersects((*it)->getGlobalBounds())) {
                    this->setPosition(this->spawnpoint);
                    this->viewPtr->setCenter(sf::Vector2f(400, 300));
                    std::cout << "DEAD" << "\n";
                }
            } else {
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
                        //Bottom Collision
                        if(playerBounds.top < objectBounds.top
                            && playerBounds.top + playerBounds.height < objectBounds.top + objectBounds.height
                            && playerBounds.left < objectBounds.left + objectBounds.width
                            && playerBounds.left + playerBounds.width > objectBounds.left) 
                        {
                            this->velocityY = 0.f;
                            this->isGrounded = true;
                            this->setPosition(playerBounds.left, objectBounds.top - playerBounds.height);
                        } 
                        //Top Collision
                        else if(playerBounds.top > objectBounds.top
                            && playerBounds.top + playerBounds.height > objectBounds.top + objectBounds.height
                            && playerBounds.left < objectBounds.left + objectBounds.width
                            && playerBounds.left + playerBounds.width > objectBounds.left)
                        {
                            this->velocityY = 0.f;
                            this->setPosition(playerBounds.left, objectBounds.top + objectBounds.height);
                        }
                        //Right collision
                        if(playerBounds.left < objectBounds.left
                            && playerBounds.left + playerBounds.width < objectBounds.left + objectBounds.width
                            && playerBounds.top < objectBounds.top + objectBounds.height
                            && playerBounds.top + playerBounds.height < objectBounds.top) 
                        {
                            this->velocityX = 0.f;
                            this->setPosition(objectBounds.left - playerBounds.width, playerBounds.top);
                        } 
                        //Left Collision
                        else if(playerBounds.left > objectBounds.left
                            && playerBounds.left + playerBounds.width > objectBounds.left + objectBounds.width
                            && playerBounds.top < objectBounds.top + objectBounds.height
                            && playerBounds.top + playerBounds.height > objectBounds.top)
                        {
                            this->velocityX = 0.f;
                            this->setPosition(objectBounds.left + objectBounds.width, playerBounds.top);
                        }
                    }

                    //Top collision
        
                    //Right collision

                    //Bottom collision
                }
            }
        }
    }
}

void Character::movement() {
    this->move(this->velocityX, this->velocityY);
}

void Character::setTimeline(GameTime *timelineObj) {
    this->timeline = timelineObj;
    this->current_game_time = this->timeline->getTime();
    this->last_game_time = this->current_game_time;
}

void Character::updateTime() {
    this->current_game_time = this->timeline->getTime();
    this->deltaT = this->current_game_time - this->last_game_time;
    this->last_game_time = this->current_game_time;
}

void Character::setSpawn(sf::Vector2f spawn) {
    this->spawnpoint = spawn;
}

void Character::setViewPtr(sf::View *view) {
    this->viewPtr = view;
}