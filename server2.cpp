#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 65432

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return -1;
    }

    // Bind socket to port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        std::cerr << "Ошибка установки параметров сокета" << std::endl;
        return -1;
    }
    address.sin_family = AF_INET;// IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP
    address.sin_port = htons(PORT); // Convert port number to network byte order

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Ошибка привязки сокета" << std::endl;
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Ошибка прослушивания" << std::endl;
        return -1;
    }

    std::cout << "Сервер запущен на порту " << PORT << std::endl;

    // Accept incoming connection    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr << "Ошибка принятия подключения" << std::endl;
        return -1;
    }

    while (true) {
        // Read message from client
        int valread = read(new_socket, buffer, 1024);
        if (valread <= 0) {
            break; // Exit loop if no data is read
        }
        std::cout << "Получено от клиента: " << buffer << std::endl;

        // Respond to client
        if (strcmp(buffer, "ping") == 0) {
            const char *response = "pong";
            send(new_socket, response, strlen(response), 0);
            std::cout << "Отправлено клиенту: " << response << std::endl;
        }

        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));
    }

    // Close the sockets
    close(new_socket);
    close(server_fd);
    return 0;
}
