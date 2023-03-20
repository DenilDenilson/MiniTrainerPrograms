#include "lib_pic/configDevice.h"

/* P R O T O T I P A D O    D E     F U N C I O N E S */
void PORT_Init(void);
void INTERRUPT_Global_Config(void);
void INTERRUPT_INTx_Config(void);

/* V A R I A B L E S */
volatile bool is_bt0_pushed = false, is_bt1_pushed = false, is_bt2_pushed = false;
        
int main ( void ) {
    
    OSCILADOR_Init();
    PORT_Init();
    INTERRUPT_Global_Config();
    INTERRUPT_INTx_Config();
    
    while ( 1 ) {
        
        if ( is_bt1_pushed ) {
            for (uint8_t i = 0; i <= 8; i++) {
            LATD = 0b10000000 >> i;
            __delay_ms(100);
            }
        }
        else if ( is_bt2_pushed ) {
            for (uint8_t i = 0; i <= 8; i++) {
                LATD = 1 << i;
                __delay_ms(100);
            }
        }
        else {
//            LATD = 0xff;
//            __delay_ms(1000);
//            LATD = 0x00;
//            __delay_ms(1000);
            LATD = 0x00;
        }
        
        
    }
}

/*************************************************************/
// Rutina de interrupción
void __interrupt() RutinaServicioInterrupt (void) {
    // Consultar por las flags
    
    // INT0 Interrupt
    if ( INTCONbits.INT0IE == 1 && INTCONbits.INT0IF == 1 ) {
        // Tarea
        is_bt0_pushed = true;
        is_bt1_pushed = false;
        is_bt2_pushed = false;
        // Reinciamos flag
        INTCONbits.INT0IF = 0;
    }
    
    // INT1 Interrupt
    if ( INTCON3bits.INT1IE == 1 && INTCON3bits.INT1IF == 1 ) {
        // Tarea 
        is_bt0_pushed = false;
        is_bt1_pushed = true;
        is_bt2_pushed = false;
        // Limpiar flag
        INTCON3bits.INT1IF = 0;
    }
    
    // INT2 Interrupt
    if ( INTCON3bits.INT2IE == 1 && INTCON3bits.INT2IF == 1 ) {
        // Tarea 
        is_bt0_pushed = false;
        is_bt1_pushed = false;
        is_bt2_pushed = true;
        // Limpiar flag
        INTCON3bits.INT2IF = 0;
    }
}

/***********************************************************/




/* F U N C I O N E S */
void PORT_Init(void) {
    /* LEDS PORT */
    ANSELD = 0x00;
    TRISD = 0x00;
}

// Configuramos las interrupciones globales
void INTERRUPT_Global_Config(void) {
    RCONbits.IPEN = 0; // Dependencia de prioridades
    
    INTCONbits.GIE = 1; // Habilita interrupciones
    INTCONbits.PEIE = 1; // Habilita interrupciones por periféricos
}

// Configuramos las interrupciones INTx
void INTERRUPT_INTx_Config(void) {
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



