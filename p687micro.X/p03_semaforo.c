/* Programa para manejo de la interrupcion externa INT
 * Objetivo: Elaborar un programa para control de un semaforo
 * para dos intersecciones, que se coloque en estado de emergencia
 * en caso de una falla en cualquier indicador de señal.
 * Las luces de la calle 1 son VerdeA=RC0 AmarilloA=RC1 RojoA=RC2
 * Las luces de la calle 2 son VerdeB=RC3 AmarilloB=RC4 RojoB=RC5
 * La entrada de señal para alerta o falla es el pin RB0 
 */
#pragma config FOSC = INTRCIO, WDTE = OFF
#include <xc.h>
#define _XTAL_FREQ 8000000
#define LEDpin PORTAbits.RA4
#define BUTpin PORTAbits.RA2
#include "peripheral.h"
unsigned int mscnt = 0, segcnt = 0, mscntled = 0, msalert = 0;
char alertON = 0; //Bandera para interrupcion alert
void TaskAlert()
{
    static char st = 0; //Variable local de estado
    if(st == 0) //Si estado es 0
    {
        PORTC = 0b000000; //Apaga luces
        st = 1;
    }
    else    //Si estado es 1
    {
        PORTC = 0b010010; //Activa luces amarillas
        st = 0;
    }
}
void TaskSemaforo()
{
    static char st = 0; //variable local de estado
    switch(st)
    {
        case 0: //Si estado es 0 
            if(segcnt == 1) //Se repite hasta el segundo 2
            {
                PORTC = 0b100001;//RAV - RAV
                st = 1;
            }
            break;
        case 1:
            if(segcnt == 5) //Se repite hasta el segundo 5
            {
                PORTC = 0b010001; //RAV - RAV
                st = 2;
            }
            break;
        case 2:
            if(segcnt == 6) //Se repite hasta el segundo 6
            {
                PORTC = 0b001100; //RAV - RAV
                st = 3;
            }
            break;
            
        case 3:
            if(segcnt == 12) //Se repite hasta el segundo 12
            {
                PORTC = 0b001010; //RAV - RAV
                st = 0; //Cambia a estado 0
                segcnt = 0; //Se reinicia el tiempo segundo
            }
            break;
    }
}
void interrupt isr()
{
    if(INTCONbits.INTF == 1) //Verifica si es la interrupcion externa
    {
        PORTC = 0b000000;
        alertON = 1;    //Activa la bandera
        INTCONbits.INTF = 0;    //Limpia la bandera de interrupcion
    }
}
void main(void) 
{
    ANSEL = 0;
    ANSELH = 0;
    OSCSetup();
    EnablePU();
    TRISC = 0; 
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA4 = 0;
    PORTC = 0;  //Apaga las luces al inicio
    INTCONbits.INTE = 1;//Habilita la interrupcion externa
    INTCONbits.GIE = 1; //Habilita las interrupciones
    while(1)
    {
        if(alertON == 1)    //Si alert esta activa
        {
            msalert = msalert + 1;
            if(msalert > 199)   //Para cada 200ms
            {
                TaskAlert();    //Ejecuta el procedimiento
                msalert = 0;
            }
        }
        else    //Si alert no esta activo
        {    
            mscnt = mscnt + 1; 
            if(mscnt > 999) //Para cada 1000ms
            {
                segcnt = segcnt + 1;    //Contador de segundos
                TaskSemaforo(); //Ejecuta el procedimiento
                mscnt = 0;
            }  
        }
        mscntled = mscntled + 1;
        if(mscntled > 299)  //Contador de ms para el LED
        {
            LEDpin = !LEDpin;   //Destella el LED
            mscntled = 0;
        }
        __delay_ms(1);
        
    }
}
