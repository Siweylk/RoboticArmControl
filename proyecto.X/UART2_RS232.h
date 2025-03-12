/* Funciones para trabajar con el modulo UART2
================================================
Comentarios: Valores sacados de la teoria
Autores: Erik Cembreros, Asier Aguilar y Telmo Sendino
Fecha: 19 de febrero
*/

// funciones que se utilizan desde otros modulos
void inic_UART2 ();

#define clrscr "\x1b[2J" //4 character: \x1b, [, 2, J
#define home "\x1b[H" //3 character: \x1b, [, H

#define CR 0x0D // carriage return
#define LF 0x0A // line feed

