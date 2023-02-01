#include "event.h"

Event::Event(std::string typeString) {
    this->type = typeString;
}

Event::Event(std::string typeString, float time, Character *obj) {
    this->type = typeString;
    this->timestamp = time;
    this->player = obj;
}

Event::Event(std::string typeString, float time, GameObject *obj1) {
    std::cout << "Create event1" << std::endl;
    this->type = typeString;
    std::cout << "Create event2" << std::endl;
    this->timestamp = time;
    std::cout << "Create event3" << std::endl;
    this->object1 = obj1;
}

Event::Event(std::string typeString, float time, Character *obj, GameObject *obj1) {
    this->type = typeString;
    this->timestamp = time;
    this->player = obj;
    this->object1 = obj1;
}