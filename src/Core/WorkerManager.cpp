#include "Core/WorkerManager.h"

WorkerManager* WorkerManager::m_sInstance = nullptr;

WorkerManager::WorkerManager() {}

WorkerManager::~WorkerManager() {}

WorkerManager* WorkerManager::getInstance()
{
    if(nullptr == m_sInstance) {
        m_sInstance = new WorkerManager();
    }

    return m_sInstance;
}
