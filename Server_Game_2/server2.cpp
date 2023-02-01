#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <thread>
#include <chrono>
#include <bits/stdc++.h>
#include <unistd.h>
#include <zmq.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstdio>
#include <v8.h>
#include <libplatform/libplatform.h>
#include "v8helpers.h"
#include "ScriptManager.h"
#include "gameObject.h"
#include "gameTime.h"
#include "character.h"
#include "connectionThread.h"
#include "serverInputThread.h"
#include "event.h"
#include "eventHandler.h"
#include "eventManager.h"
#include "leftHandler.h"
#include "rightHandler.h"
#include "ball.h"
#include "brick.h"
#include "movingPlatform.h"
#include "platform.h"

/**
 * Wrapper function because threads can't take pointers to member functions.
 */
void run_wrapper1(ConnectionThread *fe)
{
    fe->run();
}

void run_wrapper2(ServerInputThread *fe)
{
    fe->run();
}


int main()
{   
    //Networking
    zmq::context_t context (1);
    zmq::socket_t publisher (context, zmq::socket_type::pub);
    publisher.bind("tcp://*:5557");

    bool win;
    bool lose;
    bool start;

    int UUIDCount = 0;
    //Initialize Timelines
    GameTime globalTime(nullptr, 1.f);
    GameTime game(&globalTime, 1.f);
    float current_time = globalTime.getTime();
    float delta_time;
    float last_time = current_time;
    float target_delta_time = 60;
    bool isPaused = false;

    //Create window
    sf::RenderWindow window(sf::VideoMode(800, 800), "My window", sf::Style::Resize);
    

    //Create Manager and event handlers
    EventManager manager(1);
    

    //make brick objects ROW 1
    Brick brick1("static", 99, 49, 0, 0, 1);
    brick1.setId(UUIDCount++);
    Brick brick2("static", 99, 49, 100, 0, 1);
    brick2.setId(UUIDCount++);
    Brick brick3("static", 99, 49, 200, 0, 1);
    brick3.setId(UUIDCount++);
    Brick brick4("static", 99, 49, 300, 0, 1);
    brick4.setId(UUIDCount++);
    Brick brick5("static", 99, 49, 400, 0, 1);
    brick5.setId(UUIDCount++);
    Brick brick6("static", 99, 49, 500, 0, 1);
    brick6.setId(UUIDCount++);
    Brick brick7("static", 99, 49, 600, 0, 1);
    brick7.setId(UUIDCount++);
    Brick brick8("static", 99, 49, 700, 0, 1);
    brick8.setId(UUIDCount++);
    
    //ROW 2
    Brick brick9("static", 99, 49, 0, 50, 1);
    brick9.setId(UUIDCount++);
    Brick brick10("static", 99, 49, 100, 50, 1);
    brick10.setId(UUIDCount++);
    Brick brick11("static", 99, 49, 200, 50, 1);
    brick11.setId(UUIDCount++);
    Brick brick12("static", 99, 49, 300, 50, 1);
    brick12.setId(UUIDCount++);
    Brick brick13("static", 99, 49, 400, 50, 1);
    brick13.setId(UUIDCount++);
    Brick brick14("static", 99, 49, 500, 50, 1);
    brick14.setId(UUIDCount++);
    Brick brick15("static", 99, 49, 600, 50, 1);
    brick15.setId(UUIDCount++);
    Brick brick16("static", 99, 49, 700, 50, 1);
    brick16.setId(UUIDCount++);

    MovingPlatform mPlatformScripted("scripted", 200, 25, 0, 300, 0, 600, 3);
    mPlatformScripted.setTimeline(&game);
    mPlatformScripted.setId(UUIDCount++);


    Ball ball("ball", 25, 25, 400, 400, 0, 775, 2, &win, &lose);
    ball.setTimeline(&game);
    ball.setId(UUIDCount++);

    //V8 Code ------------------------------------------------------------------------------
    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.release());
    v8::V8::InitializeICU();
    v8::V8::Initialize();
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate *isolate = v8::Isolate::New(create_params);


    { // anonymous scope for managing handle scope

        v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
        v8::HandleScope handle_scope(isolate);

        // Best practice to isntall all global functions in the context ahead of time.
        v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
        // Bind the global 'print' function to the C++ Print callback.
        global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));

        // //Bind movement function of moving platform
        // global->Set(isolate, "movement", v8::FunctionTemplate::New(isolate, MovingPlatform::scriptMovement));

        //Create default context and script manager
        v8::Local<v8::Context> default_context =  v8::Context::New(isolate, NULL, global);
		v8::Context::Scope default_context_scope(default_context); // enter the context
        ScriptManager *sm = new ScriptManager(isolate, default_context);
        
        Character paddle(200, 50, 300, 700, UUIDCount++, &game, &manager, sm);
        paddle.setTimeline(&game);

        RightHandler rightHandler(1, sm);
        manager.registerType("right", &rightHandler);
        LeftHandler leftHandler(1, sm);
        manager.registerType("left", &leftHandler);

        sm->addScript("raise_left", "scripts/raise_left.js");
        sm->addScript("raise_right", "scripts/raise_right.js");
        sm->addScript("move_left", "scripts/move_left.js");
        sm->addScript("move_right", "scripts/move_right.js");
        sm->addScript("move_platform", "scripts/move_platform.js");

        //Expose objects to V8
        mPlatformScripted.exposeToV8(isolate, default_context);
        paddle.exposeToV8(isolate, default_context);
        manager.exposeToV8(isolate, default_context);

        bool reload = false;


        //--------------------------------------------------------------------------------

        //Put all objects into list of objects.
        std::list<GameObject *> worldList;
        worldList.push_back(&ball);
        worldList.push_back(&paddle);
        worldList.push_back(&mPlatformScripted);
        worldList.push_back(&brick1);
        worldList.push_back(&brick2);
        worldList.push_back(&brick3);
        worldList.push_back(&brick4);
        worldList.push_back(&brick5);
        worldList.push_back(&brick6);
        worldList.push_back(&brick7);
        worldList.push_back(&brick8);
        worldList.push_back(&brick9);
        worldList.push_back(&brick10);
        worldList.push_back(&brick11);
        worldList.push_back(&brick12);
        worldList.push_back(&brick13);
        worldList.push_back(&brick14);
        worldList.push_back(&brick15);
        worldList.push_back(&brick16);
        
        
        // worldList.push_back(&s1);
        // worldList.push_back(&s2);
        
        std::mutex m1;
        std::condition_variable cv1;
        std::mutex m2;
        std::condition_variable cv2;
        std::mutex m3;

        ConnectionThread t1(&m1, &cv1, paddle.UUID, &start);
        ServerInputThread t2(&m3, &worldList);
        std::thread first(run_wrapper1, &t1);
        std::thread second(run_wrapper2, &t2);
        
        
        // //TO DELAY START OF GAME
        // sleep(10);

        while (window.isOpen())
        {
            // std::cout << "Top" << std::endl;
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

    
            if(window.hasFocus()) {
                window.setActive(true);
                window.clear(sf::Color::Black);
            }
            window.setActive(false);


            //Framerate Governor
            current_time = globalTime.getTime();
            delta_time = current_time - last_time;
            float diff_time = target_delta_time - (fmod(delta_time, target_delta_time));
            if(diff_time > 0) {
                sleep(diff_time / 100);
            }
            last_time = current_time;

            if(start) {
                ball.velocityX = 1;
                ball.velocityY = 1;
                start = false;
            }

            //Handle object inputs
            // std::cout << "Check Input" << std::endl;
            std::unique_lock<std::mutex> lock(m1);
            std::unique_lock<std::mutex> lock2(m2);
            for (std::list<GameObject *>::iterator it = worldList.begin(); it != worldList.end(); ++it) {
                // std::cout << "for" << std::endl;
                if((*it)->hasInput) {
                    // std::cout << "if" << std::endl;
                    (*it)->input();
                    // std::cout << "endif" << std::endl;
                }
            }

            {
                std::unique_lock<std::mutex> lock3(m3);
                manager.handleEvents(game.getTime());
            }
            
            //Handle object collisions
            // std::cout << "Check Collision" << std::endl;
            for (std::list<GameObject *>::iterator it = worldList.begin(); it != worldList.end(); ++it) {
                if((*it)->hasCollision) {
                    (*it)->collision(&worldList);
                }
            }

            {
                std::unique_lock<std::mutex> lock3(m3);
                manager.handleEvents(game.getTime());
            }
            
            //Scripted platform movement
            sm->runOne("move_platform", reload);

            // std::cout << mPlatformScripted.scriptX << std::endl;

            //Handle object movement
            // std::cout << "Check Movement" << std::endl;
            for (std::list<GameObject *>::iterator it = worldList.begin(); it != worldList.end(); ++it) {
                if((*it)->hasMovement) {
                    (*it)->movement();
                }
            }

            

            //Handle previous events
            // std::cout << "Handle Events" << std::endl;
            {
                std::unique_lock<std::mutex> lock3(m3);
                manager.handleEvents(game.getTime());
            }
            //Draw objects to screen
            std::cout << "Render" << std::endl;
            for (std::list<GameObject *>::iterator it = worldList.begin(); it != worldList.end(); ++it) {
                if((*it)->hasRender) {
                    std::cout << "Render1" << std::endl;
                    (*it)->render(&window);
                }
            }

            //Send object positions and sizes to client
            std::cout << "Position" << std::endl;
            std::string s;
            if(win) {
                s += "win ";
            } else if(lose) {
                s += "lose ";
            }
        
            for (std::list<GameObject *>::iterator it = worldList.begin(); it != worldList.end(); ++it) {
                if((*it)->hasRender) {
                    s += std::to_string((*it)->UUID) + " " + std::to_string((*it)->getPosition().x) + " " + std::to_string((*it)->getPosition().y) + " " 
                    + std::to_string((*it)->getSize().x) + " " + std::to_string((*it)->getSize().y) + " ";
                }
            }
            s += "end";
            // std::cout << s << std::endl;
            publisher.send(zmq::buffer(s), zmq::send_flags::none);



            //Display screen
            std::cout << "Display" << std::endl;
            if(window.hasFocus()) {
                window.setActive(true);
                // window.setView(view);
                window.display();
            }
            window.setActive(false);

            if(win) {
                if(window.hasFocus()) {
                    window.setActive(true);
                    window.clear(sf::Color::Green);
                    window.display();
                }
                window.setActive(false);
                sleep(2);
                break;
            } else if(lose) {
                if(window.hasFocus()) {
                    window.setActive(true);
                    window.clear(sf::Color::Red);
                    window.display();
                }
                window.setActive(false);
                sleep(2);
                break;
            }

        }
    }
    
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();

    return 0;
}