/* include file for rosserial pubsub example on PSoC4 : uart status check
 *
 */

#ifndef UARTSTATUS_PSOC4_H
#define UARTSTATUS_PSOC4_H

extern "C" {
#include "device.h"
}


uint32_t getUartIntStatus() {
  return UART_GetRxInterruptSource();
}

uint8_t getUartRxBufOvf() {
  return UART_rxBufferOverflow;
}
#endif

