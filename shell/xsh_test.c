/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <stdio.h>
#include <gpio.h>
#include <fat_filelib.h>
#include <23lc.h>
#include "emulator.h"
 
shellcmd xsh_test(int nargs, char *args[])
{
	 #if 0
     uint32 c=0;
     uint8 a=0xcc;
     uint16 b=0xdecc;

     for (int i = 0; i < 16; ++i)
     {
         //sramwrite2(i,b);//
         sramwrite(i,&a,1);
          //sramwritebyte(i,0xca);
     }
     
     a=0xbb;
     for (int i = 16; i < 32; ++i)
     {
         //sramwrite2(i,b);//
         sramwrite(i,&a,1);
          //sramwritebyte(i,0xce);
     }
     a=0xff;
     for (int i = 32; i < 48; ++i)
     {
         //sramwrite2(i,b);//
         sramwrite(i,&a,1);
          //sramwritebyte(i,0xff);
     }
     a=0xaa;
     for (int i = 48; i < 64; ++i)
     {
         //sramwrite2(i,b);//
         sramwrite(i,&a,1);
          //sramwritebyte(i,0xfe);
     }
      


     a=0x33;
     for (int i = 127000; i < 127000+64; ++i)
     {
         sramwrite(i,&a,1);
     }
     

     a=0x77;
     for (int i = 128000; i < 128000+64; ++i)
     {
         sramwrite(i,&a,1);
     }


     a=0x11;
     for (int i = 129000; i < 129000+64; ++i)
     {
         sramwrite(i,&a,1);
     }

     a=0xdd;
     for (int i = 200000; i < 200000+64; ++i)
     {
         sramwrite(i,&a,1);
     }
     

     a=0xff;
     for (int i = 300000; i < 300000+64; ++i)
     {
         sramwrite(i,&a,1);
     }
     

     //384000
    
    c=0xbebecafe;
    a=0x00;
     for (int i = 0; i < 16; ++i) {
        sramwrite(300064 + (i * 4), &c, sizeof(c)); // Escribe en la dirección correcta
    }

 
     //sramwrite(300064,&c,4);
     //sramwrite(300068,&c,4);


     uint32 f;
     uint8 tmp;

     char buff[64];

        sramread(0,&buff,64);
        hexDump2(0,buff,64);

        for (int i = 0; i < 64; ++i){
            sramread(i,&tmp,1);
            printf("%x\n",tmp );
        }

        sramread(127000,&buff,64);
        hexDump2(127000,buff,64);
        
        sramread(128000,&buff,64);
        hexDump2(128000,buff,64);

        sramread(129000,&buff,64);
        hexDump2(129000,buff,64);

        sramread(200000,&buff,64);
        hexDump2(200000,buff,64);

        sramread(300000,&buff,64);
        hexDump2(300000,buff,64);
        
        sramread(300064,&buff,64);
        hexDump2(300064,buff,64);

            #if 0
    load_ram_file( 0, "riscv/kernel.bin" );

    char buff[64];
    sramread(0,&buff,64);
    hexDump2(0,buff,64);

    sramread(0x2d0b0,&buff,64);
    hexDump2(0x2d0b0,buff,64);

    sramread(0x2fc30,&buff,64);
    hexDump2(0x2fc30,buff,64);
    #endif
#endif


    //load_ram_file( 0, "riscv/kernel.bin" );
    /*char buff[64];
    int part=0;

    while(part<93880){
         sramread(part,&buff,64);
         hexDump2(part,buff,64);
         memset(buff,0,64);
         part += 64;
    }*/
    printf( "\nStarting RISC-V VM\n\n\r" );
    int c = riscv_emu();
    while ( c == EMU_REBOOT ) c = riscv_emu();

	return 0;
}
