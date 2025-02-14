#include <fat_filelib.h>
#include <stdint.h>
#include <23lc.h>

#define DRAM_SIZE MEMORY_SIZE*3//125000 // Tamaño de la memoria RAM simulada
#define BLOCK_SIZE 64
typedef struct {
    FILE *file;
    uint32_t size;
    char *p;
} Memory;


extern Memory mem;

uint32_t get_file_size(FILE *file);
Memory create_memory(const char *filename);
void free_memory(Memory *mem); 


//int memory_read(uint32_t addr, void *buf, int len);
//int memory_write(uint32_t addr, void *buf, int len);

void cache_write(uint32_t ofs, void *buf, uint32_t size);
void cache_read(uint32_t ofs, void *buf, uint32_t size);
void cache_get_stat(uint64_t *phit, uint64_t *paccessed);


#define MEMORY_TOTAL_SIZE  DRAM_SIZE // Tamaño total de la memoria externa (64 KB, ajustable)
#define BLOCK_HEADER_SIZE  sizeof(MemoryBlockHeader)

// Estructura del encabezado del bloque en memoria externa
typedef struct MemoryBlockHeader {
    uint32_t size;         // Tamaño del bloque, incluyendo el encabezado
    uint8_t isFree;        // Indicador de si el bloque está libre (1) o usado (0)
    uint32_t nextAddress;  // Dirección del siguiente bloque (0 si es el último)
} MemoryBlockHeader;



 
void Memory_Init();
//void* Memory_Allocate(uint32_t size);
//void Memory_Free(void* ptr);
uint32_t Memory_Allocate(uint32_t size);
void Memory_Free(uint32_t ptr);
uint32_t Memory_GetFreeSize();

//void Memory_Write(void* block, uint8_t* data, size_t len);
//void Memory_Read(void* block, uint8_t* buffer, size_t len);

void Memory_Write(uint32_t block, uint8_t* data, size_t len);
void Memory_Read(uint32_t block, uint8_t* buffer, size_t len);