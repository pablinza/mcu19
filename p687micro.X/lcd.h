/************************************************************************
 *  Libreria Basica para control de modulo LCD							*
 *  Autor: Pablo Zarate email:pablo@digital-bo.com						*
 *  Version: 19.0211 													*
 * Material de uso para aprendizaje en ELT436 U.E.B.					*
 *  (nota) El pin RW del LCD debe ser 0 logico							*
 *  Se debe definir los siguientes literales							*
 *  #define LCD_RS Pin de salida										*
 *  #define LCD_EN Pin de salida										*
 *  #define LCD_PORT Puerto Pines 0-3 									*
 *  #define LCD_TRIS Tris Pines 0-3										*
 ************************************************************************/
#ifndef _XTAL_FREQ
	#error "Debe definir la frecuencia de operacion _XTAL_FREQ"
#endif

#define DelayForTCY() __delay_us(1) //req 1uS
#define DelayForTCY10() __delay_ms(1) //req 100us
#define DelayPORLCD() __delay_ms(15)//era 15ms
#define DelayLCD() __delay_ms(5)//era 5ms
/* MAY BE AND*/
#define DON         0x0F
#define DOFF        0x0B
#define CURSOR_ON   0x0F
#define CURSOR_OFF  0x0D
#define BLINK_ON    0x0F
#define BLINK_OFF   0x0E
/* MAY BE AND*/
#define CLEAR       0x01
#define HOME        0x02
#define LINES2		0x28	// Function set 2l 5x7
#define LINES1		0x20	// Function set 1l
#define LCDPutc LCDWritechar
void LCDWritechar(char data);
void LCDSet(char cmd);
void LCDGoto(char DDaddr);
void LCDSetup(char mode);
void LCDWrite(char *str);
void LCDPuts(const char *str);
#include "lcd.c"
