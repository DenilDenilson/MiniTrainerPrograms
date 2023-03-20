
#ifndef SPI_MASTER_H
#define	SPI_MASTER_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

void SPI_Init(void);
void SPI_Close(void);
uint8_t SPI_ExchangeByte(uint8_t data);
void SPI_ExchangeBlock(void *block, size_t blockSize);
void SPI_WriteBlock(void *block, size_t blockSize);
void SPI_ReadBlock(void *block, size_t blockSize);
void SPI_WriteByte(uint8_t byte);
uint8_t SPI_ReadByte(void);


#endif	/* SPI_MASTER_H */

