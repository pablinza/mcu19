/*********************************************************
 *  Libreria Basica para control de modulo LCD
 *  Autor: Pablo Zarate email:pablo@digital-bo.com
 *  Material de uso para aprendizaje en ELT436 U.E.B.
 *  (nota) El pin RW del LCD debe ser 0 logico
 *  Se debe definir los siguientes literales
 *  #define LCD_RS Pin de salida
 *  #define LCD_EN Pin de salida
 *  #define LCD_PORT Puerto Pines 0-3 
 *  #define LCD_TRIS Tris Pines 0-3
 *******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
Derechos de Autor PCZ pablinza@me.com.
Se autoriza el uso, copia y distribucion de este software con fines educativos

Favor tomar en cuenta lo siguiente:
El software y documentacion provisto no implica garantia alguna en cuanto al uso 
y sus posibles resultados directos o indirectos, incluyendo posibles incidentes, 
daños personales, daños materiales, perdida de informacion, etc.
*******************************************************************************/
void LCDWritechar(char data)
{
    DelayLCD();
    LCD_RS = 1;
    LCD_TRIS &= 0xf0;
    LCD_PORT &= 0xf0;
    LCD_PORT | = (data>>4);
    LCD_EN = 1;                      // Clock command in
    DelayForTCY();
    LCD_EN = 0;
    LCD_PORT &= 0xf0;
    LCD_PORT |= data & 0x0f;
    LCD_EN = 1;                      // Clock command in
    DelayForTCY();
    LCD_EN = 0;
    LCD_TRIS |= 0x0f;
}
void LCDSet(char cmd)
{
    DelayLCD();
    LCD_RS = 0;                     // Set control bits
    LCD_TRIS &= 0xf0;
    LCD_PORT &= 0xf0;
    LCD_PORT | = (cmd >> 4);
    LCD_EN = 1;                      // Clock nibble into LCD
    DelayForTCY();
    LCD_EN = 0;
    LCD_PORT &= 0x0f;
    LCD_PORT |= cmd & 0x0f;
    LCD_EN = 1;                      // Clock nibble into LCD
    DelayForTCY();
    LCD_EN = 0;
    LCD_TRIS |= 0x0f;
}
void LCDGoto(char DDaddr)
{
    DelayLCD();
    LCD_RS = 0;
    LCD_TRIS &= 0xf0;                 // Make port output
    LCD_PORT &= 0xf0;                      // and write upper nibble
    LCD_PORT |= ((DDaddr | 0b10000000) >> 4);
    LCD_EN = 1;                              // Clock the cmd and address in
    DelayForTCY();
    LCD_EN = 0;
    LCD_PORT &= 0xf0;                      // Write lower nibble
    LCD_PORT |= DDaddr & 0x0f;
    LCD_EN = 1;                              // Clock the cmd and address in
    DelayForTCY();
    LCD_EN = 0;
    LCD_TRIS |= 0x0f;                 // Make port input
}
void LCDSetup(char mode)
{
    LCD_RS = 0;				// Register select pin made low
    LCD_EN = 0;           	// Clock pin made low
    DelayPORLCD();			// Delay for 15ms to allow for LCD Power on reset
    LCD_TRIS &= 0xf0;
    LCD_PORT &= 0xf0;
    LCD_PORT |= 0b00000011;	// Function set cmd(4-bit interface)
    LCD_EN = 1;           	// Clock the cmd and address in
    DelayForTCY();
    LCD_EN = 0;
    DelayLCD();
    LCD_PORT &= 0xf0;    	// Function set cmd(4-bit interface)
    LCD_PORT |= 0b00000011;
    LCD_EN = 1;         	// Clock the cmd and address in
    DelayForTCY();
    LCD_EN = 0;
    DelayForTCY10();
    LCD_PORT &= 0xf0;  		// Function set cmd(4-bit interface)
    LCD_PORT |= 0b00000011;
    LCD_EN = 1;          	// Clock the cmd and address in
    DelayForTCY();
    LCD_EN = 0;
    DelayForTCY10();
    LCD_PORT &= 0xf0;    	// Inicial set cmd(4-bit interface)
    LCD_PORT |= 0b00000010;
    LCD_EN = 1;         	// Clock the cmd and address in
    DelayForTCY();
    LCD_EN = 0;
    DelayForTCY10();
    LCDSet(mode);		// Function set 2l 5x7
    //LCDSet(0b00100000);	// Function set 1l 5x7
    LCDSet(DOFF);		// Clear display
    LCDSet(CLEAR);		// Turn the display on then off
    LCDSet(DON & CURSOR_OFF & BLINK_OFF);// Display ON/Blink ON
    LCDSet(0b00000100);//Cursor Shift
    LCDSet(0b00000110);//Cursor Inc
    LCDSet(HOME);
}
void LCDWrite(char *str)
{
    while(*str)         	// Write data to LCD up to null
    {
    	LCDWritechar(*str); // Write character to LCD
        str++;              // Increment buffer
    }
}

void LCDPuts(const char *str)
{
    while(*str)         	// Write data to LCD up to null
    {
    	LCDWritechar(*str); // Write character to LCD
        str++;              // Increment buffer
    }
}
