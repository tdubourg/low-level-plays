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

void init_list(ctx_list* l);
void disp_list();
void insert(list_el* el);