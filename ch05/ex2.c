/*
  This program demonstrates that opening a file with `O_APPEND` will always write
  at the end of it because when `O_APPEND` is used the file offset is set to the
  end of the file prior to each write. Even if we use `lseek` to the beginning of
  the file won't change the fact that the data will be written at the end of the file.
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void exitErr(const char* msg);

int main(__attribute__((unused)) int _argc, __attribute__((unused)) char **argv)
{
    const char *filename = "example.txt";
    int fd;

    fd = open(filename, O_RDWR | O_APPEND);

    if (fd == -1)
        exitErr("open");

    lseek(fd, 0, SEEK_SET);

    if (write(fd, "whatup", 6) == -1)
        exitErr("write");

    if (close(fd) == -1)
        exitErr("close");

    exit(EXIT_SUCCESS);
}

void exitErr(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
