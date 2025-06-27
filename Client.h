#pragma once
#pragma once
#include <WinSock2.h>
#include <string>
#include "Protocol.h"
#include <json.hpp>
#include"ShoppingCart.h"
using json = nlohmann::json;
#pragma comment(lib, "ws2_32.lib")

class Client {
public:
    Client(const std::string& serverIP = "127.0.0.1", int port = 8888);
    ~Client();

    bool Connect();
    void Disconnect();

    // 用户操作接口
    bool Login(const std::string& username, const std::string& password);
    std::string GetProducts();
    std::string SearchProducts(const std::string& keyword);
    std::string AddToCart(const std::string& productId, int quantity);
    std::string RemoveFromCart(int productId, int quantity);
    std::string CreateOrder();
    std::string PayOrder();
    std::string GetCart();

private:
    Message SendMessage(const Message& msg);

private:
    std::string serverIP_;
    int port_;
    SOCKET clientSocket_;
    bool connected_;
};