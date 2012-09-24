#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <setjmp.h>

#define RETURN_SUCCESS 0
#define RETURN_FAILURE 1



jmp_buf buf;
int main(int argc, char *argv[])
{
  
  int i = 42;
	int j=0;
	if (setjmp(buf)) {
		for (; j<5; j++) {
          printf("\nPouet: %d", i);
			i++;
		}
	}
	else {
		for (; j<5; j++) {
          printf("\nTralala: %d", i);
			i--;
		}
		longjmp(buf, ~0);
	}
	printf("\n%d\n", i );
  	return 0;
}