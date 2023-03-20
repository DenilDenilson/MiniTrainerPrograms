#include "lib_pic/configDevice.h"
#include "lib_pic/adc_10bits.h"
#include "lib_pic/lcd_2x16.h"

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
    
    Lcd_Init();
    Lcd_Clear();
    
    while(1)
    {
        temp = (((float)ADC_GetConversion(ADC_CHANNEL0))*(ADC_VREF/ADC_RESOLUCION));
        temp = ((temp - MCP_OFFSET)/FACTOR_ESCALA);
        
        sprintf(strTemp, "T = %.2f C", temp);
        Lcd_Set_Cursor(1, 1);
        Lcd_Write_String(">Temp. Ambiente<");
        Lcd_Set_Cursor(2, 3);
        Lcd_Write_String(strTemp);
        
        if ( temp >= 30.0 ) {
            LATEbits.LATE0 = 1;
        }
        else {
            LATEbits.LATE0 = 0;
        }
        
        __delay_ms(250);
    }
}


void PORT_Init (void)
{
    //Config Puerto A
    ANSELAbits.ANSA0 = 1;
    
    //Config Puerto D
    ANSELD = 0x00;
    
    //Config Puerto E
    ANSELEbits.ANSE0 = 0;
    TRISEbits.RE0 = 0;
}