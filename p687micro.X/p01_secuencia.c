/* Programa para destellar un diodo led conectado al pin RA4
 * Se mantiene en alto 100ms y en bajo 900ms 
 * Genera un desplazamiento en el PORTC conforme a los pulsadores
 * conectados en RB4by RB6 */

#pragma config FOSC = INTRCIO, WDTE = OFF
#include <xc.h>
#define _XTAL_FREQ 8000000  //Tcy=0.5uS
#define LEDpin PORTAbits.RA4
#define BUT1pin PORTBbits.RB4
#define BUT2pin PORTBbits.RB6
#include "peripheral.h"
char ledcnt = 0, dato;
char descnt = 0;
void Destello()
{
    if(ledcnt > 99)
    {
        ledcnt = 0;
        LEDpin = 1;
    }
    if(ledcnt == 9)
        LEDpin = 0;
    ledcnt = ledcnt + 1;    
}
void Secuencia()
{
    if(descnt < 10) 
    {
        descnt ++;
        return;
    }
    descnt = 0;
    if(BUT1pin == 0) //Si RB4 es 0
    {
        if(dato != 0)
            dato = dato >> 1; //Desplazamiento derecha
        else dato = 0x80;
    }
    if(BUT2pin == 0) //Si RB6 es 0
    {
        if(dato != 0)
            dato = dato << 1; //Desplazamiento izquiera
        else dato = 0x01;   
    }
    PORTC = dato;
}
void main(void)
{   
    OSCSetup(); //Ajusta el oscilador
    ANSEL = 0;  //Pines AN0-AN7 en modo digital
    ANSELH = 0; //Pines AN8-AN13 en modo digital
    TRISAbits.TRISA4 = 0; //Salida pin RA4
    TRISC = 0x00;   //Salida todos los pines del PORTC
    PORTC = 0x00;   //Inicia todos los pines PORTC en 0
    EnablePU(); //Activa las resistencias Pull-up RB
    while(true)
    {
        Destello(); //Destella el LED RA4
        Secuencia(); //Genera secuencia 
        __delay_ms(10);
    }
}
