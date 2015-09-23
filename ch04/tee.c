#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUF_SIZE 1024

void exitErr(const char* msg);
void usage(const char* name);

int
main(int argc, char *argv[])
{
    ssize_t numRead;
    char buf[BUF_SIZE];
    int outFd, append = 0;

    switch (getopt(argc, argv, "ha:")) {
    case 'h':
        usage(argv[0]);
        break;
    case 'a':
        append = 1;
        break;
    }

    int flags = O_WRONLY | O_CREAT;

    if (append) {
        flags |= O_APPEND;
    } else {
        flags |= O_TRUNC;
    }

    if (argc > 1) { // file was given
        outFd = open(argv[optind], flags, S_IRUSR | S_IWUSR /* rw------ */);
        if (outFd == -1)
            exitErr("open");
    }

    while((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        if (write(STDOUT_FILENO, buf, numRead) != numRead) {
            exitErr("write");
        }

        if (outFd) {
            if (write(outFd, buf, numRead) != numRead) {
                exitErr("write");
            }
        }
    }

    if (numRead == -1)
        exitErr("read");

    exit(EXIT_SUCCESS);
}

void
exitErr(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void
usage (const char *name)
{
    printf("%s [-a] filename\n", name);
    exit(EXIT_SUCCESS);
}
