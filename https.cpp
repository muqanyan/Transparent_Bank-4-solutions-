#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Создание сокета
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);

    // Преобразование IPv4 и IPv6 адресов из текста в бинарную форму
    if(inet_pton(AF_INET, "185.43.77.195", &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    // Соединение с сервером
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed" << std::endl;
        return -1;
    }

    // Отправка HTTP-запроса
    std::string httpRequest = "GET / HTTP/1.1\r\nHost: www.ysu.am\r\nConnection: close\r\n\r\n";
    send(sock, httpRequest.c_str(), httpRequest.length(), 0);

    // Чтение ответа сервера
    std::ofstream outFile("ysu.html");
    int bytesRead;
    while ((bytesRead = recv(sock, buffer, 1024, 0)) > 0) {
        outFile.write(buffer, bytesRead);
    }

    // Закрытие файла и сокета
    outFile.close();
    close(sock);

    std::cout << "HTTP response has been saved to ysu.html" << std::endl;

    return 0;
}
