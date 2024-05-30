#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <netinet/in.h>
#include <unistd.h>
#include "Bank.h"

class Server {
private:
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    std::vector<std::thread> threads;
    std::mutex mtx;
    int request_count = 0;
    Bank bank;
    
    void handle_client(int client_socket);
    void log_request_count();
public:
    Server(int port, int num_accounts, int max_balance);
    ~Server();
    void start();
    void shutdown();
};

Server::Server(int port, int num_accounts, int max_balance) : bank(num_accounts, max_balance) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if(listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
}

Server::~Server() {
    close(server_fd);
}

void Server::handle_client(int client_socket) {
    char buffer[1024] = {0};
    int valread;
    while((valread = read(client_socket, buffer, 1024)) > 0) {
        std::string command(buffer, valread);
        // Process command and send response to client
        std::lock_guard<std::mutex> lock(mtx);
        request_count++;
        if(request_count % 5 == 0) {
            std::cout << "Processed " << request_count << " requests" << std::endl;
        }
    }
    close(client_socket);
}

void Server::log_request_count() {
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Processed " << request_count << " requests" << std::endl;
    }
}

void Server::start() {
    std::thread log_thread(&Server::log_request_count, this);
    log_thread.detach();
    while(true) {
        int new_socket;
        if((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) >= 0) {
            threads.push_back(std::thread(&Server::handle_client, this, new_socket));
        }
    }
}

void Server::shutdown() {
    for(auto& thread : threads) {
        if(thread.joinable()) {
            thread.join();
        }
    }
}

int main(int argc, char* argv[]) {
    if(argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <port> <number_of_accounts> <max_balance>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);
    int num_accounts = std::stoi(argv[2]);
    int max_balance = std::stoi(argv[3]);

    Server server(port, num_accounts, max_balance);
    server.start();
    return 0;
}
