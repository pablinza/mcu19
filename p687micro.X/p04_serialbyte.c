/* Programa de prueba para envio y recepcion de un Byte
 * por el puerto USART a 9600BPS */

#pragma config FOSC = INTRCIO, WDTE = OFF
#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 8000000  //Tcy=0.5uS
#define LED1pin PORTAbits.RA4
#define LED2pin PORTAbits.RA5
#define BUT1pin PORTBbits.RB4
#define BUT2pin PORTBbits.RB6
#include "peripheral.h"
char rxbyte, txbyte;
char but1cnt = 0, but2cnt = 0; 
char but1st, but2st;
char rxbyte_ok = 0; //Bandera de byte recibido en USART
void interrupt isr()
{
    if(PIR1bits.RCIF == 1)
    {
        rxbyte = getch();
        rxbyte_ok = 1;
    }
}
void main(void)
{   
    OSCSetup(); //Ajusta el oscilador
    ANSEL = 0;  //Pines AN0-AN7 en modo digital
    ANSELH = 0; //Pines AN8-AN13 en modo digital
    TRISAbits.TRISA4 = 0; //Salida pin RA4
    TRISAbits.TRISA5 = 0; //Salida pin RA4
    PORTA = 0x00;   //Inicia todos los pines PORTC en 0
    USARTSetup(9600);
    EnablePU(); //Activa las resistencias Pull-up RB
    INTCONbits.PEIE = 1;
    PIE1bits.RCIE = 1;
    INTCONbits.GIE = 1;
    while(true)
    {
        if(rxbyte_ok == 1) //Si hay un byte listo
        {
            rxbyte_ok = 0; //Limpia la bandera
            switch(rxbyte)
            {
                case 'A':
                    LED1pin = 1;  
                    break;
                case 'B':
                    LED1pin = 0;
                    break;
                case 'C':
                    LED2pin = 1;
                    break;
                case 'D':
                    LED2pin = 0;
            }
        }
        if(BUT1pin != but1st) //Si pulsador cambia de estado
        {
            but1cnt = but1cnt + 1; //incrementa contador 1ms
            if(but1cnt == 199)  //Si contador es 200 x 1ms
            {
                but1cnt = 0;    //Reinicia contador
                but1st = BUT1pin; //El bit estado se igual al pulsador
                if(but1st == 0) //si bit estado es 0 (Presionado)
                    putchar('A'); //Envia el byte ascii A
                else
                    putchar('B');
            }
        }
        if(BUT2pin != but2st)
        {
            but2cnt = but2cnt + 1;
            if(but2cnt == 199)
            {
                but2cnt = 0;
                but2st = BUT2pin;
                if(but2st == 0)
                    putchar('C');
                else
                    putchar('D');
            }
        }
        __delay_ms(1);
    }
}
