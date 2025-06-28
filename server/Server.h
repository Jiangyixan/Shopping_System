#pragma once
#include <WinSock2.h>
#include <string>
#include <thread>
#include <vector>
#include "DataHandler.h"
#include "Protocol.h"
#include"ShoppingCart.h"
#pragma comment(lib, "ws2_32.lib")
#include <json.hpp>
#include <json.hpp>

using json = nlohmann::json;
class Server {
public:
    Server(int port = 8888);
    ~Server();

    bool Start();
    void Stop();

private:
    void HandleClient(SOCKET clientSocket);
    Message ProcessMessage(const Message& msg, SOCKET clientSocket);
    std::string HandleLogin(const std::string& data);
    std::string HandleGetProducts();
    std::string HandleSearchProducts(const std::string& data);
    std::string HandleAddToCart(const std::string& data);
    std::string HandleRemoveFromCart(const std::string& data);
    std::string HandleCreateOrder(const std::string& data);
    std::string HandlePayOrder(const std::string& data);
    std::string HandleGetCart();


private:
    int port_;
    SOCKET serverSocket_;
    bool running_;
    DataHandler dataHandler_;
    std::vector<std::thread> clientThreads_;
    // Server.h
public:
     User* currentUser; // 当前登录用户

};