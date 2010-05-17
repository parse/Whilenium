#include "Shell.h"

void Shell() {
	putsln("\nShell");

	char c;
	char buf[200];
	int i = 0;
	
	while (1) {
		c = 0;
		puts(">");
		while (c != '\n') {
			putc(c = getc());
			
			buf[i] = c;
			i++;
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
		newPCB(2, userProgramsAddresses[4], userProgramsNames[4], 0, New, 0);
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
		//next = strchr(cmd, '\0');
		++i; 
	}
	
	args[i] = NULL;
}
