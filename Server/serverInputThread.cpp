#include "serverInputThread.h"
ServerInputThread::ServerInputThread(std::mutex *_mutex, std::list<GameObject *> *listPtr, EventManager *man) {
    this->_mutex = _mutex;
    this->list = listPtr;
    this->manager = man;
}

void ServerInputThread::run() {
    //  Prepare our connection socket
    zmq::context_t context2 (2);
    zmq::socket_t socket (context2, zmq::socket_type::rep);
    socket.bind ("tcp://*:5556");

    while(true) {
        //Recieve input from client
        
        
        zmq::message_t inputMes;
        std::string inputString;
        std::stringstream inputStream;
        socket.recv (inputMes, zmq::recv_flags::none);
        // std::cout << "Client Connected to input" << std::endl;

        //Process message
        inputString.assign(static_cast<char *>(inputMes.data()), inputMes.size());
        // std::cout << "Recieved input" << std::endl;
        inputStream << inputString;
        std::string temp;
        inputStream >> temp;
        int id = stoi(temp);
        // std::cout << "UUID = " << id << std::endl;
        inputStream >> temp;
        int space = stoi(temp);
        inputStream >> temp;
        int left = stoi(temp);
        inputStream >> temp;
        int right = stoi(temp);

        std::cout << "space = " << space << std::endl;
        std::cout << "left = " << left << std::endl;
        std::cout << "right = " << right << std::endl;

        // std::unique_lock<std::mutex> lock(*_mutex);
        for (std::list<GameObject *>::iterator it = list->begin(); it != list->end(); ++it) {
            
            if((*it)->UUID == id) {
                if(space == 1) {
                    (*it)->space = true;
                } else {
                    (*it)->space = false;
                }
                if(left == 1) {
                    (*it)->left = true;
                } else {
                    (*it)->left = false;
                }
                if(right == 1) {
                    (*it)->right = true;
                } else {
                    (*it)->right = false;
                }
                    
            }
        }
        zmq::message_t confirm;
        socket.send (confirm, zmq::send_flags::none);
    }
    
}