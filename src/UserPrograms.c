#include "UserPrograms.h"

int _fibonacci(int n_1, int n_2, int i);
void increment(int i);

void HelloWorld() {
	block(3);
	putsln("\nHello World!");
}

void Scroller(char* msg) {	
	int i = 0, count = 0, strEnd = -1;
	
	for (i = 0; i <= 7; i++) {
		if (msg[i] == '\0') {
			strEnd = i;
			break;
		} else
			maltaText[i] = msg[i];
	}
	
	if (strEnd == -1)
		strEnd = 8;
		
	for (i = strEnd; i <= 7; i++)
		maltaText[i] = ' ';
	
	while (1) {
		displayS((uint32_t)maltaText, (uint8_t)(count%8) );
		sleep(2, 1000);
		count = (count - 1) % 8;
	}
}

void Increment(int a) {
	char buf[10];
	puts("\nIncrement to ");
	putsln(itoa(a, buf, 10));
	
	increment(a);
	
	putsln("Increment done!");
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

