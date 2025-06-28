#ifndef CARTITEM_H
#define CARTITEM_H
#include <string>

class CartItem {
public:
    int cartItemId;           // 每个购物车项的唯一编号
    std::string productName;  // 商品名称
    int quantity;             // 数量
    std::string seller; // 卖家名称
    double price;             // 商品价格
    CartItem(int cartItemId, const std::string& productName, int quantity, std::string seller, double price)
        : cartItemId(cartItemId), productName(productName), quantity(quantity), seller(seller), price(price) {
    }
};

#endif

