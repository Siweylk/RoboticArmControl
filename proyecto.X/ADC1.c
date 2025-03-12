/* Funciones para el modulo ADC1
Autores: Telmo Sendino, Erik Cembreros, Asier Aguilar
Fecha:5-3-2024
*/

#include <p24HJ256GP610A.h>
#include "commons.h"
#include "utilidades.h"


unsigned int ADCValue = 0; 
void inic_ADC1 (void)
{
// Inicializacion registro control AD1CON1 AD1CON2, AD1CON3, AD1CON4
AD1CON1 = AD1CON2= AD1CON3= AD1CON4=0; 
// Salida de 10 bits
//AD1CON1bits.AD12B = 0;  

// Comienzo digitalizacion automatico
// 111=Auto-convert / 010=TMR3 ADC1 y TMR5 ADC2 / 001=INT0 / 000= SAMP 
//AD1CON1bits.SSRC=7;    		
AD1CON1bits.SSRC=2; //fin de muestreo controlado por timer3
// Muestreo simultaneo o secuencial
//AD1CON1bits.SIMSAM = 0; 
             
// Comienzo muestreo automático o por programa (SAMP=1) 		
AD1CON1bits.ASAM = 1; //1 automatico

// Reloj con el que funciona el ADC:  0= reloj CPU; 1= RC erlojua 
AD1CON3bits.SAMC=31;   // Tiempo muestreo = 31*Tad 
AD1CON3bits.ADCS=1;   // Relacion entre TAD y Tcy TAD = Tcy(ADCS+1)
//AD1CON3bits.ADCS=1;   // Relacion entre TAD y Tcy   TAD = Tcy(ADCS+1)

//Activar muestreo de varios canales
AD1CON2bits.ALTS=0;

// Inicializacion registro AD1CHS123
AD1CHS123=0;	//seleccion del canal 1,2 eta 3

// Inicializacion registro AD1CHS0
AD1CHS0=0;
AD1CHS0bits.CH0SA=POT; // Potenciometro
//AD1CHS0bits.CH0SB = TEMP;

// Inicializacion registros AD1CSS 
// Si escaneo secuencial 1, si no 0

AD1CSSH = 0;   // 16-31 
AD1CSSL = 0;   // 0-15 

// Inicializacion registros AD1PCFG. Inicialmente todas AN como digitales
AD1PCFGH = 0xFFFF;      // 1= digital / 0= Analog
AD1PCFGL = 0xFFFF;      // Puerto B, todos digitales
// Inicializar como analogicas solo las que vayamos a usar
AD1PCFGLbits.PCFG5=0;   // potenciometro
AD1PCFGLbits.PCFG4=0;   // sensor temperatura

//Entyradas joystick
AD1PCFGLbits.PCFG8=0;   // X
AD1PCFGLbits.PCFG9=0;   // Y
AD1PCFGLbits.PCFG10=0;   // Z

// Bits y campos relacionados con las interrupciones
IFS0bits.AD1IF=0;    
IEC0bits.AD1IE=1;    
//IPC3bits.AD1IP=xx;    

//AD1CON
AD1CON1bits.ADON=1;  // Habilitar el modulo ADC
AD1CON2bits.SMPI=0;  // Un solo muestreo por interrupcion
}


// comienzo del muestreo por programa
void comienzo_muestreo ()
{
    AD1CON1bits.SAMP=1; 
}


// Funcion que recoge el valor del convertidor por encuesta
void recoger_valorADC1 () {
    ADCValue = ADC1BUF0; // Deja el valor en la variable ADCValue y repite el proceso
}
