#include "UserPrograms.h"

int _fibonacci(int n_1, int n_2, int i);
void increment(int i);

void HelloWorld() {
	putsln("\nHello World!");
}

void Scroller(char* msg) {
	int i = 0;
	int count = 0;
	int len;
	char text[8];
	//sleep(3, 10);
	for (len = 0; msg[len] != '\0' && len < 8; len++);
		text[len] = msg[len];
		
	len++;
	
	while (1) {
		for (i = 0; i < len; i++) {
			if (msg[i] == NULL)
				displayC(' ', (i+count)%8);
			else
				displayC(msg[i], (i+count)%8);
			
			
		}
		
		count = (count + 1) % 8;
	}
}

void Increment(int a) {
	char buf[10];
	puts("\nIncrement to ");
	putsln(itoa(a, buf, 10));
	
	increment(a);
}

void Fibonacci(int a) {
	char buf[10];
	puts("\nFibonacci to ");
	putsln(itoa(a, buf, 10));
	_fibonacci(1, 1, a);
}

int _fibonacci(int n_1, int n_2, int i) {
	char buf[10];
	
	puts("Fib: ");
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
		puts("Inc: ");
		putsln(itoa(i, buf, 10));
	}
}

void initUserPrograms() {
	userProgramsAddresses[0] = (int)&HelloWorld;
	userProgramsAddresses[1] = (int)&Scroller;
	userProgramsAddresses[2] = (int)&Increment;
	userProgramsAddresses[3] = (int)&Fibonacci;
	userProgramsAddresses[4] = (int)&Shell;
	
	userProgramsNames[0] = "HelloWorld";
	userProgramsNames[1] = "Scroller";
	userProgramsNames[2] = "Increment";
	userProgramsNames[3] = "Fibonacci";
	userProgramsNames[4] = "Shell";
}

