// DataHandler.h
#ifndef DATA_HANDLER_H  // ͷ�ļ������꣬��ֹ�ظ�����
#define DATA_HANDLER_H

#include "User.h"      // �û��ඨ��
#include "Product.h"   // ��Ʒ�ඨ��
#include <vector>      // ��������
#include <string>      // �ַ�����


class DataHandler {
public:
  
    static void saveUser(const User& user);

    static std::vector<User*> loadUsers();

    static void saveProduct(const Product& product);
	static void ShowProduct();
 
    static std::vector<Product*> loadProducts();

    static User* loginUser(const std::string& username, const std::string& password);
    static bool updatePassword(const std::string& username, const std::string& oldPassword, const std::string& newPassword);
};

#endif // DATA_HANDLER_H