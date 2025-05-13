//std::string 是C++标准库（<string>）提供的字符串类，用于代替C风格的 char 数组。它封装了字符串的存储和管理，提供了丰富的成员函数，使得字符串操作更加安全和方便。
#ifndef USER_H
#define USER_H // 头文件保护

#include <string>
#include <iostream>

class User {
protected:// 保护成员，子类可以访问
    std::string username;// 用户名
    std::string password;// 密码
    double balance;// 账户余额

public:
    // 构造函数，初始化用户名、密码和余额
    User(const std::string& username, const std::string& password, double balance);
    // 虚析构函数，确保派生类对象被正确销毁
    virtual ~User() = default;
    virtual std::string getUserType() const = 0;// 纯虚函数，获取用户类型，需要在派生类中实现
    virtual std::string getPassword() const = 0; // 获取密码
    std::string getUsername() const;
    bool checkPassword(const std::string& password) const;
    void updatePassword(const std::string& newPassword);
    void recharge(double amount);// 充值余额
    bool consume(double amount);// 消费金额，如果余额不足则返回false
    double getBalance() const;// 获取当前余额
};

class Seller : public User {
public:
    Seller(const std::string& username, const std::string& password, double balance);
    std::string getUserType() const override;
	std::string getPassword() const override;
};

class Consumer : public User {
public:
    Consumer(const std::string& username, const std::string& password, double balance);
    std::string getUserType() const override;
	std::string getPassword() const override;
};

#endif // USER_H
