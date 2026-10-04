#include <iostream>
#include <netdb.h>
#include <sys/socket.h>

#define IP "127.0.0.1"
#define PORT "3490"

int main() {

    addrinfo hints{};
    addrinfo* res = nullptr;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(IP, PORT, &hints, &res);

    if (status != 0) {
        std::cerr << "getaddrinfo failed: " << gai_strerror(status) << '\n';
        return 1;
    }
    //std::cout << "we got da fkin address info!" << '\n';

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (sockfd == -1) {
        std::cerr << "dumb socket not created \n";
        freeaddrinfo(res);
        return 1;
    }
    //std::cout << "socket created :> \n";

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        std::cerr << "connection failed :/ \n";
        freeaddrinfo(res);
        return 1;
    }
    std::cout << "connected to the server :> \n";

    std::string msg = "hey fang :D";

    send(sockfd, msg.data(), msg.size(), 0);

    char buffer[1024];

    int n = recv(sockfd, buffer, sizeof(buffer), 0);

    if (n > 0) {
        std::cout << "received: " << std::string(buffer, n) << '\n';
    }
    else if(n == 0) {
        std::cout << "connection closed by server\n";
    }
    else {
        std::cerr << "recv failed\n";
    }

    return 0;
}
