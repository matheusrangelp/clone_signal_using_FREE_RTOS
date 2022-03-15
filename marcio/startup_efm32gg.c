/**
 * @file startup_efm32gg.c
 * @brief CMSIS Compatible EFM32GG startup file in C.
 *        Should be used with GCC 'GNU Tools ARM Embedded'
 * @version 5.1.2
 * Date:    12 June 2014
 *
 */
/* Copyright (c) 2011 - 2014 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


/*
 * Modified by Hans 12/2017
 *   Cosmetic changes for legibility
 *   Modified definition of StackTop to enable pedantic gcc flag
 *   SystemInit can be called before or after initialization
 *
 */

#include <stdint.h>
#include "em_device.h"

/*****************************************************************************
 * CONFIGURATION
 *****************************************************************************/
/*
 * __SYSTEM_INIT_BEFORE_INIT    call SystemInit before initialization of
 *                              DATA and BSS Section
 * __SYSTEM_INIT_AFTER_INIT     call SystemInit after initialization of
 *                              DATA and BSS Section (default)
 *
 * __STARTUP_COPY_MULTIPLE      Initializes data of multiple sections
 * __STARTUP_CLEAR_BSS_MULTIPLE Clear (set to zero) multiple sections
 * __STARTUP_CLEAR_BSS          Clear (set to zero) BSS Section (default)
 *
 * __NOSYSTEM_INIT              Not necessary anymore because there is a dummy
 *                              SystemInit in this file defined as weak
 * __STACK_SIZE                 Stack size (default is 1024 bytes)
 * __HEAP_SIZE                  Heap size for malloc (default is 3072 = 12*256)
 *
 * __START                      ????
 */

/*----------------------------------------------------------------------------
  Call to SystemInit can be before or after (default) initialization of data
 *----------------------------------------------------------------------------*/

#ifndef __SYSTEM_INIT_BEFORE_INIT
#define __SYSTEM_INIT_AFTER_INIT
#endif

/* Function (in system_efm32gg.c) to update the SystemCoreClock variable */
uint32_t SystemCoreClockGet(void);

/*----------------------------------------------------------------------------
  Define how initialize DATA and BSS section
 *----------------------------------------------------------------------------*/

//#define __STARTUP_COPY_MULTIPLE
#ifndef __STARTUP_CLEAR_BSS_MULTIPLE
#define __STARTUP_CLEAR_BSS
#endif

/*****************************************************************************
 * End of Configuration
 *****************************************************************************/

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler Function Prototype
 *----------------------------------------------------------------------------*/
typedef void( *pFunc )( void );

/*----------------------------------------------------------------------------
  Linker generated Symbols
 *----------------------------------------------------------------------------*/
extern uint32_t __etext;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __copy_table_start__;
extern uint32_t __copy_table_end__;
extern uint32_t __zero_table_start__;
extern uint32_t __zero_table_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern void     __StackTop(void); /* Hans: to use pedantic flag in gcc */

/*----------------------------------------------------------------------------
  External References to BSP and
 *----------------------------------------------------------------------------*/
#if 0
#ifndef __START
extern void  _start(void) __attribute__((noreturn));    /* Pre Main (C library entry point) */
#else
extern int  __START(void) __attribute__((noreturn));    /* main entry point */
#endif

#endif //0

/*----------------------------------------------------------------------------
  STOP Macro. Can be used when stack fails
 *----------------------------------------------------------------------------*/
#define __STOP() while(1) {}

/*----------------------------------------------------------------------------
  References
 *----------------------------------------------------------------------------*/
void __attribute__((weak)) _main(void);
void __attribute__((weak)) SystemInit(void);
void __attribute__((weak)) Default_Handler(void);

extern void main(void);

/*----------------------------------------------------------------------------
  User Initial Stack & Heap
 *----------------------------------------------------------------------------*/
#ifndef __STACK_SIZE
#define __STACK_SIZE  0x00000400
#endif
static uint8_t stack[__STACK_SIZE] __attribute__ ((aligned(8), used, section(".stack")));

#ifndef __HEAP_SIZE
#define __HEAP_SIZE   0x00000C00
#endif
#if __HEAP_SIZE > 0
static uint8_t heap[__HEAP_SIZE]   __attribute__ ((aligned(8), used, section(".heap")));
#endif


