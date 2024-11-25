#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>  // for inet_pton
#include <sys/socket.h> // for sockets
#include <netinet/in.h> // for sockaddr_in

#define PORT 65432

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET; // IPv4
    serv_addr.sin_port = htons(PORT); // Convert port number to network byte order

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Неверный адрес/адрес не поддерживается" << std::endl;
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Ошибка подключения к серверу" << std::endl;
        return -1;
    }

    int mode;
    std::cout << "Выберите режим работы:\n1. Автоматический\n2. Ручной\nВведите 1 или 2: ";
    std::cin >> mode;

    if (mode == 1) {
        const char *hello = "ping";

        while (true) {
            // Send message to the server
            send(sock, hello, strlen(hello), 0);
            std::cout << "Отправлено серверу: " << hello << std::endl;

            // Read response from the server
            int valread = read(sock, buffer, 1024);
            if (valread > 0) {
                std::cout << "Получено от сервера: " << buffer << std::endl;
            }

            // Clear the buffer
            memset(buffer, 0, sizeof(buffer));
            sleep(1); // Delay between sends
        }
    } else if (mode == 2) {
        std::string userInput;

        while (true) {
            std::cout << "Введите команду (ping для отправки, exit для выхода): ";
            std::getline(std::cin >> std::ws, userInput); // Чтение строки с пробелами

            if (userInput == "exit") {
                break; // Выход из цикла
            } else if (userInput == "ping") {
                // Send message to the server
                send(sock, userInput.c_str(), userInput.length(), 0);
                std::cout << "Отправлено серверу: " << userInput << std::endl;

                // Read response from the server
                int valread = read(sock, buffer, 1024);
                if (valread > 0) {
                    std::cout << "Получено от сервера: " << buffer << std::endl;
                }
            } else {
                std::cout << "Некорректная команда. Пожалуйста, введите 'ping' или 'exit'." << std::endl;
            }

            // Clear the buffer
            memset(buffer, 0, sizeof(buffer));
        }
    } else {
        std::cout << "Некорректный режим. Программа завершена." << std::endl;
    }

    close(sock);
    return 0;
}
