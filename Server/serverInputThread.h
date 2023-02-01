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
#include "event.h"
#include "eventManager.h"

class ServerInputThread {
public:
    std::mutex *_mutex;
    std::list<GameObject *> *list;
    EventManager *manager;

    ServerInputThread(std::mutex *_mutex, std::list<GameObject *> *listPtr, EventManager *man);
    // bool isBusy();
    void run();

};