#ifndef __ASM_PROCESS__
#define __ASM_PROCESS__

#include "types.h"



/**
 * changePC()
 * Changes the PC to newPC and returns the previous PC
 * @param int newPC - The new PC
 * @return int The previous PC
 */
uint32_t changePC(uint32_t newPC);


/**
 * changeSP()
 * Changes the SP to newSP and returns the previous SP
 * @param int newSP - The new SP
 * @return int The previous SP
 */
uint32_t changeSP(uint32_t newSP);




#endif
