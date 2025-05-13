// Product.h
#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <iostream>

class Product {
protected:
    std::string name;
    std::string description;
    double price;
    int quantity;// 产品库存数量

public:
    // 构造函数（初始化产品基本信息）
    Product(const std::string& name, const std::string& description, double price, int quantity);
    virtual ~Product() = default;// 虚析构函数（支持多态销毁）
    virtual double getPrice() const = 0;// 纯虚函数：获取最终价格（需子类实现不同定价策略）
    std::string getName() const;// 获取产品名称
    std::string getDescription() const;// 获取产品描述
    int getQuantity() const; // 获取产品库存数量
    void updatePrice(double newPrice);// 更新产品价格
    void updateQuantity(int newQuantity);// 更新产品库存数量
};

class Book : public Product {
public:
    Book(const std::string& name, const std::string& description, double price, int quantity);
    double getPrice() const override; // 实现价格计算//const承诺不会修改类的任何成员变量override显式声明重写虚函数
};

class Food : public Product {
public:
    Food(const std::string& name, const std::string& description, double price, int quantity);
    double getPrice() const override;
};

class Clothing : public Product {
public:
    Clothing(const std::string& name, const std::string& description, double price, int quantity);
    double getPrice() const override;
};

#endif // PRODUCT_H
