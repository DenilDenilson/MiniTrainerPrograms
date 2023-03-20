#include "lib_pic/configDevice.h"
#include "lib_pic/adc_10bits.h"
#include "lib_pic/usart.h"

#define ADC_CHANNEL0    0
#define MCP_OFFSET      0.5f
#define FACTOR_ESCALA   0.01f


void PORT_Init (void);


//Variables
float temp;
char  strTemp[20];



int main (void)
{
    OSCILADOR_Init();
    PORT_Init();
    ADC_Init();
    USART_SerialBegin(9600);
    
    while(1)
    {
        temp = (((float)ADC_GetConversion(ADC_CHANNEL0))*(ADC_VREF/ADC_RESOLUCION));
        temp = ((temp - MCP_OFFSET)/FACTOR_ESCALA);
        
        sprintf(strTemp, "T = %.2f C\r\n", temp);
        USART_WriteString(">Temp. Ambiente<\r\n");
        USART_WriteString(strTemp);
        USART_WriteString("- - - - - - - -\r\n");
        
        if ( temp >= 30.0 ) {
            LATEbits.LATE0 = 1;
        }
        else {
            LATEbits.LATE0 = 0;
        }
        
        __delay_ms(500);
    }
}


void PORT_Init (void)
{
    //Config Puerto A
    ANSELAbits.ANSA0 = 1;
    
    
    //Config Puerto E
    ANSELEbits.ANSE0 = 0;
    TRISEbits.RE0 = 0;
}