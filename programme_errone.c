#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <setjmp.h>


static jmp_buf buf;
static int i = 0;
static int

cpt()
{
	int j = 0;
	if (setjmp(buf)) {
		printf("\n&j=%d", (int)&j);
		printf("\nj=%d", j);
		for (j=0; j<5; j++)
			i++;
	} else {
		printf("\n&j=%d", (int)&j);
		printf("\nj=%d", j);
		for (j=0; j<5; j++)
			i--;
	}
}

int main()
{
	int np = 0 ;
	cpt();
	if (! np++) {
		longjmp(buf,~0);
	}
	printf("\ni = %d\n", i );
	return 0;
}
