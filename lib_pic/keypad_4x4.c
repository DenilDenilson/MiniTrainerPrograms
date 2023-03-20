
#include "keypad_4x4.h"

uint8_t Keypad4x4[4][4] = { {'1','2','3','A'}, 
							{'4','5','6','B'}, 
							{'7','8','9','C'}, 
							{'*','0','#','D'} };

void Key_Init(void)
{
	/* Configurar los puertos que utilizará el keyboard para filas y columnas */
	KEY_FILAS_TRIS    &= ~((1<<F3_KEY)|(1<<F2_KEY)|(1<<F1_KEY)|(1<<F0_KEY));  //FILAS salida
	KEY_COLUMNAS_TRIS |=   (1<<C3_KEY)|(1<<C2_KEY)|(1<<C1_KEY)|(1<<C0_KEY); //COLUMNAS entrada
	//FILAS Reset
	KEY_FILAS_OUT |= (1<<F3_KEY)|(1<<F2_KEY)|(1<<F1_KEY)|(1<<F0_KEY);
}

void Key_Enable_Filas(uint8_t fila)
{
	//FILAS Reset
	KEY_FILAS_OUT |= (1<<F3_KEY)|(1<<F2_KEY)|(1<<F1_KEY)|(1<<F0_KEY);
	
	//Habilitar fila
	switch(fila)
	{
		case 0: KEY_FILAS_OUT &= ~(1<<F0_KEY); break;
		case 1: KEY_FILAS_OUT &= ~(1<<F1_KEY); break;
		case 2: KEY_FILAS_OUT &= ~(1<<F2_KEY); break;
		case 3: KEY_FILAS_OUT &= ~(1<<F3_KEY); break;
	}
}

uint8_t Key_Scan_Columnas(void)
{
	//Scan C0
	if( (KEY_COLUMNAS_IN & (1<<C0_KEY)) == 0 )
	{
		__delay_ms(10);
		while( (KEY_COLUMNAS_IN & (1<<C0_KEY)) == 0 );
		//Retornar columna 0
		return 0;
	}
	
	//Scan C1
	if( (KEY_COLUMNAS_IN & (1<<C1_KEY)) == 0 )
	{
		__delay_ms(10);
		while( (KEY_COLUMNAS_IN & (1<<C1_KEY)) == 0 );
		//Retornar columna 1
		return 1;
	}
	
	//Scan C2
	if( (KEY_COLUMNAS_IN & (1<<C2_KEY)) == 0 )
	{
		__delay_ms(10);
		while( (KEY_COLUMNAS_IN & (1<<C2_KEY)) == 0 );
		//Retornar columna 2
		return 2;
	}
	
	//Scan C3
	if( (KEY_COLUMNAS_IN & (1<<C3_KEY)) == 0 )
	{
		__delay_ms(10);
		while( (KEY_COLUMNAS_IN & (1<<C3_KEY)) == 0 );
		//Retornar columna 3
		return 3;
	}
	
	//Retornar 4 si no se dectecta ninguna tecla
	return 4;
}

uint8_t Key_Read(void)
{
	uint8_t key = '\0', value_column;
	
	//Habilitamos la fila 0
	Key_Enable_Filas(0);
	value_column = Key_Scan_Columnas();
	if(value_column < 4)
	{
		switch(value_column)
		{
			case 0: key = Keypad4x4[0][0]; break;
			case 1: key = Keypad4x4[0][1]; break;
			case 2: key = Keypad4x4[0][2]; break;
			case 3: key = Keypad4x4[0][3]; break;
		}
	}
	
	//Habilitamos la fila 1
	Key_Enable_Filas(1);
	value_column = Key_Scan_Columnas();
	if(value_column < 4)
	{
		switch(value_column)
		{
			case 0: key = Keypad4x4[1][0]; break;
			case 1: key = Keypad4x4[1][1]; break;
			case 2: key = Keypad4x4[1][2]; break;
			case 3: key = Keypad4x4[1][3]; break;
		}
	}
	
	//Habilitamos la fila 2
	Key_Enable_Filas(2);
	value_column = Key_Scan_Columnas();
	if(value_column < 4)
	{
		switch(value_column)
		{
			case 0: key = Keypad4x4[2][0]; break;
			case 1: key = Keypad4x4[2][1]; break;
			case 2: key = Keypad4x4[2][2]; break;
			case 3: key = Keypad4x4[2][3]; break;
		}
	}
	
	//Habilitamos la fila 3
	Key_Enable_Filas(3);
	value_column = Key_Scan_Columnas();
	if(value_column < 4)
	{
		switch(value_column)
		{
			case 0: key = Keypad4x4[3][0]; break;
			case 1: key = Keypad4x4[3][1]; break;
			case 2: key = Keypad4x4[3][2]; break;
			case 3: key = Keypad4x4[3][3]; break;
		}
	}
	
	return key; //Retorno la tecla presionada
}
