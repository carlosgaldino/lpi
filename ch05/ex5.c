/*
  Program that verifies that duplicated file descriptors share a file offset and
  open file status flags.
*/

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

void exitErr(const char* msg);

int main(__attribute__((unused)) int _argc, __attribute__((unused)) char **argv)
{
    int fd1, fd2, flagsBefore, flagsAfter;
    off_t offsetBefore, offsetAfter, offsetFd2;

    fd1 = open("example.txt", O_RDONLY);
    fd2 = dup(fd1);

    flagsBefore = fcntl(fd1, F_GETFL);
    if (flagsBefore == -1)
        exitErr("fcntl");

    if (fcntl(fd2, F_SETFL, flagsBefore | O_APPEND) == -1)
        exitErr("fcntl");

    flagsAfter = fcntl(fd1, F_GETFL);
    if (flagsAfter == -1)
        exitErr("fcntl");

    assert(flagsBefore != flagsAfter);
    assert(flagsAfter & O_APPEND);

    offsetBefore = lseek(fd1, 0, SEEK_CUR);
    if (offsetBefore == -1)
        exitErr("lseek");

    offsetFd2 = lseek(fd2, 10, SEEK_END);
    if (offsetFd2 == -1)
        exitErr("lseek");

    offsetAfter = lseek(fd1, 0, SEEK_CUR);
    if (offsetAfter == -1)
        exitErr("lseek");

    assert(offsetAfter != offsetBefore);
    assert(offsetAfter == offsetFd2);

    exit(EXIT_SUCCESS);
}

void exitErr(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
