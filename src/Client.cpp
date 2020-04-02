#include<iostream>
#include<thread>

#include "Client.h"

#pragma comment(lib,"ws2_32.lib")
using namespace std;
using std::thread;

void Client::initialClient() {
    //初始化套接字库
    WORD w_req = MAKEWORD(2, 2);//版本号
    WSADATA wsadata;
    int err;
    err = WSAStartup(w_req, &wsadata);
    if (err != 0) {
        cout << "初始化套接字库失败！" << endl;
    }

    //检测版本号
    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
    {
        cout << "套接字库版本号不符！" << endl;
        WSACleanup();
    }
 
    //填充服务端地址信息

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(1234);
    //创建套接字
    s_server = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
        cout << "服务器连接失败！" << endl;
        WSACleanup();
    }
    else {
        cout << "服务器连接成功！" << endl;
    }

    cout << "@某人给他发消息吧...，例如“hi@123”" << endl;
}

void Client::rev() {
    while (true) {
        char receiveBuf[200] = "\0";
        if (recv(s_server, receiveBuf, sizeof(receiveBuf), 0) < 0) {
            cout << "与服务器连接断开" << endl;
            return;
        }
        cout <<"收到消息" << receiveBuf << " " << endl;
    }
}

void Client::handler() {

    //发送数据
    while (true) {
        cin >> send_buf;
        send_len = send(s_server, send_buf, 100, 0);
        if (send_len < 0) {
            cout << "发送失败！" << endl;
            break;
        }
    }
    //关闭套接字
    closesocket(s_server);
    //释放DLL资源
    WSACleanup();
}