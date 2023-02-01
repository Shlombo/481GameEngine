#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "event.h"

class EventHandler {
public:
    
    virtual void handleEvent(Event e) {};

};