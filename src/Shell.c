#include "Shell.h"

// PILAR FUNKAR EJ! (inte chars)
#define RIGHTARROW 0x1b5b43
#define LEFTARROW 0x1b5b44
#define UPARROW 0x1b5b41
#define DOWNARROW 0x1b5b42

#define ESCAPE 0x1b
#define BACKSPACE 0x8
#define TAB 0x9

void Shell() {
	putsln("\nShell");

	char c;
	char buf[200];
	int i = 0;
	char buf2[10];
	char backSpace[4] = {0x8, ' ', 0x8, '\0'};
	
	while (1) {
		c = 0;
		puts(">");
		while (c != '\n') {
			c = getc();
			
			if (c == BACKSPACE) {
				puts(backSpace);
				i--;
			} else {
				putc(c);
				buf[i] = c;
				i++;
			}
		}
		
		buf[i] = '\0';
		
		parseCommand(buf);
		
		i = 0;
	}
	
	putsln("Shell ending!");
}

void parseCommand(char* str) {
	char* argv[512];
	
	split(str, argv);
	
	if (argv[0] == NULL)	
		return;
	
	
	if (strcmp(argv[0], userProgramsNames[0])) { // HelloWorld
		newPCB(2, userProgramsAddresses[0], userProgramsNames[0], 0, New, 0);
	}
	else if (strcmp(argv[0], userProgramsNames[1])) { // Scroller
		if (argv[1] != NULL)
			newPCB(1, userProgramsAddresses[1], userProgramsNames[1], (int)&argv[1], New, 0);
		else
			putsln("Error: Not sufficient arguments!");
	}
	else if (strcmp(argv[0], userProgramsNames[2])) {	// Increment
		if (argv[1] != NULL)
			newPCB(2, userProgramsAddresses[2], userProgramsNames[2], atoi(argv[1]), New, 0);
		else
			putsln("Error: Not sufficient arguments!");
	}
	else if (strcmp(argv[0], userProgramsNames[3])) {	// Fibonacci
		if (argv[1] != NULL)
			newPCB(2, userProgramsAddresses[3], userProgramsNames[3], atoi(argv[1]), New, 0);
		else
			putsln("Error: Not sufficient arguments!");
	}
	else if (strcmp(argv[0], userProgramsNames[4])) {	// Shell
		newPCB(3, userProgramsAddresses[4], userProgramsNames[4], 0, New, 0);
	}	
	else if (strcmp(argv[0], "kill")) {	// Kill
		if (argv[1] != NULL)
			kill(atoi(argv[1]));
	}	
	else if (strcmp(argv[0], "changePrio")) {	// Change Priority
		if (argv[1] != NULL && argv[2] != NULL)
			changePrio(atoi(argv[1]), atoi(argv[2]));
	}	
	else if (strcmp(argv[0], "top")) {	// Top
		top();
	}	
	else if (strcmp(argv[0], "block")) {	// Block
		if (argv[1] != NULL) {
			if (block(atoi(argv[1])) == -1)
				putsln("Error: Blocking couldn't be performed");
		}
	}	
	else if (strcmp(argv[0], "unblock")) {	// Block
		if (argv[1] != NULL) {
			if (unblock(atoi(argv[1])) == -1)
				putsln("Error: Unblocking couldn't be performed");
		}
	}
	else
		putsln("Error: Command unknown!");
}

int isspace(char s) {
	return (s == ' ' || s == '\t' || s == '\n' || s == '\r');
}

char* skipwhite(char* s)
{
	while (isspace(*s)) ++s;
	return s;
}

void split(char* cmd, char* args[512]) {
	cmd = skipwhite(cmd);
	char* next = strchr(cmd, ' ');
	int i = 0;

	while(next != NULL) {		
		next[0] = '\0';
		args[i] = cmd;
		++i;
		cmd = skipwhite(next + 1);
		next = strchr(cmd, ' ');
	}
	
	if (cmd[0] != '\0') {
		args[i] = cmd;
		++i; 
	}
	
	args[i] = NULL;
}
