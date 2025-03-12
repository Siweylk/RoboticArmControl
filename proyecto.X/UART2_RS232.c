/* Funciones para trabajar con el modulo UART2
================================================
Comentarios: Valores sacados de la teoria
Autores: Erik Cembreros, Asier Aguilar y Telmo Sendino
Fecha: 21 de marzo 2024
*/

#include "p24HJ256GP610A.h"
#include "LCD.h"
#include "timers_v2.h"

// Definiciones ligadas a la velocidad de transmision de UART2

#define Fosc 50000000 // Frecuencia de reloj de la CPU (oscillator)
#define Fcy Fosc/2 // Velocidad de ejecucion de las instrucciones
#define BAUDRATE2 19200 // Velocidad de transmision de UART2 (baudios) 
#define T_1BIT_US (1000000/BAUDRATE2)+1 // Duracion de 1 bit en us

#define BAUD_RATEREG_2_BRGH1 ((Fcy/BAUDRATE2)/4)-1 // valor U2BRG si BRGH=1
#define BAUD_RATEREG_2_BRGH0 ((Fcy/BAUDRATE2)/16)-1 // valor U2BRG si BRGH=0

// Inicializacion del modulo UART2 (lo usaremos para escribir por pantalla en un terminal)
void inic_UART2 ()
{

// Velocidad de transmision
// Hay que hacer solo una de las dos asignaciones siguientes
U2BRG = BAUD_RATEREG_2_BRGH1;
//U2BRG = BAUD_RATEREG_2_BRGH0;

// U2MODE: habilitar el modulo (UARTEN), 8 bits, paridad par (PDSEL),
// 1 bit de stop (STSEL), BRGH ...
U2MODE = 0x0000;
U2MODEbits.BRGH=1;

// U2STA: modo de interrupcion en el envio (UTXISEL), habilitacion del
// envio (UTXEN), modo de interrupcion en la recepcion (URXISEL)
U2STA = 0x0000;


// inicializacion de los bits IE e IF relacionados (IP si se quiere modificar)
IFS1bits.U2RXIF=0;
IFS1bits.U2TXIF=0;
IEC1bits.U2RXIE=1;
IEC1bits.U2TXIE=1;

//IPC7bits.U2RXIP=xx;
//IPC7bits.U2TXIP=xx;

// interrupciones debidas a errores + bug
IEC4bits.U2EIE=0;
U2STAbits.OERR=0;

// Habilitar el modulo y la linea TX.
// Siempre al final y en ese orden!!!

U2MODEbits.UARTEN=1; // habilitar UART2
U2STAbits.UTXEN=1; // habilitar transmision tras habilitar modulo


Delay_us(T_1BIT_US); // Esperar tiempo de 1 bit
}
