
#include <spi.h>
#include <interrupt.h>
#include <23lc.h>
#include <gpio.h>

#define MEMORY_SIZE    0x1e848//0x20000 // 128KB en bytes (0x20000)
#define MEMORY_COUNT   3        // Número de memorias
#define PSRAM_CMD_WRITE 0x02  // Comando de escritura para 23LC1024
#define PSRAM_CMD_READ  0x03  // Comando de lectura para 23LC1024

#define RDMR        5       // Read the Mode Register
#define WRMR        1       // Write to the Mode Register
#define READ        3       // Read command
#define WRITE       2       // Write command
#define RSTIO     0xFF      // Reset memory to SPI mode
#define ByteMode    0x00    // Byte mode (read/write one byte at a time)
#define Sequential  0x40    // Sequential mode (read/write blocks of memory)


void SRAM_SPI_WRITE( uint8_t *buf, uint32_t size) {
    // uint8_t *data = ( uint8_t *)buf;
    while (size--) {
        hal_23lc1024_spi_txrx(*buf++);
    }
}

void SRAM_SPI_READ(uint8_t *buf, uint32_t size) {
    while (size--) {
        *buf++ = hal_23lc1024_spi_txrx(0);
    }
}

static void select_memory(uint8_t memoryIndex) {
    // Poner todos los pines en alto (1)
    //hal_23lc1024_spi_select1_set(1);
    //hal_23lc1024_spi_select2_set(1);
    //hal_23lc1024_spi_select3_set(1);

    // Poner en bajo (0) solo el pin de la memoria seleccionada
    switch (memoryIndex) {
        case 0: hal_23lc1024_spi_select1_set(0); break;
        case 1: hal_23lc1024_spi_select2_set(0); break;
        case 2: hal_23lc1024_spi_select3_set(0); break;
    }
}



static void release(uint8_t memoryIndex){

	switch (memoryIndex) {
        case 0: hal_23lc1024_spi_select1_set(1); break;
        case 1: hal_23lc1024_spi_select2_set(1); break;
        case 2: hal_23lc1024_spi_select3_set(1); break;
    }
}

static void setMode(uint8_t memoryIndex){
    select_memory(0);
    hal_23lc1024_spi_txrx(WRMR);
    hal_23lc1024_spi_txrx(Sequential);
    release(0);
}



void sramwrite(uint32_t addr, void *ptr, uint32_t size){
       // uint32_t q = disable();
        uint8_t cmdAddr[5];
	    uint8_t *p = (uint8_t *)ptr;
	    uint8_t memoryIndex = addr / MEMORY_SIZE; // 0, 1 o 2
        uint32_t memoryAddr = addr % MEMORY_SIZE;

        // Asegurarse de no exceder el tamaño de la memoria
        uint32_t writeSize = (size > (MEMORY_SIZE - memoryAddr)) ? (MEMORY_SIZE - memoryAddr) : size;

        // Configurar comando y dirección
        cmdAddr[0] = PSRAM_CMD_WRITE;
        cmdAddr[1] = (memoryAddr >> 16) & 0xff;
        cmdAddr[2] = (memoryAddr >> 8) & 0xff;
        cmdAddr[3] = memoryAddr & 0xff;

        //setMode(memoryIndex);
        select_memory(memoryIndex);
        SRAM_SPI_WRITE(cmdAddr, 4);
        SRAM_SPI_WRITE(p, writeSize);
        release(memoryIndex);
        //restore(q);
}
 
void sramread(uint32_t addr,void *ptr,uint32_t size){
        
        //uint32_t q = disable();
	    uint8_t cmdAddr[5];
	    uint8_t *p = (uint8_t *)ptr;
	    uint8_t memoryIndex = addr / MEMORY_SIZE; // 0, 1 o 2
        uint32_t memoryAddr = addr % MEMORY_SIZE;

        // Asegurarse de no exceder el tamaño de la memoria
        //uint32_t readSize = (size > (MEMORY_SIZE - memoryAddr)) ? (MEMORY_SIZE - memoryAddr) : size;

        // Configurar comando y dirección
        cmdAddr[0] = PSRAM_CMD_READ;
        cmdAddr[1] = (memoryAddr >> 16) & 0xff;
        cmdAddr[2] = (memoryAddr >> 8) & 0xff;
        cmdAddr[3] = memoryAddr & 0xff;
        //setMode(memoryIndex);
        select_memory(memoryIndex);
        SRAM_SPI_WRITE(cmdAddr, 4);
        SRAM_SPI_READ(p, size);
        release(memoryIndex);
        //restore(q);
}


