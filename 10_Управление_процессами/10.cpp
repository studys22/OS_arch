#include <unistd.h>
#include <stdlib.h>
#include <iostream>
int main()
{
    pid_t p = fork();
    if (p > 0) {
        for (int i = 0; i < 60; i++) {
            std::cout << i << std::endl;
            sleep(1);
        }
    }
    else {
        exit(0);
    }
    return 0;
}