
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
    static int nextItemId; // 自增的购物车项 ID
    std::string username; // 加一个保存用户名的字段

    void loadNextId();      // 从文件加载ID
    void saveNextId();      // 保存ID到文件
public:

    ShoppingCart(const std::string& username); //构造函数
    std::string getName();
    void addItem(const std::string& productName, int quantity, std::string seller, double price);
    void removeItem(int id);
    void updateItemQuantity(int cartItemId, int quantity);
    void displayCart() const;
    const std::vector<CartItem>& getItems() const;
    void saveToFile();
    void loadFromFile();



    // 新添加的结算函数
    int checkout(Consumer* buyer, std::vector<User*>& allUsers);
    // 计算购物车总金额
    double calculateTotal() const;
    // 显示订单预览
    void displayOrderPreview() const;
};

#endif
