/*
Buffer usado para imprimir valores en el LCD 

Autores: Asier Aguilar, Erik Cembreros y Telmo Sendino
Fecha: 23 de abril 2024
 */

#include "p24HJ256GP610A.h"

//Texto a transmitir a la UART y a la LCD
unsigned char Ventana_LCD[10][18]={
     {"Proyecto fin DSE""\x0D\x0A"},  //TITULO
     {"Crono: 00:00,0  ""\x0D\x0A"},  //CRONOMETRO
     {"Poten: XXXX     ""\x0D\x0A"},  //POTENCIOMETRO
     {"Temperatura: XXX""\x0D\x0A"},  //TEMPERATURA
     {"Distancia: XXXXX""\x0D\x0A"},  //TEMPERATURA
     {"JOY:    X: ---- ""\x0D\x0A"},  //JOY STICK X
     {"Y: ---- Z: ---- ""\x0D\x0A"},  //JOY STICK Y, Z
     {"PWM:    S1:XXXX ""\x0D\x0A"},  //PWM 1
     {"S2:XXXX S3:XXXX ""\x0D\x0A"},  //PWM 2, 3
     {"S4:XXXX S5:XXXX ""\x0D\x0A"},  //PWM 4, 5
};



