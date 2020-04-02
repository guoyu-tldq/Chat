#include <winsock2.h>
#include <iostream>
#include <vector>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

class Server {

    //服务端套接字，接受请求套接字
    SOCKET s_server;
    SOCKET s_accept;
    //服务端地址客户端地址
    SOCKADDR_IN server_addr;
    SOCKADDR_IN accept_addr;

    char receiveBuf[200];                  //接收消息
    string sendBuf;                        //发送消息

    typedef struct Client
    {
        SOCKET client_socket;
        string client_name;
    }client;

    vector<Client> client_user;

public:
    void initialServer();  //初始化服务器  

    void handler();  //处理客户端连接及消息处理
};
