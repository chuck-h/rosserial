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
  #include "project.h"
}

void Uart::begin(unsigned long baud) {
  // compute clock divider D = HFCLK/(baud * oversampling_factor)
  uint32_t denom = baud * ROSSerial_UART_UART_OVS_FACTOR;
  uint16_t divider = (CYDEV_BCLK__HFCLK__HZ + (denom/2))/denom;
  ROSSerial_UART_SCBCLK_SetDividerValue(divider);
  ROSSerial_UART_Start();
};

int Uart::read(void) {
  int32_t data = -1;
  if (ROSSerial_UART_SpiUartGetRxBufferSize() != 0) {
    // data is ready
    data = (int32_t)ROSSerial_UART_SpiUartReadRxData();
  }
  // clear error flags
  if (ROSSerial_UART_CHECK_INTR_RX(ROSSerial_UART_INTR_RX_ERR)) {
    // TODO: test for meaningful error (i.e. not Buffer empty) & return -1
    ROSSerial_UART_ClearRxInterruptSource(ROSSerial_UART_INTR_RX_ERR);
  }
  return data;
};

size_t Uart::write(uint8_t data) {
  ROSSerial_UART_SpiUartWriteTxData((uint32_t)data);
  return 1;
};

Uart Uart0;

