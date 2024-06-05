/* xsh_echo.c - xsh_echo */

#include <xinu.h>
 
shellcmd xsh_test(int nargs, char *args[])
{
	
    printf("test malloc free\n");
    kprintf("Free : %10d\n", heap_free());
	char *test=malloc(20000);
	printf(" malloc \n");
    kprintf("Free : %10d\n", heap_free());
    free(test);
    printf(" free \n");
    kprintf("Free : %10d\n", heap_free());
	return 0;
}
