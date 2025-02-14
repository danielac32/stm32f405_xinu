/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <stdio.h>
#include <gpio.h>
#include <fat_filelib.h>
#include <23lc.h>
#include "emulator.h"
 
#include "mem.h"
 


void example_usage() {
    // Alocar bloques de memoria en la memoria externa


    uint32_t freeSize = Memory_GetFreeSize();
    printf(" free: %d bytes\n", freeSize);
    uint32_t block1 = Memory_Allocate(32);
    freeSize = Memory_GetFreeSize();
        printf(" free: %d bytes\n", freeSize);
    uint32_t block2 = Memory_Allocate(100000);
    freeSize = Memory_GetFreeSize();
        printf(" free: %d bytes\n", freeSize);
    uint32_t block3 = Memory_Allocate(100000);
    freeSize = Memory_GetFreeSize();
        printf(" free: %d bytes\n", freeSize);
    uint32_t block4 = Memory_Allocate(100000);


    freeSize = Memory_GetFreeSize();
   printf(" free: %d bytes\n", freeSize);
    
    if (block1 && block2) {
        // Escribir datos en block1
        uint8_t dataToWrite[32]; // Datos de ejemplo
        memset(dataToWrite,0xaa,sizeof(dataToWrite));
        Memory_Write(block1, dataToWrite, sizeof(dataToWrite));

        // Leer datos de block1
        uint8_t readBuffer[32];
        Memory_Read(block1, readBuffer, sizeof(readBuffer));

        // Imprimir datos leídos
        for (size_t i = 0; i < sizeof(readBuffer); i++) {
            printf("readBuffer[%d] = %d\n", i, readBuffer[i]);
        }

        // Liberar bloques de memoria
        Memory_Free(block1);
        Memory_Free(block2);
        Memory_Free(block3);
        Memory_Free(block4);
        freeSize = Memory_GetFreeSize();
        printf(" free: %d bytes\n", freeSize);


    } else {
        printf("Error: No se pudo alocar memoria\n");
    }
}




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

  /*cachebegin(300000);

    for (int i = 0; i < 300000; ++i)
    {
        cachewrite(i,0x33);
    }
    cachewrite(300000,0xbe);
    cachewrite(300000-1,0xbe);
    cachewrite(300000-2,0xbe);
    cachewrite(300000-3,0xca);
    cachewrite(300000-4,0xfe);
 
    uint8 c = cacheread(300000);
    printf("%02x\n",c);


    c = cacheread(0);
    printf("%02x\n",c);

    c = cacheread(300000-1);
    printf("%02x\n",c);

    c = cacheread(300000-2);
    printf("%02x\n",c);

    c = cacheread(300000-3);
    printf("%02x\n",c);

    c = cacheread(300000-4);
    printf("%02x\n",c);
*/

   Memory_Init();
   example_usage();


    printf( "\nStarting RISC-V VM\n\n\r" );
    int c = riscv_emu();
    while ( c == EMU_REBOOT ) c = riscv_emu();
  
   



	return 0;
}
