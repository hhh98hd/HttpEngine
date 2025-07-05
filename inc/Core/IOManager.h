#ifndef IO_MANAGER_H
#define IO_MANAGER_H

class IOManager {
public:
    static IOManager* getInstance();
    void run(int port);

private:
    IOManager();
    ~IOManager();
    void init(int port);

    static IOManager* m_instance;
    int m_serverFd;
    int m_epollFd;
};

#endif
