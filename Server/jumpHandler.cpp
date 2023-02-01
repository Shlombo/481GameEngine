#include "jumpHandler.h"

JumpHandler::JumpHandler(int num) {
    this->id = num;
}

void JumpHandler::handleEvent(Event e) {
    std::cout << "Jump Handler jump" << std::endl;
    // e.player->updateTime();
    e.player->velocityY = (-(e.player->jumpSpeed) * e.player->deltaT);
}