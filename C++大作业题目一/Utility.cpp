// Utility.cpp
#include "Utility.h"

namespace Utility {

    bool isValidUsername(const std::string& username) {
        return !username.empty() && username.size() >= 3;
    }

    bool isValidPassword(const std::string& password) {
        return !password.empty() && password.size() >= 6;
    }

    double getPositiveDouble() {
        double value;
        while (true) {
            std::cin >> value;
            if (std::cin.fail() || value <= 0) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "请输入有效的正数: ";
            }
            else {
                break;
            }
        }
        return value;
    }

    int getPositiveInt() {
        int value;
        while (true) {
            std::cin >> value;
            if (std::cin.fail() || value <= 0) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "请输入有效的正整数: ";
            }
            else {
                break;
            }
        }
        return value;
    }

}
