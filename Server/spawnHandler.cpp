#include "spawnHandler.h"

SpawnHandler::SpawnHandler(int num) {
    this->id = num;
}

void SpawnHandler::handleEvent(Event e) {
    std::cout << "Spawn Handler set spawn" << std::endl;
    e.player->setSpawn(e.object1->getPosition());
    // std::cout << "SPAWN SET" << std::endl;
}