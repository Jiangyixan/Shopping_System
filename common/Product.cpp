// Product.cpp
#include "Product.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
// ���캯������ʼ����Ʒ������Ϣ
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
        loadNextId(); // ֻ�ڳ����һ�δ�������ʱ��ȡһ��
    }
    id = nextId++;
    saveNextId(); // ÿ�δ�������󱣴浱ǰ ID
}
//��ȡID
int Product::getId() const {
    return id;
}
// ��ȡ��Ʒ����
std::string Product::getName() const {
    return name;
}
// ��ȡ��Ʒ����
std::string Product::getDescription() const {
    return description;
}
// ��ȡ��ǰ�������
int Product::getQuantity() const {
    return quantity;
}
// ���²�Ʒ�۸񣨴���Ч�Լ�飩
void Product::updatePrice(double newPrice) {
    if (newPrice > 0) {
        price = newPrice;
    }
}
// ���¿������
void Product::updateQuantity(int newQuantity) {
    if (newQuantity >= 0) {
        quantity = newQuantity;
    }
}
// ���캯�������û���Product�Ĺ��캯��
Book::Book(int id, const std::string& name, const std::string& description, double price, int quantity, std::string ownername)
    : Product(id, name, description, price, quantity, ownername) {
}
Book::Book(const std::string& name, const std::string& description, double price, int quantity, std::string ownername)
    : Product(name, description, price, quantity, ownername) {
}
// ��ȡ�鼮�۸�
double Book::getPrice() const {
    return price;
}
//
std::string Book::getProductType() const
{
    return "Book";
}
// ���캯�������û���Product�Ĺ��캯��
Food::Food(int id, const std::string& name, const std::string& description, double price, int quantity, std::string ownername)
    : Product(id, name, description, price, quantity, ownername) {
}
Food::Food(const std::string& name, const std::string& description, double price, int quantity, std::string ownername)
    : Product(name, description, price, quantity, ownername) {
}
// ��ȡʳƷ�۸�
double Food::getPrice() const {
    return price;
}
// ��ȡ����
std::string Food::getProductType() const
{
    return "Food";
}
// ���캯�������û���Product�Ĺ��캯��
Clothing::Clothing(int id, const std::string& name, const std::string& description, double price, int quantity, std::string ownername)
    : Product(id, name, description, price, quantity, ownername) {
}
Clothing::Clothing(const std::string& name, const std::string& description, double price, int quantity, std::string ownername)
    : Product(name, description, price, quantity, ownername) {
}
// ��ȡ��װ�۸�
double Clothing::getPrice() const {
    return price;
}
// ��ȡ����
std::string Clothing::getProductType() const
{
    return "Clothing";
}
