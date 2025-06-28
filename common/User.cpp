// User.cpp
#include "User.h"
// User 类构造函数，初始化用户名、密码和余额
User::User(const std::string& username, const std::string& password, double balance)
    : username(username), password(password), balance(balance) {
}

std::string User::getUsername() const {
    return username;// 返回当前用户的用户名
}
std::string User::getPassword() const {
    return password;// 返回当前用户的密码
}
bool User::checkPassword(const std::string& password) const {
    return this->password == password;// 比较输入的密码与存储的密码
}

void User::updatePassword(const std::string& newPassword) {
    password = newPassword;// 将密码设置为新密码
}

void User::recharge(double amount) {
    if (amount > 0) {
        balance += amount;// 增加余额
    }
}

bool User::consume(double amount) {
    if (amount > 0 && balance >= amount) {
        balance -= amount;// 扣除金额
        return true;
    }
    return false;// 金额无效或余额不足，返回消费失败
}

double User::getBalance() const {
    return balance;// 返回当前余额
}
// Seller 类构造函数，调用基类 User 的构造函数
Seller::Seller(const std::string& username, const std::string& password, double balance)
    : User(username, password, balance) {
}
std::string Seller::getPassword() const {
    return password;// 返回当前用户的密码
}
std::string Seller::getUserType() const {
    return "Seller";// 固定返回 "Seller"
}
// Consumer 类构造函数，调用基类 User 的构造函数
Consumer::Consumer(const std::string& username, const std::string& password, double balance)
    : User(username, password, balance) {
}
std::string Consumer::getPassword() const {
    return password;// 返回当前用户的密码
}
std::string Consumer::getUserType() const {
    return "Consumer";// 固定返回 "Consumer"
}
