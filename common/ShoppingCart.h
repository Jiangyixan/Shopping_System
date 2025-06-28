
#ifndef SHOPPINGCART_H
#define SHOPPINGCART_H
//#define cartnull 0
//#define not_enough_money -1
//#define quit 2
//#define paying_error 3
//#define success 1
#include <vector>
#include <string>
#include "CartItem.h"
#include "User.h"

class ShoppingCart {
private:
    std::vector<CartItem> items;
    static int nextItemId; // �����Ĺ��ﳵ�� ID
    std::string username; // ��һ�������û������ֶ�

    void loadNextId();      // ���ļ�����ID
    void saveNextId();      // ����ID���ļ�
public:

    ShoppingCart(const std::string& username); //���캯��
    std::string getName();
    void addItem(const std::string& productName, int quantity, std::string seller, double price);
    void removeItem(int id);
    void updateItemQuantity(int cartItemId, int quantity);
    void displayCart() const;
    const std::vector<CartItem>& getItems() const;
    void saveToFile();
    void loadFromFile();



    // ����ӵĽ��㺯��
    int checkout(Consumer* buyer, std::vector<User*>& allUsers);
    // ���㹺�ﳵ�ܽ��
    double calculateTotal() const;
    // ��ʾ����Ԥ��
    void displayOrderPreview() const;
};

#endif
