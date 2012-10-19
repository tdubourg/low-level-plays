#include "hw.h"
#include <signal.h>
#include "stdio.h"
#include "time.h"
#include "stdlib.h"

void ctrl_c_handler () {
	irq_disable();
	printf("(Go!...) Bwaaaaaah.\n");
	exit(0);
}

void irq_handler() {
	static int i = 0;
	irq_disable();
	printf("Pouet%d\n", i++);
	irq_enable();
}

int main(int argc, char const *argv[])
{
	setup_irq(TIMER_IRQ, irq_handler);
	start_hw();
	{
		static struct sigaction sa;
	
		sigemptyset(&sa.sa_mask);
		sa.sa_handler = ctrl_c_handler;
		/* sa.sa_flags = SA_RESTART | 0x4000000; */
		sa.sa_flags = SA_RESTART | SA_NODEFER;
		sigaction(SIGINT, &sa, (struct sigaction *)0);
	}
	int i = -1;
	for(;;) {
		printf("Huuuuuuuuuuuuuuuuuuuhu%d\n", ++i);
		sleep(2);
	}
	return 0;
}