/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Cortex-M Processor Exceptions */
void Reset_Handler           (void) __attribute__((weak));
void NMI_Handler             (void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler             (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler          (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));

/* Part Specific Interrupts */
void DMA_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void GPIO_EVEN_IRQHandler(void)     __attribute__ ((weak, alias("Default_Handler")));
void TIMER0_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void USART0_RX_IRQHandler(void)     __attribute__ ((weak, alias("Default_Handler")));
void USART0_TX_IRQHandler(void)     __attribute__ ((weak, alias("Default_Handler")));
void USB_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void ACMP0_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void ADC0_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void DAC0_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void I2C0_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void I2C1_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void GPIO_ODD_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void TIMER1_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void TIMER2_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void TIMER3_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void USART1_RX_IRQHandler(void)     __attribute__ ((weak, alias("Default_Handler")));
void USART1_TX_IRQHandler(void)     __attribute__ ((weak, alias("Default_Handler")));
void LESENSE_IRQHandler(void)       __attribute__ ((weak, alias("Default_Handler")));
void USART2_RX_IRQHandler(void)     __attribute__ ((weak, alias("Default_Handler")));
void USART2_TX_IRQHandler(void)     __attribute__ ((weak, alias("Default_Handler")));
void UART0_RX_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void UART0_TX_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void UART1_RX_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void UART1_TX_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void LEUART0_IRQHandler(void)       __attribute__ ((weak, alias("Default_Handler")));
void LEUART1_IRQHandler(void)       __attribute__ ((weak, alias("Default_Handler")));
void LETIMER0_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void PCNT0_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void PCNT1_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void PCNT2_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void RTC_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void BURTC_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void CMU_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void VCMP_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void LCD_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void MSC_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void AES_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void EBI_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void EMU_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));



