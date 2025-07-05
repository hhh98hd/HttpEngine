#include <iostream>

#include "IOManager.h"

#define PORT 8080

int main(int argc, char* argv[]) {
    IOManager::getInstance()->run(PORT);

    return 0;
}
