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
	int a=1, b=2, c=3, d=4, e=5, f=6, g=7, h=8, i=9;
	printf("%d %d %d %d %d %d %d %d %d \n", a, b, c, d, e, f, g, h, i);
	__asm__("movl %%esp, %0"
	: "=r"(x)
	: "r"(useless)
	: "%esp");
	printf("Stack pointer is equal to: %d\n", x);

	useless = x-5*4;
	__asm__("movl %1, %%esp"
	: "=r"(x)
	: "r"(useless)
	: "%esp");

	__asm__("movl %%esp, %0"
	: "=r"(x)
	: "r"(useless)
	: "%esp");
	printf("Stack pointer is now equal to: %d\n", x);

	printf("%d %d %d %d %d %d %d %d %d \n", a, b, c, d, e, f, g, h, i);

	return 0;
}