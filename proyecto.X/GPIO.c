/*
Funciones de inicializacion y apagado-encendido de los leds

Autores: Asier Aguilar, Erik Cembreros y Telmo Sendino
Fecha: 6 de febrero 2024
*/


#include "p24HJ256GP610A.h"
#include "commons.h"

//Inicializacion de leds
void inic_leds()
{
  TRISA = 0xff00; // Definir como salidas los 8 pines conectados a leds: RA7-RA0		  // El resto como entradas: 1 = In; 0 = Out
}
      
//Conmutar LED 3 (RA0)
void led_1dec()
{
    LATAbits.LATA0 = !LATAbits.LATA0;
    
}

//Conmutar LED 4 (RA1)
void led_1seg()
{
    LATAbits.LATA1 = !LATAbits.LATA1;
}
