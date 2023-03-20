
#ifndef ADC_10BITS_H
#define	ADC_10BITS_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

//Macros
#define ADC_AN0 0
#define ADC_AN1 1
#define ADC_AN2 2
#define ADC_AN3 3
#define ADC_AN4 4
#define ADC_AN5 5
#define ADC_AN6 6
#define ADC_AN7 7
#define ADC_AN8 8
#define ADC_AN9 9
#define ADC_AN10 10
#define ADC_AN11 11
#define ADC_AN12 12

//Parametros del ADC
#define ADC_VREF        5.0
#define ADC_RESOLUCION  1023.0

//Funciones
void ADC_Init(void);
void ADC_Enable(void);
void ADC_Disable(void);
void ADC_StartConversion(void);
bool ADC_StateConversion(void);
void ADC_SelectChannel(uint8_t channel);
uint16_t ADC_GetConversion(uint8_t channel);

#endif	/* ADC_10BITS_H */

