#ifndef SRAM_H_
#define SRAM_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define RDSR        5
#define WRSR        1
#define READ        3
#define WRITE       2

 
void sramread(uint32_t ,void *,uint32_t );
void sramwrite(uint32_t , void *,uint32_t );

uint8_t sramreadbyte(uint32_t );
void sramwritebyte(uint32_t , uint8_t ); 

uint16_t sramread2(uint32_t );
void sramwrite2(uint32_t , uint16_t ); 

uint32_t sramread4(uint32_t );
void sramwrite4(uint32_t , uint32_t ); 
#endif
