// DataHandler.h
#ifndef DATA_HANDLER_H  // ͷ�ļ������꣬��ֹ�ظ�����
#define DATA_HANDLER_H

#include "User.h"      // �û��ඨ��
#include "Product.h"   // ��Ʒ�ඨ��
#include <vector>      // ��������
#include <string>      // �ַ�����

const std::string DATA_PATH = "E:/SS/Data/";
class DataHandler {
public:
    static void saveUser(const User& user);

    static std::vector<User*> loadUsers();

    static void saveProduct(const Product& product);
    static std::vector<Product*> ShowProduct();

    static std::vector<Product*> loadProducts();

    static User* loginUser(const std::string& username, const std::string& password);
    static bool updatePassword(const std::string& username, const std::string& oldPassword, const std::string& newPassword);
    static std::vector<Product*> getProductByName(const std::string& name);
    static std::vector<Product*>getProductByType(const std::string& type);
    static void discountByType(const std::string& type, double discountRate, std::string ownername);
    static void updateProduct(int idToUpdate, const std::string& newName,
        const std::string& newDescription, double newPrice, int newQuantity, std::string UserName);
    static void deleteProduct(int idToDelete, std::string UserName);
    static Product* getProductById(int ID);
};
#endif // DATA_HANDLER_H