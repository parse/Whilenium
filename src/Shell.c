#include "Shell.h"

void Shell() {
	char* buf;
	while (1) {
		puts("shell> ");
		buf = gets();
		
		parseCommand(buf);
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
	else if (strcmp(argv[0], "kill")) {	// Kill
		if (argv[1] != NULL) {
			if (kill(atoi(argv[1])) == -1)
				putsln("Error: Process couldn't be destroyed");
		}
	}	
	else if (strcmp(argv[0], "changePrio")) {	// Change Priority
		if (argv[1] != NULL && argv[2] != NULL) {
			if (changePrio(atoi(argv[1]), atoi(argv[2])) == -1)
				putsln("Error: changePrio couldn't be performed");
		}
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
		}
	}
	else if (strcmp(argv[0], "sleep")) {	// Sleep
		if (argv[1] != NULL && argv[2] != NULL) {
			if (sleep(atoi(argv[1]), atoi(argv[2])) == -1)
				putsln("Error: Sleeping couldn't be performed");
		} else
			putsln("Error: Not sufficient arguments!");
	}
	else if (strcmp(argv[0], "scroller")) {
		if (argv[1] != NULL)
			scroller(argv[1]);
		else
			scroller("");
	}
	else if (strcmp(argv[0], "ASCII")) {
		spawn(PRIORITIES-2, (int)&ASCII, "Chick", (int)NULL, New, 0);
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

