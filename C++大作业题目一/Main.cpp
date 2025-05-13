// Main.cpp
#include "User.h"
#include "Product.h"
#include "DataHandler.h"
#include "Utility.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <limits> // ����������뻺����

User* currentUser; 
void displayMainMenu() {
    std::cout << "\n===== ����ƽ̨ϵͳ =====" << std::endl;
    std::cout << "1. �û�ע��" << std::endl;
    std::cout << "2. �û���¼" << std::endl;
    std::cout << "3. �޸�����" << std::endl;
    std::cout << "4. �鿴��Ʒ��Ϣ" << std::endl;
	std::cout << "5. �˳�ϵͳ" << std::endl;
    std::cout << "��ѡ��: ";
}
void registerUser() {
    std::string username, password, type;
    double balance;
    int typeInt;
    std::cout << "�������û���: ";
    std::cin >> username;
    std::cout << "����������: ";
    std::cin >> password;
    std::cout << "�������ʼ���: ";
    std::cin >> balance;
    std::cout << "�������˻����� (����1����Consumer/2����Seller): ";
    std::cin >> typeInt;
    if (typeInt == 1) {
        type = "Consumer";
    }
    else if (typeInt == 2) {
        type = "Seller";
    }
    else {
        std::cout << "��Ч���˻����ͣ�" << std::endl;
        return;
    }
    if (type == "Consumer") {
        Consumer user(username, password, balance);
        DataHandler::saveUser(user);
    }
    else if (type == "Seller") {
        Seller user(username, password, balance);
        DataHandler::saveUser(user);
    }
    else {
        std::cout << "��Ч���˻����ͣ�" << std::endl;
    }
}
void loginUser() {
    std::string username, password;

    std::cout << "�������û���: ";
    std::cin >> username;
    std::cout << "����������: ";
    std::cin >> password;

    currentUser = DataHandler::loginUser(username, password);
    std::cout << "�û�Ϊ��" << currentUser->getUserType() << std::endl;
}
void changePassword() {
    std::string username, oldPassword, newPassword;

    std::cout << "�������û���: ";
    std::cin >> username;
    std::cout << "�����������: ";
    std::cin >> oldPassword;
    std::cout << "������������: ";
    std::cin >> newPassword;

    DataHandler::updatePassword(username, oldPassword, newPassword);
}
void MassageInCommodity()
{
	std::cout << "��ӭ������Ʒ��Ϣ���棡" << std::endl;
	std::cout << "1. �鿴��Ʒ��Ϣ" << std::endl;
	std::cout << "2. ������Ʒ" << std::endl;
}
void ShowCommodity()
{
    DataHandler::ShowProduct();
}
void AddProduct()
{
	std::string name, description;
	double price;
	int quantity;
    // ��ȡ��Ʒ���ƣ����У�
    std::cout << "��������Ʒ����: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ���������
    std::getline(std::cin, name);

    // ��ȡ��Ʒ���������У�
    std::cout << "��������Ʒ����: ";
    std::getline(std::cin, description);

    // ��ȡ�۸����������ֵ�ͣ�
    std::cout << "��������Ʒ�۸�: ";
    while (!(std::cin >> price)) {
        std::cin.clear(); // �������״̬
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "������Ч������������۸�: ";
    }

    std::cout << "��������Ʒ����: ";
    while (!(std::cin >> quantity)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "������Ч����������������: ";
    }
	Product* product = new Book(name, description, price, quantity);
	DataHandler::saveProduct(*product);
	delete product; // �ͷŶ�̬������ڴ�
}
bool CommodityPage()//��ѡ���˳���Ʒ��Ϣ����ʱ������true
{
	
	int choice;
	std::cin >> choice;
	switch (choice) {
	case 1:
		std::cout << "��Ʒ��Ϣ���£�" << std::endl;
		ShowCommodity();
		break;
	case 2:
		if (currentUser->getUserType() == "Seller") {//�жϵ�ǰ�û��Ƿ�Ϊ����
			std::cout << "����������Ʒ" << std::endl;
            AddProduct();
		}
		else {
			std::cout << "��û��Ȩ��������Ʒ��" << std::endl;//�������ң�����������Ʒ
		}
		break;
	case 3:
		std::cout << "�˳���Ʒ��Ϣ����" << std::endl;
        return true;
	default:
		std::cout << "��Чѡ������ԡ�" << std::endl;
	}
	return false;//����������Ʒ��Ϣ
}

int main() {
    int choice;
    while (true) {
        displayMainMenu();
        std::cin >> choice;
        switch (choice) {
        case 1:
            registerUser();
            break;
        case 2:
            loginUser(); 
            while (true)
            {
                MassageInCommodity();

                if (CommodityPage())
                {
					break;//�˳���Ʒ��Ϣ����
				}
                else
                {
                    std::cout << "����������Ʒ��Ϣ" << std::endl;
                }
            }
            break;
        case 3:
            changePassword(); 
            return 0;
        case 4:
            std::cout << "�鿴��Ʒ��Ϣ" << std::endl;
			std::cout << "��Ʒ��Ϣ���£�" << std::endl;
            ShowCommodity();
            break;
        case 5:
            std::cout << "�˳�ϵͳ" << std::endl;
            return 0;
        default:
            std::cout << "��Чѡ������ԡ�" << std::endl;
        }
    }
}
