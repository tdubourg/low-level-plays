#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <setjmp.h>

// #define DBG 

#define ERROR_VALUE -42
#define TRUE 1
#define FALSE 0

int useless; // Has to be allocated outside the stack

typedef void (func_t)(void*);

typedef struct ctx_s_
{
	void* esp;
	void* ebp;
	func_t* f;
} ctx_s;

ctx_s pctx;
ctx_s ctx_ping;
ctx_s ctx_pong;
void f_ping(void *arg);
void f_pong(void *arg);


int try(ctx_s* pctx, func_t* f, int arg);
int throw(ctx_s* pctx, int r);
int init_ctx(ctx_s *ctx, int stack_size, func_t f, void *args);
void switch_to_ctx(ctx_s* ctx);

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

int init_ctx(ctx_s *ctx, int stack_size, func_t f, void *args) {
#ifdef DBG
	printf("init_ctx(): 1\n");
#endif
	void* stack = malloc(stack_size);
#ifdef DBG
	printf("init_ctx(): 2\n");
#endif
	ctx->esp = (void*)((int)stack + stack_size);
#ifdef DBG
	printf("init_ctx(): 2\n");
#endif
	ctx->ebp = ctx->esp;
#ifdef DBG
	printf("init_ctx(): 2\n");
#endif
	ctx->f = f;
#ifdef DBG
	printf("init_ctx(): 7\n");
#endif
	return 0;
}

void f_ping(void *args)
{
#ifdef DBG
	printf("Pouetping\n");
#endif
	while(1) {
#ifdef DBG
	printf("Pouetping2\n");
#endif
		printf("A") ;
		switch_to_ctx(&ctx_pong);
#ifdef DBG
	printf("Pouetping3\n");
#endif
			printf("B") ;
#ifdef DBG
	printf("Pouetping4\n");
#endif
		switch_to_ctx(&ctx_pong);
		printf("C") ;
#ifdef DBG
	printf("Pouetping5\n");
#endif
		switch_to_ctx(&ctx_pong);
	}
}

void f_pong(void *args)
{
#ifdef DBG
	printf("Pouetpong\n");
#endif
	while(1) {
		printf("1") ;
		switch_to_ctx(&ctx_ping);
		printf("2") ;
		switch_to_ctx(&ctx_ping);
	}
}

// Sets the context to a given parameter
#define set_ctx(A) \
__asm__("movl %1, %%ebp" \
		: "=r"(useless) \
		: "r"((A).ebp) \
		: "%ebp"); \
	__asm__("movl %1, %%esp" \
		: "=r"(useless) \
		: "r"((A).esp) \
		: "%esp");

// Gets the current value of the esp register
#define get_esp(x) \
	__asm__("movl %%esp, %0" \
		: "=r"(x) \
		: "r"(useless) \
		: "%esp");

// Gets the current value of the ebp register
#define get_ebp(x) \
	__asm__("movl %%ebp, %0" \
		: "=r"(x) \
		: "r"(useless) \
		: "%ebp");

void switch_to_ctx(ctx_s* ctx) {
#ifdef DBG
	printf("stc 1\n");
#endif
	if(ctx == &(ctx_ping)) {
		static unsigned char first_ping = 1;
#ifdef DBG
	printf("stc 2\n");
#endif
		get_esp(ctx_pong.esp);
#ifdef DBG
	printf("stc 3\n");
#endif
		 get_ebp(ctx_pong.ebp);
#ifdef DBG
	printf("stc 4\n");
#endif
	set_ctx(ctx_ping);
	if (first_ping)
	{
		first_ping = 0;
		ctx_ping.f(NULL);
	}
// 		__asm__("movl %1, %%ebp" 
// 		: "=r"(useless) 
// 		: "r"((ctx_ping).ebp) 
// 		: "%ebp"); 
// #ifdef DBG
// 	printf("stc 4.5\n");
// 	printf("esp=%d\n", (int)(ctx_ping).esp);
// #endif
// 	__asm__("movl %1, %%esp" 
// 		: "=r"(useless) 
// 		: "r"((ctx_ping).esp) 
// 		: "%esp");
#ifdef DBG
	printf("stc 5\n");
#endif

	}
	else { // ctx_pong
		static unsigned char first_pong = 1;

#ifdef DBG
	printf("stc 6\n");
#endif

		get_esp(ctx_ping.esp);

#ifdef DBG
	printf("stc 7\n");
#endif

		get_ebp(ctx_ping.ebp);

#ifdef DBG
	printf("stc 8\n");
#endif

		set_ctx(ctx_pong);
		if (first_pong)
		{
			first_pong = 0;
			ctx_pong.f(NULL);
		}
#ifdef DBG
	printf("stc 9\n");
#endif

	}

#ifdef DBG
	printf("stc 10\n");
#endif
	return;
}

int main(int argc, char const *argv[])
{
#ifdef DBG
	printf("main(): 0\n");
#endif
	init_ctx(&ctx_ping, 16384, f_ping, NULL);
	init_ctx(&ctx_pong, 16384, f_pong, NULL);
	switch_to_ctx(&ctx_ping);
	
	return 0;
}
