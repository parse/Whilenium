#include "Shell.h"

// PILAR FUNKAR EJ! (inte chars)
#define RIGHTARROW 0x43
#define LEFTARROW 0x44
#define UPARROW 0x41
#define DOWNARROW 0x42

#define ESCAPE 0x1b
#define SKIP 0x5b
#define BACKSPACE 0x8
#define TAB 0x9

void Shell() {
	char c;
	
	char buf[200];
	char lastBuf[200];
	lastBuf[0] = '\0';
	
	int i = 0;
	char backSpace[4] = {0x8, ' ', 0x8, '\0'};
	
	while (1) {
		c = 0;
		puts("shell> ");
		
		while (1) {
			// Get next char
			c = getc();
			
			// If char is escape sequence, we don't want to do any action
			while (c == ESCAPE) {
				c = getc();

				if (c == SKIP) {
					c = getc();
					
					if (c == UPARROW && lastBuf[0] != '\0') {
						while (i > 0) {
							puts(backSpace);
							i--;
						}
						puts(lastBuf);
						strcpy(buf, lastBuf);
						i = strlen(buf);
					}
					
					c = getc();
				}
			}
			
			// Line feed => break loop and parse command
			if (c == '\n')
				break;
			
			if (c == '\r' || c == TAB)
				;
			else if (c == BACKSPACE) {
				if (i > 0) {
					puts(backSpace);
					i--;
				}
			} else {
				putc(c);
				buf[i] = c;
				i++;
			}
		}
		
		buf[i] = '\0';
		putc('\n');
		
		if (i > 0)
			strcpy(lastBuf, buf);

		if (DEBUG)
			putsln("\n---Going to parse command---");
		
		parseCommand(buf);

		if (DEBUG)
			putsln("---Command parsed---");
		i = 0;
	}
}

void parseCommand(char* str) {
	char* argv[512];
	
	split(str, argv);
	
	
	if (argv[0] == NULL)	
		return;
	
	
	if (strcmp(argv[0], userProgramsNames[0])) { // HelloWorld
		spawn(2, userProgramsAddresses[0], userProgramsNames[0], (int)NULL, New, 0);
	}
	/*else if (strcmp(argv[0], userProgramsNames[1])) { // Scroller
		if (argv[1] != NULL)
			spawn(1, userProgramsAddresses[1], userProgramsNames[1], (int)argv[1], New, 0);
		else
			putsln("Error: Not sufficient arguments!");
	}*/
	else if (strcmp(argv[0], userProgramsNames[2])) {	// Increment
		if (argv[1] != NULL)
			spawn(2, userProgramsAddresses[2], userProgramsNames[2], atoi(argv[1]), New, 0);
		else
			putsln("Error: Not sufficient arguments!");
	}
	else if (strcmp(argv[0], userProgramsNames[3])) {	// Fibonacci
		if (argv[1] != NULL)
			spawn(2, userProgramsAddresses[3], userProgramsNames[3], atoi(argv[1]), New, 0);
		else
			putsln("Error: Not sufficient arguments!");
	}
	else if (strcmp(argv[0], userProgramsNames[4])) {	// Shell
		spawn(PRIORITIES-2, userProgramsAddresses[4], userProgramsNames[4], 0, New, 0);
	}	
	else if (strcmp(argv[0], userProgramsNames[5])) {	// ASCII
		spawn(PRIORITIES-2, userProgramsAddresses[5], userProgramsNames[5], 0, New, 0);
	}
	else if (strcmp(argv[0], "kill")) {	// Kill
		if (argv[1] != NULL) {
			if (kill(atoi(argv[1])) == -1)
				putsln("Error: Process couldn't be destroyed");
		}
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
	else if (strcmp(argv[0], "unblock")) {	// Unblock
		if (argv[1] != NULL) {
			if (unblock(atoi(argv[1])) == -1)
				putsln("Error: Unblocking couldn't be performed");
			putslnDebug("UNBLOCK DONE!");
		}
	}
	else if (strcmp(argv[0], "sleep")) {	// Sleeå
		if (argv[1] != NULL && argv[2] != NULL) {
			sleep(atoi(argv[1]), atoi(argv[2]));
		} else
			putsln("Error: Not sufficient arguments!");
	}
	else if (strcmp(argv[0], "scroller")) {
		if (argv[1] != NULL)
			scroller(argv[1]);
		else
			scroller("");
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
	// Points to first char != space
	cmd = skipwhite(cmd);
	
	// Find a pointer to the next space char in the string
	char* next = strchr(cmd, ' ');
	int i = 0;
	
	while (next != NULL) {
		next[0] = '\0';		// "Hello World" => "Hello\0World"
		args[i] = cmd;		// Sets pointer of i arg to same as cur cmd
		
		i++;
		
		cmd = skipwhite(next + 1);	// Jump to next char after \0 and skip all spaces
		next = strchr(cmd, ' ');	// Set next to next white space
	}
	
	// If the next command doesn't start with a \0 there is another argument
	if (cmd[0] != '\0') {
		// Set next argument to cmd
		args[i] = cmd;
		// Increment i
		i++;
	}
	
	// Set the i:th arg to NULL so that we can check for the NULL token when parsing arguments
	args[i] = NULL;
}

