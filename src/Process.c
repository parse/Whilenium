#include "Process.h"

char scroll[] = "While(1)";

void welcomeText() {
	putslnDebug("              ___________________________________________________");
	putslnDebug("            /                                                    \\");
	putslnDebug("           |    _____________________________________________     |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |  Whilenium:\\> _                             |    |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |                                             |    |");
	putslnDebug("           |   |_____________________________________________|    |");
	putslnDebug("           |                                                      |");
	putslnDebug("            \\_____________________________________________________/");
	putslnDebug("                   \\_______________________________________/");
	putslnDebug("                _______________________________________________");
	putslnDebug("             _-'    .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.  --- `-_");
	putslnDebug("          _-'.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.--.  .-.-.`-_");
	putslnDebug("       _-'.-.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-`__`. .-.-.-.`-_");
	putslnDebug("    _-'.-.-.-.-. .-----.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-----. .-.-.-.-.`-_");
	putslnDebug(" _-'.-.-.-.-.-. .---.-. .-----------------------------. .-.---. .---.-.-.-.`-_");
	putslnDebug(":-----------------------------------------------------------------------------:");
	putslnDebug("`---._.-----------------------------------------------------------------._.---'");
	putslnDebug("");
	putslnDebug(" __      __.__    .__.__                  .__");               
	putslnDebug("/  \\    /  \\  |__ |__|  |    ____    ____ |__|__ __  _____ "); 
	putslnDebug("\\   \\/\\/   /  |  \\|  |  |  _/ __ \\  /    \\|  |  |  \\/     \\ ");
	putslnDebug(" \\        /|   Y  \\  |  |__\\  ___/ |   |  \\  |  |  /  Y Y  \\");
	putslnDebug("  \\__/\\  / |___|  /__|____/ \\___  >|___|  /__|____/|__|_|  /");
	putslnDebug("       \\/       \\/              \\/      \\/               \\/ ");
	putslnDebug(" Authors: Jesper Friberg, Anders Hassis, Daniel Lervik & Oskar Wiren");
	putslnDebug(" Uppsala University 2010.\n");
}

/**
 * initOS()
 * Initiates the OS essentials (including the first process)
 */
void initOS(int memoryMin) {
	welcomeText();
	
	initPCBTable(memoryMin);
	
	// Create base processes for the OS
	kNewPCBWithArgs(PRIORITIES, (int)&Idle, "Idle process",  (int)NULL, New, 0);
	kNewPCBWithArgs(2, (int)&Scroller, "Scroller", (int)&scroll, Waiting, 0);
	kNewPCBWithArgs(PRIORITIES-1, (int)&Shell, "Shell", (int)NULL, New, 0);
}

/**
 * initPCBTable(int memoryMin)
 * Initialize PCB table
 * @param int memoryMin - The "start" memory address
 */
void initPCBTable(int memoryMin) {
	PCB* PCBTable = (PCB*)memoryMin;
	
	int i;	
	for (i = 0; i < PROCESSES; i++) {
		PCBTable[i].ID = i;
		PCBTable[i].PID = -1;
		PCBTable[i].prio = 0;
		PCBTable[i].state = New;
			
		insertPCB(&PCBTable[i]);
	}
}

/*
 * exitProcess()
 * Takes care of the process ended
 */
void exitProcess() {
	// Kill the process by making the die syscall
	syscall_die();
	
	// Error safety, making sure we never fall out into outer space
	while (1);
}

/*
 * Idle()
 * An idle process that the OS always keep running
 */
void Idle() {
	while(1);
}

