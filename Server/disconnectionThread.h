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

class DisconnectionThread {
public:
    std::mutex *_mutex;
    std::condition_variable *_condition_variable;
    std::list<GameObject *> *list;

    DisconnectionThread(std::mutex *_mutex, std::condition_variable *_condition_variable, std::list<GameObject *> *listPtr);
    // bool isBusy();
    void run();

};