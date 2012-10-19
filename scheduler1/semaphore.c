#include "semaphore.h"
#include "ctx_list.h"
#include "common.h"

sem* get_new_sem(unsigned short tokensINIT) {
	sem* s = (sem*)malloc(sizeof(sem));
	sem->tokens = tokensINIT;
	sem->waitlist = (ctx_list*)malloc(sizeof(ctx_list));
	init_list(sem->waitlist);

}
sem* wait_lock(sem* s) {
	if (sem->tokens > 0)
	{
		sem->tokens--;
	} else {
		yield();// @TODO : Check if when we go back to the calling function, we'll will not "skip the original lock
	}
}
sem* release(sem* s) {
	sem->tokens++;
	if (sem->tokens == 1) // just released the sem
	{
		// @TODO : set the context to the waitlist->head
		// @TODO : When do we release the other ones ? 
	}
}