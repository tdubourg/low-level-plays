#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <setjmp.h>

#define p() \
	volatile int x, useless;\
	__asm__("movl %%esp, %0" \
		: "=r"(x) \
		: "r"(useless) \
		: "%esp"); \
	printf("Stack pointer is equal to: %d\n", x); \
	__asm__("movl %%ebp, %0" \
		: "=r"(x) \
		: "r"(useless) \
		: "%ebp"); \
	printf("Stack base pointer is equal to: %d\n", x);

#include "facto.c" // Ugly include

int main(int argc, char const *argv[])
{
	int x, useless;
	printf("Main function.\n");

	for(;;) {
		int i;
		switch (scanf("%d", &i)) {
			case EOF:
			case 0:
				return 0;
			case 1:
				printf("%d\n", facto(i));
		}
	}
}






