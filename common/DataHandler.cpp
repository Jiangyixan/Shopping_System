// DataHandler.cpp
#include "DataHandler.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory> // ��������ָ��
void DataHandler::saveUser(const User& user) {//�û���Ϣд���ļ�
    std::ofstream file("../Data/users.txt", std::ios::app);
    if (loginUser(user.getUsername(), user.getPassword()) != NULL) {//����û��Ѿ�����
        std::cout << "�û��Ѵ��ڣ�" << std::endl;
        return;
    }
    else//�û�������
    {
        if (file.is_open()) {
            file << user.getUsername() << " " << user.getBalance() << " " << user.getUserType() << " " << user.getPassword() << "\n";
            file.close();
        }
        else {
            std::cerr << "Error: �޷���users.txt ����д�����" << std::endl;
        }
    }
}
//ʹ��User*��̬�ڴ�滮�������ļ����ݲ��������ļ����ݷ���
std::vector<User*> DataHandler::loadUsers() {
    std::vector<User*> users;
    std::ifstream file(DATA_PATH + "users.txt");
    if (file.is_open()) {
        std::string username, userType, password;
        double balance;
        while (file >> username >> balance >> userType >> password) {
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
void DataHandler::saveProduct(const Product& product) {
    std::ofstream file("../Data/products.txt", std::ios::app);
    if (file.is_open()) {
        file << product.getId() << "\t" // �����ƷID
            << product.getName() << "\t"
            << product.getDescription() << "\t"
            << product.getPrice() << "\t"
            << product.getQuantity() << "\t"
            << product.getOwnername() << "\t";

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
    std::ifstream file(DATA_PATH + "products.txt");

    if (file.is_open()) {

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            int id;
            std::string name, description, type;
            double price;
            int quantity;
            std::string ownername;

            iss >> id;                  // ��ȡ ID
            iss.ignore(1, '\t');         // ���� 1 �� \t
            std::getline(iss, name, '\t');  // ��ȡ name��ֱ�� \t��
            std::getline(iss, description, '\t');  // ��ȡ description��ֱ�� \t��
            iss >> price >> quantity;    // ��ȡ price �� quantity
            iss.ignore(1, '\t');         // ���� \t
            std::getline(iss, ownername, '\t');  // ��ȡ ownername��ֱ�� \t��
            iss >> type;                 // ��ȡ type�����һ���ֶΣ�����Ҫ ignore��
            if (type == "Book") {
                products.push_back(new Book(id, name, description, price, quantity, ownername));
            }
            else if (type == "Food") {
                products.push_back(new Food(id, name, description, price, quantity, ownername));
            }
            else if (type == "Clothing") {
                products.push_back(new Clothing(id, name, description, price, quantity, ownername));
            }
            // ��������...
        }
        file.close();
    }
    else {
        std::cerr << "Error: Unable to open products.txt for reading." << std::endl;
    }
    return products;
}
/*std::string type, name, description;
double price;
int quantity;
std::getline(iss, name, '\t'); // ������Ʊ���ָ�
std::getline(iss, description, '\t');
iss >> price >> quantity >> type;
while (file >> name >> description >> price >> quantity >> type) {
    if (type == "Book") {
        products.push_back(new Book(name, description, price, quantity));
    }
    else if (type == "Food") {
        products.push_back(new Food(name, description, price, quantity));
    }
    else if (type == "Clothing") {
        products.push_back(new Clothing(name, description, price, quantity));
    }
}*/

//��������
bool DataHandler::updatePassword(const std::string& username, const std::string& oldPassword, const std::string& newPassword) {
    std::vector<User*> users = loadUsers();
    bool found = false;

    for (User* user : users) {
        if (user->getUsername() == username && user->getPassword() == oldPassword) {
            user->updatePassword(newPassword);
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "�û�������������޷��޸����룡" << std::endl;
        return false;
    }

    // ����д���ļ�
    std::ofstream file("../Data/users.txt", std::ios::out | std::ios::trunc);
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
std::vector<Product*> DataHandler::ShowProduct()
{
    std::vector<Product*> products = DataHandler::loadProducts();
    std::cout << "��Ʒ�б�:" << std::endl;
    int show = 0;
    // ��������ʾ��Ʒ��Ϣ
    for (const auto& product : products) {

        show = 1;
    }
    if (show == 0) {
        std::cout << "û����Ʒ��Ϣ��" << std::endl;
    }
    else {
        return products;
    }
    // �ͷ��ڴ�
    for (auto& product : products) {
        delete product;
    }
}
std::vector<Product*> DataHandler::getProductByName(const std::string& name) {
    std::vector<Product*> products = loadProducts();
    std::vector<Product*> productsResult;

    for (Product* product : products) {
        if (product->getName().find(name) != std::string::npos) {
            productsResult.push_back(product);
        }
    }

    if (productsResult.empty()) {
        std::cout << "û���ҵ�����Ʒ��" << std::endl;
    }

    return productsResult;
}
Product* DataHandler::getProductById(int ID) {
    std::vector<Product*> products = loadProducts();
    for (Product* product : products) {
        if (product->getId() == ID) {
            return product;  // ֱ�ӷ���ƥ��� product
        }
    }
    std::cout << "û���ҵ�����Ʒ��" << std::endl;
    return nullptr;  // δ�ҵ�ʱ���� nullptr
}

std::vector<Product*> DataHandler::getProductByType(const std::string& type) {
    std::vector<Product*> products = loadProducts();
    std::vector<Product*> productsResult;
    for (Product* product : products) {
        if (product->getProductType() == type) {
            productsResult.push_back(product);
        }
    }
    if (productsResult.empty()) {
        std::cout << "û���ҵ���������Ʒ��" << std::endl;
    }
    return productsResult;
}
void DataHandler::discountByType(const std::string& type, double discountRate, std::string ownername) {
    std::vector<Product*> products = loadProducts(); // ����������Ʒ
    bool found = false;

    for (Product* product : products) {
        if (product->getProductType() == type && (product->getOwnername().find(ownername) != std::string::npos)) {

            double oldPrice = product->getPrice();
            double newPrice = oldPrice * discountRate;
            product->updatePrice(newPrice);
            found = true;
        }
    }

    if (!found) {
        std::cout << "û���ҵ�����Ϊ " << type << " ����Ʒ��" << std::endl;
        return;
    }

    // ���ļ�������д��
    std::ofstream file("../Data/products.txt", std::ios::trunc); // �����д��
    if (file.is_open()) {
        for (Product* product : products) {
            file << product->getId() << "\t" // �����ƷID
                << product->getName() << "\t"
                << product->getDescription() << "\t"
                << product->getPrice() << "\t"
                << product->getQuantity() << "\t"
                << product->getOwnername() << "\t";

            if (dynamic_cast<Book*>(product)) {
                file << "Book";
            }
            else if (dynamic_cast<Food*>(product)) {
                file << "Food";
            }
            else if (dynamic_cast<Clothing*>(product)) {
                file << "Clothing";
            }
            file << "\n";
        }
        file.close();
        std::cout << "�ѳɹ�������Ϊ " << type << " ����Ʒ���۲����棡" << std::endl;
    }
    else {
        std::cerr << "�޷����ļ����б��棡" << std::endl;
    }

    // �ͷ��ڴ棨�����������ָ�룩
    for (Product* p : products) {
        delete p;
    }
}
void DataHandler::updateProduct(int idToUpdate, const std::string& newName,
    const std::string& newDescription, double newPrice, int newQuantity, std::string UserName) {
    std::vector<Product*> products = loadProducts();
    bool found = false;

    for (Product* product : products) {
        if (product->getId() == idToUpdate) {
            if ((product->getOwnername().find(UserName) == std::string::npos))
            {
                std::cout << "�Ǵ���Ʒӵ���ߣ���û��Ȩ���޸ĸ���Ʒ��" << std::endl;
                return;
            }
            // �޸���Ϣ
            product->setName(newName);
            product->setDescription(newDescription);
            product->setPrice(newPrice);
            product->setQuantity(newQuantity);
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "δ�ҵ�IDΪ " << idToUpdate << " ����Ʒ��\n";
        return;
    }

    // ����д���ļ�
    std::ofstream file("../Data/products.txt", std::ios::trunc); // ����ļ�
    if (file.is_open()) {
        for (Product* product : products) {
            file << product->getId() << "\t"
                << product->getName() << "\t"
                << product->getDescription() << "\t"
                << product->getPrice() << "\t"
                << product->getQuantity() << "\t"
                << product->getOwnername() << "\t";

            if (dynamic_cast<Book*>(product)) file << "Book";
            else if (dynamic_cast<Food*>(product)) file << "Food";
            else if (dynamic_cast<Clothing*>(product)) file << "Clothing";

            file << "\n";
        }
        file.close();
        std::cout << "��Ʒ�޸ĳɹ���\n";
    }
    else {
        std::cerr << "�޷�д���ļ� products.txt\n";
    }

    for (Product* p : products) delete p; // ��ֹ�ڴ�й©
}
void DataHandler::deleteProduct(int idToDelete, std::string UserName) {
    std::vector<Product*> products = loadProducts();
    bool found = false;

    auto it = products.begin();
    while (it != products.end()) {
        if ((*it)->getId() == idToDelete) {
            if ((*it)->getOwnername().find(UserName) == std::string::npos)
            {
                std::cout << "�Ǵ���Ʒӵ���ߣ���û��Ȩ��ɾ������Ʒ��" << std::endl;
                return;
            }

            std::cout << "ȷ��ɾ�� ID Ϊ " << idToDelete << "����Ʒ����Ϊ ["
                << (*it)->getName() << "] ����Ʒ��(1.�� 2.��): ";

            int choice;
            std::cin >> choice;
            if (choice == 1) {
                delete* it;                // �ͷŸ���Ʒ�ڴ�
                it = products.erase(it);   // ���б����Ƴ�
                found = true;
                break;
            }
            else {
                std::cout << "ȡ��ɾ����Ʒ��\n";
                break;  // ��Ȼûɾ����ҲҪ����ѭ��
            }
        }
        else {
            ++it;
        }
    }

    if (!found) {
        std::cout << "δ�ҵ� ID Ϊ " << idToDelete << " ����Ʒ��\n";
    }
    else {
        // ���±���ʣ����Ʒ
        std::ofstream file("../Data/products.txt", std::ios::trunc);
        if (file.is_open()) {
            for (const auto& product : products) {
                file << product->getId() << "\t"
                    << product->getName() << "\t"
                    << product->getDescription() << "\t"
                    << product->getPrice() << "\t"
                    << product->getQuantity() << "\t"
                    << product->getOwnername() << "\t";

                if (dynamic_cast<Book*>(product)) file << "Book";
                else if (dynamic_cast<Food*>(product)) file << "Food";
                else if (dynamic_cast<Clothing*>(product)) file << "Clothing";

                file << "\n";
            }
            file.close();
            std::cout << "��Ʒɾ���ɹ���\n";
        }
        else {
            std::cerr << "�޷�д���ļ� products.txt\n";
        }
    }

    // �����Ƿ�ɾ�������Ҫ�ͷ� vector ʣ���ָ���ڴ�
    for (auto& p : products) delete p;
}


