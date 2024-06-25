#include <xinu.h>
#include <fat_filelib.h>
#include "tinyscript.h"
#include "tinyscript_lib.h"
#define ARENA_SIZE 4096*3



//char memarena[ARENA_SIZE];

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
static Val tsdelay(Val t)
{
     sleepms(t);
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
    { "delay",     (intptr_t)tsdelay, 1 },
    { "dsqr",      (intptr_t)testfunc, 2 },
    {"printf",    (intptr_t)ts_printf, 2},
    {"printf_",   (intptr_t)ts_printf_, 3},
    {"printf__",  (intptr_t)ts_printf__, 4},
    { NULL, 0 }
};



void
runscript(const char *filename)
{
    FILE *fd = fopen(filename, "r");
    int r;
    if (!fd) {
        printf("not found %s\n",filename);
        return;
    }
    // Get file size
    fseek(fd, 0, SEEK_END);
    unsigned int fileLength = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    // Read entire file
    char *data = cc_malloc(fileLength);
    fread(data, fileLength,1,fd);

    data[fileLength]='\0';
    r = TinyScript_Run(data, 0, 1);
    //err = TinyScript_Run(fibo_ts, 0, 0);
    if (r != 0) {
        printf("script error %d\n", r);
    }
    cc_free(data);
}


int runts(char *file){
	int err;
    int i;
    char *memarena = cc_malloc(ARENA_SIZE);
    err = TinyScript_Init(memarena, ARENA_SIZE);
    for (i = 0; funcdefs[i].name; i++) {
        err |= TinyScript_Define(funcdefs[i].name, CFUNC(funcdefs[i].nargs), funcdefs[i].val);
    }
    err |= ts_define_funcs();
    
    if (err != 0) {
        printf("Initialization of interpreter failed!\n");
        return 1;
    }
    
    runscript(file);
    cc_free(memarena);
    return 0;
}