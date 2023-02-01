#include "rightHandler.h"

RightHandler::RightHandler(int num, ScriptManager *man) {
    this->id = num;
    this->scriptManager = man;
}

void RightHandler::handleEvent(Event e) {
    std::cout << "Right Handler right" << std::endl;
    this->scriptManager->runOne("move_right", false);
}