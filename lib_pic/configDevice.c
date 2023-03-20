
#include "configDevice.h"

//Desarrollo de funciones
void OSCILADOR_Init(void)
{
    //Configuracion del OSC Interno
    //OSCCONbits.IRCF = 0b111; //Configuracion del OSC interno a 16 Mhz
    OSCCONbits.IRCF = 0b110; //Configuracion del OSC interno a 8 Mhz
    OSCCONbits.SCS  = 0b10;  //Habilar el OScilador interno para el sistema
}