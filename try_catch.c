#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <setjmp.h>

#define DBG

#define ERROR_VALUE -42

int useless; // Has to be allocated outside the stack

typedef int (func_t)(int);

typedef struct ctx_s_
{
	int esp;
	int ebp;
} ctx_s;

ctx_s pctx;

int try(ctx_s* pctx, func_t* f, int arg);
int throw(ctx_s* pctx, int r);

int try(ctx_s* pctx, func_t* f, int arg) {
#ifdef DBG
	printf("Saving context...\n");
#endif
	
	int result;

	__asm__("movl %%esp, %0"
		: "=r"(pctx->esp)
		: "r"(useless)
		: "%esp");
	__asm__("movl %%ebp, %0"
		: "=r"(pctx->ebp)
		: "r"(useless)
		: "%ebp");

	result = f(arg);

#ifdef DBG
	printf("Pouet3\n");
#endif

	return result;
}

int throw(ctx_s* pctx, int r) {
#ifdef DBG
	printf("Restoring context...\n");
	printf("pctx=%d\n", (int)pctx);
	printf("esp=%d\n", (int)pctx->esp);
	printf("ebp=%d\n", (int)pctx->ebp);
#endif
	static int res;
	res = r;
	// int* backup = (int*)malloc(sizeof(int));
	// *backup = r;
	__asm__("movl %1, %%ebp"
		: "=r"(useless)
		: "r"(pctx->ebp)
		: "%ebp");
	__asm__("movl %1, %%esp"
		: "=r"(useless)
		: "r"(pctx->esp)
		: "%esp");
	return res;
}

int facto(int a) {
	int b = a-1;
	if (a < 0)
	{
		throw(&pctx, ERROR_VALUE);
	}

	if (!a)
	{
		return 1;
	} else {
		return a*facto(b);
	}
}

int main(int argc, char const *argv[])
{
	for(;;) {
		int i;
		switch (scanf("%d", &i)) {
			case EOF:
			case 0:
				return 0;
				break;
			case 1:{
				int result = try(&pctx, facto, i);
				printf("Pouet66\n");
				printf("%d\n", result);
			}
			break;
		}
	}
	return 0;
}