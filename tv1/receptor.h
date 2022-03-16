/** ***************************************************************************
 * @file    button.h
 * @brief   Button HAL for EFM32GG STK3200
 * @version 1.0
******************************************************************************/
#ifndef RECEPTOR_H
#define RECEPTOR_H
#include <stdint.h>

#ifndef BIT
#define BIT(N) (1U<<(N))
#endif

/**
 * @brief  Receptor are on PB
 */
///@{
#define RECEPTOR BIT(1)
///@}

void        Receptor_Init(uint32_t buttons);
uint32_t    Receptor_Read(void);
void TRANSMISSOR_Toggle(uint32_t leds); 
#endif // RECEPTOR_H
