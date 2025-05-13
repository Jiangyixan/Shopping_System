//std::string ��C++��׼�⣨<string>���ṩ���ַ����࣬���ڴ���C���� char ���顣����װ���ַ����Ĵ洢�͹����ṩ�˷ḻ�ĳ�Ա������ʹ���ַ����������Ӱ�ȫ�ͷ��㡣
#ifndef USER_H
#define USER_H // ͷ�ļ�����

#include <string>
#include <iostream>

class User {
protected:// ������Ա��������Է���
    std::string username;// �û���
    std::string password;// ����
    double balance;// �˻����

public:
    // ���캯������ʼ���û�������������
    User(const std::string& username, const std::string& password, double balance);
    // ������������ȷ�������������ȷ����
    virtual ~User() = default;
    virtual std::string getUserType() const = 0;// ���麯������ȡ�û����ͣ���Ҫ����������ʵ��
    virtual std::string getPassword() const = 0; // ��ȡ����
    std::string getUsername() const;
    bool checkPassword(const std::string& password) const;
    void updatePassword(const std::string& newPassword);
    void recharge(double amount);// ��ֵ���
    bool consume(double amount);// ���ѽ���������򷵻�false
    double getBalance() const;// ��ȡ��ǰ���
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
