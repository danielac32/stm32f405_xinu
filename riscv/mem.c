

#include <xinu.h>
#include <mem.h>
#include <fat_filelib.h>
#include <23lc.h>

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
    

    printf("clear ram....\n");
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

 
 