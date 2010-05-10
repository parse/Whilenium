#include "UserPrograms.h"

void HelloWorld() {
	putsln("Hello World");
	syscall_display_word(150);
}

void HelloWorld2() {
	//putsln("Second Heroo!!!!");
	increment(600);
}

void HelloWorld3() {
	//putsln("Third HoN!!!!");
	fibonacci(20);
}


void fibonacci(int i) {
	_fibonacci(1, 1, i);
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
/*
void increment(int n);

int incr_main(int argc, char** argv) {
	if (argc != 2)
		return 0;
	else
		increment(atoi(argv[1]));
}
*/

void increment(int n) {
	int i;
	char buf[10];

	for (i = 0; i <= n; i++) {
		puts("Increment: ");
		putsln(itoa(i, buf, 10));
	}
}

