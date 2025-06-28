// ShoppingCart.cpp
#include "ShoppingCart.h"  // ������Ӧ��ͷ�ļ�
#include "CartItem.h"     // ���� CartItem ��
#include"User.h"
#include "DataHandler.h"
#include <iostream>        // ���ڱ�׼����������� displayCart��
#include <fstream>         // �����ļ�������saveToFile / loadFromFile��
#include <string>         // �����ַ�������
#include <sstream>
#include <vector>
#include <memory> // ��������ָ��
#include <map>           // ���� map ����
#include <algorithm>     // ���� std::remove_if
int ShoppingCart::nextItemId = 1;
void ShoppingCart::loadNextId() {
    std::ifstream in("../Data/id_" + getName() + ".txt");
    if (in.is_open()) {
        in >> nextItemId;
        in.close();
    }
}
void ShoppingCart::saveNextId() {
    std::ofstream out("../Data/id_" + getName() + ".txt");
    if (out.is_open()) {
        out << nextItemId;
        out.close();
    }
}
ShoppingCart::ShoppingCart(const std::string& username)
    : username(username) {
    loadFromFile(); // ����һ��ʼ�ͼ��ع��ﳵ
}
std::string ShoppingCart::getName()
{
    return username;
}
void ShoppingCart::addItem(const std::string& productName, int quantity, std::string seller, double price) {
    for (auto& item : items) {
        if (item.productName == productName) {
            item.quantity += quantity;
            saveToFile();  // ÿ����Ӷ�����
            return;
        }
    }
    if (nextItemId == 1) {
        loadNextId(); // ֻ�ڳ����һ�δ�������ʱ��ȡһ��
    }
    saveNextId(); // ÿ�δ�������󱣴浱ǰ ID
    items.emplace_back(nextItemId++, productName, quantity, seller, price);
    saveNextId();
    saveToFile();  //  �������ƷҲҪ����
}

void ShoppingCart::removeItem(int cartItemId) {
    items.erase(std::remove_if(items.begin(), items.end(),
        [&](const CartItem& item) { return item.cartItemId == cartItemId; }),
        items.end());
    saveToFile();  // ɾ���󱣴�
}

void ShoppingCart::updateItemQuantity(int cartItemId, int quantity) {
    for (auto& item : items) {
        if (item.cartItemId == cartItemId) {
            item.quantity = quantity;
            saveToFile();  // ���������󱣴�
            return;
        }
    }
}

void ShoppingCart::displayCart() const {

    std::cout << "���ﳵ���ݣ�" << std::endl;
    if (items.empty()) {
        std::cout << "�����ﳵΪ�գ�" << std::endl;
    }
    else {
        for (const auto& item : items) {
            std::cout << "- ���ﳵ��ID: " << item.cartItemId
                << " ��Ʒ: " << item.productName
                << " ����: " << item.quantity
                << " ����: " << item.seller
                << " �۸�: " << item.price
                << std::endl;

        }
    }
}


// ��ȡ���ﳵ�е�������Ʒ�ֻ����
// ����ֵ:
//   const std::vector<CartItem>& - ���ﳵ��ĳ�������
const std::vector<CartItem>& ShoppingCart::getItems() const {
    return items;
}

// �����ﳵ���ݱ��浽�ļ�
// �ļ���ʽ: ÿ�д洢һ����Ʒ����ʽΪ "��Ʒ����\t����"
// ����:
//   username - ��������Ψһ�ļ������� cart_username.txt��
void ShoppingCart::saveToFile() {
    std::ofstream file("../Data/cart_" + username + ".txt");
    if (file.is_open()) {
        for (const auto& item : items) {
            file << item.cartItemId << "\t"
                << item.productName << "\t"
                << item.quantity << "\t"
                << item.seller << "\t"
                << item.price << "\n";
        }
        file.close();
    }
}

void ShoppingCart::loadFromFile() {
    items.clear();
    std::ifstream file("../Data/cart_" + username + ".txt");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            int id;
            std::string productName;
            int quantity;
            std::string seller;
            double price;

            // ��ȡID
            if (!(iss >> id)) continue;

            // ��ȡ��Ʒ���ƣ����ܰ����ո�
            char quote;
            //iss >> quote; // �������ܵ����Ż�ո�
            std::getline(iss >> std::ws, productName, '\t');

            // ��ȡ����������
            if (!(iss >> quantity >> seller >> price)) continue;

            items.emplace_back(id, productName, quantity, seller, price);
        }
        file.close();
    }
}




// ... existing code ...

double ShoppingCart::calculateTotal() const {
    std::map<std::string, double> sellerTotals;
    for (const auto& item : items) {
        double itemTotal = item.quantity * item.price;  // ʹ��ʵ����Ʒ�۸�
        sellerTotals[item.seller] += itemTotal;
    }
    double total = 0.0;
    for (const auto& sellerTotal : sellerTotals) {
        total += sellerTotal.second;
    }
    return total;
}

void ShoppingCart::displayOrderPreview() const {
    std::cout << "\n========== ����Ԥ�� ==========" << std::endl;
    std::cout << "�û�: " << username << std::endl;
    std::map<std::string, double> sellerTotals;

    std::cout << "\n��Ʒ�嵥:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    for (const auto& item : items) {
        double itemTotal = item.quantity * item.price;
        sellerTotals[item.seller] += itemTotal;

        std::cout << "��ƷID: " << item.cartItemId << std::endl
            << "����: " << item.productName << std::endl
            << "����: ��" << item.price << std::endl
            << "����: " << item.quantity << std::endl
            << "С��: ��" << itemTotal << std::endl
            << "����: " << item.seller << std::endl
            << "----------------------------------------" << std::endl;
    }

    std::cout << "\n�����ҽ����" << std::endl;
    for (const auto& sellerTotal : sellerTotals) {
        std::cout << "���� " << sellerTotal.first << ": ��" << sellerTotal.second << std::endl;
    }

    std::cout << "\n�ܼƽ��: ��" << calculateTotal() << std::endl;
    std::cout << "=====================================" << std::endl;
}

