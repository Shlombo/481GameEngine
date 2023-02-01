#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

class GameTime {
private:
	std::mutex m; //thread safe
	float start_time; //time of an underlying measure of the time of creation of  timeline
	float tic; // # of time units of the underlying measure of time that correspond to one step of this timeline
	bool paused;
	float last_paused_time;
	float ellapsed_paused_time;
	GameTime *anchor;
public:
    GameTime(GameTime *anchor, float tic);
	float getTime();
	void pause();
	void unpause();
	void changeTic(float newTick);
};