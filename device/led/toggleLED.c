#include <xinu.h>
#include <gpio.h>


devcall	toggleLED( struct dentry *devptr){
    hw_toggle_pin(GPIOx(GPIO_A),8);
	return OK;
}