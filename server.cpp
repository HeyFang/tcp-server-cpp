#include <iostream>
#include <netdb.h>
#include <sys/socket.h>

#define PORT "3490"
#define BACKLOG 10 //number of pendin connections allowed in queue

int main(){
    addrinfo hints{};
    addrinfo* res = nullptr;

    hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // For wildcard IP address

    int status = getaddrinfo(nullptr, PORT, &hints, &res);

    if (status != 0) {
        std::cerr << "getaddrinfo failed: " << gai_strerror(status) << '\n';
        return 1;
    }

    //std::cout << "we got da fkin address info!" << '\n';

    /*
    std::cout << "address info: " << res->ai_addr << '\n';
    std::cout << "address length: " << res->ai_addrlen << '\n';
    std::cout << "address family: " << res->ai_family << '\n';
    std::cout << "socket type: " << res->ai_socktype << '\n';
    std::cout << "protocol: " << res->ai_protocol << '\n';
    */

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (sockfd == -1) {
        std::cerr << "dumb socket not created \n";
        freeaddrinfo(res);
        return 1;
    }

    //std::cout << "socket fd: " << sockfd << '\n';
    //std::cout << "socket created :> \n";

    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        std::cerr << "bind failed \n";
        freeaddrinfo(res);
        return 1;
    }    

    //std::cout << "socket bound! \n";

    if (listen(sockfd, BACKLOG) == -1) {
        std::cerr << "deaf, cant listen \n";
        freeaddrinfo(res);
        return 1;
    }

    std::cout << "listenin... \n";

    sockaddr_storage client_addr{};
    socklen_t client_addr_size = sizeof(client_addr);

    int new_fd = accept(sockfd,
                        reinterpret_cast<sockaddr*>(&client_addr),
                        &client_addr_size
                    );

    if (new_fd == -1) {
        std::cerr << "accept failed :/ \n";
        freeaddrinfo(res);
        return 1;
    }

    //std::cout << "listenin fd: " << sockfd << '\n';
    //std::cout << "client fd: " << new_fd << '\n';

    std::cout << "client connected! \n";

    char buffer[1024];

    int n = recv(new_fd, buffer, sizeof(buffer), 0);

    if (n > 0) {
        std::cout << "msg recieved: " << std::string(buffer, n) << '\n';
    }
    else if (n == 0) {
        std::cout << "client disconnected \n";
    }
    else {
        std::cerr << "recv failed \n";
    }

    std::string msg = "ayo welcome, have a wotamelo :D \n";
    send(new_fd, msg.data(), msg.size(), 0);




    std::cin.get(); // Wait for user input before exiting

    freeaddrinfo(res); // Free the linked list
}