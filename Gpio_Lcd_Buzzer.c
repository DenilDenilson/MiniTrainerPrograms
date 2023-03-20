/* Un temporizador con un buzzer como alarma */

#include "lib_pic/configDevice.h"
#include "lib_pic/lcd_2x16.h"

/* P R O T O T I P A D O */
void PORT_Init(void);
void Hardware_Init(void);
void TMR0_Temporizador_Init(void);
void INTERRUPT_Global_Config(void);
void INTERRUPT_INTx_Config(void);
void Print_Hour(void);

/* V A R I A B L E S */
volatile int8_t hours = 0, minutes = 0, seconds = 0;
char strReloj[20];
volatile bool is_pause = true;

uint8_t col;

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

        Print_Hour();
        
        // Lanzamos alarma
        if ( seconds == 0 && minutes == 0 && is_pause == false ) {
            LATEbits.LATE2 = 1;
            __delay_ms(250);
            LATEbits.LATE2 = 0;
            __delay_ms(250);
        }
    }
}


// Rutina de interrupción
void __interrupt() RutinaServicioInterrupt (void) {
    // Consultar por las flags
    // Timer Interrupt
    if ( INTCONbits.TMR0IE == 1 && INTCONbits.TMR0IF == 1 ) {
        // Recarga el timer y borrado de flag
        TMR0 = 3036; // Ya que al desbordarse el timer regresa a 0
        INTCONbits.TMR0IF = 0;
        // Tarea
        // Tarea LCD
        // Tarea RELOJ
//        seconds++;
//        if ( seconds >= 60 ) {
//            seconds = 0;
//            minutes++;
//            if ( minutes >= 60 ) {
//                minutes = 0;
//                hours++;
//                if ( hours >= 24 ) {
//                    hours = 0;
//                }
//            }
//        }      
        // Tarea cronómetro
        seconds--;
        if ( seconds < 0 ) {
            seconds = 59;
            minutes--;
            if ( minutes < 0 ) {
                minutes = 0;
                seconds = 0;
            }
        }
    }
    
    // INT0 Interrupt
    if ( INTCONbits.INT0IE == 1 && INTCONbits.INT0IF == 1 ) {
        // Tarea
        minutes++;
        
        // Reinciamos flag
        INTCONbits.INT0IF = 0;
    }
    
    // INT1 Interrupt
    if ( INTCON3bits.INT1IE == 1 && INTCON3bits.INT1IF == 1 ) {
        // Tarea 
        seconds++;
        
        // Limpiar flag
        INTCON3bits.INT1IF = 0;
    }
    
    // INT2 Interrupt
    if ( INTCON3bits.INT2IE == 1 && INTCON3bits.INT2IF == 1 ) {
        // Tarea 
        if ( is_pause ) {
            // Solventa un error cuando ponen solo minutos o nada e inician
            if ( seconds == 0  && minutes == 0 ) {
                minutes = 0;
                seconds = 0;
            }
            else if ( seconds == 0 ) {
                seconds = 59;
                minutes--;
            }
            else {
                seconds--;
            }
            //is_pause = !is_pause;
        }
        is_pause = !is_pause;
        
        T0CONbits.TMR0ON = ~T0CONbits.TMR0ON;
        
        // Limpiar flag
        INTCON3bits.INT2IF = 0;
    }
}

/***********************************************************/

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
    
    /* BUZZER PORT */
    ANSELEbits.ANSE2 = 0;
    TRISEbits.RE2 = 0;
    
    
}

// Configura e inicializa todo el hardware a utilizar en el proyecto
void Hardware_Init(void) {
    OSCILADOR_Init();
    PORT_Init();
    Lcd_Init();
    TMR0_Temporizador_Init();
    INTERRUPT_Global_Config();
    INTERRUPT_INTx_Config();
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
    T0CONbits.TMR0ON = 0; // Start in off
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

/****************************************************************/

// Imprime en pantalla fila 1
void Print_Hour(void) {
    Lcd_Set_Cursor(1, 1);
//    sprintf(strReloj, "Timer:  %02u%02u:%02u", hours, minutes, seconds); 
    sprintf(strReloj, "Timer:     %02u:%02u", minutes, seconds); 
    // Inicializamos el formato de relos
    Lcd_Write_String(strReloj);
}

