#include "Client.h"
#include <iostream>
#include <string>

void showMenu() {
    std::cout << "\n=== 电商系统客户端 ===" << std::endl;
    std::cout << "1. 登录" << std::endl;
    std::cout << "2. 查看所有商品" << std::endl;
    std::cout << "3. 搜索商品" << std::endl;
    std::cout << "4. 添加商品到购物车" << std::endl;
	std::cout << "5. 购物车商品" << std::endl;
    std::cout << "6. 从购物车移除商品" << std::endl;
   // std::cout << "7. 创建订单" << std::endl;
    std::cout << "7. 支付订单" << std::endl;
    std::cout << "0. 退出" << std::endl;
    std::cout << "请选择操作: ";
}

int main() {
    try {
        Client client("127.0.0.1", 8888);
        std::cout << "连接服务器..." << std::endl;

        if (!client.Connect()) {
            std::cout << "连接服务器失败！" << std::endl;
            return 1;
        }

        std::cout << "连接成功！" << std::endl;
        bool loggedIn = false;

        while (true) {
            showMenu();

            int choice;
            std::cin >> choice;
            std::cin.ignore();

            if (choice == 0) break;

            try {
                switch (choice) {
                case 1: {
                    if (loggedIn) {
                        std::cout << "您已经登录！" << std::endl;
                        break;
                    }
                    std::string username, password;
                    std::cout << "用户名: ";
                    std::getline(std::cin, username);
                    std::cout << "密码: ";
                    std::getline(std::cin, password);

                    if (client.Login(username, password)) {
                        std::cout << "登录成功！" << std::endl;
                        loggedIn = true;
                    }
                    else {
                        std::cout << "登录失败！" << std::endl;
                    }
                    break;
                }
                case 2: {
                    if (!loggedIn) {
                        std::cout << "请先登录！" << std::endl;
                        break;
                    }
                    std::string responseStr = client.GetProducts();

                    try {
                        json response = json::parse(responseStr);

                        if (response["status"] == "success") {
                            std::cout << "=== 商品列表 ===" << std::endl;
                            for (const auto& product : response["products"]) {
                                std::cout << "ID: " << product["id"] << std::endl;
                                std::cout << "名称: " << product["name"] << std::endl;
                                std::cout << "描述: " << product["description"] << std::endl;
                                std::cout << "价格: " << product["price"] << std::endl;
                                std::cout << "数量: " << product["quantity"] << std::endl;
                                std::cout << "商家: " << product["ownername"] << std::endl;
                                std::cout << "-----------------------------" << std::endl;
                            }
                        }
                        else {
                            std::cout << "获取失败: " << response["message"] << std::endl;
                        }

                    }
                    catch (json::parse_error& e) {
                        std::cerr << " 解析商品数据失败: " << e.what() << std::endl;
                        std::cerr << " 原始响应内容: " << responseStr << std::endl;
                    }
                    break;
                }
                case 3: {
                    if (!loggedIn) {
                        std::cout << "请先登录！" << std::endl;
                        break;
                    }
                    std::string keyword;
                    std::cout << "请输入搜索关键词: ";
                    std::getline(std::cin, keyword);
                    std::string responseStr = client.SearchProducts(keyword);
                    try {
                        json response = json::parse(responseStr);

                        if (response["status"] == "success") {
                            std::cout << "=== 商品列表 ===" << std::endl;
                            for (const auto& product : response["products"]) {
                                std::cout << "ID: " << product["id"] << std::endl;
                                std::cout << "名称: " << product["name"] << std::endl;
                                std::cout << "描述: " << product["description"] << std::endl;
                                std::cout << "价格: " << product["price"] << std::endl;
                                std::cout << "数量: " << product["quantity"] << std::endl;
                                std::cout << "商家: " << product["ownername"] << std::endl;
                                std::cout << "-----------------------------" << std::endl;
                            }
                        }
                        else {
                            std::cout << "获取失败: " << response["message"] << std::endl;
                        }

                    }
                    catch (json::parse_error& e) {
                        std::cerr << " 解析商品数据失败: " << e.what() << std::endl;
                        std::cerr << " 原始响应内容: " << responseStr << std::endl;
                    }

                    break;
                }
                case 4: {
                    if (!loggedIn) {
                        std::cout << "请先登录！" << std::endl;
                        break;
                    }
                    std::string productId;
                    int quantity;
                    std::cout << "商品ID: ";
                    std::getline(std::cin, productId);
                    std::cout << "数量: ";
                    std::cin >> quantity;
                    std::cin.ignore();
                    std::cout << client.AddToCart(productId, quantity) << std::endl;

                    break;
                }
                case 5: {
                    if (!loggedIn) {
                        std::cout << "请先登录！" << std::endl;
                        break;
                    }
                    std::string responseStr = client.GetCart();
                    try {
                        json response = json::parse(responseStr);
                        if (response["status"] == "success") {
                            std::cout << "=== 购物车商品 ===" << std::endl;
                            for (const auto& item : response["items"]) {
                                std::cout << "商品ID: " << item.at("cartItemId") << std::endl;
                                std::cout << "名称: " << item.at("productName") << std::endl;
                                std::cout << "数量: " << item.at("quantity") << std::endl;
                                std::cout << "价格: " << item.at("price") << std::endl;
                                std::cout << "卖家: " << item.at("seller") << std::endl;
                                std::cout << "-----------------------------" << std::endl;
                            }
                        }
                        else {
                            std::cout << "获取失败: " << response["message"] << std::endl;
                        }
                    }
                    catch (json::parse_error& e) {
                        std::cerr << " 解析购物车数据失败: " << e.what() << std::endl;
                        std::cerr << " 原始响应内容: " << responseStr << std::endl;
                    }
                    break;
                }
                case 6: {
                    if (!loggedIn) {
                        std::cout << "请先登录！" << std::endl;
                        break;
                    }
                    int productId;
                    int quantity;
                    std::cout << "商品ID: ";
                    std::cin >> productId;
                    std::cout << "数量: ";
                    std::cin >> quantity;
                    std::cin.ignore();
                    std::string responseStr = client.RemoveFromCart(productId, quantity);
                    try
                    {
                        json response = json::parse(responseStr);
                        std::cout << response["message"] << std::endl;

                    }
                    catch (json::parse_error& e) {
                        std::cerr << " 解析商品数据失败: " << e.what() << std::endl;
                        std::cerr << " 原始响应内容: " << responseStr << std::endl;
                    }
                    break;
                }
                case 7: {
                    if (!loggedIn) {
                        std::cout << "请先登录！" << std::endl;
                        break;
                    }
                    else {

                        std::string responseStr = client.GetCart();
                        try {
                            json response = json::parse(responseStr);
                            if (response["status"] == "success") {
                                std::cout << "=== 购物车商品 ===" << std::endl;
                                for (const auto& item : response["items"]) {
                                    std::cout << "商品ID: " << item.at("cartItemId") << std::endl;
                                    std::cout << "名称: " << item.at("productName") << std::endl;
                                    std::cout << "数量: " << item.at("quantity") << std::endl;
                                    std::cout << "价格: " << item.at("price") << std::endl;
                                    std::cout << "卖家: " << item.at("seller") << std::endl;
                                    std::cout << "-----------------------------" << std::endl;
                                }
                            }
                            else {
                                std::cout << "获取失败: " << response["message"] << std::endl;
                            }
                        }
                        catch (json::parse_error& e) {
                            std::cerr << " 解析购物车数据失败: " << e.what() << std::endl;
                            std::cerr << " 原始响应内容: " << responseStr << std::endl;
                        }
                        std::cout << "\n确认要购买吗？" << std::endl;
                        std::cout << "1. 确认购买" << std::endl;
                        std::cout << "2. 取消购买" << std::endl;
                        std::cout << "请选择(1/2): ";

                        int choice;
                        std::cin >> choice;
                        if (choice != 1) {
                            std::cout << "已取消购买！" << std::endl;
                            break;
                        }
                        else 
                        {
                            std::string payresponseStr = client.PayOrder();
                            try
                            {
                              json response = json::parse(payresponseStr);
                              std::cout << response["message"] << std::endl;

                            }
                          catch (json::parse_error& e) 
                          {
                              std::cerr << " 解析商品数据失败: " << e.what() << std::endl;
                             std::cerr << " 原始响应内容: " << responseStr << std::endl;
                          }
                        break;
                        }

                    }
                }
                default:
                    std::cout << "无效的选择！" << std::endl;
                }
            }
            catch (const std::exception& e) {
                std::cout << "操作失败: " << e.what() << std::endl;
            }
        }

        client.Disconnect();
    }
    catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}