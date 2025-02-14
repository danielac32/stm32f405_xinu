#ifndef SRAM_H_
#define SRAM_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define RDSR        5
#define WRSR        1
#define READ        3
#define WRITE       2

#define MEMORY_SIZE  0x20000// 0x1f400// 0x1e848//0x20000 // 128KB en bytes (0x20000)
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

 
void sramread(uint32_t ,void *,uint32_t );
void sramwrite(uint32_t , void *,uint32_t );

uint8_t sramreadbyte(uint32_t );
void sramwritebyte(uint32_t , uint8_t ); 

uint16_t sramread2(uint32_t );
void sramwrite2(uint32_t , uint16_t ); 

uint32_t sramread4(uint32_t );
void sramwrite4(uint32_t , uint32_t ); 
#endif
