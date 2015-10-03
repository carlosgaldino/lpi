/* setenv.c

   Implements and demonstrates the behavior of the setenv() function.

   Usage: setenv NAME VALUE [x]

   The program will set the environment variable identified by NAME to VALUE. If
   a third optional argument is specified and an environment variable identified
   by NAME exists the program will override the defined value of the environment
   variable identified by NAME with VALUE.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

void usage(const char *name);
void exitErr(const char* msg);
int _setenv(const char *name, const char *value, int overwrite);

int main(int argc, char **argv)
{
    int overwrite = 0;

    if (argc < 3)
        usage(argv[0]);

    if (argc == 4)
        overwrite = 1;

    if (_setenv(argv[1], argv[2], overwrite) < 0)
        exitErr("setenv");

    printf("%s=%s\n", argv[1], getenv(argv[1]));

    exit(EXIT_SUCCESS);
}

int _setenv(const char *name, const char *value, int overwrite)
{
    char *oldValue  = getenv(name);
    char *nameValue = malloc(strlen(name) + strlen(value) + 2); /* +2 for '=' and null terminator */

    if (nameValue == NULL) {
        errno = ENOMEM;
        return -1;
    }

    if (oldValue == NULL || overwrite)
        return putenv(strcat(strcat(strcpy(nameValue, name), "="), value));

    return 0;
}

void exitErr(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void usage(const char *name)
{
    printf("%s NAME VALUE\n", name);
    exit(EXIT_SUCCESS);
}
