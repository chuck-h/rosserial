/* include file for rosserial ADC example on PSoC4 with interrupt-driven conversion
 *
 */

#ifndef PROX_PSOC4_H
#define PROX_PSOC4_H

extern "C" {
#include "project.h"
}

#include "LED_RGB.h"

extern uint16 CapSense_CSD_SensorSignal[CapSense_CSD_TOTAL_SENSOR_COUNT];

/* Capsense constants */
#define PROX_RANGE_INIT     100
#define UINT16_MAX          65535

int16 prox_result[2], proximityMax;

void prox_setup() {

    LED_RGB_Start();

    /* Start CSD proximity sensor */
    CapSense_CSD_Start();
    CapSense_CSD_EnableWidget(CapSense_CSD_SENSOR_PROXIMITYSENSOR0_0__PROX);
    CapSense_CSD_EnableWidget(CapSense_CSD_SENSOR_PROXIMITYSENSOR1_0__PROX);
    CapSense_CSD_InitializeAllBaselines();
    /* Perform initial proximity read to set max and min */
    CapSense_CSD_UpdateEnabledBaselines();    
    /* Start scanning all enabled sensors */
    CapSense_CSD_ScanEnabledWidgets();
    /* Wait for scanning to complete */
    while(CapSense_CSD_IsBusy() != 0);
    
    /* Set initial limits */
    proximityMax = PROX_RANGE_INIT;
}

void prox_spin() {
    uint16 hue;
    if (CapSense_CSD_IsBusy()) {
      return;
    }
    prox_result[0] = CapSense_CSD_SensorSignal[0];
    prox_result[1] = CapSense_CSD_SensorSignal[1];
    if (prox_result[0]>proximityMax) {
      proximityMax = prox_result[0];
    }
    /* Calculate scaled hue value and write it */
    hue = ((uint32) prox_result[0]) * UINT16_MAX / proximityMax;
    LED_RGB_SetColorCircle(hue);
    CapSense_CSD_UpdateEnabledBaselines();
    /* Start scanning all enabled sensors */
    CapSense_CSD_ScanEnabledWidgets();
}

int prox_read(int chan) {
  return prox_result[chan];
}

#endif
