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
#include "platform.h"
#include "movingPlatform.h"
#include "deathBarrier.h"
#include "viewBoundary.h"
#include "connectionThread.h"
#include "serverInputThread.h"
#include "disconnectionThread.h"
#include "spawnpoint.h"
#include "event.h"
#include "eventHandler.h"
#include "eventManager.h"
#include "deathHandler.h"
#include "spawnHandler.h"
#include "collisionHandler.h"
#include "jumpHandler.h"
#include "flyHandler.h"


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

void run_wrapper3(DisconnectionThread *fe)
{
    fe->run();
}

int main()
{   
    //Networking
    zmq::context_t context (1);
    zmq::socket_t publisher (context, zmq::socket_type::pub);
    publisher.bind("tcp://*:5557");

    int UUIDCount = 0;
    //Initialize Timelines
    GameTime globalTime(nullptr, 1.f);
    GameTime game(&globalTime, 1.f);
    float current_time = globalTime.getTime();
    float delta_time;
    float last_time = current_time;
    float target_delta_time = 30;
    bool isPaused = false;

    //Create window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window", sf::Style::Resize);
    
    //Create view
    sf::View view;
    view.reset(sf::FloatRect(0,0, 800, 600));
    view.setViewport(sf::FloatRect(0,0, 1.0f, 1.0f));

    //Create Manager and event handlers
    EventManager manager(1);
    DeathHandler deathHandler(1);
    SpawnHandler spawnHandler(1);
    CollisionHandler collisionHandler(1);
    JumpHandler jumpHandler(1);
    
    manager.registerType("death", &deathHandler);
    manager.registerType("spawn", &spawnHandler);
    manager.registerType("collision", &collisionHandler);
    manager.registerType("jump", &jumpHandler);
    


    //make view boundaries
    // ViewBoundary s1(100, 2000, 700, -200, &view);
    // s1.setViewCenter(sf::Vector2f(400, 300));
    // s1.setId(UUIDCount++);
    // ViewBoundary s2(100, 2000, 800, -200, &view);
    // s2.setViewCenter(sf::Vector2f(1200, 300));
    // s2.setId(UUIDCount++);

    //make spawnpoint
    Spawnpoint spawn(100, 325);
    spawn.setId(UUIDCount++);

    //make death zone
    DeathBarrier death(3000, 50, 0, 750);
    death.setId(UUIDCount++);

    //make player object, set its timeline, and set its spawn
    // Character player(20, 50, 100, 325);
    // player.setTimeline(&game);
    // player.setSpawn(spawn.getPosition());
    // player.setViewPtr(&view);
    

    //make platform objects
    Platform platform1("static", 150, 25, 25, 400, "texture.jpg");
    platform1.setId(UUIDCount++);
    Platform platform2("static", 75, 25, 600, 400, 0);
    platform2.setId(UUIDCount++);
    Platform platform3("static", 75, 25, 700, 300, 1);
    platform3.setId(UUIDCount++);
    Platform platform4("static", 300, 25, 1000, 500, 2);
    platform4.setId(UUIDCount++);

    //make moving platform objects and set timelines
    MovingPlatform mPlatform("horizontal", 75, 25, 200, 400, 200, 400, 0);
    MovingPlatform mPlatform2("vertical", 75, 25, 500, 400, 100, 400, 1);
    MovingPlatform mPlatformScripted("scripted", 150, 25, 200, 25, 0, 400, 3);
    mPlatform.setTimeline(&game);
    mPlatform2.setTimeline(&game);
    mPlatformScripted.setTimeline(&game);
    // mPlatformScripted.setHasMovement(true);
    mPlatform.setId(UUIDCount++);
    mPlatform2.setId(UUIDCount++);
    mPlatformScripted.setId(UUIDCount++);

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

        FlyHandler flyHandler(1, sm);
        manager.registerType("fly", &flyHandler);


        sm->addScript("move_platform", "scripts/move_platform.js");
        sm->addScript("raise_fly_event", "scripts/raise_fly_event.js");
        sm->addScript("handle_fly_event", "scripts/handle_fly_event.js");

        //Expose objects to V8
        mPlatformScripted.exposeToV8(isolate, default_context);
        manager.exposeToV8(isolate, default_context);

        bool reload = false;


        //--------------------------------------------------------------------------------

        //Put all objects into list of objects.
        std::list<GameObject *> worldList;
        // worldList.push_back(&player);
        worldList.push_back(&spawn);
        worldList.push_back(&death);
        worldList.push_back(&platform1);
        worldList.push_back(&platform2);
        worldList.push_back(&platform3);
        worldList.push_back(&platform4);
        worldList.push_back(&mPlatform);
        worldList.push_back(&mPlatform2);
        worldList.push_back(&mPlatformScripted);
        
        // worldList.push_back(&s1);
        // worldList.push_back(&s2);
        
        std::mutex m1;
        std::condition_variable cv1;
        std::mutex m2;
        std::condition_variable cv2;
        std::mutex m3;

        ConnectionThread t1(&m1, &cv1, &worldList, &UUIDCount, &game, spawn.getPosition(), &manager, sm);
        ServerInputThread t2(&m3, &worldList, &manager);
        DisconnectionThread t3(&m2, &cv2, &worldList);
        std::thread first(run_wrapper1, &t1);
        std::thread second(run_wrapper2, &t2);
        std::thread third(run_wrapper3, &t3);
        
        
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

            std::cout << mPlatformScripted.scriptX << std::endl;

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

            //Send object positions to clients
            std::cout << "Position" << std::endl;
            std::string s;
            for (std::list<GameObject *>::iterator it = worldList.begin(); it != worldList.end(); ++it) {
                if((*it)->hasMovement) {
                    s += std::to_string((*it)->UUID) + " " + std::to_string((*it)->getPosition().x) + " " + std::to_string((*it)->getPosition().y) + " ";
                }
            }
            s += "end";
            // std::cout << s << std::endl;
            publisher.send(zmq::buffer(s), zmq::send_flags::none);



            //Display screen
            std::cout << "Display" << std::endl;
            if(window.hasFocus()) {
                window.setActive(true);
                window.setView(view);
                window.display();
            }
            window.setActive(false);

        }
    }
    
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();

    return 0;
}