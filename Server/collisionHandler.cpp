#include "collisionHandler.h"

CollisionHandler::CollisionHandler(int num) {
    this->id = num;
}

void CollisionHandler::handleEvent(Event e) {
    std::cout << "Collision Handler handling collsion" << std::endl;
    //Get its bounds
    sf::FloatRect playerBounds = e.player->getGlobalBounds();
    sf::FloatRect nextPos = e.player->getGlobalBounds();
    sf::FloatRect objectBounds = e.object1->getGlobalBounds();
    //Get what the player's next position would be
    nextPos.left += e.player->velocityX;
    nextPos.top += e.player->velocityY;
    std::cout << "1" << std::endl;
    if(e.object1->UUID == e.player->UUID) {
        //Do nothing, its your own collider
    } else {
        //Check if object collides
        std::cout << "0" << std::endl;
        if(objectBounds.intersects(nextPos)) {
            //CREATE COLLISION EVENT
            //Bottom Collision
            std::cout << "10" << std::endl;
            if(playerBounds.top < objectBounds.top
                && playerBounds.top + playerBounds.height < objectBounds.top + objectBounds.height
                && playerBounds.left < objectBounds.left + objectBounds.width
                && playerBounds.left + playerBounds.width > objectBounds.left) 
            {
                std::cout << "2" << std::endl;
                e.player->velocityY = 0.f;
                e.player->isGrounded = true;
                e.player->setPosition(playerBounds.left, objectBounds.top - playerBounds.height);
            } 
            //Top Collision
            else if(playerBounds.top > objectBounds.top
                && playerBounds.top + playerBounds.height > objectBounds.top + objectBounds.height
                && playerBounds.left < objectBounds.left + objectBounds.width
                && playerBounds.left + playerBounds.width > objectBounds.left)
            {
                std::cout << "3" << std::endl;
                e.player->velocityY = 0.f;
                e.player->setPosition(playerBounds.left, objectBounds.top + objectBounds.height);
            }
            //Right collision
            if(playerBounds.left < objectBounds.left
                && playerBounds.left + playerBounds.width < objectBounds.left + objectBounds.width
                && playerBounds.top < objectBounds.top + objectBounds.height
                && playerBounds.top + playerBounds.height < objectBounds.top) 
            {
                std::cout << "4" << std::endl;
                e.player->velocityX = 0.f;
                e.player->setPosition(objectBounds.left - playerBounds.width, playerBounds.top);
            } 
            //Left Collision
            else if(playerBounds.left > objectBounds.left
                && playerBounds.left + playerBounds.width > objectBounds.left + objectBounds.width
                && playerBounds.top < objectBounds.top + objectBounds.height
                && playerBounds.top + playerBounds.height > objectBounds.top)
            {
                std::cout << "5" << std::endl;
                e.player->velocityX = 0.f;
                e.player->setPosition(objectBounds.left + objectBounds.width, playerBounds.top);
            }
        }
    }
    
}