/* Xinu for STM32
 *
 * Original license applies
 * Modifications for STM32 by Robin Krens
 * Please see LICENSE and AUTHORS 
 * 
 * $LOG$
 * 2019/11/11 - ROBIN KRENS
 * Initial version 
 * 
 * $DESCRIPTION$
 *
 * */

/* Control register */

#define USART1_DIV_M       45
// USARTDIV_Fraction = 0.573 / (1 / 16) = 0.573 / 0.0625 ~= 9
#define USART1_DIV_F       9

#define PORT_TX        GPIO_A   // PA9  --> RXD
#define PIN_TX         9
#define PORT_RX        GPIO_A   // PA10 <-- TXD
#define PIN_RX         10




#define UART_EN		13 
#define UART_INTR_RX	5
#define UART_INTR_TCIE	6
#define UART_INTR_TX	7
#define UART_TX_EN 	3
#define UART_RX_EN 	2

#define PARITY		12

/* Status register */
#define UART_RXNE	0x20
#define UART_TC		0x40
#define UART_TC_TXE	0xC0

#define UART_TEST	6

#define UART_FIFO_SIZE	1
#define UARTBUF 256
#define ECHO 1
