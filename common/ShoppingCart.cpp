// ShoppingCart.cpp
#include "ShoppingCart.h"  // 包含对应的头文件
#include "CartItem.h"     // 包含 CartItem 类
#include"User.h"
#include "DataHandler.h"
#include <iostream>        // 用于标准输入输出（如 displayCart）
#include <fstream>         // 用于文件操作（saveToFile / loadFromFile）
#include <string>         // 用于字符串操作
#include <sstream>
#include <vector>
#include <memory> // 用于智能指针
#include <map>           // 用于 map 容器
#include <algorithm>     // 用于 std::remove_if
int ShoppingCart::nextItemId = 1;
void ShoppingCart::loadNextId() {
    std::ifstream in("../Data/id_" + getName() + ".txt");
    if (in.is_open()) {
        in >> nextItemId;
        in.close();
    }
}
void ShoppingCart::saveNextId() {
    std::ofstream out("../Data/id_" + getName() + ".txt");
    if (out.is_open()) {
        out << nextItemId;
        out.close();
    }
}
ShoppingCart::ShoppingCart(const std::string& username)
    : username(username) {
    loadFromFile(); // 程序一开始就加载购物车
}
std::string ShoppingCart::getName()
{
    return username;
}
void ShoppingCart::addItem(const std::string& productName, int quantity, std::string seller, double price) {
    for (auto& item : items) {
        if (item.productName == productName) {
            item.quantity += quantity;
            saveToFile();  // 每次添加都保存
            return;
        }
    }
    if (nextItemId == 1) {
        loadNextId(); // 只在程序第一次创建对象时读取一次
    }
    saveNextId(); // 每次创建对象后保存当前 ID
    items.emplace_back(nextItemId++, productName, quantity, seller, price);
    saveNextId();
    saveToFile();  //  添加新商品也要保存
}

void ShoppingCart::removeItem(int cartItemId) {
    items.erase(std::remove_if(items.begin(), items.end(),
        [&](const CartItem& item) { return item.cartItemId == cartItemId; }),
        items.end());
    saveToFile();  // 删除后保存
}

void ShoppingCart::updateItemQuantity(int cartItemId, int quantity) {
    for (auto& item : items) {
        if (item.cartItemId == cartItemId) {
            item.quantity = quantity;
            saveToFile();  // 更新数量后保存
            return;
        }
    }
}

void ShoppingCart::displayCart() const {

    std::cout << "购物车内容：" << std::endl;
    if (items.empty()) {
        std::cout << "（购物车为空）" << std::endl;
    }
    else {
        for (const auto& item : items) {
            std::cout << "- 购物车项ID: " << item.cartItemId
                << " 商品: " << item.productName
                << " 数量: " << item.quantity
                << " 卖家: " << item.seller
                << " 价格: " << item.price
                << std::endl;

        }
    }
}


// 获取购物车中的所有商品项（只读）
// 返回值:
//   const std::vector<CartItem>& - 购物车项的常量引用
const std::vector<CartItem>& ShoppingCart::getItems() const {
    return items;
}

// 将购物车数据保存到文件
// 文件格式: 每行存储一个商品，格式为 "商品名称\t数量"
// 参数:
//   username - 用于生成唯一文件名（如 cart_username.txt）
void ShoppingCart::saveToFile() {
    std::ofstream file("../Data/cart_" + username + ".txt");
    if (file.is_open()) {
        for (const auto& item : items) {
            file << item.cartItemId << "\t"
                << item.productName << "\t"
                << item.quantity << "\t"
                << item.seller << "\t"
                << item.price << "\n";
        }
        file.close();
    }
}

void ShoppingCart::loadFromFile() {
    items.clear();
    std::ifstream file("../Data/cart_" + username + ".txt");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            int id;
            std::string productName;
            int quantity;
            std::string seller;
            double price;

            // 读取ID
            if (!(iss >> id)) continue;

            // 读取商品名称（可能包含空格）
            char quote;
            //iss >> quote; // 跳过可能的引号或空格
            std::getline(iss >> std::ws, productName, '\t');

            // 读取数量和卖家
            if (!(iss >> quantity >> seller >> price)) continue;

            items.emplace_back(id, productName, quantity, seller, price);
        }
        file.close();
    }
}




// ... existing code ...

double ShoppingCart::calculateTotal() const {
    std::map<std::string, double> sellerTotals;
    for (const auto& item : items) {
        double itemTotal = item.quantity * item.price;  // 使用实际商品价格
        sellerTotals[item.seller] += itemTotal;
    }
    double total = 0.0;
    for (const auto& sellerTotal : sellerTotals) {
        total += sellerTotal.second;
    }
    return total;
}

void ShoppingCart::displayOrderPreview() const {
    std::cout << "\n========== 订单预览 ==========" << std::endl;
    std::cout << "用户: " << username << std::endl;
    std::map<std::string, double> sellerTotals;

    std::cout << "\n商品清单:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    for (const auto& item : items) {
        double itemTotal = item.quantity * item.price;
        sellerTotals[item.seller] += itemTotal;

        std::cout << "商品ID: " << item.cartItemId << std::endl
            << "名称: " << item.productName << std::endl
            << "单价: ￥" << item.price << std::endl
            << "数量: " << item.quantity << std::endl
            << "小计: ￥" << itemTotal << std::endl
            << "卖家: " << item.seller << std::endl
            << "----------------------------------------" << std::endl;
    }

    std::cout << "\n各卖家结算金额：" << std::endl;
    for (const auto& sellerTotal : sellerTotals) {
        std::cout << "卖家 " << sellerTotal.first << ": ￥" << sellerTotal.second << std::endl;
    }

    std::cout << "\n总计金额: ￥" << calculateTotal() << std::endl;
    std::cout << "=====================================" << std::endl;
}

