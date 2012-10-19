typedef void (func_t)(void*);

void f_ping(void *arg);
void f_pong(void *arg);

int init_ctx(ctx_s *ctx, int stack_size, func_t f, void *args);
int create_ctx(int stack_size, func_t f, void *args);
void yield();
void ctrl_c_handler();