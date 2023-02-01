#include "leftHandler.h"

LeftHandler::LeftHandler(int num, ScriptManager *man) {
    this->id = num;
    this->scriptManager = man;
}

void LeftHandler::handleEvent(Event e) {
    std::cout << "Left Handler left" << std::endl;
    this->scriptManager->runOne("move_left", false);
}