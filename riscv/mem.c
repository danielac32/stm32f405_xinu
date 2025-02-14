

#include <xinu.h>
#include <mem.h>
#include <fat_filelib.h>
///#include <23lc.h>

extern void exit(int);

uint32_t get_file_size(FILE *file) {
    fseek(file, 0, SEEK_END); // Moverse al final del archivo
    uint32_t size = ftell(file); // Obtener la posición actual
    fseek(file, 0, SEEK_SET);//rewind(file); // Volver al inicio del archivo
    return size; // Retornar el tamaño
}

 

Memory create_memory(const char *filename) {
    Memory mem;

    char *tmp=full_path(filename);

    mem.file = fopen(tmp, "rb"); // Abrir el archivo en modo lectura
    if (mem.file == NULL) {
        printf("Error: No se pudo abrir el archivo.\n");
        exit(-1);
    }

    // Obtener el tamaño del archivo original
    mem.size = get_file_size(mem.file);
    /*mem.p = (char *)malloc(90000);
    memset(mem.p,0,90000);
    uint8_t buffer[BLOCK_SIZE]; // Buffer para copiar datos
    size_t bytes_read;
    int part=0;
    uint32 br=0;
    fread(mem.p, mem.size,1,mem.file);*/
    uint8_t buffer[BLOCK_SIZE];
    uint32 part=0;
    uint32 br=0;
    

    printf("clear ram.... %d\n",DRAM_SIZE);
    for (int i = 0; i < DRAM_SIZE; ++i)
    {
       cache_write(i,0,1);// sramwrite(i,0,1);
    }
    /*while(part<mem.size){
            br=fread(buffer, BLOCK_SIZE,1,mem.file);
            //fwrite(buffer, BLOCK_SIZE,1,dram_file);
       //     memcpy(mem.p,buffer,br);
            sramwrite(part,buffer,br);
            memset(buffer,0,br);
            part += br;
          
    }*/


    printf("load kernel....\n");
    char r;
    while(!feof(mem.file)){
          char c = (char)fgetc(mem.file);
          cache_write(part,&c,1);//sramwrite(part,&c,1);
          cache_read(part,&r,1);//sramread(part,&r,1);
          if(r!=c){
            printf("error write :%d %x\n",part,c);
            break;
          }
          part+=1;
    }
 
    fclose(mem.file);
    //hexDump2(0,mem.p,mem.size);
    printf("END\n");
    return mem;
}

 
 
void Memory_Init() {
    MemoryBlockHeader initialBlock = {
        .size = MEMORY_TOTAL_SIZE,
        .isFree = 1,
        .nextAddress = 0
    };

    sramwrite(0,&initialBlock, BLOCK_HEADER_SIZE);
    //SPI_Write(0, &initialBlock, BLOCK_HEADER_SIZE); // Escribir encabezado en dirección 0
}


