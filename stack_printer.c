#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <setjmp.h>

int main(int argc, char const *argv[])
{
	int x, useless;
	printf("Current function.\n");
	__asm__("movl %%esp, %0"
	: "=r"(x)
	: "r"(useless)
	: "%esp");
	printf("Stack pointer is equal to: %d\n", x);
	return 0;
}