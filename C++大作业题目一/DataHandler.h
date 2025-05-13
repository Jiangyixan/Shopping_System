// DataHandler.h
#ifndef DATA_HANDLER_H  // 头文件保护宏，防止重复包含
#define DATA_HANDLER_H

#include "User.h"      // 用户类定义
#include "Product.h"   // 产品类定义
#include <vector>      // 向量容器
#include <string>      // 字符串库


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