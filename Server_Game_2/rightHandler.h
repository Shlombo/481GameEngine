#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "ScriptManager.h"
#include "eventHandler.h"

class RightHandler : public EventHandler {
public:
    int id;
    ScriptManager *scriptManager;

    RightHandler(int num, ScriptManager *man);

    void handleEvent(Event e);

};