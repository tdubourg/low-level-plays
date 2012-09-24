#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <setjmp.h>

static void mul();
static int _mul(jmp_buf first_buf);

static void mul() {
	jmp_buf j;
	if (setjmp(j))
	{
		printf("Restauration du contexte.\n");
		printf("%d\n", 0);
	} else {
		printf("%d\n", _mul(j));
	}
}

static int _mul(jmp_buf first_buf)
{
	int i;
	switch (scanf("%d", &i)) {
		case EOF:
			return 1; /* neutral element */
		case 0:
			return _mul(first_buf); /* erroneous read */
		case 1:
		if (i) {
			return i * _mul(first_buf);
		} else {
			longjmp(first_buf, ~0);
		}
	}
}

int main(int argc, char const *argv[])
{
	mul();
	return 0;
}