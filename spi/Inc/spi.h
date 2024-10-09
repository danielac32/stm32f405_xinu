
#include <stdint.h>

#ifndef HAL_W25Q_SPI_H_
#define HAL_W25Q_SPI_H_

void hal_w25q_spi_init(void);
void hal_w25q_spi_select(void);
void hal_w25q_spi_release(void);
uint8_t hal_w25q_spi_txrx(uint8_t data);

void hal_sd_spi_init(void);
void hal_sd_spi_select(void);
void hal_sd_spi_release(void);
uint8_t hal_sd_spi_txrx(uint8_t data);
void hal_sd_spi_slow(void);
void hal_sd_spi_fast(void);


void hal_23lc1024_spi_init(void);
void hal_23lc1024_spi_select1_set(int s);
void hal_23lc1024_spi_select2_set(int s);
void hal_23lc1024_spi_select3_set(int s);

void hal_23lc1024_spi_release(void);
uint8_t hal_23lc1024_spi_txrx(uint8_t data);




#endif // HAL_W25Q_SPI_H_