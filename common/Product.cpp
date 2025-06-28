// Product.cpp
#include "Product.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
// 构造函数：初始化产品基本信息
int Product::nextId = 1;
void Product::loadNextId() {
    std::ifstream in("id.txt");
    if (in.is_open()) {
        in >> nextId;
        in.close();
    }
}
void Product::saveNextId() {
    std::ofstream out("id.txt");
    if (out.is_open()) {
        out << nextId;
        out.close();
    }
}

Product::Product(int id, const std::string& name, const std::string& description, double price, int quantity, std::string ownername)
    :id(id), name(name), description(description), price(price), quantity(quantity), ownername(ownername) {

}
Product::Product(const std::string& name, const std::string& description, double price, int quantity, std::string ownername)
    : name(name), description(description), price(price), quantity(quantity), ownername(ownername) {
    if (nextId == 1) {
        loadNextId(); // 只在程序第一次创建对象时读取一次
    }
    id = nextId++;
    saveNextId(); // 每次创建对象后保存当前 ID
}
//获取ID
int Product::getId() const {
    return id;
}
// 获取产品名称
std::string Product::getName() const {
    return name;
}
// 获取产品描述
std::string Product::getDescription() const {
    return description;
}
// 获取当前库存数量
int Product::getQuantity() const {
    return quantity;
}
// 更新产品价格（带有效性检查）
void Product::updatePrice(double newPrice) {
    if (newPrice > 0) {
        price = newPrice;
    }
}
// 更新库存数量
void Product::updateQuantity(int newQuantity) {
    if (newQuantity >= 0) {
        quantity = newQuantity;
    }
}
// 构造函数：调用基类Product的构造函数
Book::Book(int id, const std::string& name, const std::string& description, double price, int quantity, std::string ownername)
    : Product(id, name, description, price, quantity, ownername) {
}
Book::Book(const std::string& name, const std::string& description, double price, int quantity, std::string ownername)
    : Product(name, description, price, quantity, ownername) {
}
// 获取书籍价格
double Book::getPrice() const {
    return price;
}
//
std::string Book::getProductType() const
{
    return "Book";
}
// 构造函数：调用基类Product的构造函数
Food::Food(int id, const std::string& name, const std::string& description, double price, int quantity, std::string ownername)
    : Product(id, name, description, price, quantity, ownername) {
}
Food::Food(const std::string& name, const std::string& description, double price, int quantity, std::string ownername)
    : Product(name, description, price, quantity, ownername) {
}
// 获取食品价格
double Food::getPrice() const {
    return price;
}
// 获取类型
std::string Food::getProductType() const
{
    return "Food";
}
// 构造函数：调用基类Product的构造函数
Clothing::Clothing(int id, const std::string& name, const std::string& description, double price, int quantity, std::string ownername)
    : Product(id, name, description, price, quantity, ownername) {
}
Clothing::Clothing(const std::string& name, const std::string& description, double price, int quantity, std::string ownername)
    : Product(name, description, price, quantity, ownername) {
}
// 获取服装价格
double Clothing::getPrice() const {
    return price;
}
// 获取类型
std::string Clothing::getProductType() const
{
    return "Clothing";
}
