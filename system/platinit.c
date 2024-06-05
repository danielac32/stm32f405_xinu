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

/* platinit.c - platinit */

#include <stm32.h>
#include <spi.h>
#include <w25qxxx.h>
#include <interrupt.h>
/*------------------------------------------------------------------------
 * platinit - platform specific initialization
 *------------------------------------------------------------------------
 */
extern void uartinit();
void platinit(void)
{
	//hal_w25q_spi_init();
    //SPI_Flash_Init();
    uartinit();
}

 