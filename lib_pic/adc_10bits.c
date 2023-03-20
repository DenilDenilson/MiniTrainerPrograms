
#include "adc_10bits.h"

//Desarrollo de la funciones
void ADC_Init(void)
{
    //Configuracion del VREF+ y VREF-
    ADCON1bits.PVCFG = 0b00; // VREF+ = VDD
    ADCON1bits.NVCFG = 0b00; // VREF- = GND
    //Formato del resultado justificado a la derecha
    ADCON2bits.ADFM  = 1;
    /*
     * PARAMETROS DE CONVERSION
     * 
     * TACQ (Tiempo de Adquisicion) >= 7.45us
     * TAD  (Tiempo que tarda el ADC en convertir cada bit del resultado)
     *  1us >= TAD <= 25us
     * 
     * Clock ADC con Fosc/2
     * Fclck_ADC = 16Mz/2
     * Fclck_ADC = 8Mhz
     * Tclck_ADC(TAD) = 1/8Mhz
     * Tclck_ADC(TAD) = 0.125 us
     * -----------------------------
     * 
     * Clock ADC con Fosc/32
     * Fclck_ADC = 16Mz/32
     * Fclck_ADC = 500 Khz
     * Tclck_ADC(TAD) = 1/500Khz
     * Tclck_ADC(TAD) = 2us
     * 
     * TACQ = 2TAD -> 2TAD * 2us = 4us
     * TACQ = 4TAD -> 4TAD * 2us = 8us
     * 
     * TACQ = 8us
     */
    ADCON2bits.ADCS = 0b010; //Prescaler x32 ->Tclck_ADC(TAD) = 2us
    ADCON2bits.ACQT = 0b010; //TACQ = 8us
}

void ADC_Enable(void)
{
    ADCON0bits.ADON = 1;
}

void ADC_Disable(void)
{
    ADCON0bits.ADON = 0;
}

void ADC_StartConversion(void)
{
    ADCON0bits.GO_nDONE = 1;
}

bool ADC_StateConversion(void)
{
    return (bool)ADCON0bits.GO_nDONE;
}

void ADC_SelectChannel(uint8_t channel)
{
    ADCON0bits.CHS = channel;
}

uint16_t ADC_GetConversion(uint8_t channel) //Super funciones
{
    ADC_SelectChannel(channel); //Seleccion del canal analogico
    ADC_Enable(); //Habilitar el modulo ADC
    ADC_StartConversion(); //Iniciar la conversion
    while(ADC_StateConversion() == true); //Espera hasta que la conversion se complete
    ADC_Disable(); //Desactivo el ADC
    return (uint16_t)((ADRESH << 8) + ADRESL); //Resultado de la conversion
}
