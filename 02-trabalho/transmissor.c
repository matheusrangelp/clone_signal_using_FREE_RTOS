/** **************************************************************************
 * @file    led.c
 * @brief   LED HAL for EFM32GG STK
 * @version 1.0
 *****************************************************************************/

#include <stdint.h>
/*
 * Including this file, it is possible to define which processor using command line
 * E.g. -DEFM32GG995F1024
 * The alternative is to include the processor specific file directly
 * #include "efm32gg995f1024.h"
 */
#include "em_device.h"

#include "transmissor.h"

static GPIO_P_TypeDef * const GPIOD = &(GPIO->P[3]);    // GPIOD

void TRANSMISSOR_Init(uint32_t leds) {

    /* Enable Clock for GPIO */
    CMU->HFPERCLKDIV |= CMU_HFPERCLKDIV_HFPERCLKEN;     // Enable HFPERCLK
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;           // Enable HFPERCKL for GPIO

    if( leds&TRANSMISSOR) {
        GPIOD->MODEL &= ~_GPIO_P_MODEL_MODE1_MASK;      // Clear bits
        GPIOD->MODEL |= GPIO_P_MODEL_MODE1_PUSHPULL;    // Set bits
    }
    // Turn Off LEDs
    TRANSMISSOR_Off(TRANSMISSOR);
}

void TRANSMISSOR_On(uint32_t leds) {
#ifdef GPIO_ALTERNATE_OPERATIONS
    GPIOD->DOUTSET = leds;
#else
    GPIOD->DOUT |= leds;
#endif
}

void TRANSMISSOR_Off(uint32_t leds) {
#ifdef GPIO_ALTERNATE_OPERATIONS
    GPIOD->DOUTCLR = leds;
#else
    GPIOD->DOUT &= ~leds;
#endif
}

void TRANSMISSOR_Write(uint32_t off, uint32_t on) {
#ifdef GPIO_ALTERNATE_OPERATIONS
    GPIOD->DOUTCLR = off;
    GPIOD->DOUTSET = on;
#else
    GPIOD->DOUT = (GPIOD->DOUT&~off)|on;
#endif
}

void TRANSMISSOR_Toggle(uint32_t leds) {

#ifdef GPIO_ALTERNATE_OPERATIONS
    GPIOD->DOUTTGL = leds;
#else
    GPIOD->DOUT ^= leds;
#endif
}