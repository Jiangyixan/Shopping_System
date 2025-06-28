#include "Server.h"
#include <iostream>

int main() {
    try {
        Server server(8888);
        std::cout << "启动服务器..." << std::endl;

        if (!server.Start()) {
            std::cout << "服务器启动失败！" << std::endl;
            return 1;
        }

        // 服务器会在Start()中持续运行，直到被中断
        server.Stop();
    }
    catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}