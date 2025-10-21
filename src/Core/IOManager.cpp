#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

#include <iostream>
#include <cstring>
#include <thread>

#include "Core/IOManager.h"
#include "common.h"

IOManager* IOManager::m_instance = nullptr;

IOManager::IOManager() {}

IOManager::~IOManager()
{
    if(m_serverFd >= 0) {
        close(m_serverFd);
    }
    if(m_epollFd >= 0) {
        close(m_epollFd);
    }
}

IOManager *IOManager::getInstance()
{
    if(nullptr == m_instance) {
        m_instance = new IOManager();
    }

    return m_instance;
}

void IOManager::init(int port)
{
    // Create a server sockets
    m_serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if(m_serverFd < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    // Make the server socket non-blocking
    int flags = fcntl(m_serverFd, F_GETFL, 0);
    fcntl(m_serverFd, F_SETFL, flags | O_NONBLOCK);

    // Allow the socket to be reused immediately after the program exits
    int opt = 1;
    setsockopt(m_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind the socket to the specified port and address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    bind(m_serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    if(listen(m_serverFd, SOMAXCONN) < 0) {
        throw std::runtime_error("Failed to listen on socket");
    }

    m_epollFd = epoll_create1(0);
    if(m_epollFd < 0) {
        throw std::runtime_error("Failed to create epoll instance");
    }

    epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = m_serverFd;
    epoll_ctl(m_epollFd, EPOLL_CTL_ADD, m_serverFd, &event);
}

void IOManager::run(int port)
{
    epoll_event events[MAX_EVENTS];
    init(port);

    std::cout << "Server is running on port " << port << std::endl;

    while (true)
    {
        int n = epoll_wait(m_epollFd, events, MAX_EVENTS, -1);

        for(int i = 0; i < n; i++){
            int fd = events[i].data.fd;

            // Accept new connections
            if(fd == m_serverFd) {
                while(true) {
                    int clientFd = accept(m_serverFd, nullptr, nullptr);
                    if(clientFd < 0) {
                        if(errno == EAGAIN || errno == EWOULDBLOCK) {
                            break;
                        }
                        throw std::runtime_error("Error accepting connection");
                        continue;
                    }

                    // Make the client socket non-blocking
                    int clientFlags = fcntl(clientFd, F_GETFL, 0);
                    fcntl(clientFd, F_SETFL, clientFlags | O_NONBLOCK);

                    // Add the client socket to the epoll instance
                    epoll_event clientEvent;
                    clientEvent.events = EPOLLIN | EPOLLET;
                    clientEvent.data.fd = clientFd;
                    epoll_ctl(m_epollFd, EPOLL_CTL_ADD, clientFd, &clientEvent);
                }

            } else if(events[i].events & EPOLLIN) {
                char buffer[BUFFER_SIZE];
                int bytesRead = recv(fd, buffer, sizeof(buffer), 0);
                
                if(bytesRead > 0) {
                    buffer[bytesRead] = '\0'; // Null-terminate the buffer
                }
                else if(bytesRead < 0) {
                    // EAGAIN and EWOULDBLOCK are not errors in non-blocking mode
                    if(errno != EAGAIN && errno != EWOULDBLOCK) {
                        close(fd);
                    }
                    continue;

                } else if(bytesRead == 0) {
                    close(fd);
                    continue;
                }
                
                // Switch to EPOLLOUT to send a response
                epoll_event outEvent;
                outEvent.events = EPOLLOUT | EPOLLET;
                outEvent.data.fd = fd;
                epoll_ctl(m_epollFd, EPOLL_CTL_MOD, fd, &outEvent);

            } else if(events[i].events & EPOLLOUT) {
                const char* response = "HTTP/1.1 401 OK\r\n"
                           "Content-Type: text/plain\r\n"
                           "\r\n"
                           "Hello, World!";

                send(fd, response, strlen(response), 0);
                close(fd);
            }
        }        
    }
}
