// User.cpp
#include "User.h"
// User �๹�캯������ʼ���û�������������
User::User(const std::string& username, const std::string& password, double balance)
    : username(username), password(password), balance(balance) {
}

std::string User::getUsername() const {
    return username;// ���ص�ǰ�û����û���
}
std::string User::getPassword() const {
    return password;// ���ص�ǰ�û�������
}
bool User::checkPassword(const std::string& password) const {
    return this->password == password;// �Ƚ������������洢������
}

void User::updatePassword(const std::string& newPassword) {
    password = newPassword;// ����������Ϊ������
}

void User::recharge(double amount) {
    if (amount > 0) {
        balance += amount;// �������
    }
}

bool User::consume(double amount) {
    if (amount > 0 && balance >= amount) {
        balance -= amount;// �۳����
        return true;
    }
    return false;// �����Ч�����㣬��������ʧ��
}

double User::getBalance() const {
    return balance;// ���ص�ǰ���
}
// Seller �๹�캯�������û��� User �Ĺ��캯��
Seller::Seller(const std::string& username, const std::string& password, double balance)
    : User(username, password, balance) {
}
std::string Seller::getPassword() const {
    return password;// ���ص�ǰ�û�������
}
std::string Seller::getUserType() const {
    return "Seller";// �̶����� "Seller"
}
// Consumer �๹�캯�������û��� User �Ĺ��캯��
Consumer::Consumer(const std::string& username, const std::string& password, double balance)
    : User(username, password, balance) {
}
std::string Consumer::getPassword() const {
    return password;// ���ص�ǰ�û�������
}
std::string Consumer::getUserType() const {
    return "Consumer";// �̶����� "Consumer"
}
