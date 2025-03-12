/*
Funciones de relacionadas con la configuración de timers, esperas y cronometro

Autores: Asier Aguilar, Erik Cembreros y Telmo Sendino
Fecha: 6 de febrero 2024
*/


#include "p24HJ256GP610A.h"
#include "commons.h"
#include "timers_v2.h"
#include "utilidades.h"
#include "LCD.h"
#include "mensajes.h"
#include "GPIO.h"

unsigned int mili,deci,seg,min;
unsigned char min_str[2];
unsigned char seg_str[2];
unsigned char deci_str[2];

// Timer T2 para generar señal PWM (por interrupción) 
void inic_Timer2()
{
    TMR2 = 0 ; 	// Inicializar el registro de cuenta
    PR2 =  312;	// Periodo del timer 
    T2CONbits.TCKPS = 1;	// escala del prescaler XXX
    
    T2CONbits.TCS = 0;	// reloj interno
    T2CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    T2CONbits.TON = 1;	// puesta en marcha del timer 

}	

/*
 * Timer que indica fin de muestreo. Usado para hacer 2000muestras/seg de ADC
 * @attention Este timer marca el TIEMPO DE MUESTREO (no el de digitalizacion)
 * 
 * Recordemos: 1 muestreo por digitalizacion. Periodo digitalizacion es 12*Tad -> ciclos adc (aprox): 10 ciclos datos 10 bits + 2 ciclos extra.
 * Tad=2*Tcy (1 ciclo adc = 2 ciclos micro) -> 24 ciclos micro (digitalizacion)
 * Periodo del timer -> 0.5 ms (periodo de 1 muestra) = 12500 ciclos  - 24 ciclos digitalizacion  -1
*/
void inic_Timer3()
{
    T3CON = 0x0000;	
    TMR3 = 0; 	 
    PR3 = 12500-24-1; // Periodo del timer -> 0.5 ms (periodo de 1 muestra) - ciclos digit
	IFS0bits.T3IF = 0;  
    IEC0bits.T3IE = 0;  
    T3CONbits.TON = 1;	
}

// Timer para controlar el cronometro (PR4 se inicializa en inic_crono)
void inic_Timer4 ()
{
    TMR4 = 0 ; 	// Inicializar el registro de cuenta
    T4CONbits.TCS = 0;	// reloj interno
    T4CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    T4CONbits.TON = 1;	// puesta en marcha del timer 

}	

// Timer para el Sensor Distancia para marcar el tiempo de medición cada vez
void inic_Timer6 ()
{
    TMR6 = 0 ; 	// Inicializar el registro de cuenta
    PR6 =  27344 ;	// Periodo del timer
		// Queremos que cuente 4 ms.
		// Fosc= 50 MHz (vease Inic_oscilator()) de modo que
		// Fcy = 25 MHz (cada instruccion dos ciclos de reloj)
		// Por tanto, Tcy = 40 ns para ejecutar una instruccion
		// Para contar 4 ms se necesitan 100000 ciclos.
    T6CONbits.TCKPS = 2;	// escala del prescaler XXX
    T6CONbits.TCS = 0;	// reloj interno
    T6CONbits.TGATE = 0;	// Deshabilitar el modo Gate
    IEC2bits.T6IE = 0;     
    IFS2bits.T6IF = 0;      // Puesta a 0 del flag IF del modulo
	T6CONbits.TON = 1;	// puesta en marcha del timer 
}

// Timer para esperas activas en las funciones Delay_ms y Delay_us
void inic_Timer8 ()
{
    IEC3bits.T8IE = 0;
    IFS3bits.T8IF = 0;      // Puesta a 0 del flag IF del modulo
}	



//Activar interrupciones timer2
void inic_INT_TEMP_T2()
{
	IFS0bits.T2IF = 0;      // Puesta a 0 del flag IF del modulo
    IEC0bits.T2IE = 1;      // Habilitar interrupcion del temp
}


