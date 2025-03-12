/*
 Utilidades miscelanea: Conversión de tipos y función calculo media
 *
 * 
Autores: Asier Aguilar, Erik Cembreros y Telmo Sendino
Fecha: 14 de marzo 2024
 */

#include "p24HJ256GP610A.h"

// Array definido para la obtencion numeros
unsigned char tabla_carac[16]="0123456789";

// Funcion para obtener los caracteres de un valor de dos digitos (<100)
// Parametros:
// val, valor numerico del que se quieren obtener los caracteres ASCII
// dir, direccion en la que la funcion devuelve los dos codigos ASCII
// En dir estara el caracter de mas peso y en dir+1 el de menos peso
void conversion_tiempo (unsigned char * dir, unsigned int val)
{
    unsigned char dig;
    if (val>99) {
        while(1);    
    } else {
        dig=val/10;
        dig=tabla_carac[dig];
        *dir=dig;
        dir++;
        dig=val%10;
        dig=tabla_carac[dig];
        *dir=dig;
    }
}
	
// Dada una cadena de caracteres de 2 numeros, realiza la conversion a un valor entero
//
// Sirve para convertir de char[] a un numero el valor de las decimas de segundo
void conversion_tiempo_deci (unsigned char * dir, unsigned int val)
{
    unsigned char dig;
    if (val>99) {
        while(1);    
    } else {
        dig=tabla_carac[val];
        *dir=dig;
    }
}

// Dada una cadena de caracteres de 4 numeros, realiza la conversion a un valor entero
void conversion_4digitos (unsigned char * dir, unsigned int val)
{
    unsigned char dig;
    
    dig=val/1000;
    dig=tabla_carac[dig];
    dir[0]=dig;
    val=val%1000;
    
    dig=val/100;
    dig=tabla_carac[dig];
    dir[1]=dig;
    val=val%100;
    
    dig=val/10;
    dig=tabla_carac[dig];
    dir[2]=dig;
    val=val%10;
    
    dig=val;
    dig=tabla_carac[dig];
    dir[3]=dig;    
}

// Dada una cadena de caracteres de 6 numeros, realiza la conversion a un valor entero
void conversion_6digitos (unsigned char * dir, unsigned int val)
{
    unsigned char dig;
    
    dig=val/100000;
    dig=tabla_carac[dig];
    dir[0]=dig;
    val=val%100000;
    
    dig=val/10000;
    dig=tabla_carac[dig];
    dir[1]=dig;
    val=val%1000;
    
    dig=val/1000;
    dig=tabla_carac[dig];
    dir[2]=dig;
    val=val%1000;
    
    dig=val/100;
    dig=tabla_carac[dig];
    dir[3]=dig;
    val=val%100;
    
    dig=val/10;
    dig=tabla_carac[dig];
    dir[4]=dig;
    val=val%10;
    
    dig=val;
    dig=tabla_carac[dig];
    dir[5]=dig;    
}

// Funcion que realiza el calculo de la media dado un vector y la cantidad de elementos
unsigned int calc_media(unsigned int * vector, int cant_datos){
    
    int i;
    unsigned int res=0;	//unsigned int
    for(i=0; i<cant_datos; i++){
        res+=vector[i];
    }
    res=(int)(res/cant_datos);
    return res;
}



