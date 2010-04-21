#ifndef __IOHANDLER__
#define __IOHANDLER__

#include "Process.h"
#include "types.h"

typedef struct {
  uint8_t thr; /* 0x00: Transmitter Holding Register */
  uint8_t iir; /* 0x01: Interrupt Identity Register */
  uint8_t fcr; /* 0x02: FIFO Control  Register */
  uint8_t lcr; /* 0x03: Line Control  Register */
  uint8_t mcr; /* 0x04: Modem Control  Register */
  
  /*  0x05: Line Status Register */
  
  struct {
    uint8_t dr   : 1;  /* bit 0: Data Ready                   */
    uint8_t oe   : 1;  /* bit 1: Overrun Error                */
    uint8_t pe   : 1;  /* bit 2: Parity Error                 */
    uint8_t fe   : 1;  /* bit 3: Framing Error                */
    uint8_t bi   : 1;  /* bit 4: Break Interrupt              */
    uint8_t thre : 1;  /* bit 5: Transmitter Holding Register */
    uint8_t temt : 1;  /* bit 6: Transmitter Empty            */
  } lsr;
  
} tty_t;

void initIO();

#endif
