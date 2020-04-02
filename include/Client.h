#include<iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

class Client {
    
    int send_len = 0;
    int recv_len = 0;
    
    char send_buf[200];
    char recv_buf[200];

    SOCKET s_server;
    SOCKADDR_IN server_addr;

public:
    void initialClient();  //初始化服务器  

    void rev();  //接受消息
    void handler();  //发送消息
};