int ShoppingCart::checkout(Consumer* buyer, std::vector<User*>& allUsers) {
    if (items.empty()) {
        std::cout << "���ﳵΪ�գ��޷����㣡" << std::endl;
        return 0;//
    }

    // ����ÿ�����ҵ��ܽ��
    std::map<std::string, double> sellerTotals;
    for (const auto& item : items) {
        double itemTotal = item.quantity * item.price;
        sellerTotals[item.seller] += itemTotal;
    }

    // �����ܽ��
    double totalAmount = calculateTotal();

    // ����������Ƿ��㹻
    if (buyer->getBalance() < totalAmount) {
        std::cout << "���㣡" << std::endl;
        std::cout << "�����ܶ�: ��" << totalAmount << std::endl;
        std::cout << "��ǰ���: ��" << buyer->getBalance() << std::endl;
        std::cout << "����: ��" << totalAmount - buyer->getBalance() << std::endl;
        return -1;
    }

    // ��ʾ����Ԥ��
    displayOrderPreview();

    //// ȷ�Ϲ���
    //std::cout << "\nȷ��Ҫ������" << std::endl;
    //std::cout << "1. ȷ�Ϲ���" << std::endl;
    //std::cout << "2. ȡ������" << std::endl;
    //std::cout << "��ѡ��(1/2): ";

    //int choice;
    //std::cin >> choice;
    //if (choice != 1) {
    //    std::cout << "��ȡ������" << std::endl;
    //    return quit;
    //}

    // ִ�н���
    std::cout << "\n���ڴ�����..." << std::endl;

    // �ȿ۳���ҵ�Ǯ
    /*if (!buyer->consume(totalAmount)) {
        std::cout << "�ۿ�ʧ�ܣ�����ȡ����" << std::endl;
        return false;
    }*/
    for (User* user : allUsers) {
        if (user->getUsername() == buyer->getUsername() &&
            user->getUserType() == "Consumer") {
            user->consume(totalAmount);
            std::cout << " �ѿ۳���� " << buyer->getUsername()
                << " ��� ��" << totalAmount << std::endl;
            break;
        }
    }
    // ��ÿ������ת��
    bool allTransfersSuccessful = true;
    for (const auto& sellerTotal : sellerTotals) {
        bool sellerFound = false;
        for (User* user : allUsers) {
            if (user->getUsername() == sellerTotal.first &&
                user->getUserType() == "Seller") {
                user->recharge(sellerTotal.second);
                std::cout << " �������� " << sellerTotal.first
                    << " ת�� ��" << sellerTotal.second << std::endl;
                sellerFound = true;
                break;
            }
        }
        if (!sellerFound) {
            std::cout << "�� ����δ�ҵ����� " << sellerTotal.first << std::endl;
            allTransfersSuccessful = false;
        }
    }

    if (allTransfersSuccessful) {
        // �����û��ļ�
        std::ofstream file("../Data/users.txt", std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << "�����޷����û��ļ����и��£�" << std::endl;
            // �ع����н���
            //buyer->recharge(totalAmount);
            for (User* user : allUsers) {
                if (user->getUsername() == buyer->getUsername() &&
                    user->getUserType() == "Consumer") {
                    user->recharge(totalAmount);
                    break;
                }
            }
            for (const auto& sellerTotal : sellerTotals) {
                for (User* user : allUsers) {
                    if (user->getUsername() == sellerTotal.first &&
                        user->getUserType() == "Seller") {
                        user->consume(sellerTotal.second);
                        break;
                    }
                }
            }
            std::cout << "�� ����ʧ�ܣ��޷������û����ݣ�" << std::endl;
            std::cout << " �ѻع������ʽ�ת��" << std::endl;
            return 3 ;
        }

        // д����º���û�����
        for (User* user : allUsers) {
            file << user->getUsername() << " "
                << user->getBalance() << " "
                << user->getUserType() << " "
                << user->getPassword() << "\n";
        }
        file.close();

        // ��չ��ﳵ
        items.clear();
        saveToFile();
        std::cout << "\n ���׳ɹ���ɣ�" << std::endl;
        std::cout << " ��֧�����: ��" << totalAmount << std::endl;
        //std::cout << " ��ǰ���: ��" << buyer->getBalance() << std::endl;

        return 1;
    }
    else {
        // ������κ�ת��ʧ�ܣ��˻���ҵ�Ǯ
        for (User* user : allUsers) {
            if (user->getUsername() == buyer->getUsername() &&
                user->getUserType() == "Consumer") {
                user->recharge(totalAmount);
                break;
            }
        }

        // �����û��ļ�����Ϊ�Ѿ��˻�����ҵ�Ǯ��
        std::ofstream file("../Data/users.txt", std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << "���棺�޷������û��ļ������ʽ����˻���" << std::endl;
            return 3;
        }

        // д����º���û�����
        for (User* user : allUsers) {
            file << user->getUsername() << " "
                << user->getBalance() << " "
                << user->getUserType() << " "
                << user->getPassword() << "\n";
        }
        file.close();

        std::cout << "\n�� ����ʧ�ܣ�" << std::endl;
        std::cout << " ���˻����: ��" << totalAmount << std::endl;
        std::cout << " ��ǰ���: ��" << buyer->getBalance() << std::endl;

        return 3;
    }
}