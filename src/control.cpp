#include "Server.h"
#include "Client.h"
#include "constant.h"
#include <thread>

int main(int argc, char* argv[]) {
    using std::cout;
    using std::endl;
    using std::stoi;
    using std::thread;

    cout << "welcome to Chat." << endl;

    if (argc > 1) {
        if (strcmp(argv[1], "client") == 0) {
            if (argc == 2) {
                Client client;
                client.initialClient();
                std::thread t1(&Client::rev, &client);
                t1.detach();
                client.handler();
                return OK;
            }
            else {
                cout << "input error. " << endl;
                cout << "you should input like this: client" << endl;
                return ERR;
            }
        }
        else if (strcmp(argv[1], "server") == 0) {
            if (argc == 2) {
                Server server;
                server.initialServer();
              
                server.handler();
                return OK;
            }
            else {
                cout << "input error. " << endl;
                cout << "you should input like this: server" << endl;
                return ERR;
            }
        }
    }
}