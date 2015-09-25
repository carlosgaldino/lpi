#include <fcntl.h>

int _dup(int oldfd)
{
    int newfd;

    newfd = fcntl(oldfd, F_DUPFD, 0);
    if (newfd == -1)
        return -1;

    return newfd;
}
