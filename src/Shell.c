#include "Shell.h"

void Shell() {
	putsln("\nShell");

	char c;
	char buf[200];
	uint32_t argv[4];
	
	char i;
	
	while (1) {
		c = 0;
		puts(">");
		while (c != '\n') {
			putc(c = getc());
			
			buf[++i] = c;
		}
		
		i = 0;
	}
	
	putsln("Shell ending!");
}

int isspace(char s) {
	return (s == ' ');
}

char* skipwhite(char* s)
{
	while (isspace(*s)) ++s;
	return s;
}

char* split(char* cmd)
{
	char* args[512];
	
	cmd = skipwhite(cmd);
	int i = 0;

	if (cmd[0] != '\0') {
		args[i] = cmd;
		++i; 
	}

	return args;
}
