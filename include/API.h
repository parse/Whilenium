#ifndef __API__
#define __API__

#include "Structs.h"
#include "IOHandler.h"
#include "_Boot.h"
#include "stdlib.h"

/*
 * putc(char c)
 * Output character c
 * @param char c - Character to output
 */
void putc(char c); // From IOHandler

/*
 * puts(char* text)
 * Output string text
 * @param const char* text - String to output
 */
void puts(char* text); // From IOHandler

/*
 * putsln(char* text)
 * Output text with line-break
 * @param const char* text - String to output
 */
void putsln(char* text); // From IOHandler

/*
 * getc()
 * If there is a char on the buffer it is returned, else while loop until char ready
 * @return The char on the buffer
 */
char getc(); // From IOHandler

/* 
 * displayC(uint8_t word, uint8_t pos)
 * Display a char on the Malta display.
 * @param uint8_t word - Word to display
 * @param uint8_t pos - Position on the display
 */
void displayC(uint8_t c, uint8_t pos); // From IOHandler

/* 
 * displayNumber(uint32_t word)
 * Display a value on the Malta display.
 * @param uint32_t word - Number to show on the Malta display
 */
void displayNumber(uint32_t word); // From IOHandler

/*
 * kill(int PID)
 * Kill the process with the given PID with a syscall, process 1 can't be destroyed
 * @param int PID - Process to kill
 */
int kill(int PID);

/*
 * sleep(int PID, int sleep)
 * Sleep the process PID for sleep iterations, if PID == 0: sleep on current process
 * @param int PID - Process to sleep
 * @param int sleep - Time to sleep
 */
int sleep(int PID, int sleep);

/*
 * changePrio(int PID, int prio)
 * Change priority for process PID
 * @param int PID - Process to change
 * @param int prio - New priority
 */
int changePrio(int PID, int prio);

/*
 * block(int PID)
 * Block process PID
 * @param int PID - Process to block
 */
int block(int PID);

/*
 * unblock(int PID)
 * Unblock process PID
 * @param int PID - Process to unblock
 */
int unblock(int PID);

/*
 * top()
 * Show process information for the whole system
 */
void top();

/*
 * getPrio(int PID)
 * Get priority for process PID
 * @param int PID - Process to get priority for
 */
int getPrio(int PID);

/*
 * getState(int PID)
 * Get state for process PID
 * @param int PID - Process to get state for
 */
State getState(int PID);

/*
 * getName(int PID)
 * Get name for process PID
 * @param int PID - Process to get name for
 */
char* getName(int PID);

/*
 * spawn(int prio, int PC, char* name, uint32_t arg, State state, int sleep) 
 * Spawn a new process with given parameters and run it with a syscall
 * @param int prio - Priority to use, from 0-PRIORITIES 
 * @param int PC - Address to the program
 * @param char* name - Name of the program
 * @param uint32_t arg - Argument to pass to our program
 * @param State state - State of the process
 * @param int sleep - If state is Waiting, enter sleeptime here
 * @return -1 if fails
 */
int spawn(int prio, int PC, char* name, uint32_t arg, State state, int sleep);

/*
 * scroller(char* msg)
 * Display string msg on Malta display
 * @param char* msg - String to display
 * @return -1
 */
int scroller(char* msg);

/*
 * displayS(uint32_t str, uint8_t offset)
 * Display string S on malta with offset
 * @param uint32_t str - String to display
 * @param uint8_t offset - Offset to use
 */
void displayS(uint32_t str, uint8_t offset);

#endif
