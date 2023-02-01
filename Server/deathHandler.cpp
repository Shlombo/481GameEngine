#include "deathHandler.h"

DeathHandler::DeathHandler(int num) {
    this->id = num;
}

void DeathHandler::handleEvent(Event e) {
    std::cout << "Death Handler set position" << std::endl;
    e.player->setPosition(e.player->spawnpoint);
}