//#ifndef _EMULATOR_H
//#define _EMULATOR_H

#include <stdint.h>

enum emulatorCode{
    EMU_POWEROFF,
    EMU_REBOOT,
    EMU_UNKNOWN
};

int riscv_emu();
int load_ram_file( uint32_t part,  char *filename );

//#endif