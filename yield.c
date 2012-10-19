/** yield.c: Implementation of a "volunteered scheduler" where the processes give
 * by themselve the CPU back to the scheduler. And the scheduler just goes to the next process in the list.
 * There is no actual "shceduling", it is just multitasking and going to next process. 
 * No preemption.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <setjmp.h>

// #define DBG 

#define ERROR_VALUE -42
#define STACK_SIZE 16384
#define TRUE 1
#define FALSE 0

int useless; // Has to be allocated outside the stack

typedef void (func_t)(void*);

typedef struct ctx_s_
{
	void* esp;
	void* ebp;
	func_t* f;
	unsigned char instanciated;
} ctx_s;

typedef struct list_el_ 
{
	struct list_el_* next;
	ctx_s* val;
} list_el;

typedef struct list_ 
{
	unsigned size;
	list_el* head;
	list_el* tail;
} list;

list ctx_list;
list_el* current_ctx;
void f_ping(void *arg);
void f_pong(void *arg);

int init_ctx(ctx_s *ctx, int stack_size, func_t f, void *args);
int create_ctx(int stack_size, func_t f, void *args);
void yield();


void insert(list_el* el) {
#ifdef DBG
	printf("inserting element (%d) in the list...\n", (int)el);
	disp_list();
#endif
	if (!ctx_list.size)
	{
		ctx_list.head = el;
		ctx_list.tail = el;
	}
	ctx_list.tail->next = el;
	ctx_list.tail = el;
	el->next = ctx_list.head;
	ctx_list.size++;
#ifdef DBG
	printf("inserting in the DONE...\n");
	disp_list();
#endif
}

void disp_list() {
	printf("printing list:\n");
	unsigned sz = ctx_list.size;
	list_el* el = ctx_list.head;
	printf("size: %d\n", sz);
	while(sz) {
		list_el* next = el->next;
		printf("\telement 1 addr: %d\n", (int)el);
		el = next;
		sz--;
	}
}

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
		yield();

		printf("B");

		yield();
		printf("C");

		yield();

	}
}

void f_pong(void *args)
{
#ifdef DBG
	printf("Pouetpong\n");
#endif
	while(1) {
		printf("1");
		yield();
		printf("2");
		yield();
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
	set_ctx(*current_ctx->val);
	if (!current_ctx->val->instanciated)
	{
		current_ctx->val->instanciated = TRUE;
		current_ctx->val->f(NULL);
	}

#ifdef DBG
	printf("stc 5\n");
#endif

	return;

}

int main(int argc, char const *argv[])
{

#ifdef DBG
	printf("main(): 0\n");
#endif

	ctx_list.size = 0; // init the size of the scheduler's linked list
	ctx_list.head = NULL;
	ctx_list.tail = NULL;

#ifdef DBG
	printf("main(): 1\n");
#endif
	
	create_ctx(STACK_SIZE, f_pong, NULL);

#ifdef DBG
	printf("main(): 2\n");
#endif

	create_ctx(STACK_SIZE, f_ping, NULL);

#ifdef DBG
	printf("main(): 3\n");
#endif

	current_ctx = ctx_list.head; // init the value of the current context
	disp_list();
	// return 0;
	yield();

	final_destroy();
	
	return 0;
}
