#include "ctx_list.h"

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

void init_list(ctx_list* l) {
	l.size = 0;
	l.head = NULL;
	l.tail = NULL;
}


