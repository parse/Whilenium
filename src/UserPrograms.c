#include "UserPrograms.h"
#include "_Boot.h"

void HelloWorld() {
	putsln("Hello World");
}

void HelloWorld2() {
	putsln("Second Heroo!!!!");
}

void HelloWorld3() {
	putsln("Third HoN!!!!");
}

/*
void fibo_main(int i) {
	_fibonacci(1, 1, i);
}

int _fibonacci(int n_1, int n_2, int i) {
	print(n_1+n_2);
	
	if (i <= 0)
		return;
	
	i--;
	_fibonacci(n_2, n_1+n_2);
}
void increment(int n);

int incr_main(int argc, char** argv) {
	if (argc != 2)
		return 0;
	else
		increment(atoi(argv[1]));
}

void increment(int n) {
	int i;
	
	for (i = 0; i <= n; i++)
		printf("%d\n", i);
}
*/

