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
	printf("Stack pointer is equal to: %d\n", x);

int facto(int a) {
	printf("&a=%d\n", (int)&a);
	p();
	volatile int b = a-1;
	if (!a)
	{
		return 1;
	} else {
		return a*facto(b);
	}
}


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





