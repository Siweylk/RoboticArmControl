/**********************************************************************
LCD erabiltzeko funtzioak:
	Hasieraketa
	Komando bat bidali
	Datu bat bistaratu
	Testu bat bistaratu
	Funtzio bereziak: 1. lerroan kokatu // 2. lerroan kokatu
 * 
 * Autores: Asier Aguilar, Erik Cembreros y Telmo Sendino
Fecha: 16 de marzo 2024
**********************************************************************/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "timers_v2.h"
#include "LCD.h"


/*****	LCD SUBROUTINES  *****/ 
void lcd_cmd (char cmd)        // subroutine for lcd commands
{
  RW = 0;             // RW=0, para escribir
  RS = 0;	      // RS=0, se va a escribir un comando
  DATA &= 0xFF00;     // pines RD0-RD7 (dato que se envía): poner a 0
  DATA |= cmd;        // copiar a esos pines el parametro cmd
  E = 1;              // E=1. Debe permanecer activado al menos 230 ns. 
  Delay_us(1);
  E = 0;		// desactivar E
  RW = 1;		// desactivar escritura
}


void lcd_data (char data)      // subroutine for lcd data
{
  RW = 0;       	// RW=0, para escribir
  RS = 1;            	// RS=1, se va a escribir un caracter
  DATA &= 0xFF00;    	// pines RD0-RD7 (dato que se envía): poner a 0
  DATA |= data;      	// copiar a esos pines el parametro data
  E = 1;             	// E=1. Debe permanecer activado al menos 230 ns.
  Delay_us(1);
  E = 0;             	// desactivar E
  RS = 0;            	// desactivar RS
  RW = 1;		// desactivar escritura
}


void Init_LCD ()             // initialize LCD display
{
  // 15mS delay after Vdd reaches nnVdc before proceeding 
  // with LCD initialization
  // not always required and is based on system Vdd rise rate
  Delay_ms (15); // 15 ms delay
			
  /* set initial states for the data and control pins */
  DATA &= 0xFF00;	
  RW = 0;                       // R/W state set low
  RS = 0;                       // RS state set low
  E = 0;                        // E state set low

  /* set data and control pins to outputs */
  TRISDATA &= 0xFF00;
  RW_TRIS = 0;                  // RW pin set as output
  RS_TRIS = 0;                  // RS pin set as output
  E_TRIS = 0;                   // E pin set as output

  /* LCD initialization sequence */ 
  lcd_cmd (0x38);				// function set (3 veces)
  Delay_ms (5);         // 5 ms delay
  lcd_cmd (0x38);
  Delay_us (100);       // 100 us delay
  lcd_cmd (0x38);
  Delay_us (40);      // 40 delay (function set) (mirar tabla en los apuntes)
  lcd_cmd (0x38);  
  Delay_us (40);      // 40 delay (function set) 
  lcd_cmd (0x0C);       // Display on/off control, cursor blink off (0x0C)
  Delay_us (40);      // 40 delay (display on off )
  lcd_cmd (0x06);	// Entry mode set (0x06)
  Delay_us (40);      // 40 delay (entry mode set ) 
}

// Escribe en lcd
void puts_lcd (unsigned char *data, unsigned char count){
    while (count){
        lcd_data(*data++);
        Delay_us (40);	// 110 ns (0.11 us)
        count--;
    }	
}

// Posiciona cursor al comienzo de la linea 1 del lcd
void line_1()
{
    lcd_cmd(0x80);  	// Set DDRAM address (@0)
    Delay_us (40); 	// ???? delay (mirar tabla en los apuntes)
}
// Posiciona cursor al comienzo de la linea 2 del lcd
void line_2()
{
    lcd_cmd(0xC0);  	// Set DDRAM address (@40)
    Delay_us (40); 	// ???? delay (mirar tabla en los apuntes)
}

// Posiciona cursor en la posicion i
void place_in_char(int i)
{
    int dir = 192 + i;
    lcd_cmd( (char) dir);  	// Set DDRAM address (@40)
    Delay_us (40); 	// ???? delay (mirar tabla en los apuntes)
}

