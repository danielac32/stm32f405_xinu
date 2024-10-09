#include <xinu.h>
#include <w25qxxx.h>
#include <gpio.h>
#include <23lc.h>
//#include <fat_filelib.h>
#include <mem.h>
#include "emulator.h"


 

int time_divisor = 256;
uint8_t fast_mode = 1;
Memory mem;
static uint32_t HandleException( uint32_t ir, uint32_t retval );
static uint32_t HandleControlStore( uint32_t addy, uint32_t val );
static uint32_t HandleControlLoad( uint32_t addy );
static void HandleOtherCSRWrite( uint8_t *image, uint16_t csrno, uint32_t value );
static uint32_t HandleOtherCSRRead( uint8_t *image, uint16_t csrno );

#define INSTRS_PER_FLIP 1024
#define MICROSECOND_TICKS cycleCount() 

#define MINIRV32WARN( x... ) printf( x );
#define MINIRV32_DECORATE static
#define MINI_RV32_RAM_SIZE 16*1024*1024
#define MINIRV32_IMPLEMENTATION
#define MINIRV32_POSTEXEC( pc, ir, retval )             \
	{                                                   \
		if ( retval > 0 )                               \
				retval = HandleException( ir, retval ); \
	}

#define MINIRV32_HANDLE_MEM_STORE_CONTROL( addy, val ) if ( HandleControlStore( addy, val ) ) return val;
#define MINIRV32_HANDLE_MEM_LOAD_CONTROL( addy, rval ) rval = HandleControlLoad( addy );
#define MINIRV32_OTHERCSR_WRITE( csrno, value ) HandleOtherCSRWrite( image, csrno, value );
#define MINIRV32_OTHERCSR_READ( csrno, rval )  { rval = HandleOtherCSRRead( image, csrno ); }

#define MINIRV32_CUSTOM_MEMORY_BUS
 
 static void MINIRV32_STORE4(uint32_t ofs, uint32_t val) {
   // write_memory_4("dram", ofs, val);
	//psram_write(ofs,&val,4);
	cache_write(ofs,&val,4);
    //sramwrite(ofs,&val,4);
}

static void MINIRV32_STORE2(uint32_t ofs, uint16_t val) {
    //write_memory_2("dram", ofs, val);
    //psram_write(ofs,&val,2);
    cache_write(ofs,&val,2);
    //sramwrite(ofs,&val,2);
}

static void MINIRV32_STORE1(uint32_t ofs, uint8_t val) {
    //write_memory_1("dram", ofs, val);
    //psram_write(ofs,&val,1);
    cache_write(ofs,&val,1);
    //sramwrite(ofs,&val,1);
}

static uint32_t MINIRV32_LOAD4(uint32_t ofs) {
    uint32_t val = 0;
    //return read_memory_4("dram", ofs);
    //psram_read(ofs,&val,4);
    cache_read(ofs,&val,4);
    //sramread(ofs,&val,4);
    return val;
}

static uint16_t MINIRV32_LOAD2(uint32_t ofs) {
    uint16_t val = 0;
    //return read_memory_2("dram", ofs);
    //psram_read(ofs,&val,2);
    cache_read(ofs,&val,2);
    //sramread(ofs,&val,2);
    return val;
}

static uint8_t MINIRV32_LOAD1(uint32_t ofs) {
	uint8_t val = 0;
      //return read_memory_1("dram", ofs);//(uint8_t)ram_image[ofs];
    //psram_read(ofs,&val,1);
    cache_read(ofs,&val,1);
    //sramread(ofs,&val,1);
    return val;
}

#include "mini-rv32ima.h"

struct MiniRV32IMAState core;

