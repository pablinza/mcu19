/* Microcontroladores U.E.B. / Pablo Zarate Arancibia (2019)    *
 * Programa para el control de 2 servomotores empleado en brazo *
 * de 2 ejes utilizando el modulo TMR1 y la comunicacion USART  *
 * con el siguiente protocolo: <C:1><valor:2><0x10>.            *
 * donde C puede ser 'X'(Eje1) / 'Y'(Eje2) valor (entre 0 - 99) *
 */
#pragma config FOSC = INTRCIO, WDTE = OFF
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#define _XTAL_FREQ 8000000  //Tcy=0.5uS
#define LED1pin PORTAbits.RA4
#define LED2pin PORTAbits.RA5
#define BUT1pin PORTBbits.RB4
#define BUT2pin PORTBbits.RB6
#include "peripheral.h"
#define svnum 2	//Define la cantidad de servos a controlar
#define svslot (65536UL - (2000UL/svnum)) //Calculo del slot de tiempo a cada servo
#define sv1pin PORTCbits.RC0	//Pin del servo1
#define sv2pin PORTCbits.RC1	//Pin del servo2
#define left 900 		//Definicion de tiempo para posicion 0°	(900ms)
#define center 1400		//Definicion de tiempo para posicion 90° (1400ms)
#define right 1900		//Definicion de tiempo para posicion 180° (1900ms)
volatile unsigned int svtime, sv1val = center, sv2val = center;
char svon = 0;
unsigned int ledcnt = 0;
volatile char msg[8], dataOK = 0, rxcnt = 0;
char rxbyte, sigx = 0, sigy = 0;
void interrupt isr()    //Rutina de servicio a la interrupcion
{
    while(PIR1bits.RCIF == 1)
    {
        rxbyte = getch();
        if(rxcnt > 7)
            rxcnt = 0;
        msg[rxcnt] = rxbyte;
        if(rxbyte == '\n')
        {
            dataOK = 1;
            msg[rxcnt] = 0;
            rxcnt = 0;
            LED2pin = 1;
        }
        rxcnt ++;
    }
    if(PIR1bits.TMR1IF == 1) //Condicion de desbordamiento con tiempo variable
    {
        switch(svon)
        {
            case 0:
                sv1pin = !sv1pin; //Genera pulso de control servo1
                if(sv1pin)
                    svtime = 65536 - sv1val; //Tiempo para finalizar pulso
                else
                {
                    svtime = svslot - sv1val; //Tiempo para iniciar pulso
                    svon ++;
                }
                break;
            case 1:
                sv2pin = !sv2pin; //Genera pulso de control servo2
                if(sv2pin)
                    svtime = 65536 - sv2val; //Tiempo para finalizar pulso
                else
                {
                    svtime = svslot - sv2val; //Tiempo para iniciar pulso
                    svon ++;
                }
        }
        if(svon >= svnum) //Si el control supera al maximo de servos 
            svon = 0;
        TMR1Stop(); //Para el contrador
        TMR1Setval(svtime); //Ajusta registro para nuevo tiempo
        TMR1Start(); //Arranca el contador
        PIR1bits.TMR1IF = 0; //Limpia la badera
    }
}

void main(void) //Funcion principal
{
    OSCSetup();
    ANSEL = 0;  //Configura los pines AN0-AN7 en modo digital
    ANSELH = 0; //Configura los pines AN8-AN15 en modo digital
    EnablePU(); //Acvita las resistencias PullUP
    TRISA = 0;  //Configura como salida todos los pines del PORTA
    PORTA = 0;  //Coloca en 0 logico los pines del PORTA
    TRISB = 0xff;
    TRISC = 0;
    PORTC = 0;
    TMR1Setup(TIMER, T1PRE2); //Configura el TMR1 con pre-escala 1:1
    USARTSetup(9600);
    INTCONbits.PEIE = 1; //Cada pulso capturado es de 1uS (4*Tosc)
    PIE1bits.TMR1IE = 1; //Activa la interrupcion del TMR1
    PIE1bits.RCIE = 1;
    INTCONbits.GIE = 1; //Habilitador Global
    TMR1Start(); //Arranca el modulo TMR1
    while(1)
    {
        if(dataOK == 1)
        {
            dataOK = 0;
            if(msg[0] == 'X')
            {
                sigx = atoi(&msg[1]);
                sv1val = 900 + (sigx * 10);
            }
            if(msg[0] == 'Y')
            {   
                sigy = atoi(&msg[1]);
                sv2val = 900 + (sigy * 10);
            }
            rxcnt = 0;
            LED2pin = 0;
        }
        ledcnt ++;
        if(ledcnt > 199) //Aproximado 200x1ms = 200ms
        {
            ledcnt = 0; //Reinicia contador de ciclos (1ms)
            LED1pin = !LED1pin; //Destella LED conectado a RE2
        }
        __delay_ms(1);
    }
}
