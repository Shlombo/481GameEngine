#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <thread>
#include <chrono>
#include <bits/stdc++.h>
#include <unistd.h>
#include <zmq.hpp>
#include <iostream>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gameObject.h"
#include "gameTime.h"
#include "character.h"
#include "platform.h"
#include "movingPlatform.h"
#include "deathBarrier.h"
#include "viewBoundary.h"
#include "inputThread.h"
#include "event.h"

void run_wrapper1(InputThread *fe)
{
    fe->run();
}

int main()
{   
    XInitThreads();
    //For framerate governing
    int UUIDCount = 0;
    GameTime global(nullptr, 1.f);
    float current_time = global.getTime();
    float delta_time;
    float last_time = current_time;
    float target_delta_time = 30;

    //Create window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window", sf::Style::Resize);
    
    //Create view
    sf::View view;
    view.reset(sf::FloatRect(0,0, 800, 600));
    view.setViewport(sf::FloatRect(0,0, 1.0f, 1.0f));

    //CONNECT TO SERVER {
    zmq::context_t context (1);
    zmq::socket_t socket (context, zmq::socket_type::req);
    std::cout << "Connecting to server..." << std::endl;
    socket.connect ("tcp://localhost:5555");
    std::cout << "Connected" << std::endl;

    std::string req;
    req = std::to_string(0);
    socket.send (zmq::buffer(req), zmq::send_flags::none);
    std::cout << "Connection Request Sent" << std::endl;

    zmq::message_t status;
    socket.recv(status, zmq::recv_flags::none);
    std::cout << "Connection Status Recieved" << std::endl;
    std::string idString;
    std::stringstream idStream;
    idString.assign(static_cast<char *>(status.data()), status.size());
    idStream << idString;
    std::string temp;
    idStream >> temp;
    int UUID = stoi(temp);
    std::cout << "UUID set to " << UUID << std::endl;
    // } CONNECT TO SERVER

    //make view boundaries
    ViewBoundary s1(100, 2000, 800, -200, &view);
    s1.setViewCenter(sf::Vector2f(400, 300));
    // s1.setId(UUIDCount++);
    ViewBoundary s2(100, 2000, 800, -200, &view);
    s2.setViewCenter(sf::Vector2f(1200, 300));
    // s2.setId(UUIDCount++);

    //make spawnpoint
    Spawnpoint spawn(100, 325);
    spawn.setId(UUIDCount++);

    //make death zone
    DeathBarrier death(3000, 50, 0, 750);
    death.setId(UUIDCount++);

    //make player object, set its timeline, and set its spawn
    Character player(20, 50, 100, 325, UUID);

    

    //make platform objects
    Platform platform("static", 150, 25, 25, 400, "texture.jpg");
    platform.setId(UUIDCount++);
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
    mPlatform.setId(UUIDCount++);
    mPlatform2.setId(UUIDCount++);
    mPlatformScripted.setId(UUIDCount++);


    

    //Put all objects into list of objects.
    std::list<GameObject *> worldList;
    // worldList.push_back(&player);
    worldList.push_back(&spawn);
    worldList.push_back(&death);
    worldList.push_back(&platform);
    worldList.push_back(&platform2);
    worldList.push_back(&platform3);
    worldList.push_back(&platform4);
    worldList.push_back(&mPlatform);
    worldList.push_back(&mPlatform2);
    worldList.push_back(&mPlatformScripted);

    //Subscribe to object updates
    zmq::context_t context2 (1);
    zmq::socket_t subscriber (context2, zmq::socket_type::sub);
    int conflate = 1;
    subscriber.setsockopt(ZMQ_CONFLATE, &conflate, sizeof(conflate));
    subscriber.connect("tcp://localhost:5557");
    
    
    char none[0];
    subscriber.setsockopt(ZMQ_SUBSCRIBE, none);

    InputThread t1(UUID, &window);
    std::thread first(run_wrapper1, &t1);
    
    while (window.isOpen())
    {
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
        current_time = global.getTime();
        delta_time = current_time - last_time;
        float diff_time = target_delta_time - (fmod(delta_time, target_delta_time));
	    if(diff_time > 0) {
	    	sleep(diff_time / 100);
	    }
	    last_time = current_time;

        std::cout << "start reading server" << std::endl;
        std::string temp;
        std::string serverMes;
        zmq::message_t serverMessage;
        std::stringstream serverMesStream;
        subscriber.recv(serverMessage, zmq::recv_flags::none);
        serverMes.assign(static_cast<char *>(serverMessage.data()), serverMessage.size());
        std::cout << serverMes << "\n";
        serverMesStream << serverMes;

        serverMesStream >> temp;
        std::cout << temp << std::endl;
        bool found = false;
        while(temp != "end") {
            found = false;
            int id = stoi(temp);
            serverMesStream >> temp;
            float xPos = stof(temp);
            serverMesStream >> temp;
            float yPos = stof(temp);
            for (std::list<GameObject *>::iterator it = worldList.begin(); it != worldList.end(); ++it) {
                if((*it)->UUID == id) {
                    (*it)->setPosition(sf::Vector2f(xPos, yPos));
                    found = true;
                }
            }
            if(id == UUID) {
                if(xPos <= s1.getPosition().x) {
                    view.setCenter(s1.center);
                } else {
                    view.setCenter(s2.center);
                }
            }
            if(!found) {
                sf::RectangleShape playerTemp(sf::Vector2f(20, 50));
                playerTemp.setPosition(xPos, yPos);
                if(id % 4 == 0) {
                    playerTemp.setFillColor(sf::Color::Red);
                } else if(id % 4 == 1) {
                    playerTemp.setFillColor(sf::Color::Blue);
                } else if(id % 4 == 2) {
                    playerTemp.setFillColor(sf::Color::Magenta);
                } else {
                    playerTemp.setFillColor(sf::Color::Yellow);
                }
                 if(window.hasFocus()) {
                    window.setActive(true);
                    window.draw(playerTemp);
                }
                window.setActive(false);
            }
            serverMesStream >> temp;
        }

        std::cout << "Render" << std::endl;
        for (std::list<GameObject *>::iterator it = worldList.begin(); it != worldList.end(); ++it) {
            if((*it)->hasRender) {
                (*it)->render(&window);
            }
        }


        //Display screen
        if(window.hasFocus()) {
            window.setActive(true);
            window.setView(view);
            window.display();
        }
        window.setActive(false);

    }

    //DISCONNECT FROM SERVER {
    zmq::context_t context3 (1);
    zmq::socket_t socket2 (context3, zmq::socket_type::req);
    std::cout << "Disconnecting from server..." << std::endl;
    socket2.connect ("tcp://localhost:5558");
    std::cout << "Connected" << std::endl;

    std::string dis;
    dis = std::to_string(UUID);
    socket2.send (zmq::buffer(dis), zmq::send_flags::none);
    std::cout << "Connection Request Sent" << std::endl;

    zmq::message_t exit;
    socket2.recv(exit, zmq::recv_flags::none);
    // } DISCONNECT FROM SERVER

    

    return 0;
}