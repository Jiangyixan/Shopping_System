#include "Server.h"
#include <iostream>

int main() {
    try {
        Server server(8888);
        std::cout << "����������..." << std::endl;

        if (!server.Start()) {
            std::cout << "����������ʧ�ܣ�" << std::endl;
            return 1;
        }

        // ����������Start()�г������У�ֱ�����ж�
        server.Stop();
    }
    catch (const std::exception& e) {
        std::cout << "����: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}