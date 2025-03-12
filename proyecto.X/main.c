/*
Conversor analogico a digital por interrupcion (potenciometro y termometro)
Control de temporizador por interrupción.

Autores: Asier Aguilar, Erik Cembreros y Telmo Sendino
Fecha: 21 de marzo 2024
*/
#include "p24HJ256GP610A.h"
#include "srf08.h"
#include "oscilator.h"
#include "GPIO.h"
#include "timers_v2.h"
#include "commons.h"
#include "UART2_RS232.h"
#include "INT_TEMP.h"
#include "LCD.h"
#include "ADC1.h"
#include "utilidades.h"
#include "i2c_funciones.h"
#include "mensajes.h"


//Variables ADC
extern unsigned int valores_temp[8], valores_pot[8], valores_X[8], valores_Y[8], valores_Z[8],PWMchange;
unsigned int media_temp = 0, media_pot = 0, media_X = 0, media_Y = 0, media_Z = 0, dist_i2c = 0,medicion =0;
extern unsigned int cont_ADC, analog_ctrl_servos;
unsigned char arrayMediciones[2], arrayDistancia[6], lcd[4];


//Variables de motores
extern unsigned int PWM1, PWM2, PWM3, PWM4, PWM5;
unsigned int lcd_first_line = 0, lcd_second_line = 1;

