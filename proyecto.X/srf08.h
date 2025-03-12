/*
Funciones para utilizar el sensor de distancia

Autores: Asier Aguilar, Erik Cembreros y Telmo Sendino
Fecha: 08/04/2024
 */
unsigned int inic_medicion_dis (unsigned char dirI2C);
unsigned int leer_medicion (unsigned char dirI2C, unsigned char *dis) ;
void cambiar_direccion (unsigned char dirI2C, unsigned char newdirI2C);
unsigned int detectar_direccion (unsigned char *dirI2C);
