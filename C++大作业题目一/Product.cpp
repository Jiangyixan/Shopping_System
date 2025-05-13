// Product.cpp
#include "Product.h"
// ���캯������ʼ����Ʒ������Ϣ
Product::Product(const std::string& name, const std::string& description, double price, int quantity)
    : name(name), description(description), price(price), quantity(quantity) {
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
Book::Book(const std::string& name, const std::string& description, double price, int quantity)
    : Product(name, description, price, quantity) {
}
// ��ȡ�鼮�۸�
double Book::getPrice() const {
    return price;
}
// ���캯�������û���Product�Ĺ��캯��
Food::Food(const std::string& name, const std::string& description, double price, int quantity)
    : Product(name, description, price, quantity) {
}
// ��ȡʳƷ�۸�
double Food::getPrice() const {
    return price;
}
// ���캯�������û���Product�Ĺ��캯��
Clothing::Clothing(const std::string& name, const std::string& description, double price, int quantity)
    : Product(name, description, price, quantity) {
}
// ��ȡ��װ�۸�
double Clothing::getPrice() const {
    return price;
}
