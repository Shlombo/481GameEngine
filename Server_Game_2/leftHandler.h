#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "ScriptManager.h"
#include "eventHandler.h"

class LeftHandler : public EventHandler {
public:
    int id;
    ScriptManager *scriptManager;

    LeftHandler(int num, ScriptManager *man);

    void handleEvent(Event e);

};