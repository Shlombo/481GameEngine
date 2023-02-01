#include "gameTime.h"

GameTime::GameTime(GameTime *anchor, float tic) {
    this->anchor = anchor;
    this->tic = tic;
    this->start_time = -1;
    this->paused = false;
    this->start_time = getTime();
    this->ellapsed_paused_time = 0;
}

float GameTime::getTime() {
    if(paused) {
        return last_paused_time;
    } else if(this->anchor == nullptr && this->start_time == -1) {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        float cur_time = std::chrono::duration<float, std::milli>(start.time_since_epoch()).count();
        return cur_time;
    } else if(this->start_time == -1){
        return (anchor->getTime());
    } else if(anchor == nullptr){ 
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        float cur_time = std::chrono::duration<float, std::milli>(start.time_since_epoch()).count();
        return (cur_time - this->start_time - this->ellapsed_paused_time) / this->tic;
    } else {
        return ((anchor->getTime() - this->start_time) / this->tic) - this->ellapsed_paused_time;
    }
}

void GameTime::pause() {
    this->last_paused_time = getTime();
    this->paused = true;
}

void GameTime::unpause() {
    this->paused = false;
    this->ellapsed_paused_time = getTime() - this->last_paused_time + this->ellapsed_paused_time;
}

void GameTime::changeTic(float newTick) {
    this->tic = newTick;
}