int main(){

    inic_oscilator();	// Seleccion e inicializacion del reloj: 80 MHz
    inic_leds();	// Inicializacio³n leds: sentido y valor inicial.
    inic_Timer8();
    inic_INT_TEMP_T2();
    // Pulsadores en el puerto 
    TRISDbits.TRISD6 = 1;	// S3
    TRISDbits.TRISD7 = 1;	// S6
    TRISDbits.TRISD13 = 1;  // S4
    TRISAbits.TRISA7 = 1;   // s5
    
    TRISAbits.TRISA6 = 0;   // Led control analogico

    //Salidas PWMs servos
    TRISDbits.TRISD0 = 0;  //Servo1
    TRISDbits.TRISD1 = 0;  //Servo2
    TRISDbits.TRISD2 = 0;  //Servo3
    TRISDbits.TRISD3 = 0;  //Servo4
    TRISDbits.TRISD8 = 0;  //Servo5
    
    //Entrada boton joystick    
    TRISBbits.TRISB0 = 1; //input
    AD1PCFGH=0xFFFF;
    
    //Inicializar LCD
    Init_LCD();
    
    //Inicializar UART
    inic_UART2();
    
    //Escribir linea 1 y 2 en panntalla
    line_1();
    puts_lcd(Ventana_LCD[0], 16);
    line_2();
    puts_lcd(Ventana_LCD[1], 16);
    
    while (PORTDbits.RD6); //Espera a pulsar boton
    
    InitI2C_1();
    inic_CN();
    inic_Timer2();
    
    inic_INT_TEMP_T4(); // Habilita interrupciones de T4
    inic_crono();	//Inicializacion variables cronometro. Periodo 4 ms
    inic_Timer4();	//  Arranca T4
        
    inic_medicion_dis((unsigned char)0xe2);
    inic_Timer6();
    inic_UART2();
    U2TXREG=0; // inicializar buffer envio uart
    inic_ADC1();
    inic_Timer3();
	while(1)
    {   
        cronometro();
  
        // Actualizacion en LCD de valores de servos
        if(PWMchange)
        {
            PWMchange=0;
            conversion_4digitos(lcd, PWM1);
            Ventana_LCD[7][11]=lcd[0];
            Ventana_LCD[7][12]=lcd[1];
            Ventana_LCD[7][13]=lcd[2];
            Ventana_LCD[7][14]=lcd[3];
            
            conversion_4digitos(lcd, PWM2);
            Ventana_LCD[8][3]=lcd[0];
            Ventana_LCD[8][4]=lcd[1];
            Ventana_LCD[8][5]=lcd[2];
            Ventana_LCD[8][6]=lcd[3];
            
            conversion_4digitos(lcd, PWM3);
            Ventana_LCD[8][11]=lcd[0];
            Ventana_LCD[8][12]=lcd[1];
            Ventana_LCD[8][13]=lcd[2];
            Ventana_LCD[8][14]=lcd[3];
            
            conversion_4digitos(lcd, PWM4);
            Ventana_LCD[9][3]=lcd[0];
            Ventana_LCD[9][4]=lcd[1];
            Ventana_LCD[9][5]=lcd[2];
            Ventana_LCD[9][6]=lcd[3];
            
            conversion_4digitos(lcd, PWM5);
            Ventana_LCD[9][11]=lcd[0];
            Ventana_LCD[9][12]=lcd[1];
            Ventana_LCD[9][13]=lcd[2];
            Ventana_LCD[9][14]=lcd[3];
        }
            
        //Escribir entradas analogicas  (Temp, pot, X, Y, Z) y si esta en modo analogico lee actualiza los PWM con los valores de X, Y, Z
        if(cont_ADC==8){
            
            //Temperatura
            media_temp = calc_media(valores_temp, cont_ADC);
            conversion_4digitos(lcd, media_temp);
            Ventana_LCD[3][12]=lcd[0];
            Ventana_LCD[3][13]=lcd[1];
            Ventana_LCD[3][14]=lcd[2];
            Ventana_LCD[3][15]=lcd[3];
            
            //Potenciometro
            media_pot = calc_media(valores_pot, cont_ADC);
            conversion_4digitos(lcd, media_pot);
            Ventana_LCD[2][7]=lcd[0];
            Ventana_LCD[2][8]=lcd[1];
            Ventana_LCD[2][9]=lcd[2];
            Ventana_LCD[2][10]=lcd[3];
            
            //Eje X
            media_X = calc_media(valores_X, cont_ADC);
            conversion_4digitos(lcd, media_X);
            if(analog_ctrl_servos ==1){
                if(PWM1>=DUTY_MIN+20){
                    if(media_X<200){
                        PWM1-=10;
                        PWMchange=1;

                    }
                    else if(media_X<400){
                        PWM1-=5;
                        PWMchange=1;

                    }
                }
                if(PWM1<=DUTY_MAX-20){
                    if(media_X>800){
                        PWM1+=10;
                        PWMchange=1;

                    }
                    else if(media_X>600){
                        PWM1+=5;
                        PWMchange=1;

                    }
                }
            }
            Ventana_LCD[5][11]=lcd[0];
            Ventana_LCD[5][12]=lcd[1];
            Ventana_LCD[5][13]=lcd[2];
            Ventana_LCD[5][14]=lcd[3];
            
            //Eje Y
            media_Y = calc_media(valores_Y, cont_ADC);
            conversion_4digitos(lcd, media_Y);
            if(analog_ctrl_servos==1){
                if(PWM2>=DUTY_MIN+20){
                    if(media_Y<200){
                        PWM2-=10;
                        PWMchange=1;
                    }
                    else if(media_Y<400){
                        PWM2-=5;
                        PWMchange=1;

                    }
                }
                if(PWM2<=DUTY_MAX-20){
                    if(media_Y>800){
                        PWM2+=10;
                        PWMchange=1;

                    }
                    else if(media_Y>600){
                        PWM2+=5;
                        PWMchange=1;                        
                    }
                }
            }
            Ventana_LCD[6][3]=lcd[0];
            Ventana_LCD[6][4]=lcd[1];
            Ventana_LCD[6][5]=lcd[2];
            Ventana_LCD[6][6]=lcd[3];
            
            //Eje Z (PALANCA JOYSTICK)
            media_Z = calc_media(valores_Z, cont_ADC);
            conversion_4digitos(lcd, media_Z);
             if(analog_ctrl_servos==1){
                if(PWM3>=DUTY_MIN+20){
                    if(media_Z<200){
                        PWM3-=10;
                        PWMchange=1;
                    }
                    else if(media_Z<400){
                        PWM3-=5;
                        PWMchange=1;

                    }
                }
                if(PWM3<=DUTY_MAX-20){
                    if(media_Z>800){
                        PWM3+=10;
                        PWMchange=1;

                    }
                    else if(media_Z>600){
                        PWM3+=5;
                        PWMchange=1;

                    }
                }
            }
            Ventana_LCD[6][11]=lcd[0];
            Ventana_LCD[6][12]=lcd[1];
            Ventana_LCD[6][13]=lcd[2];
            Ventana_LCD[6][14]=lcd[3];
            cont_ADC=0;
        }
        
        // Sensor Distancia: mostrar en LCD
        if (IFS2bits.T6IF==1){  
            leer_medicion((unsigned char)0xe4, arrayMediciones);
            medicion = (arrayMediciones[0]<<8)+arrayMediciones[1];
            conversion_6digitos(arrayDistancia, medicion);
            Ventana_LCD[4][10]=arrayDistancia[0];
            Ventana_LCD[4][11]=arrayDistancia[1];
            Ventana_LCD[4][12]=arrayDistancia[2];
            Ventana_LCD[4][13]=arrayDistancia[3];
            Ventana_LCD[4][14]=arrayDistancia[4];
            Ventana_LCD[4][15]=arrayDistancia[5];
            inic_medicion_dis((unsigned char)0xE4);
            IFS2bits.T6IF = 0;
         }
        
        //Escribir en la LCD
        line_1();
	    puts_lcd(Ventana_LCD[lcd_first_line], 16);
        line_2();
	    puts_lcd(Ventana_LCD[lcd_second_line], 16);  
    }
	return (0);
}
	





