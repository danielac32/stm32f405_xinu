//#include <os.h>
#include <memory.h>
#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>

extern void _estack(void); // fake definition, will be filled in by linker script.

// Hang, let the watchdog reboot us.
// TODO(lvd): reset usart0 and report unexpected irq
void default_IRQ_Handler(void) {
    uint32 * current_sp;

   asm volatile("\
        tst lr, #4; \
        ite eq; \
        mrseq %0, msp; \
        mrsne %0, psp; \
    " : "=r" (current_sp));


    for (int i = 0; i < 15; i++) {
        kprintf("STACK[%d]: %x\n", i, current_sp[i]);
    }
    
}
/* Define el prototipo de los manejadores de interrupciones */
void Reset_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void NMI_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));

/* Interrupciones específicas del STM32F405 */
void WWDG_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void PVD_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TAMP_STAMP_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RTC_WKUP_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FLASH_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RCC_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI0_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI1_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI2_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI3_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI4_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream0_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream1_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream2_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream3_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream4_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream5_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream6_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void ADC_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN1_TX_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN1_RX0_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN1_RX1_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN1_SCE_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI9_5_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_BRK_TIM9_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_UP_TIM10_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_TRG_COM_TIM11_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM1_CC_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM2_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM3_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM4_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C1_EV_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C1_ER_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C2_EV_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C2_ER_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI1_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI2_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void /*USART1_IRQHandler*/USART1_Handler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USART2_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USART3_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void EXTI15_10_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void RTC_Alarm_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void OTG_FS_WKUP_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM8_BRK_TIM12_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM8_UP_TIM13_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM8_TRG_COM_TIM14_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM8_CC_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA1_Stream7_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FMC_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SDIO_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM5_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void SPI3_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void UART4_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void UART5_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM6_DAC_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void TIM7_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream0_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream1_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream2_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream3_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream4_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void ETH_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void ETH_WKUP_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN2_TX_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN2_RX0_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN2_RX1_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void CAN2_SCE_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void OTG_FS_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream5_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream6_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DMA2_Stream7_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void USART6_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C3_EV_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void I2C3_ER_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void OTG_HS_EP1_OUT_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void OTG_HS_EP1_IN_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void OTG_HS_WKUP_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void OTG_HS_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void DCMI_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void HASH_RNG_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));
void FPU_IRQHandler(void) __attribute__((weak, alias("default_IRQ_Handler")));

