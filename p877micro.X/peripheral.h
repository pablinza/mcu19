/************************************************************************
* Libreria de rutinas basicas para los Recursos/Perifericos de los MCU	*
* 				PIC16F87xA /PIC16F87x 			   				* 
*    Company: Universidad Evangelica Boliviana 							*
*     Author: Pablo Zarate A. pablinza@me.com							*
*    Version: Dec 2018 V18.12 			    							*
*    Summary: Es una libreria de funciones y procedimientos de uso base *
*			  para los PIC serie 16F87x empleados en la materia ELT-436.*
************************************************************************/
#if !defined(_16F877) && !defined(_16F876) && !defined(_16F874) && !defined(_16F873)\
&& !defined(_16F877A) && !defined(_16F876A) && !defined(_16F874A) && !defined(_16F873A)
	#error "Libreria rutinas.c no corresponde al PIC seleccionado"
#endif
#ifndef _XTAL_FREQ
	#error "Debe definir la frecuencia de operacion _XTAL_FREQ"
#endif
#define true 1
#define false 0
#define EnablePU() OPTION_REGbits.nRBPU = 0 //Activa las pullups del PORTB
/************************************************
 * 				Modulo USART					*
 ************************************************/
#ifndef USART_LIB
void USARTSetup(unsigned int baud);	//Inicia y configura la velocidad
void USARTCheck();					//Verifica si hay errores y reinicia el receptor
#endif
#define getche getch
void putch(char byte);				//Envia un byte por el modulo USART
char getch();						//Recibe un byte del modulo USART
/************************************************
 * 				Modulo EEPROM					*
 ************************************************/
void EEWrite(char addr, char data); //Escribe una byte en memoria EEPROM
char EERead(char addr);				//Lee un byte de la memoria EEPROM

/************************************************
 * 			Modulo Conversor ADC				*
 ************************************************/
#define AD01234567  0b0000   //Set ANS0-ANS7
#define AD012H4567  0b0001   //Set ANS0-ANS2,ANS3(Vref+),ANS4-ANS7
#define AD01234     0b0010   //Set ANS0-ANS4
#define AD012H4     0b0011   //Set ANS0-ANS2,ANS3(Vref+),ANS4
#define AD013       0b0100   //Set ANS0-ANS3
#define AD01H       0b0101   //Set ANS0-AN1,ANS3(Vref+)
#define ADOFF       0b0111   //All Digital
#define AD01LH4567  0b1000   //Set ANS0-ANS1,ANS2(Vref-),ANS3(Vref+),ANS4-ANS7
#define AD012345    0b1001    //Set ANS0-ANS5
#define AD012H45    0b1010    //Set ANS0-ANS2, ANS3(Vref+), ANS4-ANS5
#define AD01LH45    0b1011    //Set ANS0,ANS2(Vref-), ANS3(Vref+), ANS4-ANS5
#define AD01LH4     0b1100    //Set ANS0-ANS2(Vref-), ANS3(Vref+), ANS4
#define AD01LH      0b1101    //Set ANS0-ANS2(Vref-), ANS3(Vref+)
#define AD0         0b1110    //Set ANS0
#define AD0LH       0b1100    //Set ANS0, ANS2(Vref-), ANS3(Vref+)
void ADCSetup(char mode); 
void ADCStart(char ch);
unsigned int ADCRead();
/************************************************
 * 			Modulo TIMER0           			*
 ************************************************/
#define COUNTER 1
#define TIMER 0
#define T0PRE256 7
#define T0PRE128 6
#define T0PRE64 5
#define T0PRE32 4
#define T0PRE16 3
#define T0PRE8 2
#define T0PRE4 1
#define T0PRE2 0
#define T0PRE1 8
void TMR0Setup(char cs, char pre);
#define TMR0Setval(x) TMR0 = x
char TMR0Getval();
/********************************************
 * 			Modulo TIMER1					*
 ********************************************/
#define T1PRE8 3
#define T1PRE4 2
#define T1PRE2 1
#define T1PRE1 0
void TMR1Setup(char cs, char pre);
#define TMR1Start() T1CONbits.TMR1ON = 1
#define TMR1Stop() T1CONbits.TMR1ON = 0
#define TMR1Setval(x) {TMR1L = x; TMR1H = x >> 8;}
unsigned int TMR1Getval();
/********************************************
 * 			Modulo TIMER2					*
 ********************************************/
#define T2PRE1 0
#define T2PRE4 1
#define T2PRE16 2
#define T2POS1 0
#define T2POS2 1
#define T2POS3 2
#define T2POS4 3
#define T2POS5 4
#define T2POS6 5
#define T2POS7 6
#define T2POS8 7
#define T2POS9 8
#define T2POS10 9
#define T2POS11 10
#define T2POS12 11
#define T2POS13 12
#define T2POS14 13
#define T2POS15 14
#define T2POS16 15
void TMR2Setup(char pre, char post);
#define TMR2Start() T2CONbits.TMR2ON = 1
#define TMR2Stop() T2CONbits.TMR2ON = 0
#define TMR2Setperiod(x) PR2 = x
#define TMR2Setval(x) TMR2 = x
char TMR2Getval();

#include "peripheral.c"
