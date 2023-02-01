#include "connectionThread.h"

    ConnectionThread::ConnectionThread(std::mutex *_mutex, std::condition_variable *_condition_variable, int id, bool *start)
    {
        this->_mutex = _mutex;
        this->_condition_variable = _condition_variable;
        this->id = id;
        this->start = start;
    }

    // bool Thread::isBusy()
    // {
    //     std::lock_guard<std::mutex> lock(*_mutex);  // this locks the mutuex until the variable goes out of scope (i.e., when the function returns in this case)
    //     return busy;
    // }

    void ConnectionThread::run()
    {
        zmq::context_t context1 (2);
        zmq::socket_t socket (context1, zmq::socket_type::rep);
        socket.bind ("tcp://*:5555");
        while(true) {
            std::cout << "Top" << std::endl;

            zmq::message_t request;
            socket.recv (request, zmq::recv_flags::none);
            std::cout << "Client Connected" << std::endl;

            int id = this->id;
            

            std::string s;
            s = std::to_string(id);

            socket.send(zmq::buffer(s), zmq::send_flags::none);
            std::cout << "Id Reply sent" << std::endl;
            
            sleep(5);
            *(this->start) = true;
        }
        

    }

