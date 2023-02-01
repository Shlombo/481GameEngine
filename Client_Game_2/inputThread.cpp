#include "inputThread.h"
    InputThread::InputThread(int id, sf::RenderWindow *windowObj)
    {
        this->UUID = id;
        this->window = windowObj;
    }

    // bool Thread::isBusy()
    // {
    //     std::lock_guard<std::mutex> lock(*_mutex);  // this locks the mutuex until the variable goes out of scope (i.e., when the function returns in this case)
    //     return busy;
    // }

    void InputThread::run()
    {
        zmq::context_t context2 (1);
        zmq::socket_t socket (context2, zmq::socket_type::req);

        std::cout << "Connecting Input to server..." << std::endl;
        socket.connect ("tcp://localhost:5556");
        std::cout << "Connected Input" << std::endl;
        
        while(true) {
            // std::cout << "Top Input" << std::endl;
            std::string input;
            // int space = 0;
            int left = 0;
            int right = 0;
            // std::cout << "1" << std::endl;
            // if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->window->hasFocus()) {
            //     Event event("jump");
            //     this->eventList.push_back(event);
            // }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && this->window->hasFocus()) {
                left = 1;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && this->window->hasFocus()) {
                right = 1;
            }
            // std::cout << "left = " << left << std::endl;
            // std::cout << "right = " << right << std::endl;
            

            input = std::to_string(this->UUID) + " " + std::to_string(left) + " " + std::to_string(right);
            //Send input to server
            socket.send (zmq::buffer(input), zmq::send_flags::none);
            // std::cout << "input sent" << std::endl;
            //When proceessed by server
            zmq::message_t confirm;
            socket.recv(confirm, zmq::recv_flags::none);
            // std::cout << "input recieved" << std::endl;

        }
    }