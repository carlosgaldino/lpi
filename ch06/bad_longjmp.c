/*
  This program demonstrates incorrect use of longjmp() to jump into a function
  that has already returned.

  Usage: bad_longjmp [x]

  The optional argument is used to decide if the program will execute a recursive
  function between the `setjmp` and the `longjmp` calls.
*/

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

static jmp_buf env;

void setJump2()
{
    printf("Entered setJump2()\n");
    setjmp(env);
    printf("Exiting setJump2()\n");
}

void setJump()
{
    printf("Entered setJump()\n");
    setJump2();
    printf("Exiting setJump()\n");
}

void doJump()
{
    longjmp(env, 1);
}

void recur(int n)
{
    printf("Entering recur(%d)\n", n);

    if (n > 0) {
        recur(n - 1);
    }

    printf("Exiting recur()\n");
}

int main(int argc, char **argv)
{
    setJump();

    if (argc > 1) {
        recur(2);
    }

    doJump();

    printf("Bye.\n");

    return EXIT_SUCCESS;
}
