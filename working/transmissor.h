/**************************************************************************//**
 * @file    led.h
 * @brief   LED HAL for EFM32GG STK3200
 * @version 1.0
******************************************************************************/
#ifndef TRANSMISSOR_H
#define TRANSMISSOR_H
#include <stdint.h>

#ifndef BIT
#define BIT(N) (1U<<(N))
#endif

/**
 * @brief  LEDs are on PE
 */
///@{
#define TRANSMISSOR BIT(1)
///@}

void TRANSMISSOR_Init(uint32_t leds);
void TRANSMISSOR_On(uint32_t leds);
void TRANSMISSOR_Off(uint32_t leds);
void TRANSMISSOR_Write(uint32_t off, uint32_t on);

#endif // TRANSMISSOR_H
