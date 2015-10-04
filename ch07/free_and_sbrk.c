#define MAX_ALLOCS 1000000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void usage(const char *name);
void exitErr(const char *msg);
static int getInt(const char *n);

int main(int argc, char **argv)
{
    char *ptr[MAX_ALLOCS];
    int freeStep, freeMin, freeMax, blockSize, numAllocs, j;

    printf("\n");

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usage(argv[0]);

    numAllocs = getInt(argv[1]);
    if (numAllocs > MAX_ALLOCS)
        exitErr("num-allocs > max");

    blockSize = getInt(argv[2]);

    freeStep = (argc > 3) ? getInt(argv[3]) : 1;
    freeMin  = (argc > 4) ? getInt(argv[4]) : 1;
    freeMax  = (argc > 5) ? getInt(argv[5]) : numAllocs;

    if (freeMax > numAllocs)
        exitErr("free-max > num-allocs");

    printf("Initial program break: %10p\n", sbrk(0));

    printf("Allocating %d * %d bytes\n", numAllocs, blockSize);

    for (j = 0; j < numAllocs; j++) {
        ptr[j] = malloc(blockSize);
        printf("Program break is now: %10p\n", sbrk(0));
        if (ptr[j] == NULL)
            exitErr("malloc");
    }

    printf("Finished allocation.\nProgram break is now: %10p\n", sbrk(0));

    printf("Freeing blocks from %d to %d in steps of %d\n", freeMin, freeMax, freeStep);

    for (j = freeMin; j < freeMax; j += freeStep)
        free(ptr[j]);

    printf("After free(), program break is: %10p\n", sbrk(0));

    exit(EXIT_SUCCESS);
}

void usage(const char *name)
{
    printf("%s num-allocs block-size [step [min [max]]]\n", name);
    exit(EXIT_SUCCESS);
}

void exitErr(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

static int getInt(const char *n)
{
    int num;

    num = atoi(n);

    return num;
}
