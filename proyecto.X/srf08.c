/*
Funciones para utilizar el sensor de distancia (se controla con el protocolo digital I2C)

Autores: Asier Aguilar, Erik Cembreros y Telmo Sendino
Fecha: 08/04/2024
 */

#include "i2c_funciones.h"
#include "timers_v2.h"

// Puesta en marcha de una medicion
// Parametro dirI2C corresponde a la direccion I2C del sensor
unsigned int inic_medicion_dis (unsigned char dirI2C)
{
  return LDByteWriteI2C_1(dirI2C, 0, 0x51);
}

// Funcion para leer la distancia medida
// Parametro dirI2C corresponde a la direccion I2C del sensor
// Parametro dis proporciona la direccion de un array en el que devolver la medicion
// Cada medicion lee 2 bytes
unsigned int leer_medicion (unsigned char dirI2C, unsigned char *dis) 
{
  return   LDByteReadI2C_1(dirI2C, 2, dis, 2); //registro 3 low byte first
}


