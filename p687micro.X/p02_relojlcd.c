/* Programa para el manejo de una pantalla LCD16x2
 * el mismo estara conectado al PORTC y mostrara un
 * mensaje con la hora en [hh:mm:ss] */

#pragma config FOSC = INTRCIO, WDTE = OFF
#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 8000000  //Tcy=0.5uS
#define LEDpin PORTAbits.RA4
#define BUT1pin PORTBbits.RB4
#define BUT2pin PORTBbits.RB6
#include "peripheral.h"
#define LCD_RS PORTCbits.RC5
#define LCD_EN PORTCbits.RC4
#define LCD_PORT PORTC
#define LCD_TRIS TRISC
#include "lcd.h"
char ledcnt = 0, dato;
char tickcnt = 0; //contador de repeticion 10ms
char msg[16];   //arreglo char de 16 caracteres
char hora = 0, minuto = 0, segundo = 0;
void Destello() //funcion para generar destello en el LED
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
void main(void)
{   
    OSCSetup(); //Ajusta el oscilador
    ANSEL = 0;  //Pines AN0-AN7 en modo digital
    ANSELH = 0; //Pines AN8-AN13 en modo digital
    TRISAbits.TRISA4 = 0; //Salida pin RA4
    TRISC = 0x00;   //Salida todos los pines del PORTC
    PORTC = 0x00;   //Inicia todos los pines PORTC en 0
    EnablePU(); //Activa las resistencias Pull-up RB
    LCDSetup(LINES2);
    LCDGoto(0);
    LCDWrite("HORA ACTUAL");
    while(true)
    {
        Destello(); //Destella el LED RA4
        __delay_ms(10);
        tickcnt = tickcnt + 1;
        if(tickcnt > 99) //Condicion se cumple cada segundo
        {
            tickcnt = 0;
            segundo = segundo + 1;
            if(segundo > 59) //Condicion se cumple cada minuto
            {
                segundo = 0;
                minuto = minuto + 1;
                if(minuto > 59) //Condicion se cumple cada hora
                {
                    minuto = 0;
                    hora = hora + 1;
                    if(hora > 23) //Condicion se cumple cada dia
                        hora = 0;
                }
            }
            sprintf(msg, "[%02u:%02u:%02u]", hora, minuto, segundo);
            LCDGoto(0x40);  //Posiciona el cursor LCD en la segunda linea
            LCDWrite(msg);
        }
    }
}
