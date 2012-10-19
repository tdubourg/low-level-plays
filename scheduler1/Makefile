all:
	gcc -c -m32 main.c ctx_list.c semaphore.c
	gcc -o execute_me main.o ctx_list.o semaphore.o libhw.a -m32 