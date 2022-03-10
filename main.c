/** ***************************************************************************
 * @file    main.c
 * @brief   Simple UART Demo for EFM32GG_STK3700
 * @version 1.0
******************************************************************************/

#include <stdint.h>
#include <stdio.h>
/*
 * Including this file, it is possible to define which processor using command line
 * E.g. -DEFM32GG995F1024
 * The alternative is to include the processor specific file directly
 * #include "efm32gg995f1024.h"
 */
#include "em_device.h"
#include "em_chip.h"
#include "em_system.h"

#include "clock_efm32gg.h"

#include "FreeRTOS.h"
#include "task.h"

#include "led.h"
#include "button.h"
#include "receptor.h"
#include "transmissor.h"

#define TASKDELAYUNTIL

/***************************************************************************
 *                                                                         *
 *              Global variables and definitions                           *
 *                                                                         *
 ***************************************************************************/

/// Global variables
bool transmitting = false;
bool receiving = false;
bool start_receiving = false;

/**
 * @brief Control blinking period
 */

//@{
uint32_t semiperiodmax  = 5000;
uint32_t semiperiodmin  = 100;
uint32_t semiperiodstep = 100;

uint32_t semiperiod0    = 1250;         // 1 sec
uint32_t semiperiod1    = 1000;        // 1.5 sec

/***************************************************************************
 *                                                                         *
 *              Tasks                                                      *
 *                                                                         *
 ***************************************************************************/

int cont = 0;
int dados[20010]={0};

/**
 * @brief  Reception Task
 *
 */
//@{
void Task_Reception(void *pvParameters){
const portTickType xFrequency = 1;
portTickType xLastWakeTime=xTaskGetTickCount();

    while(1) {
        if( receiving ) {
            if( start_receiving ){
                if((RECEPTOR_Read()&RECEPTOR) == 0)     dados[cont] = 1;
                else                                    dados[cont] = 0;
                cont++;
                if(cont == 20000){
                    receiving = false;
                    start_receiving = false;
                    cont = 0;
                    LED_Write(LED1|LED2,0);
                }
            }
            else if((RECEPTOR_Read()&RECEPTOR) == 0){
                start_receiving = true;
            }
        }

#ifdef TASKDELAYUNTIL
        vTaskDelayUntil(&xLastWakeTime,xFrequency);
#else
        vTaskDelay(semiperiod1);
#endif

    }
}
//@}


/**
 * @brief  Transmission Task
 *
 */
//@{
void Task_Transmission(void *pvParameters){
const portTickType xFrequency = 1;
portTickType xLastWakeTime=xTaskGetTickCount();

    while(1) {
        if( transmitting ) {
            if( dados[cont] == 0 )      TRANSMISSOR_Off(TRANSMISSOR);
            else                        TRANSMISSOR_On(TRANSMISSOR);
            cont++;
            if( cont == 20000 ) {
                LED_Write(LED1|LED2,0);
                TRANSMISSOR_Off(TRANSMISSOR);
                transmitting = false;
                cont = 0;
            }
        }

#ifdef TASKDELAYUNTIL
        vTaskDelayUntil(&xLastWakeTime,xFrequency);
#else
        vTaskDelay(semiperiod1);
#endif

    }
}
//@}

/**
 * @brief  Switch Control Task
 *
 */
//@{
#define DEBOUNCE_TIME 40
void Task_Button(void *pvParameters) {
    while(1) {
        if( (Button_Read()&BUTTON0) == 0 ) {
            transmitting = true;
            receiving = false;
            LED_Write(LED1,LED2);
            vTaskDelay(DEBOUNCE_TIME);
            while( (Button_Read()&BUTTON0) == 0 ) {
                vTaskDelay(DEBOUNCE_TIME);
            }
        }
        else if( (Button_Read()&BUTTON1) == 0 ) {
            receiving = true;
            transmitting = false;
            LED_Write(LED2,LED1);
            vTaskDelay(DEBOUNCE_TIME);
            while( (Button_Read()&BUTTON1) == 0 ) {
                vTaskDelay(DEBOUNCE_TIME);
            }
        }
    }
}
//@}

static void STOP(void) { while(1) {} }

/**************************************************************************//**
 * @brief  Main function
 *
 * @note   Using external crystal oscillator
 *         HFCLK = HFXO
 *         HFCORECLK = HFCLK
 *         HFPERCLK  = HFCLK
 */

int main(void) {
BaseType_t rc;

    // Apply erratas
    CHIP_Init();

    /* Configure LEDs */
    LED_Init(LED1|LED2);

    // Set clock source to external crystal: 48 MHz
    (void) SystemCoreClockSet(CLOCK_HFXO,1,1);

    Button_Init(BUTTON0|BUTTON1);
    TRANSMISSOR_Init(TRANSMISSOR);
    RECEPTOR_Init(RECEPTOR);

    SysTick_Config(SystemCoreClock/1000);   /* 1 ms */

    rc = xTaskCreate(Task_Button,"Button", 1000,0,1,0);
    if( rc != pdPASS )
        STOP();
    
    rc = xTaskCreate(Task_Transmission,"Transmission", 1000,0,5,0);
    if( rc != pdPASS )
       STOP();

    rc = xTaskCreate(Task_Reception,"Reception", 1000,0,5,0);
    if( rc != pdPASS )
       STOP();

    vTaskStartScheduler();

    while(1) {}
}