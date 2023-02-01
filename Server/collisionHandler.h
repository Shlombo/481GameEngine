#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "eventHandler.h"

class CollisionHandler : public EventHandler {
public:
    int id;

    CollisionHandler(int num);

    void handleEvent(Event e);

};