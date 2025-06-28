#pragma once
// Product.h
#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <iostream>
#include"User.h"
class Product {
protected:
    static int nextId;
    static void loadNextId();      // 从文件加载ID
    static void saveNextId();      // 保存ID到文件

    int id;
    std::string name;
    std::string description;
    double price;
    int quantity;// 产品库存数量
    std::string ownername;
public:
    Product(const std::string& name, const std::string& description, double price, int quantity, std::string ownername);
    // 构造函数（初始化产品基本信息）
    Product(int id, const std::string& name, const std::string& description, double price, int quantity, std::string ownername);

    virtual ~Product() = default;// 虚析构函数（支持多态销毁）
    int getId() const;
    virtual double getPrice() const = 0;// 纯虚函数：获取最终价格（需子类实现不同定价策略）
    std::string getName() const;// 获取产品名称
    std::string getDescription() const;// 获取产品描述
    int getQuantity() const; // 获取产品库存数量
    std::string getOwnername() const { return ownername; } // 获取产品拥有者名称
    void setName(const std::string& n) { name = n; }
    void setDescription(const std::string& d) { description = d; }
    void setPrice(double p) { price = p; }
    void setQuantity(int q) { quantity = q; }
    std::string setOwnername(const std::string& o) { ownername = o; } // 设置产品拥有者名称
    void updatePrice(double newPrice);// 更新产品价格
    void updateQuantity(int newQuantity);// 更新产品库存数量
    virtual std::string getProductType() const = 0; // 获得产品类型
};

class Book : public Product {
public:
    Book(int id, const std::string& name, const std::string& description, double price, int quantity, std::string ownername);
    Book(const std::string& name, const std::string& description, double price, int quantity, std::string ownername);

    double getPrice() const override; // 实现价格计算//const承诺不会修改类的任何成员变量override显式声明重写虚函数
    std::string getProductType() const override;

};

class Food : public Product {
public:
    Food(int id, const std::string& name, const std::string& description, double price, int quantity, std::string ownername);
    Food(const std::string& name, const std::string& description, double price, int quantity, std::string ownername);

    double getPrice() const override;
    std::string getProductType() const override;
};

class Clothing : public Product {
public:
    Clothing(int id, const std::string& name, const std::string& description, double price, int quantity, std::string ownername);
    Clothing(const std::string& name, const std::string& description, double price, int quantity, std::string ownername);
    double getPrice() const override;
    std::string getProductType() const override;
};

#endif // PRODUCT_H
