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
// Envio comando
void lcd_cmd(char cmd);	

// Envio datos
void lcd_data(char data);

// Inicializacion lcd
void Init_LCD();

// Escribe en pantalla countr caracteres
void puts_lcd (unsigned char *data, unsigned char count);

// Posiciona cursor en inicio de line 1 o line 2
void line_1();
void line_2();
// Posicion en caracter (posicion) i de la linea
void place_in_char(int i);
