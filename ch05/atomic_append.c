/*
 *  This program demonstrates that by not using `O_APPEND` it can not be
 *  guaranteed that the write will happen at the expected location after
 *  executing `lseek`. The program might run out of time slice and be suspended
 *  between `lseek` and `write` and then when it comes back the offset is not
 *  exactly at the actual end of the file.
 *
 *  This can be demonstrated by executing the program with the flag to use and
 *  also not use `O_APPEND`:
 *
 *      $ ./atomic_append f1 1000000   & ./atomic_append f1 1000000
 *      $ ./atomic_append f2 1000000 x & ./atomic_append f2 1000000 x
 *
 *  The final filesize will be something like:
 *
 *      -rw-r--r--  1 carlosgaldino  staff  2000000 Sep 23 21:31 f1
 *      -rw-r--r--  1 carlosgaldino  staff  1012311 Sep 23 21:37 f2
 *
 *  Using `O_APPEND` (first execution example) will guarantee that the write
 *  will always be at the end of the file since before each write the file
 *  offset will be set to the end of it and both operations (set offset and
 *  write) will happen atomically.
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

typedef enum { false, true } bool;

void usage(const char *name);
void exitErr(const char* msg);

int main(int argc, char **argv)
{
    int fd, flags;
    char *filename, *byte = "a";
    size_t numBytes;
    bool useAppend;

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usage(argv[0]);

    filename = argv[1];
    numBytes = atol(argv[2]);
    flags    = O_CREAT | O_RDWR;

    if (argc == 3) {
        useAppend = true;
    } else {
        useAppend = false;
    }

    if (useAppend)
        flags |= O_APPEND;

    fd = open(filename, flags, S_IRUSR | S_IWUSR);
    if (fd == -1)
        exitErr("open");

    for (size_t i = 0; i < numBytes; i++) {
        if (!useAppend)
            lseek(fd, 0, SEEK_END);

        if(write(fd, byte, 1) == -1)
            exitErr("write");
    }

    exit(EXIT_SUCCESS);
}

void exitErr(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void usage(const char *name)
{
    printf("%s filename num-bytes [x]\n", name);
    exit(EXIT_SUCCESS);
}
