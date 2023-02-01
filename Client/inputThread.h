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

class InputThread {
public:
    int UUID;
    sf::RenderWindow *window;
    std::list<Event> eventList;

    InputThread(int id, sf::RenderWindow *windowObj);
    // bool isBusy();
    void run();

};