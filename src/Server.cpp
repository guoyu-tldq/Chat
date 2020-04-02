#pragma once
#include<iostream>
#include <sstream>
#include <regex>
#include <vector>
#include "Server.h"

using namespace std;
using std::stringstream;

//string转换函数
template<typename T> string toString(const T& t) {
    stringstream ss{};     //创建一个格式化输出流
    ss << t;               //把值传递如流中
    return ss.str();
}

//字符串按“特定字符delim”分割
vector<string> testSplit11(const string& in, const string& delim)
{
    vector<string> ret;
    try
    {
        regex re{ delim };
        return vector<string>{
            sregex_token_iterator(in.begin(), in.end(), re, -1),
                sregex_token_iterator()
        };
    }
    catch (const std::exception & e)
    {
        cout << "error:" << e.what() << std::endl;
    }
    return ret;
}

void Server::initialServer() {
    //初始化套接字库
    WORD w_req = MAKEWORD(2, 2);//版本号
    WSADATA wsadata;
    int err;
    err = WSAStartup(w_req, &wsadata);
    if (err != 0) {
        cout << "初始化套接字库失败。" << endl;
    }
   
    //检测版本号
    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
        cout << "套接字库版本号不符。" << endl;
        WSACleanup();
    }
  
    //服务端地址信息
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(1234);
    //创建套接字
    s_server = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
        cout << "绑定失败。" << endl;
        WSACleanup();
    }
    //设置套接字为监听状态
    if (listen(s_server, SOMAXCONN) < 0) {
        cout << "设置监听状态失败。" << endl;
        WSACleanup();
    }
    cout << "服务端正在监听连接...." << endl;

}

void Server::handler() {
    while (true) {

        fd_set fdRead;
        FD_ZERO(&fdRead);                 
        FD_SET(s_server, &fdRead);         
        for (auto item : client_user) {    
            FD_SET(item.client_socket, &fdRead);
        }

        //select函数监测是否激活
        int ret = select(0, &fdRead, nullptr, nullptr, nullptr);

        //如果服务端s_server被激活，接受客户端连接请求
        if (FD_ISSET(s_server, &fdRead)) {
            FD_CLR(s_server, &fdRead);   
            SOCKADDR_IN clientSocket;
            SOCKET c_socket = INVALID_SOCKET;
            int len = sizeof(SOCKADDR_IN);
            if (c_socket = accept(s_server, (SOCKADDR*)&clientSocket, &len)) {
                client c;
                c.client_socket = c_socket;
                auto name = toString<SOCKET>(c_socket);
                c.client_name = name;
                client_user.push_back(c);
                cout << c_socket << "连接到服务器" << endl;
            }
        }

        //接收信息并转发到目标用户
        for (int i = 0; i < fdRead.fd_count; i++) {
            auto client_name = fdRead.fd_array[i];

            if (recv(client_name, receiveBuf, sizeof(receiveBuf), 0) < 0) {
                cout << client_name << " 断开连接" << endl;
                for (auto client = client_user.begin(); client != client_user.end();) {
                    if (client->client_socket == client_name) {
                        client = client_user.erase(client);
                        closesocket(client_name);
                    }
                    else {
                        client++;
                    }
                }
            }
            else {
                cout << "收到消息: " << receiveBuf << " from: " << client_name;

                vector<string>ret = testSplit11(receiveBuf, "@");
                cout << "   to: " << ret[1] <<  endl;

                for (auto client : client_user) {
                    if (client.client_name == ret[1]) {
                        sendBuf = "【" + toString<SOCKET>(client_name) + "】: " + ret[0];
                        send(client.client_socket, sendBuf.c_str(), 200, 0);
                        cout << "转发消息: " << sendBuf << " to: " << client.client_name << endl;
                    }
                }
            }
        }
   
    }
    //关闭套接字
    closesocket(s_server);
    for (auto client : client_user) {
        closesocket(client.client_socket);
    }
    //释放DLL资源
    WSACleanup();
}
