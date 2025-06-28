#include "Server.h"
#include <iostream>
#include <sstream>
#include <WS2tcpip.h>
#include <windows.h>
#include <string>
// 将 GBK 编码的字符串转换为 UTF-8 编码
std::string GBKToUTF8(const std::string& gbkStr) {
    // 第一步：GBK（多字节） → Unicode（宽字符）
    int wideLen = MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, NULL, 0);
    if (wideLen <= 0) return "";

    std::wstring wstr(wideLen, 0);
    MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, &wstr[0], wideLen);

    // 第二步：Unicode（宽字符） → UTF-8（多字节）
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (utf8Len <= 0) return "";

    std::string utf8Str(utf8Len, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8Str[0], utf8Len, NULL, NULL);

    return utf8Str;
}

Server::Server(int port) : port_(port), running_(false) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {//Windows 专用的 Socket 初始化函数
        throw std::runtime_error("WSAStartup failed");
    }
}

Server::~Server() {
    Stop();
    WSACleanup();
}

bool Server::Start() {
    serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_ == INVALID_SOCKET) {
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port_);
	serverAddr.sin_addr.s_addr = INADDR_ANY; // 监听所有可用的网络接口

    if (bind(serverSocket_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(serverSocket_);
        return false;
    }

    if (listen(serverSocket_, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(serverSocket_);
        return false;
    }

    running_ = true;
    std::cout << "服务器启动成功，监听端口: " << port_ << std::endl;

    while (running_) {
        SOCKET clientSocket = accept(serverSocket_, nullptr, nullptr);
        if (clientSocket != INVALID_SOCKET) {
            clientThreads_.emplace_back(&Server::HandleClient, this, clientSocket);//
        }
    }

    return true;
}

void Server::Stop() {
    running_ = false;
    closesocket(serverSocket_);

    for (auto& thread : clientThreads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    clientThreads_.clear();
}



void Server::HandleClient(SOCKET clientSocket) {
    char buffer[8096];
    while (running_) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            break;
        }

        std::string msgStr(buffer, bytesReceived);
        std::cout << "[Server] 收到原始数据：" << msgStr << std::endl;

        Message receivedMsg;
        try {
            auto j = json::parse(msgStr);
            receivedMsg.type = static_cast<MessageType>(j.at("type").get<int>());

            // 判断 data 是否为 JSON 对象
            if (j["data"].is_object()) {
                // 如果是对象，处理为 JSON 字符串
                receivedMsg.data = j["data"].dump();
            }
            else {
                // 如果是字符串，直接获取
                receivedMsg.data = j.at("data").get<std::string>();
            }
        }
        catch (const std::exception& e) {
            std::cerr << "[Server] JSON解析失败：" << e.what() << std::endl;
            continue;
        }

        Message response = ProcessMessage(receivedMsg, clientSocket);

        json jResponse;
        jResponse["type"] = response.type;
        jResponse["data"] = response.data;

        std::string responseStr = jResponse.dump();
        send(clientSocket, responseStr.c_str(), responseStr.length(), 0);

        std::cout << "[Server] 已发送响应：" << responseStr << std::endl;
    }

    closesocket(clientSocket);
}

Message Server::ProcessMessage(const Message& msg, SOCKET clientSocket) {
    Message response;
    response.type = MessageType::Error;
    try {
        switch (msg.type) {
        case MessageType::LOGIN_REQUEST:
            response.data = HandleLogin(msg.data);
            response.type = MessageType::LOGIN_RESPONSE;
            break;
        case MessageType::GET_PRODUCTS:
            response.data = HandleGetProducts();
            response.type = MessageType::PRODUCTS_RESPONSE;
            break;
        case MessageType::SEARCH_PRODUCTS:
            response.data = HandleSearchProducts(msg.data);
            response.type = MessageType::SEARCH_RESPONSE;
            break;
        case MessageType::ADD_TO_CART:

            response.data = HandleAddToCart(msg.data);
            response.type = MessageType::CART_RESPONSE;
            break;
        case MessageType::REMOVE_FROM_CART:
            response.data = HandleRemoveFromCart(msg.data);
            response.type = MessageType::CART_RESPONSE;
            break;
        case MessageType::CREATE_ORDER:
            response.data = HandleCreateOrder(msg.data);
            response.type = MessageType::ORDER_RESPONSE;
            break;
        case MessageType::PAY_ORDER:
          response.data = HandlePayOrder(msg.data);
            response.type = MessageType::PAYMENT_RESPONSE;
            break;
        case MessageType::GET_CART:
        {
            response.data = HandleGetCart();
            response.type = MessageType::CART_RESPONSE;
            break;
        }


        }
    }
    catch (const std::exception& e) {
        response.type = MessageType::Error;
        response.data = e.what();
    }

    return response;
}

