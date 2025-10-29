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
        size_t bodyLength;
        size_t totalBodyBytesReceived;

        ConnectionState()
            : headerCompleted(false), hasBody(false), bodyCompleted(false), bodyLength(0), totalBodyBytesReceived(0) {}
    };

    IOManager();
    ~IOManager();
    void init(int port);
    size_t parseContentLength(const std::string& headers);

    static IOManager* m_instance;

    int m_serverFd;
    int m_epollFd;

    std::unordered_map<int, ConnectionState> m_connections;
};

#endif
