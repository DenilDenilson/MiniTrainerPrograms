
#ifndef USART_H
#define	USART_H

#define _XTAL_FREQ 8000000UL
#include <xc.h>
#include <stdint.h>

//Funciones
void USART_SerialBegin(uint32_t baudios);
void USART_TxEnable(void);
void USART_TxDisable(void);
void USART_RxEnable(void);
void USART_RxDisable(void);
void USART_WriteByte(char data);
void USART_WriteString(char *pString);
char USART_ReadByte(void);
void putch(char c);
void USART_RxInterruptEnable(void);
void USART_RxInterruptDisable(void);

#endif	/* USART_H */