std::string Server::HandleLogin(const std::string& data) {
    try {
       // std::cout << "[Server] 收到登录请求: " << data << std::endl;

        json request = json::parse(data);

        std::string username = request["username"];
        std::string password = request["password"];
       // std::cout << "[Server] 解析用户名: " << username << ", 密码: " << password << std::endl;

        User* user = dataHandler_.loginUser(username, password);
       // std::cout << "[Server] loginUser 返回值: " << (user ? "成功" : "失败") << std::endl;

        if (user != nullptr) {
            json response;
            response["status"] = "success";
            response["username"] = user->getUsername();
            response["userType"] = user->getUserType();
            response["balance"] = user->getBalance();
			currentUser = user; // 保存当前登录用户
			std::cout << "[Server] 登录成功，用户名: " << currentUser->getUsername() << std::endl;
            //delete user;
            return response.dump();
        }
        else {
            return json{
                {"status", "error"},
                {"message", "用户名或密码错误"}
            }.dump();
        }
    }
    catch (const std::exception& e) {
        return json{
            {"status", "error"},
            {"message", std::string("解析或处理失败: ") + e.what()}
        }.dump();
    }
}

std::string Server::HandleGetProducts() {
    // TODO: 实现获取商品列表逻辑
	std::vector<Product*> products = dataHandler_.ShowProduct();
	json response;
	if (products.empty()) {
		response["status"] = "error";
		response["message"] = "没有商品信息";
	}
	else {
		response["status"] = "success";
		for (const auto& product : products) {
			//std::cout << "[Server] 处理商品 ID: " << product->getId() << ", 名称: " << product->getName() << std::endl;
			//std::cout << "[Server] 商品描述: " << product->getDescription() << ", 价格: " << product->getPrice() << ", 数量: " << product->getQuantity() << std::endl;
			//std::cout << "[Server] 商品拥有者: " << product->getOwnername() << std::endl;

			json productJson;
			productJson["id"] = product->getId();
			productJson["name"] =/*GBKToUTF8(*/product->getName();
			productJson["description"] = /*GBKToUTF8(*/product->getDescription();
			productJson["price"] = product->getPrice();
			productJson["quantity"] = product->getQuantity();
			productJson["ownername"] = product->getOwnername();
			response["products"].push_back(productJson);
            //
		}
	}
    return response.dump(-1, ' ', false); // 第三个参数 false → 不转义中文

}

std::string Server::HandleSearchProducts(const std::string& data) {
    json request;
    json response;

    try {
        // 解析客户端传来的 JSON 数据
        request = json::parse(data);

        // 提取 keyword 字段
        std::string keyword = request.value("keyword", "");

        if (keyword.empty()) {
            response["status"] = "error";
            response["message"] = "搜索关键词为空";
            return response.dump(-1, ' ', false); // 返回错误信息
        }

        // 调用 DataHandler 查询商品
        std::vector<Product*> results = dataHandler_.getProductByName(keyword);

        if (results.empty()) {
            response["status"] = "error";
            response["message"] = "未找到匹配的商品";
        }
        else {
            response["status"] = "success";
            for (Product* product : results) {
				//std::cout << "[Server] 处理搜索结果商品 ID: " << product->getId() << ", 名称: " << product->getName() << std::endl;
				//std::cout << "[Server] 商品描述: " << product->getDescription() << ", 价格: " << product->getPrice() << ", 数量: " << product->getQuantity() << std::endl;
				//std::cout << "[Server] 商品拥有者: " << product->getOwnername() << std::endl;
                json productJson;
                productJson["id"] = product->getId();
                productJson["name"] = product->getName();
                productJson["description"] = product->getDescription();
                productJson["price"] = product->getPrice();
                productJson["quantity"] = product->getQuantity();
                productJson["ownername"] = product->getOwnername();
                response["products"].push_back(productJson);
            }
        }
    }
    catch (json::parse_error& e) {
        response["status"] = "error";
        response["message"] = std::string("JSON解析失败: ") + e.what();
    }

    return response.dump(-1, ' ', false); // 不转义中文
}

