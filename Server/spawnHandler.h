#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "eventHandler.h"

class SpawnHandler : public EventHandler {
public:
    int id;

    SpawnHandler(int num);

    void handleEvent(Event e);

};