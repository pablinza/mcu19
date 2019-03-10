#pragma config FOSC = INTRC_NOCLKOUT, LVP = OFF, WDTE = OFF
#include <xc.h>
#include <stddef.h>
#include <stdio.h>
#define _XTAL_FREQ 8000000
#include "peripheral.h"
enum st1{ ON, OFF} ledst;
#define LEDpin PORTAbits.RA0
#define BUTpin PORTBbits.RB0
char butevent = 0;
void TaskLed()
{
    switch(ledst)
    {
        case OFF:
        if(butevent == true)
        {    
            ledst = ON;
            butevent = 0;
        }
            break;
        case ON:
        if(butevent == true)
        {
            ledst = OFF;
            butevent = 0;
        }
        break;
    }
}
void TaskButton()
{
    static char butsav = 0;
    static int cnt = 0;
    if(butsav != BUTpin)
    {
        cnt ++;
        if(cnt > 1000)
        {
            butsav = BUTpin;
            if(butsav == 0) butevent = 1;
        }
       
    }
    else cnt = 0;
}

void main(void) 
{
    OSCSetup();
    ANSEL = 0;
    ANSELH = 0;
    TRISAbits.TRISA0 = 0;
    TRISBbits.TRISB0 = 1;
    EnablePU();
    ledst = OFF;
    while(true)
    {
        TaskLed();
        TaskButton();
        __delay_ms(1);
    }
}
