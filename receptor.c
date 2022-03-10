/** **************************************************************************
 * @file    button.c
 * @brief   Button HAL for EFM32GG STK
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

#include "receptor.h"


static GPIO_P_TypeDef * const GPIOD = &(GPIO->P[3]);    // GPIOD

static uint32_t lastread = 0;
static uint32_t inputpins = 0;

void RECEPTOR_Init(uint32_t buttons) {

    /* Enable Clock for GPIO */
    CMU->HFPERCLKDIV |= CMU_HFPERCLKDIV_HFPERCLKEN;     // Enable HFPERCLK
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;           // Enable HFPERCKL for GPIO

    if ( buttons&RECEPTOR ) {
        GPIOD->MODEL &= ~(_GPIO_P_MODEL_MODE1_MASK);    // Clear bits
        GPIOD->MODEL |= GPIO_P_MODEL_MODE1_INPUT;       // Set bits
        inputpins |= RECEPTOR;
    }
    // First read
    lastread = GPIOD->DIN;
}

uint32_t RECEPTOR_Read(void) {
    lastread = GPIOD->DIN;
    return lastread&inputpins;
}