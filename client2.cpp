#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>  // for inet_pton
#include <sys/socket.h> // for sockets
#include <netinet/in.h> // for sockaddr_in

#define PORT 65432

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    const char *hello = "ping";
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

    close(sock);
    return 0;
}
