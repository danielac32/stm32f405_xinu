
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

 void kernel_printf(const char *format, ...) {
    // Este es un stub; la dirección real será reubicada por el cargador ELF.
}


int main(int argc, char *argv[])
{
	 
  kernel_printf("hola\n");

   

	return 0;
}