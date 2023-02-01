#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <zmq.hpp>
#include <unistd.h>
#include "character.h"
#include "gameTime.h"
#include "eventManager.h"
#include "ScriptManager.h"

class ConnectionThread {
public:
    std::mutex *_mutex;
    std::condition_variable *_condition_variable;
    std::list<GameObject *> *list;
    int *idPtr;
    GameTime *timeline;
    sf::Vector2f spawnpoint;
    EventManager *manager;

    ScriptManager *scriptManager;

    ConnectionThread(std::mutex *_mutex, std::condition_variable *_condition_variable, std::list<GameObject *> *listPtr, int *id, GameTime *timelineObj, sf::Vector2f spawn, EventManager *man, ScriptManager *scriptMan);
    // bool isBusy();
    void run();

};