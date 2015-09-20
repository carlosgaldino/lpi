#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

void exitErr(const char* msg);
void usage(const char* name);

int
main(int argc, char *argv[])
{
    int inFd, outFd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        usage(argv[0]);
    }

    inFd = open(argv[1], O_RDONLY);
    if (inFd == -1)
        exitErr("open");

    outFd = open(argv[2], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    if (outFd == -1)
        exitErr("open");

    while((numRead = read(inFd, buf, BUF_SIZE)) > 0) {
        if (numRead == -1)
            exitErr("read");

        if (write(outFd, buf, numRead) != numRead) {
            exitErr("write");
        }

        lseek(outFd, 10000, SEEK_END);
    }

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
    printf("%s source_file target_file\n", name);
    exit(EXIT_SUCCESS);
}
