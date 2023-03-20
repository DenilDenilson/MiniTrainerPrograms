
#ifndef KEYPAD_4X4_H
#define	KEYPAD_4X4_H

#define _XTAL_FREQ 8000000UL
#include <xc.h>
#include <stdint.h>

//****************************************************************************
//    CONFIGURACIÓN DE LOS PINES KEYPAD
//****************************************************************************

/* Define el puerto a donde se conectaran las filas del keypad */
#define  KEY_FILAS_OUT       LATB        // Registro LATx del puerto
#define  KEY_FILAS_TRIS      TRISB       // Registro TRISx del puerto

/* Define el puerto a donde se conectaran las columnas del keypad */
#define  KEY_COLUMNAS_IN     PORTB      // Registro PORTx del puerto
#define  KEY_COLUMNAS_TRIS   TRISB      // Registro TRISx del puerto

/* Define los numeros de bits a donde se conectará las filas y culumnas del keypad */
//FILAS
#define F0_KEY	0	//define el pin del MCU conectado KEY F0
#define F1_KEY	1	//define el pin del MCU conectado KEY F1
#define F2_KEY	2	//define el pin del MCU conectado KEY F2
#define F3_KEY	3	//define el pin del MCU conectado KEY F3
//COLUMNAS
#define C0_KEY	4	//define el pin del MCU conectado KEY C0
#define C1_KEY	5	//define el pin del MCU conectado KEY C1
#define C2_KEY	6	//define el pin del MCU conectado KEY C2
#define C3_KEY	7	//define el pin del MCU conectado KEY C3

void Key_Init(void);
void Key_Enable_Filas(uint8_t fila);
uint8_t Key_Scan_Columnas(void);
uint8_t Key_Read(void);



#endif	/* KEYPAD_4X4_H */

