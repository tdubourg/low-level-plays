#include "hw.h"
#include <signal.h>
#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include <unistd.h>
#include <setjmp.h>
#include "ctx_list.h"

#define DBG

#define ERROR_VALUE -42
#define STACK_SIZE 16384
#define TRUE 1
#define FALSE 0
int useless; // Has to be allocated outside the stack
list ctx_list;
list_el* current_ctx;



int create_ctx(int stack_size, func_t f, void *args) {
#ifdef DBG
	printf("cctx(): 0\n");
#endif
	list_el* el = (list_el*)malloc(sizeof(list_el));
#ifdef DBG
	printf("cctx(): 1\n");
#endif

	el->val = (ctx_s*)malloc(sizeof(ctx_s));	

#ifdef DBG
	printf("cctx(): 2\n");
#endif

	insert(el);

#ifdef DBG
	printf("cctx(): 3\n");
#endif

	init_ctx(el->val, stack_size, f, args);

#ifdef DBG
	printf("cctx(): 42\n");
#endif
	return 0;
}

void final_destroy() {
	unsigned sz = ctx_list.size;
	list_el* el = ctx_list.head;
	while(sz) {
		list_el* next = el->next;
		free(el->val);
		free(el);
		el = next;
		sz--;
	}
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
	printf("init_ctx(): 2.5\n");
#endif

	ctx->ebp = ctx->esp;

#ifdef DBG
	printf("init_ctx(): 3\n");
#endif

	ctx->f = f;

#ifdef DBG
	printf("init_ctx(): 4\n");
#endif

	ctx->instanciated = 0;

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

		printf("A");

		printf("B");

		printf("C");

	}
}

void f_pong(void *args)
{
#ifdef DBG
	printf("Pouetpong\n");
#endif
	while(1) {
		printf("1");
		// yield();
		printf("2");
		// yield();
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

void yield() {
	irq_disable();
	if (NULL == current_ctx)
	{
		current_ctx = ctx_list.head;
	}
#ifdef DBG
	printf("yield(): current context is %d\n", current_ctx);
#endif

	

#ifdef DBG
	printf("stc 1\n");
#endif
	
	get_esp(current_ctx->val->esp);

#ifdef DBG
	printf("stc 3\n");
#endif

	get_ebp(current_ctx->val->ebp);

#ifdef DBG
	printf("stc 4\n");
#endif
	current_ctx = current_ctx->next;
#ifdef DBG
	printf("%d\n", (int)current_ctx->val->esp - (int)current_ctx->val->ebp);
#endif

	if ((STACK_SIZE - ((int)current_ctx->val->esp - (int)current_ctx->val->ebp)) < 60)
	{
		exit(-1);
		printf("STACK OVERFLOW\n");
		ctrl_c_handler();
	}

	set_ctx(*current_ctx->val);
	irq_enable();
	if (!current_ctx->val->instanciated)
	{
		current_ctx->val->instanciated = TRUE;
		current_ctx->val->f(NULL);
	}

#ifdef DBG
	printf("stc 5\n");
#endif

	return; // Return will cause the actual context switch

}

void ctrl_c_handler () {
	irq_disable();
	final_destroy();
	printf("(Go!...) Bwaaaaaah.\n");
	exit(0);
}

void irq_handler() {
	irq_disable(); // just in case, do it before the yield()
#ifdef DBG
	static int i = 0;
	printf("Pouet%d\n", i++);
#endif
	yield();
}

void start_scheduled() {
// Scheduler init
	init_list(ctx_list);

	{
		static struct sigaction sa;

		sigemptyset(&sa.sa_mask);
		sa.sa_handler = ctrl_c_handler;
		/* sa.sa_flags = SA_RESTART | 0x4000000; */
		sa.sa_flags = SA_RESTART | SA_NODEFER;
		sigaction(SIGINT, &sa, (struct sigaction *)0);
	}
	setup_irq(TIMER_IRQ, irq_handler);
	start_hw();
}

int main(int argc, char const *argv[])
{
	start_scheduled();
	
	create_ctx(STACK_SIZE, f_pong, NULL);

#ifdef DBG
	printf("main(): 2\n");
#endif

	create_ctx(STACK_SIZE, f_ping, NULL);

#ifdef DBG
	printf("main(): 3\n");
#endif
	sleep(2);

	return 0;
}
