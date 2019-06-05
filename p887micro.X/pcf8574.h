/************************************************************
 *  Libreria PCF8574 para el modulo LCD por I2C				*
 *  Autor: Pablo Zarate email:pablo@digital-bo.com			*
 *  facebook group: ELECTRONICA MCU 						*
 *  Material de uso para aprendizaje en ELT436 U.E.B.		*
 * 	Requiere libreria I2C Addr: 0100xxxR/W					*
 *  PIN Mapping LCD <DB7><DB6><DB5><DB4><LED><EN><RW><RS>	*
 *              PCF < P7>< P6>< P5>< P4>< P3><P2><P1><P0>	*
 ***********************************************************/

#define DelayForTCY() __delay_us(1) //req 1uS
#define DelayForTCY10() __delay_ms(1) //req 100us
#define DelayPORLCD() __delay_ms(15)//era 15ms
#define DelayLCD() __delay_ms(5)//era 5ms

#define DON         0x0F
#define DOFF        0x0B
#define CURSOR_ON   0x0F
#define CURSOR_OFF  0x0D
#define BLINK_ON    0x0F
#define BLINK_OFF   0x0E

#define CLEAR       0x01
#define HOME        0x02
#define LINES2		0x28	// Function set 2l 5x8 dots
#define LINES1		0x20	// Function set 1l 5x8 dots

#ifndef PCFADDR
#define PCFADDR 0b01001110 //A2=1 A1=1 A0=1 PCF8574 20h to 27h
//#define PCFADDR 0b01111110 //A2=1 A1=1 A0=1 PCF8574A 38h to 3Fh
#endif
#define PCFPutcLCD  PCFWritecharLCD
char pcfport = 0; 
void PCFWritecharLCD(char data);
void PCFWriteLCD(char *str); //Until null char
void PCFSetLCD(char cmd);
void PCFGotoLCD(char DDaddr);
void PCFGotolnLCD(char line);
void PCFSetupLCD(char mode);
void PCFPutsLCD(const char *str);
void PCFSetRS(char mode);
void PCFSetRW(char mode);
void PCFSetEN(char mode);
void PCFSetLED(char mode); //Backlight ON/OFF
void PCFSetPORT(char data);
char PCFGetPORT();
#include "pcf8574.c"

