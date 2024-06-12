/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <fat_filelib.h>
#include "tinyscript.h"
#include "tinyscript_lib.h"
#define ARENA_SIZE 4096
FILE* fd;
char memarena[ARENA_SIZE];

void outchar(int c) {
    putc(CONSOLE,c);
}


static Val getcnt_fn()
{
    return 0;
}

static Val pinout_fn(Val pin, Val onoff)
{
    
    return 0;
}
static Val pinin_fn(Val pin)
{
     
    return 0;
}

static Val testfunc(Val x, Val y)
{
    return x*x + y*y;
}

void _ts_printf(ts_list *format, ...) {
  va_list args;
  va_start(args, format);
  char *format_string = ts_list_to_string(format);
  //vprintf(format_string, args);
  ts_free(format_string);
}

void ts_printf(ts_list *format, Val a) {
  _ts_printf(format, a);
}

void ts_printf_(ts_list *format, Val a, Val b) {
  _ts_printf(format, a, b);
}

void ts_printf__(ts_list *format, Val a, Val b, Val c) {
  _ts_printf(format, a, b, c);
}


struct def {
    const char *name;
    intptr_t val;
    int nargs;
} funcdefs[] = {
    { "getcnt",    (intptr_t)getcnt_fn, 0 },
    { "pinout",    (intptr_t)pinout_fn,  2 },
    { "pinin",     (intptr_t)pinin_fn, 1 },
    { "dsqr",      (intptr_t)testfunc, 2 },
    {"printf",    (intptr_t)ts_printf, 2},
    {"printf_",   (intptr_t)ts_printf_, 3},
    {"printf__",  (intptr_t)ts_printf__, 4},
    { NULL, 0 }
};



void
runscript(const char *filename)
{
    FILE *f = fopen(filename, "r");
    int r;
    if (!f) {
        printf("not found %s\n",filename);
        return;
    }
    // Get file size
    fseek(fd, 0, SEEK_END);
    unsigned int fileLength = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    // Read entire file
    char *elfData = malloc(fileLength);
    if (fread((uint8 *)elfData, fileLength,1,fd) != fileLength) {
        fclose(fd);
        free(elfData);
        return;
    }

    r = TinyScript_Run(elfData, 0, 1);
    //err = TinyScript_Run(fibo_ts, 0, 0);
    if (r != 0) {
        printf("script error %d\n", r);
    }
    free(elfData);
}


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
    int err;
    int i;
    
    err = TinyScript_Init(memarena, sizeof(memarena));
    for (i = 0; funcdefs[i].name; i++) {
        err |= TinyScript_Define(funcdefs[i].name, CFUNC(funcdefs[i].nargs), funcdefs[i].val);
    }
    err |= ts_define_funcs();
    
    if (err != 0) {
        printf("Initialization of interpreter failed!\n");
        return 1;
    }
    char *tmp=full_path((char*)args[1]);
    if (tmp==NULL)return -1;
    runscript(tmp);
    
    return 0;
}
