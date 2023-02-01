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
#include "inputThread.h"
#include "gameTime.h"

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
    sf::RenderWindow window(sf::VideoMode(800, 800), "My window", sf::Style::Resize);


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
    
    bool win = false;
    bool lose = false;

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
        if(temp == "win") {
            win = true;
            serverMesStream >> temp;
        } else if(temp == "lose") {
            lose = true;
            serverMesStream >> temp;
        }
        while(temp != "end") {
            found = false;
            int id = stoi(temp);
            serverMesStream >> temp;
            float xPos = stof(temp);
            serverMesStream >> temp;
            float yPos = stof(temp);
            serverMesStream >> temp;
            float xLen = stof(temp);
            serverMesStream >> temp;
            float yLen = stof(temp);
            sf::RectangleShape tempRect(sf::Vector2f(xLen, yLen));
            tempRect.setPosition(xPos, yPos);
            if(id <= 15) {
                tempRect.setFillColor(sf::Color::Green);
            } else if(id == 16) {
                tempRect.setFillColor(sf::Color::Magenta);
            } else if(id == 17) {
                tempRect.setFillColor(sf::Color::Yellow);
            } else if(id == 18) {
                tempRect.setFillColor(sf::Color::Red);
            }
            if(window.hasFocus()) {
                window.setActive(true);
                window.draw(tempRect);
            }
            serverMesStream >> temp;
        }


        //Display screen
        if(window.hasFocus()) {
            window.setActive(true);
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

    //DISCONNECT FROM SERVER {
    // zmq::context_t context3 (1);
    // zmq::socket_t socket2 (context3, zmq::socket_type::req);
    // std::cout << "Disconnecting from server..." << std::endl;
    // socket2.connect ("tcp://localhost:5558");
    // std::cout << "Connected" << std::endl;

    // std::string dis;
    // dis = std::to_string(UUID);
    // socket2.send (zmq::buffer(dis), zmq::send_flags::none);
    // std::cout << "Connection Request Sent" << std::endl;

    // zmq::message_t exit;
    // socket2.recv(exit, zmq::recv_flags::none);
    // } DISCONNECT FROM SERVER

    

    return 0;
}