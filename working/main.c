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

#include "pwm.h"

#define TASKDELAYUNTIL

/***************************************************************************
 *                                                                         *
 *              Global variables and definitions                           *
 *                                                                         *
 ***************************************************************************/

TaskHandle_t myTaskHandle = NULL;
TaskHandle_t myTaskHandle1 = NULL;


/// Blinking state (ative|inactive)
bool transmissao = false;
bool recepcao = false;
bool iniciou = false;


//TaskHandle_t myTaskHandle = NULL;

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

/**
 * @brief  Blink LED2 Task
 *
 */

int cont = 0;
#define tamanho 18000 
int dados[tamanho]={0};

void Task_BlinkLED2(void *pvParameters){
const portTickType xFrequency = 1;
portTickType xLastWakeTime=xTaskGetTickCount();

    while(1) {
        if( transmissao == true ) {
            //TRANSMISSOR_On(TRANSMISSOR);
            if(dados[cont] == 0)    PWM_Write(TIMER0,0,0);
            else    PWM_Write(TIMER0,0,0x00FF);
            cont++;
            if(cont == (tamanho-10)){
                LED_Write(LED1|LED2,0);
                PWM_Write(TIMER0,0,0);
                transmissao = false;
                vTaskResume(myTaskHandle1);
            }

        } else if(recepcao == true) {
            if(iniciou == true){
                if((Receptor_Read()&RECEPTOR) == 0) dados[cont] = 1;
                else    dados[cont] = 0;
                cont++;
                if(cont == (tamanho-10)){
                    recepcao = false;
                    iniciou = false;
                    LED_Write(LED1|LED2,0);
                    vTaskResume(myTaskHandle1);
                }
            }
            else if((Receptor_Read()&RECEPTOR) == 0){
                iniciou = true;
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
            transmissao = true;
            recepcao = false;
            cont = 0;
            LED_Write(LED1,LED2);
            vTaskSuspend(myTaskHandle1);
            vTaskDelay(DEBOUNCE_TIME);
            while( (Button_Read()&BUTTON0) == 0 ) {
                vTaskDelay(DEBOUNCE_TIME);
            }
        }
        else if( (Button_Read()&BUTTON1) == 0 ) {
            recepcao = true;
            transmissao = false;
            cont = 1;
            LED_Write(LED2,LED1);
            vTaskSuspend(myTaskHandle1);
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
    TRANSMISSOR_Init(TRANSMISSOR);
    
    // Set clock source to external crystal: 48 MHz
    (void) SystemCoreClockSet(CLOCK_HFXO,1,1);

    Button_Init(BUTTON0|BUTTON1);
    Receptor_Init(RECEPTOR);

    SysTick_Config(SystemCoreClock/1000);   /* 1 ms */

    // Configure PWM output 
    PWM_Init(TIMER0,PWM_LOC3,PWM_PARAMS_CH0_ENABLEPIN);
    
    PWM_Write(TIMER0,0,0); //desligado

    PWM_Start(TIMER0);

    rc = xTaskCreate(Task_BlinkLED2,"0", 1000,0,0,&myTaskHandle);
    if( rc != pdPASS )
       STOP();

    rc = xTaskCreate(Task_Button,"Button", 1000,0,1,&myTaskHandle1);
    if( rc != pdPASS )
        STOP();

    vTaskStartScheduler();

    while(1) {}
}