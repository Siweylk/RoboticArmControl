// definiciones generales 

#define Nop()   __builtin_nop()

#define _ISR_NO_PSV __attribute__((interrupt, no_auto_psv))

/////////////////////////////////////
// definiciones para la pantalla LCD
/////////////////////////////////////

/* 
   For Explorer 16 board, here are the data and control signal definitions
   RS -> RB15
   E  -> RD4
   RW -> RD5
   DATA -> RE0 - RE7   
*/

// Control signal data pins 
#define  RW  LATDbits.LATD5       // LCD R/W signal
#define  RS  LATBbits.LATB15      // LCD RS signal
#define  E   LATDbits.LATD4       // LCD E signal 

// Control signal pin direction 
#define  RW_TRIS	TRISDbits.TRISD5 
#define  RS_TRIS	TRISBbits.TRISB15
#define  E_TRIS		TRISDbits.TRISD4

// Data signals and pin direction
#define  DATA      LATE           // Port for LCD data
#define  DATAPORT  PORTE
#define  TRISDATA  TRISE          // I/O setup for data Port


/////////////////////
// definiciones posiciones cronometro
//////////////////////
#define posds 13
#define posseg 10
#define posmin 7

#define POT 5
#define TEMP 4

// Numero de pin AN (RB) de las entradas analogicas generadas por el joystick (Z es palanca)
#define X 8 
#define Y 9
#define Z 10

//
// Definiciones relacionadas con los servos y PWM
//=========================================================  
#define PR20ms 62500 	// Periodo adecuado para conseguir 20 ms
                        // teniendo en cuenta un prescaler de 1:8.
                        // Fosc= 50 Fcy= 25 Tcy=1/25
#define MINPWM 0.5      // Duracion minima pulso PWM (en milisegundos)
#define MAXPWM 2.1      // Duracion maxima pulso PWM (en milisegundos)



#define DUTY_MIN (PR20ms/20)*MINPWM	// valor minimo y maximo de DUTY. Se calculan 
#define DUTY_MAX (PR20ms/20)*MAXPWM	// mediante los "define" PR20ms, MINPWM y MAXPWM

#define ULTIMA_FILA 10 // 11 filas a imprimir en pantalla
 
