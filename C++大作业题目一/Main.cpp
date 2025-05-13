// Main.cpp
#include "User.h"
#include "Product.h"
#include "DataHandler.h"
#include "Utility.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <limits> // 用于清除输入缓冲区

User* currentUser; 
void displayMainMenu() {
    std::cout << "\n===== 电商平台系统 =====" << std::endl;
    std::cout << "1. 用户注册" << std::endl;
    std::cout << "2. 用户登录" << std::endl;
    std::cout << "3. 修改密码" << std::endl;
    std::cout << "4. 查看商品信息" << std::endl;
	std::cout << "5. 退出系统" << std::endl;
    std::cout << "请选择: ";
}
void registerUser() {
    std::string username, password, type;
    double balance;
    int typeInt;
    std::cout << "请输入用户名: ";
    std::cin >> username;
    std::cout << "请输入密码: ";
    std::cin >> password;
    std::cout << "请输入初始余额: ";
    std::cin >> balance;
    std::cout << "请输入账户类型 (输入1代表Consumer/2代表Seller): ";
    std::cin >> typeInt;
    if (typeInt == 1) {
        type = "Consumer";
    }
    else if (typeInt == 2) {
        type = "Seller";
    }
    else {
        std::cout << "无效的账户类型！" << std::endl;
        return;
    }
    if (type == "Consumer") {
        Consumer user(username, password, balance);
        DataHandler::saveUser(user);
    }
    else if (type == "Seller") {
        Seller user(username, password, balance);
        DataHandler::saveUser(user);
    }
    else {
        std::cout << "无效的账户类型！" << std::endl;
    }
}
void loginUser() {
    std::string username, password;

    std::cout << "请输入用户名: ";
    std::cin >> username;
    std::cout << "请输入密码: ";
    std::cin >> password;

    currentUser = DataHandler::loginUser(username, password);
    std::cout << "用户为：" << currentUser->getUserType() << std::endl;
}
void changePassword() {
    std::string username, oldPassword, newPassword;

    std::cout << "请输入用户名: ";
    std::cin >> username;
    std::cout << "请输入旧密码: ";
    std::cin >> oldPassword;
    std::cout << "请输入新密码: ";
    std::cin >> newPassword;

    DataHandler::updatePassword(username, oldPassword, newPassword);
}
void MassageInCommodity()
{
	std::cout << "欢迎来到商品信息界面！" << std::endl;
	std::cout << "1. 查看商品信息" << std::endl;
	std::cout << "2. 增加商品" << std::endl;
}
void ShowCommodity()
{
    DataHandler::ShowProduct();
}
void AddProduct()
{
	std::string name, description;
	double price;
	int quantity;
    // 读取商品名称（整行）
    std::cout << "请输入商品名称: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清除缓冲区
    std::getline(std::cin, name);

    // 读取商品描述（整行）
    std::cout << "请输入商品描述: ";
    std::getline(std::cin, description);

    // 读取价格和数量（数值型）
    std::cout << "请输入商品价格: ";
    while (!(std::cin >> price)) {
        std::cin.clear(); // 清除错误状态
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "输入无效，请重新输入价格: ";
    }

    std::cout << "请输入商品数量: ";
    while (!(std::cin >> quantity)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "输入无效，请重新输入数量: ";
    }
	Product* product = new Book(name, description, price, quantity);
	DataHandler::saveProduct(*product);
	delete product; // 释放动态分配的内存
}
bool CommodityPage()//当选择退出商品信息界面时，返回true
{
	
	int choice;
	std::cin >> choice;
	switch (choice) {
	case 1:
		std::cout << "商品信息如下：" << std::endl;
		ShowCommodity();
		break;
	case 2:
		if (currentUser->getUserType() == "Seller") {//判断当前用户是否为卖家
			std::cout << "可以增加商品" << std::endl;
            AddProduct();
		}
		else {
			std::cout << "您没有权限增加商品！" << std::endl;//不是卖家，不能增加商品
		}
		break;
	case 3:
		std::cout << "退出商品信息界面" << std::endl;
        return true;
	default:
		std::cout << "无效选项，请重试。" << std::endl;
	}
	return false;//继续操作商品信息
}

int main() {
    int choice;
    while (true) {
        displayMainMenu();
        std::cin >> choice;
        switch (choice) {
        case 1:
            registerUser();
            break;
        case 2:
            loginUser(); 
            while (true)
            {
                MassageInCommodity();

                if (CommodityPage())
                {
					break;//退出商品信息界面
				}
                else
                {
                    std::cout << "继续操作商品信息" << std::endl;
                }
            }
            break;
        case 3:
            changePassword(); 
            return 0;
        case 4:
            std::cout << "查看商品信息" << std::endl;
			std::cout << "商品信息如下：" << std::endl;
            ShowCommodity();
            break;
        case 5:
            std::cout << "退出系统" << std::endl;
            return 0;
        default:
            std::cout << "无效选项，请重试。" << std::endl;
        }
    }
}
