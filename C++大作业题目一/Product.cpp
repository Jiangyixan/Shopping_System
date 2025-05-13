// Product.cpp
#include "Product.h"
// 构造函数：初始化产品基本信息
Product::Product(const std::string& name, const std::string& description, double price, int quantity)
    : name(name), description(description), price(price), quantity(quantity) {
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
Book::Book(const std::string& name, const std::string& description, double price, int quantity)
    : Product(name, description, price, quantity) {
}
// 获取书籍价格
double Book::getPrice() const {
    return price;
}
// 构造函数：调用基类Product的构造函数
Food::Food(const std::string& name, const std::string& description, double price, int quantity)
    : Product(name, description, price, quantity) {
}
// 获取食品价格
double Food::getPrice() const {
    return price;
}
// 构造函数：调用基类Product的构造函数
Clothing::Clothing(const std::string& name, const std::string& description, double price, int quantity)
    : Product(name, description, price, quantity) {
}
// 获取服装价格
double Clothing::getPrice() const {
    return price;
}
