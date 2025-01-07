/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <stdio.h>
#include <gpio.h>
#include <fat_filelib.h>
#include <23lc.h>
#include "emulator.h"
 
shellcmd xsh_test(int nargs, char *args[])
{
	 



   /* int cc;
    int r;
    for (int i = 0; i < 125000; ++i)
    {     
          cc = i % 0xff;
          sramwrite(i,&cc,1);
          sramread(i,&r,1);
          if(r!=cc){
            printf("error write :%d %x %x\n",i,cc,r);
            break;
          }
    }
    return 0;
 */
    printf( "\nStarting RISC-V VM\n\n\r" );
    int c = riscv_emu();
    while ( c == EMU_REBOOT ) c = riscv_emu();

	return 0;
}
