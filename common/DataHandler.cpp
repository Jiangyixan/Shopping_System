// DataHandler.cpp
#include "DataHandler.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory> // 用于智能指针
void DataHandler::saveUser(const User& user) {//用户信息写入文件
    std::ofstream file("../Data/users.txt", std::ios::app);
    if (loginUser(user.getUsername(), user.getPassword()) != NULL) {//如果用户已经存在
        std::cout << "用户已存在！" << std::endl;
        return;
    }
    else//用户不存在
    {
        if (file.is_open()) {
            file << user.getUsername() << " " << user.getBalance() << " " << user.getUserType() << " " << user.getPassword() << "\n";
            file.close();
        }
        else {
            std::cerr << "Error: 无法打开users.txt 进行写入操作" << std::endl;
        }
    }
}
//使用User*动态内存规划，读出文件内容并将整个文件内容返回
std::vector<User*> DataHandler::loadUsers() {
    std::vector<User*> users;
    std::ifstream file(DATA_PATH + "users.txt");
    if (file.is_open()) {
        std::string username, userType, password;
        double balance;
        while (file >> username >> balance >> userType >> password) {
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
void DataHandler::saveProduct(const Product& product) {
    std::ofstream file("../Data/products.txt", std::ios::app);
    if (file.is_open()) {
        file << product.getId() << "\t" // 添加商品ID
            << product.getName() << "\t"
            << product.getDescription() << "\t"
            << product.getPrice() << "\t"
            << product.getQuantity() << "\t"
            << product.getOwnername() << "\t";

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
    std::ifstream file(DATA_PATH + "products.txt");

    if (file.is_open()) {

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            int id;
            std::string name, description, type;
            double price;
            int quantity;
            std::string ownername;

            iss >> id;                  // 读取 ID
            iss.ignore(1, '\t');         // 跳过 1 个 \t
            std::getline(iss, name, '\t');  // 读取 name（直到 \t）
            std::getline(iss, description, '\t');  // 读取 description（直到 \t）
            iss >> price >> quantity;    // 读取 price 和 quantity
            iss.ignore(1, '\t');         // 跳过 \t
            std::getline(iss, ownername, '\t');  // 读取 ownername（直到 \t）
            iss >> type;                 // 读取 type（最后一个字段，不需要 ignore）
            if (type == "Book") {
                products.push_back(new Book(id, name, description, price, quantity, ownername));
            }
            else if (type == "Food") {
                products.push_back(new Food(id, name, description, price, quantity, ownername));
            }
            else if (type == "Clothing") {
                products.push_back(new Clothing(id, name, description, price, quantity, ownername));
            }
            // 创建对象...
        }
        file.close();
    }
    else {
        std::cerr << "Error: Unable to open products.txt for reading." << std::endl;
    }
    return products;
}
/*std::string type, name, description;
double price;
int quantity;
std::getline(iss, name, '\t'); // 如果用制表符分隔
std::getline(iss, description, '\t');
iss >> price >> quantity >> type;
while (file >> name >> description >> price >> quantity >> type) {
    if (type == "Book") {
        products.push_back(new Book(name, description, price, quantity));
    }
    else if (type == "Food") {
        products.push_back(new Food(name, description, price, quantity));
    }
    else if (type == "Clothing") {
        products.push_back(new Clothing(name, description, price, quantity));
    }
}*/

//更新密码
bool DataHandler::updatePassword(const std::string& username, const std::string& oldPassword, const std::string& newPassword) {
    std::vector<User*> users = loadUsers();
    bool found = false;

    for (User* user : users) {
        if (user->getUsername() == username && user->getPassword() == oldPassword) {
            user->updatePassword(newPassword);
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "用户名或密码错误，无法修改密码！" << std::endl;
        return false;
    }

    // 重新写入文件
    std::ofstream file("../Data/users.txt", std::ios::out | std::ios::trunc);
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
std::vector<Product*> DataHandler::ShowProduct()
{
    std::vector<Product*> products = DataHandler::loadProducts();
    std::cout << "商品列表:" << std::endl;
    int show = 0;
    // 遍历并显示商品信息
    for (const auto& product : products) {

        show = 1;
    }
    if (show == 0) {
        std::cout << "没有商品信息！" << std::endl;
    }
    else {
        return products;
    }
    // 释放内存
    for (auto& product : products) {
        delete product;
    }
}
std::vector<Product*> DataHandler::getProductByName(const std::string& name) {
    std::vector<Product*> products = loadProducts();
    std::vector<Product*> productsResult;

    for (Product* product : products) {
        if (product->getName().find(name) != std::string::npos) {
            productsResult.push_back(product);
        }
    }

    if (productsResult.empty()) {
        std::cout << "没有找到该商品！" << std::endl;
    }

    return productsResult;
}
Product* DataHandler::getProductById(int ID) {
    std::vector<Product*> products = loadProducts();
    for (Product* product : products) {
        if (product->getId() == ID) {
            return product;  // 直接返回匹配的 product
        }
    }
    std::cout << "没有找到该商品！" << std::endl;
    return nullptr;  // 未找到时返回 nullptr
}

std::vector<Product*> DataHandler::getProductByType(const std::string& type) {
    std::vector<Product*> products = loadProducts();
    std::vector<Product*> productsResult;
    for (Product* product : products) {
        if (product->getProductType() == type) {
            productsResult.push_back(product);
        }
    }
    if (productsResult.empty()) {
        std::cout << "没有找到该类型商品！" << std::endl;
    }
    return productsResult;
}
void DataHandler::discountByType(const std::string& type, double discountRate, std::string ownername) {
    std::vector<Product*> products = loadProducts(); // 加载所有商品
    bool found = false;

    for (Product* product : products) {
        if (product->getProductType() == type && (product->getOwnername().find(ownername) != std::string::npos)) {

            double oldPrice = product->getPrice();
            double newPrice = oldPrice * discountRate;
            product->updatePrice(newPrice);
            found = true;
        }
    }

    if (!found) {
        std::cout << "没有找到类型为 " << type << " 的商品！" << std::endl;
        return;
    }

    // 打开文件并覆盖写入
    std::ofstream file("../Data/products.txt", std::ios::trunc); // 清空再写入
    if (file.is_open()) {
        for (Product* product : products) {
            file << product->getId() << "\t" // 添加商品ID
                << product->getName() << "\t"
                << product->getDescription() << "\t"
                << product->getPrice() << "\t"
                << product->getQuantity() << "\t"
                << product->getOwnername() << "\t";

            if (dynamic_cast<Book*>(product)) {
                file << "Book";
            }
            else if (dynamic_cast<Food*>(product)) {
                file << "Food";
            }
            else if (dynamic_cast<Clothing*>(product)) {
                file << "Clothing";
            }
            file << "\n";
        }
        file.close();
        std::cout << "已成功对类型为 " << type << " 的商品打折并保存！" << std::endl;
    }
    else {
        std::cerr << "无法打开文件进行保存！" << std::endl;
    }

    // 释放内存（如果不是智能指针）
    for (Product* p : products) {
        delete p;
    }
}
void DataHandler::updateProduct(int idToUpdate, const std::string& newName,
    const std::string& newDescription, double newPrice, int newQuantity, std::string UserName) {
    std::vector<Product*> products = loadProducts();
    bool found = false;

    for (Product* product : products) {
        if (product->getId() == idToUpdate) {
            if ((product->getOwnername().find(UserName) == std::string::npos))
            {
                std::cout << "非此商品拥有者，你没有权限修改该商品！" << std::endl;
                return;
            }
            // 修改信息
            product->setName(newName);
            product->setDescription(newDescription);
            product->setPrice(newPrice);
            product->setQuantity(newQuantity);
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "未找到ID为 " << idToUpdate << " 的商品。\n";
        return;
    }

    // 重新写入文件
    std::ofstream file("../Data/products.txt", std::ios::trunc); // 清空文件
    if (file.is_open()) {
        for (Product* product : products) {
            file << product->getId() << "\t"
                << product->getName() << "\t"
                << product->getDescription() << "\t"
                << product->getPrice() << "\t"
                << product->getQuantity() << "\t"
                << product->getOwnername() << "\t";

            if (dynamic_cast<Book*>(product)) file << "Book";
            else if (dynamic_cast<Food*>(product)) file << "Food";
            else if (dynamic_cast<Clothing*>(product)) file << "Clothing";

            file << "\n";
        }
        file.close();
        std::cout << "商品修改成功。\n";
    }
    else {
        std::cerr << "无法写入文件 products.txt\n";
    }

    for (Product* p : products) delete p; // 防止内存泄漏
}
void DataHandler::deleteProduct(int idToDelete, std::string UserName) {
    std::vector<Product*> products = loadProducts();
    bool found = false;

    auto it = products.begin();
    while (it != products.end()) {
        if ((*it)->getId() == idToDelete) {
            if ((*it)->getOwnername().find(UserName) == std::string::npos)
            {
                std::cout << "非此商品拥有者，你没有权限删除该商品！" << std::endl;
                return;
            }

            std::cout << "确定删除 ID 为 " << idToDelete << "，商品名称为 ["
                << (*it)->getName() << "] 的商品吗？(1.是 2.否): ";

            int choice;
            std::cin >> choice;
            if (choice == 1) {
                delete* it;                // 释放该商品内存
                it = products.erase(it);   // 从列表中移除
                found = true;
                break;
            }
            else {
                std::cout << "取消删除商品。\n";
                break;  // 虽然没删，但也要跳出循环
            }
        }
        else {
            ++it;
        }
    }

    if (!found) {
        std::cout << "未找到 ID 为 " << idToDelete << " 的商品。\n";
    }
    else {
        // 重新保存剩余商品
        std::ofstream file("../Data/products.txt", std::ios::trunc);
        if (file.is_open()) {
            for (const auto& product : products) {
                file << product->getId() << "\t"
                    << product->getName() << "\t"
                    << product->getDescription() << "\t"
                    << product->getPrice() << "\t"
                    << product->getQuantity() << "\t"
                    << product->getOwnername() << "\t";

                if (dynamic_cast<Book*>(product)) file << "Book";
                else if (dynamic_cast<Food*>(product)) file << "Food";
                else if (dynamic_cast<Clothing*>(product)) file << "Clothing";

                file << "\n";
            }
            file.close();
            std::cout << "商品删除成功。\n";
        }
        else {
            std::cerr << "无法写入文件 products.txt\n";
        }
    }

    // 无论是否删除，最后都要释放 vector 剩余的指针内存
    for (auto& p : products) delete p;
}


