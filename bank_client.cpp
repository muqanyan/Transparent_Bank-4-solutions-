#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class Client {
private:
    int sock = 0;
    struct sockaddr_in serv_addr;
public:
    Client(const std::string& ip, int port);
    void send_command(const std::string& command);
};

Client::Client(const std::string& ip, int port) {
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if(inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address / Address not supported" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Client::send_command(const std::string& command) {
    send(sock, command.c_str(), command.length(), 0);
    char buffer[1024] = {0};
    int valread = read(sock, buffer, 1024);
    std::string response(buffer, valread);
    std::cout << response << std::endl;
}

int main(int argc, char* argv[]) {
    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
        return 1;
    }

    std::string ip = argv[1];
    int port = std::stoi(argv[2]);

    Client client(ip, port);

    std::string command;
    while(std::getline(std::cin, command)) {
        if(command == "exit") {
            break;
        }
        client.send_command(command);
    }

    return 0;
}
