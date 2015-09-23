/*
  The content of `example2.txt` will be "Gidday world" because the file offset for
  `fd3` will be at the beginning of the file. Only `fd1` and `fd2` share the same
  file offset, that's why when writing "HELLO," replaces "Hello," but the next write
  (with `fd3`) happens at beginning and then we are left with "Gidday world".
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(__attribute__((unused)) int _argc, __attribute__((unused)) char **argv)
{
    int fd1, fd2, fd3;
    const char *filename = "example2.txt";

    fd1 = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    fd2 = dup(fd1);
    fd3 = open(filename, O_RDWR);

    write(fd1, "Hello,", 6);
    write(fd2, " world", 6);
    lseek(fd2, 0, SEEK_SET);
    write(fd1, "HELLO,", 6);
    write(fd3, "Gidday", 6);

    exit(EXIT_SUCCESS);
}
