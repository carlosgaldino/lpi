/* unsetenv.c

   Implements and demonstrates the behavior of the unsetenv() function.

   Usage: unsetenv NAME

   The program will unset the environment variable identified by NAME to VALUE.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

extern char **environ;

void usage(const char *name);
void exitErr(const char* msg);
int _unsetenv(const char *name);
void printValue(const char *name);

int main(int argc, char **argv)
{
    if (argc < 2)
        usage(argv[0]);

    printValue(argv[1]);

    if (_unsetenv(argv[1]) < 0)
        exitErr("unsetenv");

    printf("After unset()\n");

    printValue(argv[1]);

    exit(EXIT_SUCCESS);
}

int _unsetenv(const char *name)
{
    char **ep, **nep;
    char **newEnviron = malloc(sizeof(environ));
    size_t len;

    if (getenv(name) == NULL) {
        errno = EINVAL;
        return -1;
    }

    len = strlen(name);

    for (ep = environ, nep = newEnviron; *ep != NULL; ep++) {
        if (strncmp(*ep, name, len) != 0) {
            *nep = *ep;
            nep++;
        }
    }

    environ = newEnviron;

    return 0;
}

void exitErr(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void usage(const char *name)
{
    printf("%s NAME\n", name);
    exit(EXIT_SUCCESS);
}

void printValue(const char *name)
{
    char *value = getenv(name);

    if (value != NULL)
        printf("%s=%s\n", name, value);
    else
        printf("%s is not defined\n", name);
}
