#include "Core/IOManager.h"
#include "Core/WorkerManager.h"

#define PORT 8080

int main(int argc, char* argv[]) {
    WorkerManager::getInstance();
    IOManager::getInstance()->run(PORT);


    return 0;
}
