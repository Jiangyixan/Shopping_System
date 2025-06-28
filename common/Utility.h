// Utility.h
#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <iostream>

namespace Utility {
    bool isValidUsername(const std::string& username);
    bool isValidPassword(const std::string& password);
    double getPositiveDouble();
    int getPositiveInt();
}

#endif // UTILITY_H
