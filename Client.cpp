#include "Client.h"
#include <iostream>
#include <WS2tcpip.h>

Client::Client(const std::string& serverIP, int port)
    : serverIP_(serverIP), port_(port), connected_(false) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("WSAStartup failed");
    }
}

Client::~Client() {
    Disconnect();
    WSACleanup();
}

bool Client::Connect() {
    clientSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket_ == INVALID_SOCKET) {
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port_);
    inet_pton(AF_INET, serverIP_.c_str(), &serverAddr.sin_addr);

    if (connect(clientSocket_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(clientSocket_);
        return false;
    }

    connected_ = true;
    return true;
}

void Client::Disconnect() {
    if (connected_) {
        closesocket(clientSocket_);
        connected_ = false;
    }
}
std::string GbkToUtf8(const std::string& gbkStr) {
    // Step 1: GBK → WideChar (UTF-16)
    int wLen = MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, NULL, 0);
    std::wstring wstr(wLen, 0);
    MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, &wstr[0], wLen);

    // Step 2: WideChar (UTF-16) → UTF-8
    int uLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string utf8str(uLen, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8str[0], uLen, NULL, NULL);

    // 去掉尾部的 null 字符
    utf8str.pop_back();
    return utf8str;
}
//Message Client::SendMessage(const Message& msg) {
//    if (!connected_) {
//        throw std::runtime_error("Not connected to server");
//    }
//
//    // TODO: 将Message转换为JSON字符串
//    std::string msgStr = ""; // 临时
//
//    send(clientSocket_, msgStr.c_str(), msgStr.length(), 0);
//
//    char buffer[4096];
//    int bytesReceived = recv(clientSocket_, buffer, sizeof(buffer), 0);
//    if (bytesReceived <= 0) {
//        throw std::runtime_error("Connection closed by server");
//    }
//
//    // TODO: 解析接收到的JSON字符串为Message
//    Message response;
//    return response;
//}
Message Client::SendMessage(const Message& msg) {
    if (!connected_) {
        throw std::runtime_error("Not connected to server");
    }

    // ⭐ 将Message对象转换为JSON字符串
    json j;
    j["type"] = static_cast<int>(msg.type);  // enum转int
    j["data"] = msg.data;

    std::string msgStr = j.dump();

   // std::cout << "[Client] 发送内容：" << msgStr << std::endl;

    // 📨 发送JSON字符串到服务器
    send(clientSocket_, msgStr.c_str(), msgStr.length(), 0);

    // 📩 接收服务器响应
    char buffer[4096];
    int bytesReceived = recv(clientSocket_, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
        throw std::runtime_error("Connection closed by server");
    }

    std::string responseStr(buffer, bytesReceived);
    //std::cout << "[Client] 接收到服务器响应：" << responseStr << std::endl;

    // ✅ 解析JSON字符串为Message对象
    Message response;
    json respJson = json::parse(responseStr);
    response.type = static_cast<MessageType>(respJson["type"].get<int>()); // int转enum
    response.data = respJson["data"].get<std::string>();

    return response;
}

bool Client::Login(const std::string& username, const std::string& password) {
    Message msg;
    msg.type = MessageType::LOGIN_REQUEST;

    // 使用 JSON 构造登录数据
    json loginData;
    loginData["username"] = username;
    loginData["password"] = password;
    //std::cout << "[Client] Login JSON 发送内容: " << loginData.dump() << std::endl;

    msg.data = loginData.dump();  // 转为字符串

    Message response = SendMessage(msg);
    return response.type == MessageType::LOGIN_RESPONSE;
}
std::string Client::GetProducts() {
    Message msg;
    msg.type = MessageType::GET_PRODUCTS;
    msg.data = "{}";

    Message response = SendMessage(msg);
    return response.data;
}

std::string Client::SearchProducts(const std::string& keyword) {
    Message msg;
    msg.type = MessageType::SEARCH_PRODUCTS;

    // 构造搜索数据 JSON
    json requestData;
    requestData["keyword"] = keyword;
    msg.data = requestData.dump();  // 转成字符串

    Message response = SendMessage(msg);
    return response.data;
}

//std::string Client::AddToCart(const std::string& productId, int quantity) {
//    Message msg;
//    msg.type = MessageType::ADD_TO_CART;
//    // TODO: 构造购物车数据JSON
//   // 构造购物车数据 JSON
//    json requestData;
//    requestData["productId"] = productId;
//    requestData["quantity"] = quantity;
//
//    msg.data = requestData.dump();  // 转为字符串
//
//    Message response = SendMessage(msg);
//    return response.data;
//}
std::string Client::AddToCart(const std::string& productId, int quantity) {
    // 构造要发送的数据 JSON
    json cartData;
    cartData["productId"] = productId;
    cartData["quantity"] = quantity;

    // 构造消息
    Message msg;
    msg.type = MessageType::ADD_TO_CART;
    msg.data = cartData.dump();  // 转为字符串形式
    // 发送消息并接收响应
    Message response = SendMessage(msg);

    //// 打印响应内容
    //std::cout << "[Client] 接收到服务器响应内容：" << response.data << std::endl;

    //// 尝试解析服务器返回的 data 字段（也是 JSON 字符串）
    //try {
    //    json respJson = json::parse(response.data);
    //    return respJson.value("message", "无返回信息");  // 优雅地获取 message 字段
    //}
    //catch (const std::exception& e) {
    //    std::cerr << "[Client] 响应 JSON 解析失败：" << e.what() << std::endl;
    //    return "响应解析失败";
    //}
    return response.data;
}
std::string Client::GetCart()
{
	Message msg;
	msg.type = MessageType::GET_CART;
	msg.data = "{}";  // 空 JSON 对象
	Message response = SendMessage(msg);

	// 打印响应内容
	//std::cout << "[Client] 接收到服务器响应内容：" << response.data << std::endl;
	// 尝试解析服务器返回的 data 字段（也是 JSON 字符串）
	try {
		json respJson = json::parse(response.data);
		return respJson.dump();  // 美化输出
	}
	catch (const std::exception& e) {
		std::cerr << "[Client] 响应 JSON 解析失败：" << e.what() << std::endl;
		return "响应解析失败";
	}
}



std::string Client::RemoveFromCart(int productId, int quantity) {
    Message msg;
    msg.type = MessageType::REMOVE_FROM_CART;
    // TODO: 构造购物车数据JSON
    // 构造购物车数据 JSON
    json requestData;
    requestData["productId"] = productId;
    requestData["quantity"] = quantity;

    msg.data = requestData.dump();  // 转为字符串发送

    Message response = SendMessage(msg);
    return response.data;
}

std::string Client::CreateOrder() {
    Message msg;
    msg.type = MessageType::CREATE_ORDER;
    msg.data = "{}";

    Message response = SendMessage(msg);
    return response.data;
}

std::string Client::PayOrder() {
    
    // TODO: 构造支付数据JSON
    // 创建购物车对象


    Message msg;
    msg.type = MessageType::PAY_ORDER;
    msg.data = "{}";
    Message response = SendMessage(msg);
    return response.data;

}