std::string Server::HandleAddToCart(const std::string& data) {
    json cartData = json::parse(data);  // data 是一个转义过的 JSON 字符串
    std::string productIdStr = cartData["productId"];
    int quantity = cartData["quantity"];


    int productId = std::stoi(productIdStr);  // 转换为整数 ID
	std::cout << "[Server] 处理添加到购物车请求，商品 ID: " << productId << ", 数量: " << quantity << std::endl;
    // 调用你已有的函数查找商品
    Product* product = dataHandler_.getProductById(productId);

    json response;
    if (product == nullptr) {
        response["status"] = "error";
        response["message"] = "未找到该商品";
    }
    else {
        // 获取当前用户名（假设你已有 currentUser 成员变量）
        std::string username = currentUser->getUsername();
		//std::cout << "[Server] 当前用户名: " << username << std::endl;

        // 创建购物车对象并添加商品
        ShoppingCart cart(username);
        cart.addItem(
            product->getName(),
            quantity,
            product->getOwnername(),
            product->getPrice()
        );

        response["status"] = "success";
        response["message"] = "Product added to cart successfully";
    }

    std::cout << "[Server] 生成的响应数据：" << response.dump() << std::endl;
    return response.dump(-1, ' ', false);  // 返回 JSON 字符串，不转义中文
}

std::string Server::HandleGetCart() {
	// 获取当前用户名
	std::string username = currentUser->getUsername();
	std::cout << "[Server] 处理获取购物车请求，用户名: " << username << std::endl;
	// 创建购物车对象
	ShoppingCart cart(username);
	cart.loadFromFile();  // 从文件加载购物车内容
	json response;
	response["status"] = "success";
	response["items"] = json::array();
	// 遍历购物车中的商品
	for (const auto& item : cart.getItems()) {
		json itemJson;
		itemJson["cartItemId"] = item.cartItemId;
		itemJson["productName"] = item.productName;
		itemJson["quantity"] = item.quantity;
		itemJson["seller"] = item.seller;
		itemJson["price"] = item.price;
		response["items"].push_back(itemJson);
	}
	std::cout << "[Server] 生成的响应数据：" << response.dump() << std::endl;
	return response.dump(-1, ' ', false);  // 返回 JSON 字符串，不转义中文
}
std::string Server::HandleRemoveFromCart(const std::string& data) {
    // TODO: 实现移除购物车逻辑
	json request = json::parse(data);
	int cartItemId = request["productId"];
	int quantity = request["quantity"];
	std::string username = currentUser->getUsername();
    json response;

	std::cout << "[Server] 处理移除购物车请求，用户名: " << username << ", 购物车项 ID: " << cartItemId << ", 数量: " << quantity << std::endl;
	// 创建购物车对象
    ShoppingCart cart(username);
	cart.loadFromFile();  // 从文件加载购物车内容
	if (currentUser->getUserType() == "Consumer") {
		if (quantity < 0) {
			response["status"] = "error";
			response["message"] = "Quantity must be greater than 0";
			return response.dump();
		}
        else if (quantity == 0)
        {
            cart.removeItem(cartItemId);
            response["status"] = "success";
            response["message"] = "Product delete successfully";

        }
        else
        {
            cart.updateItemQuantity(cartItemId, quantity);
            response["status"] = "success";
			response["message"] = "Product quantity updated successfully";
        }

	}
	else {
        response["status"] = "error";
        response["message"] = "Only customers can purchase goods";
	}
    return response.dump();
}

std::string Server::HandleCreateOrder(const std::string& data) {
    // TODO: 实现创建订单逻辑
    return "{}";
}

