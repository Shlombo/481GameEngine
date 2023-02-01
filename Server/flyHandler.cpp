#include "flyHandler.h"

FlyHandler::FlyHandler(int num, ScriptManager *man) {
    this->id = num;
    this->scriptManager = man;
}

void FlyHandler::handleEvent(Event e) {
    std::cout << "Fly Handler fly" << std::endl;
    this->scriptManager->runOne("handle_fly_event", false);
}