int riscv_emu()
{
	
   // cache_reset();
	//mem = create_memory("riscv/kernel.bin");

	printf("........\n");
	load_ram_file( 0, "riscv/kernel.bin" );

	//mem = create_memory("riscv/kernel.bin");

	core.regs[10] = 0x00; // hart ID
	core.regs[11] = 0;
	core.extraflags |= 3; // Machine-mode.

	core.pc = MINIRV32_RAM_IMAGE_OFFSET;
	long long instct = -1;
    int instrs_per_flip = 1024;
    uint64_t lastTime = cycleCount();
	while(1){
		
		int ret;
		uint64_t *this_ccount = ((uint64_t*)&core.cyclel);
		uint32_t elapsedUs = cycleCount() - lastTime;
		lastTime += elapsedUs;

        //uint32_t q = disable();
		ret = MiniRV32IMAStep( &core, NULL, 0, elapsedUs, instrs_per_flip ); // Execute upto 1024 cycles before breaking out.
		switch ( ret )
		{
			case 0: break;
			case 1:
				 sleepms(1);
				break;
			case 3: instct = 0; break;
			case 0x7777:
				printf( "\n\rREBOOT@0x%08x%08x\n\r", (unsigned int)core.cycleh,  (unsigned int)core.cyclel );
				 
				//cache_reset();
				return EMU_REBOOT; // syscon code for reboot
			case 0x5555:
				printf( "\n\rPOWEROFF@0x%08x%08x\n\r",  (unsigned int)core.cycleh,  (unsigned int)core.cyclel );
				return EMU_POWEROFF; // syscon code for power-off
			default:
				printf( "\n\rUnknown failure\n" );
				return EMU_UNKNOWN;
				break;
		}
        //restore(q);
	}

	 
	return EMU_UNKNOWN;
}

//////////////////////////////////////////////////////////////////////////
// Functions for the emulator
//////////////////////////////////////////////////////////////////////////

// Exceptions handling

static uint32_t HandleException( uint32_t ir, uint32_t code )
{
	// Weird opcode emitted by duktape on exit.
	if ( code == 3 )
	{
		// Could handle other opcodes here.
	}
	return code;
}

// CSR handling (Linux HVC console)


int load_ram_file( uint32_t addr,  char *filename )
{
    FILE* fd;
    //uint32_t q=disable();

    char *tmp=full_path(filename);
    if (tmp==NULL){
        //restore(q);
        return -1;
    }
    if (!(fd = fopen(tmp,"r"))){
        printf("%s not found\n", tmp);
       // restore(q);
        return -1;
    }
    uint8 buff[64];
    unsigned int part=0;
    fseek(fd, 0, SEEK_END);
    unsigned int fileLength = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    printf("size file %d\n",fileLength);
    
    printf( "Loading image into RAM\n\r" );
    uint32 br=0;
    
    #if 0
    while(part<fileLength){
         //syscallp.seek(fs,part,LFS_SEEK_SET);
         br=fread(buff, 64,1,fd);
         SPI_Flash_Write( buff,addr+part, br );
         memset(buff,0,br);
         printf("%d->%d\n",part, fileLength);
         part += br;
         hw_toggle_pin(GPIOx(GPIO_C),13);
    }
    #else
    char r;
    while(!feof(fd)){
          char c = (char)fgetc(fd);
          sramwrite(addr,&c,1);
          sramread(addr,&r,1);
          if(r!=c){
          	printf("error write :%d %x\n",addr,c);
          	break;
          }
          addr+=1;
    }
    #endif
    fclose(fd);
    update_path();
    printf("end load\n");
   // restore(q);
    return 0;
}


static inline void HandleOtherCSRWrite( uint8_t *image, uint16_t csrno, uint32_t value )
{
	if( csrno == 0x402 ){
		printf( "%c", value );
	}else if( csrno == 0x136 )
	{
		printf( "%d", value ); 
	}else if( csrno == 0x137 )
	{
		printf( "%08x", value ); 
	}else if (csrno == 0x401){//upload
		uint32_t start = value - MINIRV32_RAM_IMAGE_OFFSET;
		printf("%08x\n", start);
		//load_ram_file( start, "riscv/fs2.img" ); 
	}

}

static inline uint32_t HandleOtherCSRRead( uint8_t *image, uint16_t csrno )
{
	 

	return 0;
}

// MMIO handling (8250 UART)


extern void putcharuart(char ch);
extern char uart_get(void);
extern bool uart_available(void);

static uint32_t HandleControlStore( uint32_t addy, uint32_t val )
{
	if ( addy == 0x10000000 ) // UART 8250 / 16550 Data Buffer
		putcharuart( val );

	return 0;
}

static uint32_t HandleControlLoad( uint32_t addy )
{

	if( addy == 0x10000005 )
		return uart_available();
	else if( addy == 0x10000000 )
		return uart_get();

	return 0;
}