/* Vector table */
__attribute__((section(".isr_vector")))
const uint32_t *vector_table[] = {
    (uint32_t *)_estack,      /* Initial Stack Pointer (RAM end address) */
    (uint32_t *)Reset_Handler,   /* Reset Handler */
    (uint32_t *)NMI_Handler,     /* NMI Handler */
    (uint32_t *)HardFault_Handler,   /* Hard Fault Handler */
    (uint32_t *)MemManage_Handler,   /* MPU Fault Handler */
    (uint32_t *)BusFault_Handler,    /* Bus Fault Handler */
    (uint32_t *)UsageFault_Handler,  /* Usage Fault Handler */
    0, 0, 0, 0,                 /* Reserved */
    (uint32_t *)SVC_Handler,     /* SVCall Handler */
    (uint32_t *)DebugMon_Handler,    /* Debug Monitor Handler */
    0,                          /* Reserved */
    (uint32_t *)PendSV_Handler,  /* PendSV Handler */
    (uint32_t *)SysTick_Handler, /* SysTick Handler */

    /* External Interrupts */
    (uint32_t *)WWDG_IRQHandler,             /* Window WatchDog */
    (uint32_t *)PVD_IRQHandler,              /* PVD through EXTI Line detection */
    (uint32_t *)TAMP_STAMP_IRQHandler,       /* Tamper and TimeStamps through EXTI Line */
    (uint32_t *)RTC_WKUP_IRQHandler,         /* RTC Wakeup through EXTI Line */
    (uint32_t *)FLASH_IRQHandler,            /* FLASH */
    (uint32_t *)RCC_IRQHandler,              /* RCC */
    (uint32_t *)EXTI0_IRQHandler,            /* EXTI Line0 */
    (uint32_t *)EXTI1_IRQHandler,            /* EXTI Line1 */
    (uint32_t *)EXTI2_IRQHandler,            /* EXTI Line2 */
    (uint32_t *)EXTI3_IRQHandler,            /* EXTI Line3 */
    (uint32_t *)EXTI4_IRQHandler,            /* EXTI Line4 */
    (uint32_t *)DMA1_Stream0_IRQHandler,     /* DMA1 Stream 0 */
    (uint32_t *)DMA1_Stream1_IRQHandler,     /* DMA1 Stream 1 */
    (uint32_t *)DMA1_Stream2_IRQHandler,     /* DMA1 Stream 2 */
    (uint32_t *)DMA1_Stream3_IRQHandler,     /* DMA1 Stream 3 */
    (uint32_t *)DMA1_Stream4_IRQHandler,     /* DMA1 Stream 4 */
    (uint32_t *)DMA1_Stream5_IRQHandler,     /* DMA1 Stream 5 */
    (uint32_t *)DMA1_Stream6_IRQHandler,     /* DMA1 Stream 6 */
    (uint32_t *)ADC_IRQHandler,              /* ADC1, ADC2 and ADC3 */
    (uint32_t *)CAN1_TX_IRQHandler,          /* CAN1 TX */
    (uint32_t *)CAN1_RX0_IRQHandler,         /* CAN1 RX0 */
    (uint32_t *)CAN1_RX1_IRQHandler,         /* CAN1 RX1 */
    (uint32_t *)CAN1_SCE_IRQHandler,         /* CAN1 SCE */
    (uint32_t *)EXTI9_5_IRQHandler,          /* External Line[9:5]s */
    (uint32_t *)TIM1_BRK_TIM9_IRQHandler,    /* TIM1 Break and TIM9 */
    (uint32_t *)TIM1_UP_TIM10_IRQHandler,    /* TIM1 Update and TIM10 */
    (uint32_t *)TIM1_TRG_COM_TIM11_IRQHandler,/* TIM1 Trigger and Commutation and TIM11 */
    (uint32_t *)TIM1_CC_IRQHandler,          /* TIM1 Capture Compare */
    (uint32_t *)TIM2_IRQHandler,             /* TIM2 */
    (uint32_t *)TIM3_IRQHandler,             /* TIM3 */
    (uint32_t *)TIM4_IRQHandler,             /* TIM4 */
    (uint32_t *)I2C1_EV_IRQHandler,          /* I2C1 Event */
    (uint32_t *)I2C1_ER_IRQHandler,          /* I2C1 Error */
    (uint32_t *)I2C2_EV_IRQHandler,          /* I2C2 Event */
    (uint32_t *)I2C2_ER_IRQHandler,          /* I2C2 Error */
    (uint32_t *)SPI1_IRQHandler,             /* SPI1 */
    (uint32_t *)SPI2_IRQHandler,             /* SPI2 */
    (uint32_t *)USART1_Handler,//(uint32_t *)USART1_IRQHandler,           /* USART1 */
    (uint32_t *)USART2_IRQHandler,           /* USART2 */
    (uint32_t *)USART3_IRQHandler,           /* USART3 */
    (uint32_t *)EXTI15_10_IRQHandler,        /* External Line[15:10]s */
    (uint32_t *)RTC_Alarm_IRQHandler,        /* RTC Alarm (A and B) through EXTI Line */
    (uint32_t *)OTG_FS_WKUP_IRQHandler,      /* USB OTG FS Wakeup through EXTI line */
    (uint32_t *)TIM8_BRK_TIM12_IRQHandler,   /* TIM8 Break and TIM12 */
    (uint32_t *)TIM8_UP_TIM13_IRQHandler,    /* TIM8 Update and TIM13 */
    (uint32_t *)TIM8_TRG_COM_TIM14_IRQHandler,/* TIM8 Trigger and Commutation and TIM14 */
    (uint32_t *)TIM8_CC_IRQHandler,          /* TIM8 Capture Compare */
    (uint32_t *)DMA1_Stream7_IRQHandler,     /* DMA1 Stream7 */
    (uint32_t *)FMC_IRQHandler,              /* FMC */
    (uint32_t *)SDIO_IRQHandler,             /* SDIO */
    (uint32_t *)TIM5_IRQHandler,             /* TIM5 */
    (uint32_t *)SPI3_IRQHandler,             /* SPI3 */
    (uint32_t *)UART4_IRQHandler,            /* UART4 */
    (uint32_t *)UART5_IRQHandler,            /* UART5 */
    (uint32_t *)TIM6_DAC_IRQHandler,         /* TIM6 and DAC1&2 underrun errors */
    (uint32_t *)TIM7_IRQHandler,             /* TIM7 */
    (uint32_t *)DMA2_Stream0_IRQHandler,     /* DMA2 Stream 0 */
    (uint32_t *)DMA2_Stream1_IRQHandler,     /* DMA2 Stream 1 */
    (uint32_t *)DMA2_Stream2_IRQHandler,     /* DMA2 Stream 2 */
    (uint32_t *)DMA2_Stream3_IRQHandler,     /* DMA2 Stream 3 */
    (uint32_t *)DMA2_Stream4_IRQHandler,     /* DMA2 Stream 4 */
    (uint32_t *)ETH_IRQHandler,              /* Ethernet */
    (uint32_t *)ETH_WKUP_IRQHandler,         /* Ethernet Wakeup through EXTI line */
    (uint32_t *)CAN2_TX_IRQHandler,          /* CAN2 TX */
    (uint32_t *)CAN2_RX0_IRQHandler,         /* CAN2 RX0 */
    (uint32_t *)CAN2_RX1_IRQHandler,         /* CAN2 RX1 */
    (uint32_t *)CAN2_SCE_IRQHandler,         /* CAN2 SCE */
    (uint32_t *)OTG_FS_IRQHandler,           /* USB OTG FS */
    (uint32_t *)DMA2_Stream5_IRQHandler,     /* DMA2 Stream 5 */
    (uint32_t *)DMA2_Stream6_IRQHandler,     /* DMA2 Stream 6 */
    (uint32_t *)DMA2_Stream7_IRQHandler,     /* DMA2 Stream 7 */
    (uint32_t *)USART6_IRQHandler,           /* USART6 */
    (uint32_t *)I2C3_EV_IRQHandler,          /* I2C3 event */
    (uint32_t *)I2C3_ER_IRQHandler,          /* I2C3 error */
    (uint32_t *)OTG_HS_EP1_OUT_IRQHandler,   /* USB OTG HS End Point 1 Out */
    (uint32_t *)OTG_HS_EP1_IN_IRQHandler,    /* USB OTG HS End Point 1 In */
    (uint32_t *)OTG_HS_WKUP_IRQHandler,      /* USB OTG HS Wakeup through EXTI */
    (uint32_t *)OTG_HS_IRQHandler,           /* USB OTG HS */
    (uint32_t *)DCMI_IRQHandler,             /* DCMI */
    (uint32_t *)HASH_RNG_IRQHandler,         /* Hash and RNG */
    (uint32_t *)FPU_IRQHandler               /* FPU */
};

 

/* Implementa los manejadores de interrupciones adicionales según sea necesario */
