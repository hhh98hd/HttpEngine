#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <string>
#include <unordered_map>

class IOManager {
public:
    static IOManager* getInstance();
    void run(int port);

private:
    struct ConnectionState
    {
        std::string requestHeader;
        bool headerCompleted;
        bool hasBody;
        bool bodyCompleted;
        size_t totalBodyBytesReceived;

        ConnectionState()
            : headerCompleted(false), hasBody(false), bodyCompleted(false), totalBodyBytesReceived(0) {}
    };

    IOManager();
    ~IOManager();
    void init(int port);

    static IOManager* m_instance;

    int m_serverFd;
    int m_epollFd;

    std::unordered_map<int, ConnectionState> m_connections;
};

#endif
