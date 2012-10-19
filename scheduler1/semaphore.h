
typedef struct sem_ {
	ctx_list* waitlist;
	unsigned short tokens;
} sem;

sem* get_new_sem(unsigned short tokensINIT);
sem* wait_lock(sem* s);
sem* release(sem* s);
