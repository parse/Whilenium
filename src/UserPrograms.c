#include "UserPrograms.h"

int _fibonacci(int n_1, int n_2, int i);
void increment(int i);

void HelloWorld() {
	putsln("\n----------Hello World");
	putsln("Hello World!");
	putsln("------------Hello World done!\n");
	
	/*char c;
	char i;
	while (c != '\n') {
		putc(c = getc());
		
		displayC(c, i);
		i = (i + 1)%8;
	}*/
}

void Scroller(char* msg) {
	
}

void Shell() {
	putsln("\nShell");

	char c;
	//char i;
	
	while (1) {
		c = 0;
		puts(">");
		while (c != '\n') {
			putc(c = getc());
		}
	}
	
	putsln("Shell ending!");
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

