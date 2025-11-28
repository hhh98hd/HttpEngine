#ifndef WORKER_MANAGER_H
#define WORKER_MANAGER_H

class WorkerManager {
public:
    static WorkerManager* getInstance();
private:
    WorkerManager();
    ~WorkerManager();

    static WorkerManager* m_sInstance;
};

#endif