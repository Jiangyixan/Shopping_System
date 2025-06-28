#ifndef CARTITEM_H
#define CARTITEM_H
#include <string>

class CartItem {
public:
    int cartItemId;           // ÿ�����ﳵ���Ψһ���
    std::string productName;  // ��Ʒ����
    int quantity;             // ����
    std::string seller; // ��������
    double price;             // ��Ʒ�۸�
    CartItem(int cartItemId, const std::string& productName, int quantity, std::string seller, double price)
        : cartItemId(cartItemId), productName(productName), quantity(quantity), seller(seller), price(price) {
    }
};

#endif

