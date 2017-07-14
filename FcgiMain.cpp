#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "stdio.h"
#include "AuthProc.h"

void fork_main(int argc, char *argv[])
{
    FCGI_Accepter<CAuthProc>::dispatch();
}

int main(int argc, char *argv[])
{
    int count = 1;
    while (count-- > 0) {
        if (fork() == 0) {
            fork_main(argc, argv);
            break;
        }
    }

    int state = 0;
    while (waitpid(-1, &state, 0) > 0) {
        if (WIFSIGNALED(state) || WIFEXITED(state)) {
            if (fork() == 0) {
                fork_main(argc, argv);
                break;
            }
        }
    }

    return 0;
}
