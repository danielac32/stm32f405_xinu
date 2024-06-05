
#include <xinu.h>
#include <gpio.h>


void USART1_Handler(){
uint32 q = disable();
ttyhandler(1,USART1->DR,0);
restore(q);
}


void uartinit(){
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    // IO port A clock enable
//    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    hw_cfg_pin(GPIOx(PORT_TX), PIN_TX, GPIOCFG_MODE_ALT | 7 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(PORT_RX), PIN_RX, GPIOCFG_MODE_ALT | 7 | GPIOCFG_OSPEED_VHIGH | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    USART1->CR1 &= ~ (1 << UART_EN);

    USART1->BRR = (USART1_DIV_M << 4) | USART1_DIV_F;
    // OVER8 = 0: oversampling by 16
    // TE = 1: Transmitter is enabled
    // RE = 1: Receiver is enabled and begins searching for a start bit
    // UE = 1: USART enable
    USART1->CR1 |= (1 << UART_INTR_RX) | (1 << UART_TX_EN) | (1 << UART_RX_EN);
    USART1->CR1 |= (1 << UART_EN);
    NVIC_EnableIRQ(USART1_IRQn);
}

void putcharuart(char ch)
{
	while (!(USART1->SR & USART_SR_TXE));
	USART1->DR = ch;
}

