/* ========================================
 *
 * The following firmware was developed by Chuck Harrison
 * This work is licensed under a Creative Commons Attribution 3.0 Unported License.
 * 
 * http://creativecommons.org/licenses/by/3.0/deed.en_US
 * 
 * You are free to:
 * -To Share — to copy, distribute and transmit the work 
 * -To Remix — to adapt the work 
 * -To make commercial use of the work
 *
 * ========================================
 */

#include "ros.h"

#include "Uarts.h"
extern "C" {
#include "device.h"
}

void Uart::begin(unsigned long baud) {
  UART_Start();
}; // TBD: set baud rate

int Uart::read(void) {
  int32_t data = -1;
  if (UART_SpiUartGetRxBufferSize() != 0) {
    // data is ready
    data = (int32_t)UART_SpiUartReadRxData();
  }
  // clear error flags
  if (UART_CHECK_INTR_RX(UART_INTR_RX_ERR)) {
    // TODO: test for meaningful error (i.e. not Buffer empty) & return -1
    UART_ClearRxInterruptSource(UART_INTR_RX_ERR);
  }
  return data;
};

size_t Uart::write(uint8_t data) {
  UART_SpiUartWriteTxData((uint32_t)data);
  return 1;
};

Uart Uart0;

