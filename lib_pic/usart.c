
#include "usart.h"


void USART_SerialBegin(uint32_t baudios)
{
    uint16_t n = 0;
    
    //Configurar Pines Tx y Rx
    TRISCbits.TRISC6 = 0; //Salida Tx
    ANSELCbits.ANSC6 = 0; //Digital
    TRISCbits.TRISC7 = 1; //Entrada Rx
    ANSELCbits.ANSC7 = 0; //Digital
    
    //Configurar los registros de USART
    TXSTA1   = 0b00100100; 
    RCSTA1   = 0b10010000; 
    BAUDCON1 = 0b00001000; 
    
    //Configuracion de baudios
    n = (uint16_t)((_XTAL_FREQ / baudios) / 4UL) - 1UL;
    SPBRG1  = (uint8_t)n;
    SPBRGH1 = (uint8_t)(n >> 8);
}

void USART_TxEnable(void)
{
    TXSTA1bits.TXEN = 1;
}

void USART_TxDisable(void)
{
    TXSTA1bits.TXEN = 0;
}

void USART_RxEnable(void)
{
    RCSTA1bits.CREN = 1;
}

void USART_RxDisable(void)
{
    RCSTA1bits.CREN = 0;
}

void USART_RxInterruptEnable(void)
{
    PIE1bits.RCIE = 1;
}

void USART_RxInterruptDisable(void)
{
    PIE1bits.RCIE = 0;
}

void USART_WriteByte(char data)
{
    while(TXSTA1bits.TRMT == 0); //Esperar que se transmita el byte datos
    TXREG1 = data;
}

void USART_WriteString(char *pString)
{
    while(*pString != '\0')
    {
        USART_WriteByte(*pString);
        pString++;
    }
}

char USART_ReadByte(void)
{
    char dataRx;
    
    while(PIR1bits.RCIF == 0); //Esperar que se reciba un byte
    
    if(RCSTA1bits.FERR == 1 || RCSTA1bits.OERR == 1)
    {
        //Borrar deshabilitando la recepcion
        USART_RxDisable();
        NOP(); //Hacer un tiempo de un Ciclo maquina
        USART_RxEnable();
    }
    else
    {
        dataRx = RCREG1; //Leer dato recibido
    }
    
    return dataRx;
}

//Implementar funcion necesaria para usar printf();
void putch(char c)
{
    while(TXSTA1bits.TRMT1 == 0); 
    TXREG1 = c;
}
