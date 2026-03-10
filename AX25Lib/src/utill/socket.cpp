#include "socket.hpp"

#include <sstream>

socket_t connect_kiss(const std::string host, int port)
{
    socket_t sockfd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
    if (sockfd == INVALID_SOCK)
    {
        std::ostringstream message;
        message << "Failed to create a TCP socket for Direwolf at " << host << ':' << port << '.';
        direwolf_fatal::fail("Socket creation failed", message);
    }
    
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0)
    {
        socket_close(sockfd);
        std::ostringstream message;
        message << "The configured Direwolf host \"" << host
            << "\" is not a valid IPv4 address. Example: 127.0.0.1";
        direwolf_fatal::fail("Invalid Direwolf host address", message);
    }
    
    if (connect(sockfd, (sockaddr*)&addr, sizeof(addr)))
    {
        close_socket(sockfd);
        std::ostringstream message;
        message << "Failed to connect to Direwolf at " << host << ':' << port
            << ". Ensure Direwolf is running, KISS TCP is enabled, and the port is correct.";
        direwolf_fatal::fail("Connection to Direwolf failed", message);
    }
    
    return sockfd;
}