std::string Server::HandlePayOrder(const std::string& data) {
    // TODO: 实现支付订单逻辑
    std::string username = currentUser->getUsername();
    std::cout << "[Server] 处理获取购物车请求，用户名: " << username << std::endl;
     //创建购物车对象
   ShoppingCart cart(username);
    cart.loadFromFile();  // 从文件加载购物车内容
    json response;
    if (currentUser->getUserType()=="Consumer")
    {
        Consumer buyer(currentUser->getUsername(), currentUser->getPassword(),currentUser->getBalance());
        std::vector<User*> allusers = dataHandler_.loadUsers();
        int result=cart.checkout(&buyer, allusers);
        currentUser->setBalance(buyer.getBalance());
        if (result == 0)
        {
            response["status"] = "error";
            response["message"] = "nothing in your cart!";
        }
        else if (result == -1)
        {
            response["status"] = "error";
            response["message"] = "your money is not enough";
        }
        else if (result ==3)
        {
            response["status"] = "error";
            response["message"] = "something wrong when paying the order";
        }
        else
        {
            response["status"] = "success";
            response["message"] = "purchase success!";
        }
    }

    return response.dump();
}











//void Server::HandleClient(SOCKET clientSocket) {
//    char buffer[4096];
//    while (running_) {
//        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
//        if (bytesReceived <= 0) {
//            break;
//        }
//
//        std::string msgStr(buffer, bytesReceived);
//        std::cout << "[Server] 收到原始数据：" << msgStr << std::endl;
//
//        Message receivedMsg;
//        try {
//            auto j = json::parse(msgStr);
//            receivedMsg.type = static_cast<MessageType>(j.at("type").get<int>());
//            receivedMsg.data = j.at("data").get<std::string>();
//        }
//        catch (const std::exception& e) {
//            std::cerr << "[Server] JSON解析失败：" << e.what() << std::endl;
//            continue;
//        }
//
//        Message response = ProcessMessage(receivedMsg, clientSocket);
//
//        json jResponse;
//        jResponse["type"] = response.type;
//        jResponse["data"] = response.data;
//
//        std::string responseStr = jResponse.dump();
//        send(clientSocket, responseStr.c_str(), responseStr.length(), 0);
//
//        std::cout << "[Server] 已发送响应：" << responseStr << std::endl;
//    }
//
//    closesocket(clientSocket);
//}

//std::string GBKToUTF8(const std::string& gbkStr) {
//    // 先转换成宽字符
//    int wideLen = MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, nullptr, 0);
//    if (wideLen == 0) return "";
//
//    wchar_t* wideStr = new wchar_t[wideLen];
//    MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, wideStr, wideLen);
//
//    // 再从宽字符转换成 UTF-8
//    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
//    if (utf8Len == 0) {
//        delete[] wideStr;
//        return "";
//    }
//
//    char* utf8Str = new char[utf8Len];
//    WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, utf8Str, utf8Len, nullptr, nullptr);
//
//    std::string result(utf8Str);
//
//    delete[] wideStr;
//    delete[] utf8Str;
//
//    return result;
//}


// 具体的处理函数实现
/*std::string Server::HandleLogin(const std::string& data) {
    // TODO: 实现登录逻辑
    try {
        // 解析JSON数据
        size_t usernamePos = data.find("\"username\":\"");
        size_t passwordPos = data.find("\"password\":\"");
        if (usernamePos == std::string::npos || passwordPos == std::string::npos) {
            return "{\"status\":\"error\",\"message\":\"无效的登录数据格式\"}";
        }

        // 提取用户名和密码
        usernamePos += 11; // 跳过 "username":"
        size_t usernameEnd = data.find("\"", usernamePos);
        std::string username = data.substr(usernamePos, usernameEnd - usernamePos);

        passwordPos += 11; // 跳过 "password":"
        size_t passwordEnd = data.find("\"", passwordPos);
        std::string password = data.substr(passwordPos, passwordEnd - passwordPos);

        // 验证用户登录
        User* user = dataHandler_.loginUser(username, password);

        if (user != nullptr) {
            // 登录成功，构造响应JSON
            std::string response = "{\"status\":\"success\",";
            response += "\"username\":\"" + user->getUsername() + "\",";
            response += "\"userType\":\"" + user->getUserType() + "\",";
            response += "\"balance\":" + std::to_string(user->getBalance()) + "}";

            delete user; // 释放动态分配的内存
            return response;
        }
        else {
            // 登录失败
            return "{\"status\":\"error\",\"message\":\"用户名或密码错误\"}";
        }
    }
    catch (const std::exception& e) {
        return "{\"status\":\"error\",\"message\":\"" + std::string(e.what()) + "\"}";
    }
}*/