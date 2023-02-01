#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "event.h"

class CompareEvent {
public:
    bool operator() (Event a, Event b)
    {
        return (a.timestamp < b.timestamp);
    }
};