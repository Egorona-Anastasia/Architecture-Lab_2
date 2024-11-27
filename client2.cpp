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
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET; // IPv4
    serv_addr.sin_port = htons(PORT); // Convert port number to network byte order

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/address is not supported" << std::endl;
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Server connection error" << std::endl;
        return -1;
    }

    int mode;
    std::cout << "Choose mode:\n1. Auto\n2. Manual\nEnter 1 or 2: ";
    std::cin >> mode;

    if (mode == 1) {
        const char *hello = "ping";

        while (true) {
            // Send message to the server
            send(sock, hello, strlen(hello), 0);
            std::cout << "Sent to the server: " << hello << std::endl;

            // Read response from the server
            int valread = read(sock, buffer, 1024);
            if (valread > 0) {
                std::cout << "Received from the server: " << buffer << std::endl;
            }
            else{
                std::cerr << "Error. Server is closed." << std::endl;
                break;
            }

            // Clear the buffer
            memset(buffer, 0, sizeof(buffer));
            sleep(1); // Delay between sends
        }
    } else if (mode == 2) {
        std::string userInput;

        while (true) {
            std::cout << "Enter the command ('ping' to send, 'exit' to stop): ";
            std::getline(std::cin >> std::ws, userInput); // Чтение строки с пробелами

            if (userInput == "exit") {
                break; // Выход из цикла
            } else if (userInput == "ping") {
                // Send message to the server
                send(sock, userInput.c_str(), userInput.length(), 0);
                int valread = read(sock, buffer, 1024);
                if (valread <= 0){
                    std::cerr << "Error. Server is closed." << std::endl;
                    break;
                }

                std::cout << "Sent to the server: " << userInput << std::endl;

                // Read response from the server
                if (valread > 0) {
                    std::cout << "Received from the server: " << buffer << std::endl;
                }

            } else {
                std::cout << "Invalid command. Please, enter 'ping' or 'exit'." << std::endl;
            }

            // Clear the buffer
            memset(buffer, 0, sizeof(buffer));
        }
    } else {
        std::cout << "Invalid mode. Program has finished." << std::endl;
    }

    close(sock);
    return 0;
}