/*
void* Memory_Allocate(uint32_t size) {
    uint32_t currentAddress = 0;
    MemoryBlockHeader currentBlock;

    while (currentAddress < MEMORY_TOTAL_SIZE) {
        //SPI_Read(currentAddress, &currentBlock, BLOCK_HEADER_SIZE);
        sramread(currentAddress,&currentBlock, BLOCK_HEADER_SIZE);
        if (currentBlock.isFree && currentBlock.size >= size + BLOCK_HEADER_SIZE) {
            // Dividir el bloque si sobra espacio
            uint32_t remainingSize = currentBlock.size - size - BLOCK_HEADER_SIZE;
            if (remainingSize > BLOCK_HEADER_SIZE) {
                MemoryBlockHeader newBlock = {
                    .size = remainingSize,
                    .isFree = 1,
                    .nextAddress = currentBlock.nextAddress
                };

                // Escribir el nuevo bloque en la memoria
                uint32_t newBlockAddress = currentAddress + BLOCK_HEADER_SIZE + size;
                sramwrite(newBlockAddress, &newBlock, BLOCK_HEADER_SIZE);

                // Actualizar el bloque actual
                currentBlock.size = size + BLOCK_HEADER_SIZE;
                currentBlock.nextAddress = newBlockAddress;
            }

            currentBlock.isFree = 0;
            sramwrite(currentAddress, &currentBlock, BLOCK_HEADER_SIZE);

            return (void*)(currentAddress + BLOCK_HEADER_SIZE);
        }

        // Mover al siguiente bloque
        if (currentBlock.nextAddress == 0) break;
        currentAddress = currentBlock.nextAddress;
    }

    return NULL; // Sin espacio disponible
}


void Memory_Free(void* ptr) {
    uint32_t blockAddress = (uint32_t)ptr - BLOCK_HEADER_SIZE;
    MemoryBlockHeader currentBlock;

    sramread(blockAddress, &currentBlock, BLOCK_HEADER_SIZE);
    currentBlock.isFree = 1;
    sramwrite(blockAddress, &currentBlock, BLOCK_HEADER_SIZE);

    // Intentar combinar bloques adyacentes
    uint32_t nextAddress = currentBlock.nextAddress;
    if (nextAddress != 0) {
        MemoryBlockHeader nextBlock;
        sramread(nextAddress, &nextBlock, BLOCK_HEADER_SIZE);

        if (nextBlock.isFree) {
            currentBlock.size += nextBlock.size;
            currentBlock.nextAddress = nextBlock.nextAddress;
            sramwrite(blockAddress, &currentBlock, BLOCK_HEADER_SIZE);
        }
    }
}
*/
uint32_t Memory_Allocate(uint32_t size) {
    uint32_t currentAddress = 0;
    MemoryBlockHeader currentBlock;

    while (currentAddress < MEMORY_TOTAL_SIZE) {
        // Leer el bloque actual de la memoria externa
        sramread(currentAddress, &currentBlock, BLOCK_HEADER_SIZE);
        
        // Verificar si el bloque es libre y suficientemente grande
        if (currentBlock.isFree && currentBlock.size >= size + BLOCK_HEADER_SIZE) {
            // Dividir el bloque si sobra espacio
            uint32_t remainingSize = currentBlock.size - size - BLOCK_HEADER_SIZE;
            if (remainingSize > BLOCK_HEADER_SIZE) {
                MemoryBlockHeader newBlock = {
                    .size = remainingSize,
                    .isFree = 1,
                    .nextAddress = currentBlock.nextAddress
                };

                // Escribir el nuevo bloque en la memoria
                uint32_t newBlockAddress = currentAddress + BLOCK_HEADER_SIZE + size;
                sramwrite(newBlockAddress, &newBlock, BLOCK_HEADER_SIZE);

                // Actualizar el bloque actual
                currentBlock.size = size + BLOCK_HEADER_SIZE;
                currentBlock.nextAddress = newBlockAddress;
            }

            // Marcar el bloque actual como ocupado
            currentBlock.isFree = 0;
            sramwrite(currentAddress, &currentBlock, BLOCK_HEADER_SIZE);

            // Retornar la dirección de inicio del bloque asignado (después del encabezado)
            return currentAddress + BLOCK_HEADER_SIZE;
        }

        // Mover al siguiente bloque
        if (currentBlock.nextAddress == 0) break;
        currentAddress = currentBlock.nextAddress;
    }

    return 0; // Sin espacio disponible
}

void Memory_Free(uint32_t address) {
    uint32_t blockAddress = address - BLOCK_HEADER_SIZE;
    MemoryBlockHeader currentBlock;

    // Leer el bloque actual
    sramread(blockAddress, &currentBlock, BLOCK_HEADER_SIZE);
    currentBlock.isFree = 1; // Marcar como libre
    sramwrite(blockAddress, &currentBlock, BLOCK_HEADER_SIZE);

    // Intentar combinar con el bloque siguiente
    uint32_t nextAddress = currentBlock.nextAddress;
    if (nextAddress != 0) {
        MemoryBlockHeader nextBlock;
        sramread(nextAddress, &nextBlock, BLOCK_HEADER_SIZE);

        if (nextBlock.isFree) {
            // Combinar bloques
            currentBlock.size += nextBlock.size;
            currentBlock.nextAddress = nextBlock.nextAddress;
            sramwrite(blockAddress, &currentBlock, BLOCK_HEADER_SIZE);
        }
    }
}


uint32_t Memory_GetFreeSize() {
    uint32_t currentAddress = 0;
    uint32_t freeSize = 0;
    MemoryBlockHeader currentBlock;

    // Recorrer todos los bloques en la memoria
    while (currentAddress < MEMORY_TOTAL_SIZE) {
        // Leer el encabezado del bloque actual
        sramread(currentAddress, &currentBlock, BLOCK_HEADER_SIZE);

        // Si el bloque está libre, sumar su tamaño
        if (currentBlock.isFree) {
            freeSize += currentBlock.size;
        }

        // Mover al siguiente bloque
        if (currentBlock.nextAddress == 0) break;
        currentAddress = currentBlock.nextAddress;
    }

    return freeSize;
}




void Memory_Write(uint32_t block, uint8_t* data, size_t len) {
    //uint32_t address = (uint32_t)block;  // Convertir el puntero a dirección
    sramwrite(block, data, len);
}

void Memory_Read(uint32_t block, uint8_t* buffer, size_t len) {
    //uint32_t address = (uint32_t)block;  // Convertir el puntero a dirección
    sramread(block, buffer, len);
}
