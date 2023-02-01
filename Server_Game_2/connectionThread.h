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
    int id;
    bool *start;

    ConnectionThread(std::mutex *_mutex, std::condition_variable *_condition_variable, int id, bool *start);
    // bool isBusy();
    void run();

};