/* Un temporizador con un buzzer como alarma */

#include "lib_pic/configDevice.h"
#include "lib_pic/lcd_2x16.h"

/* P R O T O T I P A D O */
void PORT_Init(void);
void Hardware_Init(void);
void TMR0_Temporizador_Init(void);
void INTERRUPT_Global_Config(void);
void INTERRUPT_INTx_Config(void);

/* V A R I A B L E S */
uint8_t hours = 0, minutes = 0, seconds = 0, col;
char strReloj[20];

int main( void ) {
    
    Hardware_Init();
    
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    sprintf(strReloj, "Timer:  %02u:%02u:%02u", hours, minutes, seconds); 
    // Inicializamos el formato de relos
    Lcd_Write_String(strReloj);
    
    
    while ( 1 ) {
//        for (col = 1; col <=17; col++) {
//            Lcd_Set_Cursor(2, col);
//            Lcd_Write_String("*");
//            __delay_ms(250);
//            Lcd_Set_Cursor(2, col);
//            Lcd_Write_String(" ");
//            __delay_ms(50);
//        }        
//        /* G E N E R A    Q U E   S E     R O M P A   L A     P A N T A L L A */
    }
}

// Configura los pines a usar en el proyecto
void PORT_Init(void) {
    /* LCD PORTS */
    ANSELD = 0x00;
    
    /* BUTTONS PORTS */
    ANSELBbits.ANSB0 = 0;
    TRISBbits.RB0 = 1;
    ANSELBbits.ANSB1 = 0;
    TRISBbits.RB1 = 1;
    ANSELBbits.ANSB2 = 0;
    TRISBbits.RB2 = 1;
    
    
}

// Rutina de interrupción
void __interrupt() RutinaServicioInterrupt (void) {
    // Consultar por las flags
    if ( INTCONbits.TMR0IE == 1 && INTCONbits.TMR0IF == 1 ) {
        // Recarga el timer y borrado de flag
        TMR0 = 3036; // Ya que al desbordarse el timer regresa a 0
        INTCONbits.TMR0IF = 0;
        // Tarea
        // Tarea LCD
        seconds++;
        if ( seconds >= 60 ) {
            seconds = 0;
            minutes++;
            if ( minutes >= 60 ) {
                minutes = 0;
                hours++;
                if ( hours >= 24 ) {
                    hours = 0;
                }
            }
        }
        Lcd_Set_Cursor(1, 1);
        sprintf(strReloj, "Timer:  %02u:%02u:%02u", hours, minutes, seconds); 
        // Inicializamos el formato de relos
        Lcd_Write_String(strReloj);
        
        // Funcionalidad del reloj digital
        
    }
}

/***********************************************************/

// Configura e inicializa todo el hardware a utilizar en el proyecto
void Hardware_Init(void) {
    OSCILADOR_Init();
    PORT_Init();
    Lcd_Init();
    TMR0_Temporizador_Init();
    INTERRUPT_Global_Config();
}

// Configuramos las interrupciones globales
void INTERRUPT_Global_Config(void) {
    RCONbits.IPEN = 0; // Dependencia de prioridades
    
    INTCONbits.GIE = 1; // Habilita interrupciones
    INTCONbits.PEIE = 1; // Habilita interrupciones por periféricos
}

// Inicializamos el uso del timer0
void TMR0_Temporizador_Init(void) {
    T0CONbits.T08BIT = 0; // Resol del contador a 16 bits
    T0CONbits.T0CS = 0; // Modo temporizador F_osc/4
    T0CONbits.PSA = 0; // Preescaler activado
    T0CONbits.T0PS = 0b101; // Preescaler x64
    
    // Carga del timer
    TMR0 = 3036;
    
    // Config Interrupción por desbordamiento
    INTCONbits.TMR0IE = 1; // Habilitamos la interrupción
    INTCONbits.TMR0IF = 0; // Borramos el flag al iniciar
    
    // Habilitamos el timer
    T0CONbits.TMR0ON = 1; // Enable timer
}


// Configuramos las interrupciones INTx
void INTERRUPT_INTx_Config(void) {
    INTCONbits.INT0IE = 1; // Enable INTO interrupt
    INTCONbits.INT0IF = 0; // Start flag in 0
    
    INTCON2bits.INTEDG0 = 0; // INT0 por flanco de bajada
}