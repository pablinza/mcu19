/************************************************************************
* Libreria de rutinas basicas para los Recursos/Perifericos de los MCU	*
* 				PIC16F882/F883/F884/F886/F887 			   				* 
*    Company: Universidad Evangelica Boliviana 							*
*     Author: Pablo Zarate A. pablinza@me.com							*
*    Version: Dec 2018 V18.12  										*
*    Summary: Es una libreria de funciones y procedimientos de uso base *
*			  para los PIC serie 16F88x empleados en la materia ELT-436.*
************************************************************************/
#ifndef  PERIPHERAL_LIB
#define  PERIPHERAL_LIB

#if !defined(_16F882) && !defined(_16F884) && !defined(_16F886) && !defined(_16F887)
	#error "Libreria rutinas.c no corresponde al PIC seleccionado"
#endif
#ifndef _XTAL_FREQ
	#error "Debe definir la frecuencia de operacion _XTAL_FREQ"
#endif
#define true 1
#define false 0
#define EnablePU() OPTION_REGbits.nRBPU = 0 //Activa las pullups del PORTB
/************************************************
 * 				Modulo OSCILADOR				*
 ************************************************/
void OSCSetup();					//Ajusta el oscilador interno a _XTAL_FREQ

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
void ADCSetup(); 
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
#endif