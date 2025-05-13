// DataHandler.cpp
#include "DataHandler.h"
#include <fstream>
#include <iostream>

void DataHandler::saveUser(const User& user) {//用户信息写入文件
    std::ofstream file("users.txt", std::ios::app);
    if (file.is_open()) {
        file << user.getUsername() << " " << user.getBalance() << " " << user.getUserType() << " " << user.getPassword()<<"\n";
        file.close();
    }
    else {
        std::cerr << "Error: 无法打开users.txt 进行写入操作" << std::endl;
    }
}
//使用User*动态内存规划，读出文件内容并将整个文件内容返回
std::vector<User*> DataHandler::loadUsers() {
    std::vector<User*> users;
    std::ifstream file("users.txt");
    if (file.is_open()) {
        std::string username, userType,password;
        double balance;
        while (file >> username >> balance >> userType>>password) {
            if (userType == "Seller") {
                users.push_back(new Seller(username, password, balance));
            }
            else if (userType == "Consumer") {
                users.push_back(new Consumer(username, password, balance));
            }
        }
        file.close();
    }
    else {
        std::cerr << "Error: 无法读取 users.txt " << std::endl;
    }
    return users;
}

void DataHandler::saveProduct(const Product& product) {
    std::ofstream file("products.txt", std::ios::app);
    if (file.is_open()) {
        file << product.getName() << " "
            << product.getDescription() << " "
            << product.getPrice() << " "
            << product.getQuantity() << " ";

        if (dynamic_cast<const Book*>(&product)) {
            file << "Book";
        }
        else if (dynamic_cast<const Food*>(&product)) {
            file << "Food";
        }
        else if (dynamic_cast<const Clothing*>(&product)) {
            file << "Clothing";
        }

        file << "\n";
        file.close();
    }
    else {
        std::cerr << "Error: 无法打开 products.txt 进行写入" << std::endl;
    }
}
//使用Product*动态内存规划，读出文件内容并将整个文件内容返回

std::vector<Product*> DataHandler::loadProducts() {
    std::vector<Product*> products;
    std::ifstream file("products.txt");

    if (file.is_open()) {
        std::string type, name, description;
        double price;
        int quantity;

        while (file >> type >> name >> price >> quantity) {
            if (type == "Book") {
                products.push_back(new Book(name, "图书商品", price, quantity));
            }
            else if (type == "Food") {
                products.push_back(new Food(name, "食品商品", price, quantity));
            }
            else if (type == "Clothing") {
                products.push_back(new Clothing(name, "服装商品", price, quantity));
            }
        }
        file.close();
    }
    else {
        std::cerr << "Error: Unable to open products.txt for reading." << std::endl;
    }

    return products;
}
//登录用户
User* DataHandler::loginUser(const std::string& username, const std::string& password) {
    std::vector<User*> users = loadUsers();
    for (User* user : users) {
        //std::cout << user->getUsername() <<user->getBalance()<<user->getPassword()<< std::endl;
        if (user->getUsername() == username && user->checkPassword(password)) {//用户和密码匹配
            std::cout << "欢迎, " << username << "! 登录成功。" << std::endl;
            return user;
        }
    }
    std::cout << "用户名或密码错误！" << std::endl;
    return NULL;

}

//更新密码
bool DataHandler::updatePassword(const std::string& username, const std::string& oldPassword, const std::string& newPassword) {
    std::vector<User*> users = loadUsers();
    bool found = false;

    for (User* user : users) {
        if (user->getUsername() == username && user->getPassword() == oldPassword) {
            user ->updatePassword( newPassword);
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "用户名或密码错误，无法修改密码！" << std::endl;
        return false;
    }

    // 重新写入文件
    std::ofstream file("users.txt", std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "无法打开用户文件进行更新！" << std::endl;
        return false;
    }

    for (User* user : users) {
        file << user->getUsername() << " " 
            << user->getBalance() << " " 
            << user->getUserType() << " " 
            << user->getPassword() << "\n";
    }

    file.close();
    std::cout << "密码修改成功！" << std::endl;
    return true;
}
void DataHandler::ShowProduct()
{
	std::vector<Product*> products = DataHandler::loadProducts();
	std::cout << "商品列表:" << std::endl;
    int show = 0;
	// 遍历并显示商品信息
	for (const auto& product : products) {
		std::cout << "名称: " << product->getName() << ", 描述: " << product->getDescription() << ", 价格: " << product->getPrice() << ", 库存: " << product->getQuantity() << std::endl;
        show = 1;
	}
	if (show == 0) {
		std::cout << "没有商品信息！" << std::endl;
	}
	else {
		std::cout << "商品信息显示完毕！" << std::endl;
	}
	// 释放内存
	for (auto& product : products) {
		delete product;
	}
}