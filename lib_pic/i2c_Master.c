
#include "i2c_Master.h"

//Desarrollo de funciones
void I2C_Init(void)
{
    //Configurar pines RB0 y RB1
    TRISBbits.RB0 = 1; //SDA
    TRISBbits.RB1 = 1; //SCL
    ANSELBbits.ANSB0 = 0;
    ANSELBbits.ANSB1 = 0;
     
    //Config I2C
    SSP1STATbits.SMP = 1; //Velocidad: Modo Estandar 100Khz
    SSP1CON1bits.SSPEN = 1; //Habilitacion del puerte serie pines SDA y SCL como I2C
    SSP1CON1bits.SSPM = 0b1000; //Modo maestro I2C, Reloj = FOSC / (4 * (SSPADD + 1))
    
    //Configuracion del baud rate a 100Khz
    SSP1ADD = (uint8_t)( ( _XTAL_FREQ / ( 4UL * F_SCL ) ) - 1UL);
}

void I2C_Start(void)
{
    __delay_ms(1);
    SSP1CON2bits.SEN = 1; //Generacion de la condicion de inicio
    while(SSPCON2bits.SEN == 1); //Esperar que la condicion de START sea transmitida
}

void I2C_Restart(void)
{
    __delay_ms(1);
    SSP1CON2bits.RSEN = 1; //Generacion del Restart
    while(SSP1CON2bits.RSEN == 1); //Esperar que el la condicion de Restart sea transmitida
}

void I2C_Write_Address_Slave(uint8_t address, uint8_t data_dir)
{
    SSP1BUF = ( (address & 0b11111110) | (data_dir & 0b00000001) );
    while(SSP1STATbits.BF == 1); //Espera mientras el SSPBUF esta lleno
    while(SSP1CON2bits.ACKSTAT == 1); //Esperar que se complete la transmision y reciba el ACK
}

void I2C_Write_Data_Slave(uint8_t data)
{
    SSP1BUF = data;
    while(SSP1STATbits.BF == 1); //Espera mientras el SSPBUF esta lleno
    while(SSP1CON2bits.ACKSTAT == 1); //Esperar que se complete la transmision y reciba el ACK
}

void I2C_Stop(void)
{
    __delay_ms(1);
    SSP1CON2bits.PEN = 1; //Generacion de la condicion de STOP
    while(SSP1CON2bits.PEN == 1); //Esperar que se transmita la condicion de STOP
}

void I2C_Read_Data(uint8_t *Ptr_Data, uint8_t var_Ack_nAck)
{
    SSP1CON2bits.RCEN = 1; //Habilitamos la recepcion
    if(var_Ack_nAck == I2C_ACK)
    {
        while(SSP1STATbits.BF == 0); //Esperar mientras SSPBUF esta vacio 
        *Ptr_Data = SSP1BUF; //Leo el dato recibido
    }
    else if(var_Ack_nAck == I2C_nACK)
    {
        while(SSP1STATbits.BF == 0); //Esperar mientras SSPBUF esta vacio 
        *Ptr_Data = SSP1BUF; //Leo el dato recibido
        SSP1CON2bits.ACKDT = 1; //Preparo el bit nACK
        SSP1CON2bits.ACKEN = 1; //Procedemos a enviar el nACK
        while(SSP1CON2bits.ACKEN == 1);//Esperar hasta que el bit nACK sea enviado
    }
}