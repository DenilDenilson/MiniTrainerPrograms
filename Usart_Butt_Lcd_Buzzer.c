#include "lib_pic/configDevice.h"
#include "lib_pic/usart.h"
#include "lib_pic/lcd_2x16.h"

void PORT_Init(void);
void INTERRUPT_Global_Config(void);
void INTERRUPT_INTx_Config(void);

// Variables
char datoRx;
volatile uint8_t counter = 0; // volatile permite que main y el interrupt puedan compartir el dato
char strCounter[20];

int main (void) {
    OSCILADOR_Init();
    PORT_Init();
    USART_SerialBegin(9600); // 9600 baudios
    INTERRUPT_INTx_Config();
    INTERRUPT_Global_Config();
    
    Lcd_Init();
    Lcd_Clear();
    
    while(1) {
        Lcd_Set_Cursor(1, 1);
        sprintf(strCounter, "Counter:     %03u", counter);
        Lcd_Write_String(strCounter);
        sprintf(strCounter, "Counter: %03u\r\n", counter);
        USART_WriteString(strCounter);
//        USART_WriteString("LED 1 ON \r\n");
        //datoRx = USART_ReadByte(); // leer el caracter recibido
        
        if (counter % 10 == 0 && counter != 0) {
            LATEbits.LATE2 = 1;
        }
        else {
            LATEbits.LATE2 = 0;
        }
        __delay_ms(250);
        
    }
}

// Implementaci?n de rutinas de interrupci?n
void __interrupt() RutinaServicioInterrupcion_ISR(void) {
    // Consultar por las flags
    
    // INT0 Interrupt
    if ( INTCONbits.INT0IE == 1 && INTCONbits.INT0IF == 1 ) {
        // Tarea
        counter = 0;
        // Reinciamos flag
        INTCONbits.INT0IF = 0;
    }
    
    // INT1 Interrupt
    if ( INTCON3bits.INT1IE == 1 && INTCON3bits.INT1IF == 1 ) {
        // Tarea 
        counter--;
        // Limpiar flag
        INTCON3bits.INT1IF = 0;
    }
    
    // INT2 Interrupt
    if ( INTCON3bits.INT2IE == 1 && INTCON3bits.INT2IF == 1 ) {
        // Tarea 
        counter++;
        // Limpiar flag
        INTCON3bits.INT2IF = 0;
    }
}

void PORT_Init(void) {    
    // Config Relé
    ANSELEbits.ANSE0 = 0;
    TRISEbits.RE0 = 0;
    
    // Config Lcd
    ANSELD = 0X00;
    
    // Config Button
    ANSELBbits.ANSB0 = 0; // Digital
    TRISBbits.RB0 = 1; // Entrada digital
    
    // Config Buzzer
    ANSELEbits.ANSE2 = 0;
    TRISEbits.RE2 = 0;

}



void INTERRUPT_Global_Config(void) {
    // Modo de trabajo de interrupciones: Modo convencional
    RCONbits.IPEN = 0; // No establece niveles de prioridad en las interrupciones

    // Habilitar las interrupciones globales
    INTCONbits.GIE = 1;
}


void INTERRUPT_INTx_Config(void){
    /* INT 0 */
    INTCONbits.INT0IE = 1; // Enable INTO interrupt
    INTCONbits.INT0IF = 0; // Start flag in 0
    INTCON2bits.INTEDG0 = 0; // INT0 por flanco de bajada

    /* INT 1 */
    INTCON3bits.INT1IE = 1;
    INTCON3bits.INT1F = 0;
    INTCON2bits.INTEDG1 = 0;
    
    /* INT2 */
    INTCON3bits.INT2IE = 1;
    INTCON3bits.INT2IF = 0;
    INTCON2bits.INTEDG2 = 0;
}


