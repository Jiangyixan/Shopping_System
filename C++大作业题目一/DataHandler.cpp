// DataHandler.cpp
#include "DataHandler.h"
#include <fstream>
#include <iostream>

void DataHandler::saveUser(const User& user) {//�û���Ϣд���ļ�
    std::ofstream file("users.txt", std::ios::app);
    if (file.is_open()) {
        file << user.getUsername() << " " << user.getBalance() << " " << user.getUserType() << " " << user.getPassword()<<"\n";
        file.close();
    }
    else {
        std::cerr << "Error: �޷���users.txt ����д�����" << std::endl;
    }
}
//ʹ��User*��̬�ڴ�滮�������ļ����ݲ��������ļ����ݷ���
std::vector<User*> DataHandler::loadUsers() {
    std::vector<User*> users;
    std::ifstream file("users.txt");
    if (file.is_open()) {
        std::string username, userType,password;
        double balance;
        while (file >> username >> balance >> userType>>password) {
            if (userType == "Seller") {
                users.push_back(new Seller(username, password, balance));
            }
            else if (userType == "Consumer") {
                users.push_back(new Consumer(username, password, balance));
            }
        }
        file.close();
    }
    else {
        std::cerr << "Error: �޷���ȡ users.txt " << std::endl;
    }
    return users;
}

void DataHandler::saveProduct(const Product& product) {
    std::ofstream file("products.txt", std::ios::app);
    if (file.is_open()) {
        file << product.getName() << " "
            << product.getDescription() << " "
            << product.getPrice() << " "
            << product.getQuantity() << " ";

        if (dynamic_cast<const Book*>(&product)) {
            file << "Book";
        }
        else if (dynamic_cast<const Food*>(&product)) {
            file << "Food";
        }
        else if (dynamic_cast<const Clothing*>(&product)) {
            file << "Clothing";
        }

        file << "\n";
        file.close();
    }
    else {
        std::cerr << "Error: �޷��� products.txt ����д��" << std::endl;
    }
}
//ʹ��Product*��̬�ڴ�滮�������ļ����ݲ��������ļ����ݷ���

std::vector<Product*> DataHandler::loadProducts() {
    std::vector<Product*> products;
    std::ifstream file("products.txt");

    if (file.is_open()) {
        std::string type, name, description;
        double price;
        int quantity;

        while (file >> type >> name >> price >> quantity) {
            if (type == "Book") {
                products.push_back(new Book(name, "ͼ����Ʒ", price, quantity));
            }
            else if (type == "Food") {
                products.push_back(new Food(name, "ʳƷ��Ʒ", price, quantity));
            }
            else if (type == "Clothing") {
                products.push_back(new Clothing(name, "��װ��Ʒ", price, quantity));
            }
        }
        file.close();
    }
    else {
        std::cerr << "Error: Unable to open products.txt for reading." << std::endl;
    }

    return products;
}
//��¼�û�
User* DataHandler::loginUser(const std::string& username, const std::string& password) {
    std::vector<User*> users = loadUsers();
    for (User* user : users) {
        //std::cout << user->getUsername() <<user->getBalance()<<user->getPassword()<< std::endl;
        if (user->getUsername() == username && user->checkPassword(password)) {//�û�������ƥ��
            std::cout << "��ӭ, " << username << "! ��¼�ɹ���" << std::endl;
            return user;
        }
    }
    std::cout << "�û������������" << std::endl;
    return NULL;

}

//��������
bool DataHandler::updatePassword(const std::string& username, const std::string& oldPassword, const std::string& newPassword) {
    std::vector<User*> users = loadUsers();
    bool found = false;

    for (User* user : users) {
        if (user->getUsername() == username && user->getPassword() == oldPassword) {
            user ->updatePassword( newPassword);
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "�û�������������޷��޸����룡" << std::endl;
        return false;
    }

    // ����д���ļ�
    std::ofstream file("users.txt", std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "�޷����û��ļ����и��£�" << std::endl;
        return false;
    }

    for (User* user : users) {
        file << user->getUsername() << " " 
            << user->getBalance() << " " 
            << user->getUserType() << " " 
            << user->getPassword() << "\n";
    }

    file.close();
    std::cout << "�����޸ĳɹ���" << std::endl;
    return true;
}
void DataHandler::ShowProduct()
{
	std::vector<Product*> products = DataHandler::loadProducts();
	std::cout << "��Ʒ�б�:" << std::endl;
    int show = 0;
	// ��������ʾ��Ʒ��Ϣ
	for (const auto& product : products) {
		std::cout << "����: " << product->getName() << ", ����: " << product->getDescription() << ", �۸�: " << product->getPrice() << ", ���: " << product->getQuantity() << std::endl;
        show = 1;
	}
	if (show == 0) {
		std::cout << "û����Ʒ��Ϣ��" << std::endl;
	}
	else {
		std::cout << "��Ʒ��Ϣ��ʾ��ϣ�" << std::endl;
	}
	// �ͷ��ڴ�
	for (auto& product : products) {
		delete product;
	}
}