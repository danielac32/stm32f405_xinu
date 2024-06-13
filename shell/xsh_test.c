/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <fat_filelib.h>
#include "tinyscript.h"
#include "tinyscript_lib.h"
#define ARENA_SIZE 4096*3
 
extern int runts(char *);

shellcmd xsh_test(int nargs, char *args[])
{
	
    /*printf("test malloc free\n");
    kprintf("Free : %10d\n", heap_free());
	char *test=malloc(20000);
	printf(" malloc \n");
    kprintf("Free : %10d\n", heap_free());
    free(test);
    printf(" free \n");
    kprintf("Free : %10d\n", heap_free());


    printf("test malloc cc free\n");
    kprintf("Free : %10d\n", heap_freecc());
    test=cc_malloc(20000);
    printf(" malloc \n");
    kprintf("Free : %10d\n", heap_freecc());
    cc_free(test);
    printf(" free \n");
    kprintf("Free : %10d\n", heap_freecc());*/

    char *tmp=full_path((char*)args[1]);
    if (tmp==NULL)return -1;

    return runts(tmp);
}
