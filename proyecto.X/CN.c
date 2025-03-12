/*
 Funciones relacionadas con el modulo CN
 
Autores: Asier Aguilar, Erik Cembreros y Telmo Sendino
Fecha: 2 de mayo 2024
*/

#include "p24HJ256GP610A.h"

// Funcion para inicializar el modulo CN: Activación de interrupciones del módulo y de los pines seleccionados
//==================
void inic_CN()
{
    CNEN2bits.CN16IE = 1;   // puulsador s6
    CNEN1bits.CN15IE = 1;   // pulsador s3
    CNEN2bits.CN19IE = 1;   // pulsador s4
    CNEN2bits.CN23IE = 1;   // pulsador s5
    CNEN1bits.CN2IE = 1;    // RB0, boton del joystick
    
    IEC1bits.CNIE = 1;      // habilitacion de la interrupcion general de CN
    
    IFS1bits.CNIF = 0;      // Puesta a 0 del flag IF del modulo
}
