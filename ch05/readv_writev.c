#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/uio.h>

#define STR_SIZE 13

ssize_t _readv(int fd, const struct iovec *iov, int iovcnt);
ssize_t _writev(int fd, const struct iovec *iov, int iovcnt);
void exitErr(const char* msg);

int main(__attribute__((unused)) int _argc, __attribute__((unused)) char **argv)
{
    int fd;
    struct iovec iovIn[3];
    struct iovec iovOut[3];

    ssize_t total, numRequired = 0;

    char c = 'a';                        /* First buffer  */
    int  x = 10;                         /* Second buffer */
    char str[STR_SIZE] = "readv writev"; /* Third buffer  */

    /* Buffers to hold the values read */
    char cRead;
    int  xRead;
    char strRead[STR_SIZE];

    char tmpFile[] = "/tmp/readv_writev_XXXXXX";

    fd = mkstemp(tmpFile);
    if (fd == -1)
        exitErr("mkstemp");

    unlink(tmpFile);

    iovOut[0].iov_base = &c;
    iovOut[0].iov_len  = sizeof(c);
    numRequired       += sizeof(c);

    iovOut[1].iov_base = &x;
    iovOut[1].iov_len  = sizeof(x);
    numRequired       += sizeof(x);

    iovOut[2].iov_base = str;
    iovOut[2].iov_len  = STR_SIZE;
    numRequired       += STR_SIZE;

    total = _writev(fd, iovOut, 3);
    if (total == -1)
        exitErr("_writev");

    if (total != numRequired) {
        printf("Total written is different from required.");
        exit(EXIT_FAILURE);
    }

    iovIn[0].iov_base = &cRead;
    iovIn[0].iov_len  = sizeof(cRead);

    iovIn[1].iov_base = &xRead;
    iovIn[1].iov_len  = sizeof(xRead);

    iovIn[2].iov_base = strRead;
    iovIn[2].iov_len  = STR_SIZE;

    lseek(fd, 0, SEEK_SET);
    total = _readv(fd, iovIn, 3);
    if (total == -1)
        exitErr("_readv");

    if (total != numRequired) {
        printf("Total read is different from required.");
        exit(EXIT_FAILURE);
    }

    printf("c: %c, x: %d, str: %s\n", cRead, xRead, strRead);

    if (close(fd) == -1)
        exitErr("close");

    exit(EXIT_SUCCESS);
}

ssize_t _writev(int fd, const struct iovec *iov, int iovcnt)
{
    int i;
    size_t bytesCopied = 0;
    ssize_t bytesWritten, bytesRequired = 0;
    void *buf;

    for (i = 0; i < iovcnt; ++i) {
        bytesRequired += iov[i].iov_len;
    }

    buf = malloc(bytesRequired);
    if (buf == NULL)
        exitErr("malloc");

    for (i = 0; i < iovcnt; ++i) {
        memcpy(buf + bytesCopied, iov[i].iov_base, iov[i].iov_len);
        bytesCopied += iov[i].iov_len;
    }

    bytesWritten = write(fd, buf, bytesRequired);
    if (bytesWritten == -1)
        exitErr("write");

    free(buf);

    return bytesWritten;
}

ssize_t _readv(int fd, const struct iovec *iov, int iovcnt)
{
    int i;
    size_t bytesCopied = 0;
    ssize_t bytesRead, bytesRequired = 0;
    void *buf;

    for (i = 0; i < iovcnt; ++i) {
        bytesRequired += iov[i].iov_len;
    }

    buf = malloc(bytesRequired);
    if (buf == NULL)
        exitErr("malloc");

    bytesRead = read(fd, buf, bytesRequired);
    if (bytesRead == -1)
        exitErr("read");

    for (i = 0; i < iovcnt; ++i) {
        memcpy(iov[i].iov_base, buf + bytesCopied, iov[i].iov_len);
        bytesCopied += iov[i].iov_len;
    }

    free(buf);

    return bytesRead;
}

void
exitErr(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
