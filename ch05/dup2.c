#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int _dup2(int oldfd, int newfd)
{
    int flags;

    if (oldfd == newfd)
        return newfd;

    flags = fcntl(oldfd, F_GETFL);
    if (flags == -1 || newfd < 0) {
        errno = EBADF;
        return -1;
    }

    flags = fcntl(newfd, F_GETFL);
    if (flags != -1) {
        if (close(newfd) == -1)
            return -1;
    }

    return fcntl(oldfd, F_DUPFD, newfd);
}
