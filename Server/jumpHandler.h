#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "eventHandler.h"

class JumpHandler : public EventHandler {
public:
    int id;

    JumpHandler(int num);

    void handleEvent(Event e);

};