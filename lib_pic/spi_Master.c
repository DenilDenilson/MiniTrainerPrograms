
#include "spi_Master.h"

void SPI_Init(void)
{
    /* Definición de pines SPI*/
    TRISBbits.TRISB0 = 1; /* RB0 como entrada (SDI)*/
    TRISBbits.TRISB1 = 0; /* RB1 como salida (SCK) */
    TRISBbits.TRISB3 = 0; /* RC7 como salida (SDO) */
    ANSELBbits.ANSB0 = 0; /* Digital */
    ANSELBbits.ANSB1 = 0; /* Digital */
    ANSELBbits.ANSB3 = 0; /* Digital */
    /**
    WPUx registers
    */
    WPUB = 0x00;
    INTCON2bits.nRBPU = 1;
    
    //SPI setup
    SSP1STAT = 0x40;
    SSP1CON1 = 0x02;
    //SSP1ADD  = 0x27;
    SSP1CON1bits.SSPEN = 1;
}

void SPI_Close(void)
{
    SSP1CON1bits.SSPEN = 0;
}

uint8_t SPI_ExchangeByte(uint8_t data)
{
    SSP1BUF = data;
    while(!PIR1bits.SSP1IF);
    PIR1bits.SSP1IF = 0;
    return SSP1BUF;
}

void SPI1_ExchangeBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        SSP1BUF = *data;
        while(!PIR1bits.SSP1IF);
        PIR1bits.SSP1IF = 0;
        *data++ = SSP1BUF;
    }
}

// Half Duplex SPI Functions
void SPI1_WriteBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        SPI_ExchangeByte(*data++);
    }
}

void SPI1_ReadBlock(void *block, size_t blockSize)
{
    uint8_t *data = block;
    while(blockSize--)
    {
        *data++ = SPI_ExchangeByte(0);
    }
}

void SPI1_WriteByte(uint8_t byte)
{
    SSP1BUF = byte;
}

uint8_t SPI1_ReadByte(void)
{
    return SSP1BUF;
}
