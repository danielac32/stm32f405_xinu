#include <xinu.h>
#include <gpio.h>

devcall	initLED( struct dentry *devptr){
	hw_cfg_pin(GPIOx(GPIO_A),     8,     GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
	return OK;
}