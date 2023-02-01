#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "eventHandler.h"

class DeathHandler : public EventHandler {
public:
    int id;

    DeathHandler(int num);

    void handleEvent(Event e);

};