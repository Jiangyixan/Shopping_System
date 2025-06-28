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
    static void loadNextId();      // ���ļ�����ID
    static void saveNextId();      // ����ID���ļ�

    int id;
    std::string name;
    std::string description;
    double price;
    int quantity;// ��Ʒ�������
    std::string ownername;
public:
    Product(const std::string& name, const std::string& description, double price, int quantity, std::string ownername);
    // ���캯������ʼ����Ʒ������Ϣ��
    Product(int id, const std::string& name, const std::string& description, double price, int quantity, std::string ownername);

    virtual ~Product() = default;// ������������֧�ֶ�̬���٣�
    int getId() const;
    virtual double getPrice() const = 0;// ���麯������ȡ���ռ۸�������ʵ�ֲ�ͬ���۲��ԣ�
    std::string getName() const;// ��ȡ��Ʒ����
    std::string getDescription() const;// ��ȡ��Ʒ����
    int getQuantity() const; // ��ȡ��Ʒ�������
    std::string getOwnername() const { return ownername; } // ��ȡ��Ʒӵ��������
    void setName(const std::string& n) { name = n; }
    void setDescription(const std::string& d) { description = d; }
    void setPrice(double p) { price = p; }
    void setQuantity(int q) { quantity = q; }
    std::string setOwnername(const std::string& o) { ownername = o; } // ���ò�Ʒӵ��������
    void updatePrice(double newPrice);// ���²�Ʒ�۸�
    void updateQuantity(int newQuantity);// ���²�Ʒ�������
    virtual std::string getProductType() const = 0; // ��ò�Ʒ����
};

class Book : public Product {
public:
    Book(int id, const std::string& name, const std::string& description, double price, int quantity, std::string ownername);
    Book(const std::string& name, const std::string& description, double price, int quantity, std::string ownername);

    double getPrice() const override; // ʵ�ּ۸����//const��ŵ�����޸�����κγ�Ա����override��ʽ������д�麯��
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
