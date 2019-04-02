/* Programa para destellar un diodo led conectado al pin RA4
 * Se mantiene en alto 100ms y en bajo 900ms */

#pragma config FOSC = INTRCIO, WDTE = OFF
#include <xc.h>
#define _XTAL_FREQ 8000000  //Tcy=0.5uS
#define LEDpin PORTAbits.RA4 
#include "peripheral.h"
char ledcnt = 0; //Contador 8-bits
void main(void)
{   
    OSCSetup(); //Ajusta el oscilador
    ANSEL = 0; //Pines en modo digital AN0-AN7  
    ANSELH = 0; //Pines en modo digital AN8-AN13
    TRISAbits.TRISA4 = 0; //Salida pin RA0
    while(true)
    {
        if(ledcnt > 99)
        {
            ledcnt = 0;
            LEDpin = 1; //Activa el LED 
        }
        if(ledcnt == 9)
        {
            LEDpin = 0; //Apaga el LED
        }
        ledcnt = ledcnt + 1; //incrementa la variable
        __delay_ms(10);
    }
}
