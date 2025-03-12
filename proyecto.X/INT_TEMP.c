/*
 Funciones relacionadas con las interrupciones del temporizador,adc y uart: inicializacion del modulo
 y rutina de atencion.

Autores: Asier Aguilar, Erik Cembreros y Telmo Sendino
Fecha: 3 de mayo 2024
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "LCD.h"
#include "UART2_RS232.h"
#include "mensajes.h"

extern unsigned int mili, deci, seg, min;
extern unsigned int ADCValue;
extern unsigned int dist_i2c, lcd_first_line, lcd_second_line;
unsigned int PWMchange=0;
unsigned int i=0, fila=0, estadoUART=0, muestreo=0, cont_temp=0, cont_pot=0, estadoOC=0;

//Asignaciones para el ADC
unsigned int valores_temp[8], valores_pot[8], valores_X[8], valores_Y[8], valores_Z[8];
unsigned int cont_ADC = 0;
unsigned int estadoADC = 0;
unsigned int analog_ctrl_servos = 0;

unsigned int PWM1=1992, PWM2=6517, PWM3=6157, PWM4=2122, PWM5=5802;
unsigned int resto = 62500;

//Actualiza el contador del cronometro
void _ISR_NO_PSV _T4Interrupt()
{	
    mili +=4;
    IFS1bits.T4IF = 0;
}

//interrupcion del timer2 cada vez que interrumpe pone a 1 la señal de un servo y a 0 la del anterior o pone todos a 0 hasta que pase el tiempo necesario para volver a empezar
void _ISR_NO_PSV _T2Interrupt()
{	
    //Control de servos
    switch(estadoOC){
        case 0:  //Activar servo 1
            LATDbits.LATD0=1;
            resto = PR20ms - PWM1;
            TMR2=0;
            PR2=PWM1;
            estadoOC=1;
        break;
        case 1: //Activar servo 2
            LATDbits.LATD0=0;
            LATDbits.LATD1=1;
            resto = PR20ms - PWM2;
            TMR2=0;
            PR2=PWM2;
            estadoOC=2;
        break;
        case 2: //Activar servo 3
            LATDbits.LATD1=0;
            LATDbits.LATD2=1;
            resto = PR20ms - PWM3;
            TMR2=0;
            PR2=PWM3;
            estadoOC=3;
        break;
        case 3: //Activar servo 4
            LATDbits.LATD2=0;
            LATDbits.LATD3=1;
            resto = PR20ms - PWM4;
            TMR2=0;
            PR2=PWM4;
            estadoOC=4;
        break;
        case 4: //Activar servo 5
            LATDbits.LATD3=0;
            LATDbits.LATD8=1;
            resto = PR20ms - PWM5;
            TMR2=0;
            PR2=PWM5;
            estadoOC=5;
        break;
        case 5: //Esperar el resto
            LATDbits.LATD8=0;
            TMR2=0;
            PR2=resto;
            estadoOC=0;
        break;
    }
    
    IFS0bits.T2IF = 0;
}

// RUTINA DE ATENCION A LA INTERRUPCION DE CN
//==============================
void _ISR_NO_PSV _CNInterrupt()
{    
    //CN2 (pin AN0), Coloca el brazo en una posicion segura
    if (!PORTBbits.RB0) 
    {
        //Posicion segura
        PWM1 = 1992;
        PWM2 = 6517;
        PWM3 = 6157;
        PWM4 = 2122;
        PWM5 = 5802;
        PWMchange=1;
     }
        
    //pulsador s3 stop/start del cronometro
    if (!PORTDbits.RD6)     
    {
        T4CONbits.TON = !T4CONbits.TON;
    }

     //pulsador s6 reinica el cronometro
    if (!PORTDbits.RD7)     
    {
        T4CONbits.TON = 0;
        TMR4 = 0 ;
        mili = deci = seg = min = 0;
        Ventana_LCD[1][7]='0';
        Ventana_LCD[1][8]='0';
        Ventana_LCD[1][10]='0';
        Ventana_LCD[1][11]='0';
        Ventana_LCD[1][13]='0';
    }
    
    // pulsador S5 hace scroll de la LCD
    if (!PORTAbits.RA7)     
    {
      if (lcd_first_line+1 == (ULTIMA_FILA))
          lcd_first_line = 0;
      else
         lcd_first_line++;
          
      if (lcd_second_line+1 == (ULTIMA_FILA)) // si es 10 ha llegado al final
          lcd_second_line = 0;
      else 
          lcd_second_line++;
    }
    
    //s4 conmuta que el control de los servos se haga por seniales analogicas o digitales (teclado)
    if (!PORTDbits.RD13)     
    {
       analog_ctrl_servos = !analog_ctrl_servos;
       LATAbits.LATA6 = analog_ctrl_servos;
    }

    IFS1bits.CNIF = 0;  
}

//Rutina que recoge pulsaciones desde la UART para parar/continuar/reiniciar el cronometro o controlar los servomotores
// Rutina de atencion a la recepcion de la UART
void _ISR_NO_PSV _U2RXInterrupt()
{
    unsigned int caracter = U2RXREG; // valor de tecla pulsada
    
    //s3 stop/start crono
    if (caracter=='p' || caracter == 'P')     
    {
        T4CONbits.TON = 0;
    }
    if (caracter=='c' || caracter == 'C')     
    {
        T4CONbits.TON = 1;
    }
    
    //s6 reinicia cronometro (lo pone a 0)
    if (caracter=='I' || caracter == 'i')     
    {
        T4CONbits.TON = 0;	// para el T4
        TMR4 = 0;
        //Escribir 0s
        mili = deci = seg = min = 0;
        Ventana_LCD[1][7]='0';
        Ventana_LCD[1][8]='0';
        Ventana_LCD[1][10]='0';
        Ventana_LCD[1][11]='0';
        Ventana_LCD[1][13]='0';
    }
  
    //Si se controlan por UART (si control de servos por senales analogicas desactivado)
    if (!analog_ctrl_servos) {
        // servo 1
        if (caracter=='Q' || caracter == 'q')     
        {
                if (PWM1 <= DUTY_MAX-10){
                    PWM1 += 10;
                    PWMchange=1;
                } 
        }
        if (caracter=='A' || caracter == 'a')     
        {
                if (PWM1 >= DUTY_MIN+10){
                    PWM1 -= 10;
                    PWMchange=1;
                }
        }
        // servo 2
        if (caracter=='W' || caracter == 'w')     
        {
                if (PWM2 <= DUTY_MAX-10){
                    PWM2 += 10;
                    PWMchange=1;
                } 
        }
        if (caracter=='S' || caracter == 's')     
        {
                if (PWM2 >= DUTY_MIN+10){
                    PWM2 -= 10;
                    PWMchange=1;
                } 
        }
        //servo 3
        if (caracter=='E' || caracter == 'e')     
        {
            if (PWM3 <= DUTY_MAX-10){
                PWM3 += 10;
                PWMchange=1;
            } 
        } 
        if (caracter=='D' || caracter == 'd')     
        {
            if (PWM3 >= DUTY_MIN+10){
                PWM3 -= 10;
                PWMchange=1;
            }
        }
    }
    
    //Siempre se controlan por UART -> (serv4 y 5)
    //servo 4
    if (caracter=='R' || caracter == 'r')     
    {
        if (PWM4 <= DUTY_MAX-10){
            PWM4 += 10;
            PWMchange=1;
        } 
    }
    if (caracter=='F' || caracter == 'f')     
    {
        if (PWM4 >= DUTY_MIN+10){
            PWM4 -= 10;
            PWMchange=1;
        }
    }
    //servo 5
    if (caracter=='T' || caracter == 't')     
    {
        if (PWM5 <= DUTY_MAX-10){
            PWM5 += 10;
            PWMchange=1;
        }
    }
    if (caracter=='G' || caracter == 'g')     
    {
        if (PWM5 >= DUTY_MIN+10){
            PWM5 -= 10;
            PWMchange=1;
        }
    }

    IFS1bits.U2RXIF=0;  
} 

//Manda a la UART todos los datos a escrbir en la terminal
// Rutina de atencion al envio del UART
void _ISR_NO_PSV _U2TXInterrupt(){
    
    switch(estadoUART){
        case 0: //Limpiar pantalla
            U2TXREG = clrscr[i];
            i++;
            if(i==4){
                i=0;
                estadoUART=1;
            }
            break;
        case 1: //Caracter home (volver a inicio)
            U2TXREG = home[i];
            i++;
            if (i==3) 
            { 
                estadoUART=2;
                i=0;
            }
            break;
        case 2: //Transmitir caracteres
            U2TXREG = Ventana_LCD[fila][i];
            i++;
            if (i==18) 
            {   
                i=0;
                fila++;
            }
            if (fila >= ULTIMA_FILA){
                estadoUART = 1;
                fila=0;
            }
            break;
    }
    IFS1bits.U2TXIF=0;
} 

// Recoge el valor de una entrada analogica y prepara la siguiente entrada 
// Rutina de atencion a la interrupcion del ADC 
//
// Se activa cuando ha digitalizado 1 dato (1 muestreo por digitalizacion): Se ejecuta cada 0.5 ms aprox.--> (fin de muestreo controlado por T3)
// Segun se termina de ejecutar, automaticamente comienza el proximo muestreo.
void _ISR_NO_PSV _ADC1Interrupt(){
    
    ADCValue=ADC1BUF0;
    
    //Si se han hecho 8 conversiones y no se han leido no hara nada
    if(cont_ADC!=8){
     switch(estadoADC){
        case 0:  //Muestreo de potenciometro
            valores_pot[cont_ADC]=ADCValue;
            AD1CHS0bits.CH0SA=TEMP;
            estadoADC=1;
            break;
         case 1: //Muestreo de temperatura
            valores_temp[cont_ADC]=ADCValue;
            AD1CHS0bits.CH0SA=X;
            estadoADC=2;
            break;
        case 2: //Muestreo de eje X del joistick
            valores_X[cont_ADC]=ADCValue;
            AD1CHS0bits.CH0SA=Y;
            estadoADC=3;
            break;
        case 3: //Muestreo de eje Y del joistick
            valores_Y[cont_ADC]=ADCValue;
            AD1CHS0bits.CH0SA=Z;
            estadoADC=4;
            break;
        case 4: //Muestreo de eje Z del joistick
            valores_Z[cont_ADC]=ADCValue;
            AD1CHS0bits.CH0SA=POT;
            estadoADC=0;
            cont_ADC++;
            break;
     }
    }
   
    IFS0bits.AD1IF=0; 
}