//Activar interrupciones timer4
void inic_INT_TEMP_T4()
{
	IFS1bits.T4IF = 0;      // Puesta a 0 del flag IF del modulo
    IEC1bits.T4IE = 1;      // Habilitar interrupcion del temp 
}


// inicializacion de las variables del cronometro
void inic_crono()	
{
	//Poner preescaler a 01
    T4CONbits.TCKPS1 = 0;
    T4CONbits.TCKPS0 = 1;
    PR4= 12500-1; // cada 4 ms
}

// Contro del cronometro
/*Actualiza los valores del tiempo en base a la variable mili que se actualiza en la interrupcion del timer 4 cada 4 ms. 
 * Ademas de actualizar los valores los escribe en la matriz de datos cada vez que cambian*/
void cronometro()	
{
    if(mili==100)
    {
        deci++;
        mili=0;
        led_1dec();
        if (deci <10)
        {
            conversion_tiempo_deci(deci_str, deci);
            Ventana_LCD[1][13]=deci_str[0];
        }
   
        if(deci==10)
        {
            led_1seg();
            seg++;
            deci=0;
            conversion_tiempo(deci_str, deci);
            Ventana_LCD[1][13]=deci_str[0];
            conversion_tiempo(seg_str, seg);
            Ventana_LCD[1][10]=seg_str[0];
            Ventana_LCD[1][11]=seg_str[1];
            if(seg==60)
            {
                min++;
                seg=0;      
                //segundos
                conversion_tiempo(seg_str, seg);
                Ventana_LCD[1][10]=seg_str[0];
                Ventana_LCD[1][11]=seg_str[1];
                //minutos
                conversion_tiempo(min_str, min);
                Ventana_LCD[1][7]=min_str[0];
                Ventana_LCD[1][8]=min_str[1];
            }
        }
    }
}

// Funcion que espera ms milisegundos
void Delay_ms(unsigned int ms) {
    unsigned long ticks = (unsigned long)ms * (25000);   // Calcular ticks necesarios para el delay
    T8CON = 0;
    TMR8 = 0;
    IFS3bits.T8IF = 0;

    if(ticks < 65536) {
        T8CONbits.TCKPS0 = 0;
        T8CONbits.TCKPS1 = 0;
    } else if(ticks < 65536 * 8) {
        T8CONbits.TCKPS0 = 1;
        T8CONbits.TCKPS1 = 0;
        ticks = ticks/8;
    } else if(ticks < 65536 * 64) {
        T8CONbits.TCKPS0 = 0;
        T8CONbits.TCKPS1 = 1;
        ticks /= 64;
    } else {
        T8CONbits.TCKPS0 = 1;
        T8CONbits.TCKPS1 = 1;
        ticks /= 256;
    }

    PR8 = (int) ticks - 1; // Configurar periodo para el delay
    T8CONbits.TON = 1; // Encender el temporizador
    while(IFS3bits.T8IF == 0); // Esperar por el flag de interrupciÃ³n
    T8CONbits.TON = 0;
    IFS3bits.T8IF = 0;
}

// Funcion que espera us microsegundos
void Delay_us(unsigned int us) {
    unsigned long ticks = (unsigned long)us * (25);
    T8CON = 0;
    TMR8 = 0;
    IFS3bits.T8IF = 0;

    // Seleccionar el prescaler
    if(ticks < 65536) {
        T8CONbits.TCKPS0 = 0;
        T8CONbits.TCKPS1 = 0;
    } else if(ticks < 65536 * 8) {
        T8CONbits.TCKPS0 = 1;
        T8CONbits.TCKPS1 = 0;
        ticks /= 8;
    } else if(ticks < 65536 * 64) {
        T8CONbits.TCKPS0 = 0;
        T8CONbits.TCKPS1 = 1;
        ticks /= 64;
    } else {
        T8CONbits.TCKPS0 = 1;
        T8CONbits.TCKPS1 = 1;
        ticks /= 256;
    }

    PR8 = (int) ticks - 1;
    T8CONbits.TON = 1;
    while(IFS3bits.T8IF == 0);    
    T8CONbits.TON = 0;
    IFS3bits.T8IF = 0;
}
