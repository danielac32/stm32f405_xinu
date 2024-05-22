#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <memory.h>
#include <kernel.h>
#include <delay.h>
#include <gpio.h>
#include <uart.h>
#include <math.h>
//#include <usb.h>
//#include "usb_cdc.h"
//#include "usb-msc.h"
#include "sd-spi.h"
#include "disk.h"
#include "fat_filelib.h"
#include <flash.h>
#include <usb_cdc_conf.h>


extern uint32_t SystemCoreClock;
extern void check_msc();






int main()
{

    //char buff[100];

	//kprintf("bootloader sdcard\n");
     

    //sprintf(buff,"(%s)", menu[0]);
    //GFX_print(buff);
    
    //sprintf(buff,"(%s)", menu[1]);
    //GFX_print(buff);

    //hw_cfg_pin(GPIOx(GPIO_C),13,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(GPIO_A),0,GPIOCFG_MODE_INP | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_OPEN | GPIOCFG_PUPD_PUP);
    //hw_set_pin(GPIOx(GPIO_C),13, 1);
    //hw_cfg_pin(GPIOx(GPIO_C),9,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    hw_cfg_pin(GPIOx(GPIO_A),8,GPIOCFG_MODE_OUT | GPIOCFG_OSPEED_VHIGH  | GPIOCFG_OTYPE_PUPD | GPIOCFG_PUPD_PUP);
    
    
     while(1){
        if(!hw_get_pin(GPIOx(GPIO_A),0)){
            break;
        }
        
        hw_toggle_pin(GPIOx(GPIO_A),8);


        //usbd_poll(&udev);
        delay(70);
    }
    

    //check_msc();

    uint32 size;
    again:
    size = sd_init();
    if(size<0){
        kprintf("error init memory\n");
         
        return -1;
    }
    fl_init();
    if (fl_attach_media(sd_readsector, sd_writesector) != FAT_INIT_OK){
        kprintf("ERROR: Failed to init file system\n");
         
        if (fl_format(size, "")){
            kprintf("format ok\n");
            
            goto again;
        }
      return -1;
    }
    fl_listdirectory("/");
 
    
    panic("ok");
    while(1);
}