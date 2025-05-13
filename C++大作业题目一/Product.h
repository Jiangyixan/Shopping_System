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
    int quantity;// ��Ʒ�������

public:
    // ���캯������ʼ����Ʒ������Ϣ��
    Product(const std::string& name, const std::string& description, double price, int quantity);
    virtual ~Product() = default;// ������������֧�ֶ�̬���٣�
    virtual double getPrice() const = 0;// ���麯������ȡ���ռ۸�������ʵ�ֲ�ͬ���۲��ԣ�
    std::string getName() const;// ��ȡ��Ʒ����
    std::string getDescription() const;// ��ȡ��Ʒ����
    int getQuantity() const; // ��ȡ��Ʒ�������
    void updatePrice(double newPrice);// ���²�Ʒ�۸�
    void updateQuantity(int newQuantity);// ���²�Ʒ�������
};

class Book : public Product {
public:
    Book(const std::string& name, const std::string& description, double price, int quantity);
    double getPrice() const override; // ʵ�ּ۸����//const��ŵ�����޸�����κγ�Ա����override��ʽ������д�麯��
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