/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/
const pFunc __Vectors[] __attribute__ ((section(".vectors"))) = {
  /* Cortex-M Exception Handlers */
  (pFunc) &__StackTop,                      /*      Initial Stack Pointer     */
  Reset_Handler,                            /*      Reset Handler             */
  NMI_Handler,                              /*      NMI Handler               */
  HardFault_Handler,                        /*      Hard Fault Handler        */
  MemManage_Handler,                        /*      MPU Fault Handler         */
  BusFault_Handler,                         /*      Bus Fault Handler         */
  UsageFault_Handler,                       /*      Usage Fault Handler       */
  Default_Handler,                          /*      Reserved                  */
  Default_Handler,                          /*      Reserved                  */
  Default_Handler,                          /*      Reserved                  */
  Default_Handler,                          /*      Reserved                  */
  SVC_Handler,                              /*      SVCall Handler            */
  DebugMon_Handler,                         /*      Debug Monitor Handler     */
  Default_Handler,                          /*      Reserved                  */
  PendSV_Handler,                           /*      PendSV Handler            */
  SysTick_Handler,                          /*      SysTick Handler           */

  /* External interrupts */

  DMA_IRQHandler,                           /*   0 - DMA            */
  GPIO_EVEN_IRQHandler,                     /*   1 - GPIO_EVEN      */
  TIMER0_IRQHandler,                        /*   2 - TIMER0         */
  USART0_RX_IRQHandler,                     /*   3 - USART0_RX      */
  USART0_TX_IRQHandler,                     /*   4 - USART0_TX      */
  USB_IRQHandler,                           /*   5 - USB            */
  ACMP0_IRQHandler,                         /*   6 - ACMP0          */
  ADC0_IRQHandler,                          /*   7 - ADC0           */
  DAC0_IRQHandler,                          /*   8 -  DAC0          */
  I2C0_IRQHandler,                          /*   9 -  I2C0          */
  I2C1_IRQHandler,                          /*  10 - I2C1           */
  GPIO_ODD_IRQHandler,                      /*  11 - GPIO_ODD       */
  TIMER1_IRQHandler,                        /*  12 - TIMER1         */
  TIMER2_IRQHandler,                        /*  13 - TIMER2         */
  TIMER3_IRQHandler,                        /*  14 - TIMER3         */
  USART1_RX_IRQHandler,                     /*  15 - USART1_RX      */
  USART1_TX_IRQHandler,                     /*  16 - USART1_TX      */
  LESENSE_IRQHandler,                       /*  17 - LESENSE        */
  USART2_RX_IRQHandler,                     /*  18 - USART2_RX      */
  USART2_TX_IRQHandler,                     /*  19 - USART2_TX      */
  UART0_RX_IRQHandler,                      /*  20 - UART0_RX       */
  UART0_TX_IRQHandler,                      /*  21 - UART0_TX       */
  UART1_RX_IRQHandler,                      /*  22 - UART1_RX       */
  UART1_TX_IRQHandler,                      /*  23 - UART1_TX       */
  LEUART0_IRQHandler,                       /*  24 - LEUART0        */
  LEUART1_IRQHandler,                       /*  25 - LEUART1        */
  LETIMER0_IRQHandler,                      /*  26 - LETIMER0       */
  PCNT0_IRQHandler,                         /*  27 - PCNT0          */
  PCNT1_IRQHandler,                         /*  28 - PCNT1          */
  PCNT2_IRQHandler,                         /*  29 - PCNT2          */
  RTC_IRQHandler,                           /*  30 - RTC            */
  BURTC_IRQHandler,                         /*  31 - BURTC          */
  CMU_IRQHandler,                           /*  32 - CMU            */
  VCMP_IRQHandler,                          /*  33 - VCMP           */
  LCD_IRQHandler,                           /*  34 - LCD            */
  MSC_IRQHandler,                           /*  35 - MSC            */
  AES_IRQHandler,                           /*  36 - AES            */
  EBI_IRQHandler,                           /*  37 - EBI            */
  EMU_IRQHandler,                           /*  38 - EMU            */

};

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void __attribute__((weak,naked)) Reset_Handler(void) {
uint32_t *pSrc, *pDest;
uint32_t *pTable __attribute__((unused));

#ifdef __SYSTEM_INIT_BEFORE_INIT
/* SystemInit runs with unitialized data */
  SystemInit();
#endif

/*  Firstly it copies data from read only memory to RAM. There are two schemes
 *  to copy. One can copy more than one sections. Another can only copy
 *  one section.  The former scheme needs more instructions and read-only
 *  data to implement than the latter.
 *  Macro __STARTUP_COPY_MULTIPLE is used to choose between two schemes.  */

#ifdef __STARTUP_COPY_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of triplets, each of which specify:
 *    offset 0: LMA of start of a section to copy from
 *    offset 4: VMA of start of a section to copy to
 *    offset 8: size of the section to copy. Must be multiply of 4
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
  pTable = &__copy_table_start__;

  for (; pTable < &__copy_table_end__; pTable = pTable + 3)
  {
    pSrc  = (uint32_t*)*(pTable + 0);
    pDest = (uint32_t*)*(pTable + 1);
    for (; pDest < (uint32_t*)(*(pTable + 1) + *(pTable + 2)) ; )
    {
      *pDest++ = *pSrc++;
    }
  }
#else
/*  Single section scheme.
 *
 *  The ranges of copy from/to are specified by following symbols
 *    __etext: LMA of start of the section to copy from. Usually end of text
 *    __data_start__: VMA of start of the section to copy to
 *    __data_end__: VMA of end of the section to copy to
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
  pSrc  = &__etext;
  pDest = &__data_start__;

  for ( ; pDest < &__data_end__ ; )
  {
    *pDest++ = *pSrc++;
  }
#endif /*__STARTUP_COPY_MULTIPLE */

/*  This part of work usually is done in C library startup code. Otherwise,
 *  define this macro to enable it in this startup.
 *
 *  There are two schemes too. One can clear multiple BSS sections. Another
 *  can only clear one section. The former is more size expensive than the
 *  latter.
 *
 *  Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
 *  Otherwise efine macro __STARTUP_CLEAR_BSS to choose the later.
 */
#ifdef __STARTUP_CLEAR_BSS_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __zero_table_start__ and __zero_table_end__,
 *  there are array of tuples specifying:
 *    offset 0: Start of a BSS section
 *    offset 4: Size of this BSS section. Must be multiply of 4
 */
  pTable = &__zero_table_start__;

  for (; pTable < &__zero_table_end__; pTable = pTable + 2)
  {
    pDest = (uint32_t*)*(pTable + 0);
    for (; pDest < (uint32_t*)(*(pTable + 0) + *(pTable + 1)) ; )
    {
      *pDest++ = 0;
    }
  }
#elif defined (__STARTUP_CLEAR_BSS)
/*  Single BSS section scheme.
 *
 *  The BSS section is specified by following symbols
 *    __bss_start__: start of the BSS section.
 *    __bss_end__: end of the BSS section.
 *
 *  Both addresses must be aligned to 4 bytes boundary.
 */
  pDest = &__bss_start__;

  for ( ; pDest < &__bss_end__ ; )
  {
    *pDest++ = 0ul;
  }
#endif /* __STARTUP_CLEAR_BSS_MULTIPLE || __STARTUP_CLEAR_BSS */

#ifdef __SYSTEM_INIT_AFTER_INIT
/* SystemInit runs with itialized data */
  SystemInit();

#endif

#ifndef DO_NO_INITIALIZE_SYSTEM_CORE_CLOCK
    SystemCoreClockGet();
#endif
    /* Initialize C library */
    _main();

    /* Call main routine (user code) */
    main();

    /* Stop */
    __STOP();
}

/*----------------------------------------------------------------------------
  Dummy SystemInit. Only used when there is no other SystemInit defined
 *----------------------------------------------------------------------------*/

void SystemInit(void) {

}

/*----------------------------------------------------------------------------
  Dummy _main. Only used for library initialization.
 *----------------------------------------------------------------------------*/

void _main(void) {

}


/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
uint32_t FaultInfo = 0;
uint8_t  VecActive = 0;
void Default_Handler(void) {

    FaultInfo = SCB->ICSR;
    VecActive = FaultInfo&0xFF;
    __STOP();
}
