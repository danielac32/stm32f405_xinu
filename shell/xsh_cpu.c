/* xsh_echo.c - xsh_echo */

#include <xinu.h>
#include <stdio.h>

 extern uint32_t idleCycles;
extern uint32_t totalCycles;


#define NUM_MEASUREMENTS 10  // Número de mediciones para hacer un promedio
#define MEASUREMENT_INTERVAL 1000  // Intervalo de espera entre mediciones en milisegundos

void calculateCpuUsage(void) {
    uint32_t cpuUsage = 0;
    uint32_t totalCpuUsage = 0;  // Para acumular el uso total de CPU
    uint32_t totalIdleCycles = 0;  // Para acumular los ciclos de inactividad
    uint32_t totalCyclesLocal = 0;  // Para acumular el total de ciclos
    uint32_t totalTicks = 0;  // Contador global de ticks para medición de tiempo

    // Realizamos varias mediciones para hacer un promedio
    for (int i = 0; i < NUM_MEASUREMENTS; i++) {
        printf("Measurement %d: total: %d, idle: %d\n", i + 1, totalCycles, idleCycles);
        
        totalCyclesLocal = totalCycles;  // Acumulamos los ciclos totales
        totalCpuUsage += (totalCyclesLocal - idleCycles);  // Acumulamos el uso de CPU
        totalIdleCycles += idleCycles;  // Acumulamos los ciclos inactivos

        // Reseteamos los contadores después de cada medición para la siguiente iteración
        totalCycles = 0;
        idleCycles = 0;

        // Esperar un poco antes de la siguiente medición (ajusta según lo que necesites)
        sleepms(1);  // Espera 1 segundo entre mediciones
        totalTicks++;  // Incrementa el contador de ticks
    }

    // Ahora calculamos el promedio de uso de CPU
    if ((totalCpuUsage + totalIdleCycles) > 0) {
        cpuUsage = ((totalCpuUsage) * 100) / (totalCpuUsage + totalIdleCycles);  // Calculamos el porcentaje de uso de CPU
    }

    // Imprimimos el resultado promedio
    printf("Average CPU Usage: %d%%\n", cpuUsage % 100);
}



shellcmd xsh_cpu(int nargs, char *args[])
{
	 
 calculateCpuUsage();

	return 0;
}