int ShoppingCart::checkout(Consumer* buyer, std::vector<User*>& allUsers) {
    if (items.empty()) {
        std::cout << "购物车为空，无法结算！" << std::endl;
        return 0;//
    }

    // 计算每个卖家的总金额
    std::map<std::string, double> sellerTotals;
    for (const auto& item : items) {
        double itemTotal = item.quantity * item.price;
        sellerTotals[item.seller] += itemTotal;
    }

    // 计算总金额
    double totalAmount = calculateTotal();

    // 检查买家余额是否足够
    if (buyer->getBalance() < totalAmount) {
        std::cout << "余额不足！" << std::endl;
        std::cout << "订单总额: ￥" << totalAmount << std::endl;
        std::cout << "当前余额: ￥" << buyer->getBalance() << std::endl;
        std::cout << "还差: ￥" << totalAmount - buyer->getBalance() << std::endl;
        return -1;
    }

    // 显示订单预览
    displayOrderPreview();

    //// 确认购买
    //std::cout << "\n确认要购买吗？" << std::endl;
    //std::cout << "1. 确认购买" << std::endl;
    //std::cout << "2. 取消购买" << std::endl;
    //std::cout << "请选择(1/2): ";

    //int choice;
    //std::cin >> choice;
    //if (choice != 1) {
    //    std::cout << "已取消购买！" << std::endl;
    //    return quit;
    //}

    // 执行交易
    std::cout << "\n正在处理交易..." << std::endl;

    // 先扣除买家的钱
    /*if (!buyer->consume(totalAmount)) {
        std::cout << "扣款失败，交易取消！" << std::endl;
        return false;
    }*/
    for (User* user : allUsers) {
        if (user->getUsername() == buyer->getUsername() &&
            user->getUserType() == "Consumer") {
            user->consume(totalAmount);
            std::cout << " 已扣除买家 " << buyer->getUsername()
                << " 余额 ￥" << totalAmount << std::endl;
            break;
        }
    }
    // 给每个卖家转账
    bool allTransfersSuccessful = true;
    for (const auto& sellerTotal : sellerTotals) {
        bool sellerFound = false;
        for (User* user : allUsers) {
            if (user->getUsername() == sellerTotal.first &&
                user->getUserType() == "Seller") {
                user->recharge(sellerTotal.second);
                std::cout << " 已向卖家 " << sellerTotal.first
                    << " 转账 ￥" << sellerTotal.second << std::endl;
                sellerFound = true;
                break;
            }
        }
        if (!sellerFound) {
            std::cout << "× 错误：未找到卖家 " << sellerTotal.first << std::endl;
            allTransfersSuccessful = false;
        }
    }

    if (allTransfersSuccessful) {
        // 更新用户文件
        std::ofstream file("../Data/users.txt", std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << "错误：无法打开用户文件进行更新！" << std::endl;
            // 回滚所有交易
            //buyer->recharge(totalAmount);
            for (User* user : allUsers) {
                if (user->getUsername() == buyer->getUsername() &&
                    user->getUserType() == "Consumer") {
                    user->recharge(totalAmount);
                    break;
                }
            }
            for (const auto& sellerTotal : sellerTotals) {
                for (User* user : allUsers) {
                    if (user->getUsername() == sellerTotal.first &&
                        user->getUserType() == "Seller") {
                        user->consume(sellerTotal.second);
                        break;
                    }
                }
            }
            std::cout << "× 交易失败：无法更新用户数据！" << std::endl;
            std::cout << " 已回滚所有资金转账" << std::endl;
            return 3 ;
        }

        // 写入更新后的用户数据
        for (User* user : allUsers) {
            file << user->getUsername() << " "
                << user->getBalance() << " "
                << user->getUserType() << " "
                << user->getPassword() << "\n";
        }
        file.close();

        // 清空购物车
        items.clear();
        saveToFile();
        std::cout << "\n 交易成功完成！" << std::endl;
        std::cout << " 总支付金额: ￥" << totalAmount << std::endl;
        //std::cout << " 当前余额: ￥" << buyer->getBalance() << std::endl;

        return 1;
    }
    else {
        // 如果有任何转账失败，退还买家的钱
        for (User* user : allUsers) {
            if (user->getUsername() == buyer->getUsername() &&
                user->getUserType() == "Consumer") {
                user->recharge(totalAmount);
                break;
            }
        }

        // 更新用户文件（因为已经退还了买家的钱）
        std::ofstream file("../Data/users.txt", std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << "警告：无法更新用户文件，但资金已退还！" << std::endl;
            return 3;
        }

        // 写入更新后的用户数据
        for (User* user : allUsers) {
            file << user->getUsername() << " "
                << user->getBalance() << " "
                << user->getUserType() << " "
                << user->getPassword() << "\n";
        }
        file.close();

        std::cout << "\n× 交易失败！" << std::endl;
        std::cout << " 已退还金额: ￥" << totalAmount << std::endl;
        std::cout << " 当前余额: ￥" << buyer->getBalance() << std::endl;

        return 3;
    }
}