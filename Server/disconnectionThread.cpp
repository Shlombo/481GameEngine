#include "disconnectionThread.h"

    DisconnectionThread::DisconnectionThread(std::mutex *_mutex, std::condition_variable *_condition_variable, std::list<GameObject *> *listPtr)
    {
        this->_mutex = _mutex;
        this->_condition_variable = _condition_variable;
        this->list = listPtr;

    }

    void DisconnectionThread::run()
    {
        zmq::context_t context1 (2);
        zmq::socket_t socket (context1, zmq::socket_type::rep);
        socket.bind ("tcp://*:5558");
        while(true) {
            std::cout << "Top" << std::endl;
            //Recieve disconnect message
            zmq::message_t inputMes;
            std::string inputString;
            std::stringstream inputStream;
            socket.recv (inputMes, zmq::recv_flags::none);

            //Process message
            inputString.assign(static_cast<char *>(inputMes.data()), inputMes.size());
            inputStream << inputString;
            std::string temp;
            inputStream >> temp;

            int id = stoi(temp);
            {
                std::unique_lock<std::mutex> lock(*_mutex);
                for (std::list<GameObject *>::iterator it = list->begin(); it != list->end(); ++it) {
                    if((*it)->UUID == id) {
                        it = list->erase(it);
                        break;
                    }
                }
            }
        
            zmq::message_t confirm;
            socket.send (confirm, zmq::send_flags::none);
        }
        

    }