#include "UserPrograms.h"

void HelloWorld() {
	//putsln("Hello World");
	//syscall_display_c('A', 3);
	
	//while(1)
	//	putc('S');
	putsln("\n----------Hello:");
	putsln("Hello World!");
	putsln("----------Hello done!\n");
}

void Increment(int a) {
	char buf[10];
	puts("\n----------Increment to ");
	putsln(itoa(a, buf, 10));
	
	increment(a);
	putsln("----------Increment done!\n");
}

void Fibonacci(int a) {
	char buf[10];
	puts("\n----------Fibonacci to ");
	putsln(itoa(a, buf, 10));
	_fibonacci(1, 1, a);
	putsln("----------Fibonacci done!\n");
}

int _fibonacci(int n_1, int n_2, int i) {
	char buf[10];
	
	puts("Fibonacci: ");
	putsln(itoa(n_1+n_2, buf, 10));
	
	if (i <= 0)
		return 0;
	
	i--;
	_fibonacci(n_2, n_1+n_2, i);
	
	return 0;
}

void increment(int n) {
	int i;
	char buf[10];

	for (i = 0; i <= n; i++) {
		puts("Increment: ");
		putsln(itoa(i, buf, 10));
	}
}

