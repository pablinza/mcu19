/************************************************************
 *  Libreria PCF8574 para el modulo LCD por I2C				*
 *  Autor: Pablo Zarate email:pablo@digital-bo.com			*
 *  facebook group: ELECTRONICA MCU 						*
 *  Material de uso para aprendizaje en ELT436 U.E.B.		*
 * 	Requiere libreria I2C Addr: 0100xxxR/W					*
 *  PIN Mapping LCD <DB7><DB6><DB5><DB4><LED><EN><RW><RS>	*
 *              PCF < P7>< P6>< P5>< P4>< P3><P2><P1><P0>	*
 ***********************************************************/
void PCFSetRS(char mode) //RS ON/OFF
{
    pcfport &= 0b11111110;
    if(mode) pcfport |= 0b00000001;
    I2CIdle(); //Wait for Idle State
    I2CStart();
    I2CWrite(PCFADDR);
    I2CWrite(pcfport);
    I2CStop();
}
void PCFSetRW(char mode) //RW ON/OFF
{
    pcfport &= 0b11111101;
    if(mode) pcfport |= 0b00000010;
    I2CIdle(); //Wait for Idle State
    I2CStart();
    I2CWrite(PCFADDR);
    I2CWrite(pcfport);
    I2CStop();
}
void PCFSetEN(char mode) //Enable ON/OFF
{
    pcfport &= 0b11111011;
    if(mode) pcfport |= 0b00000100;
    I2CIdle(); //Wait for Idle State
    I2CStart();
    I2CWrite(PCFADDR);
    I2CWrite(pcfport);
    I2CStop();
}
void PCFSetLED(char mode) //Backlight ON
{
    pcfport &= 0b11110111;
    if(mode) pcfport |= 0b00001000;
    I2CIdle(); //Wait for Idle State
    I2CStart();
    I2CWrite(PCFADDR);
    I2CWrite(pcfport); //set LED
    I2CStop();
}
void PCFSetDB(char nibble) //Set only DB data
{
    //char pcfdata;
    //I2CIdle(); 
    //I2CStart();
    //I2CWrite(PCFADDR | 1); 
    //pcfdata = I2CRead(); //Read PCF data
    //I2CStop();
    
    pcfport &= 0b00001111;
    nibble <<= 4;
    pcfport |= nibble;
    I2CIdle(); //Wait for Idle State
    I2CStart();
    I2CWrite(PCFADDR);
    I2CWrite(pcfport); 
    I2CStop();
}
void PCFSetPORT(char data) //Set all Port with Data
{
    I2CIdle();
    I2CStart();
    I2CWrite(PCFADDR);
    I2CWrite(data); 
    I2CStop();
    pcfport = data;
}
char PCFGetPORT() //Get all Port with Data
{
    char data;
    I2CIdle(); 
    I2CStart();
    I2CWrite(PCFADDR | 1); 
    data = I2CRead(); //Read PCF data
    I2CStop();
    return data;
}
void PCFSetupLCD(char mode) //<DB7><DB6><DB5><DB4><LED><EN><RW><RS>
{ 
    PCFSetPORT(0); //EN=0 RW=0 RS=0 
    DelayPORLCD();// Delay for 15ms to allow for LCD Power on reset
    PCFSetDB(0b0011); // Function set cmd(4-bit interface)
    PCFSetEN(1);
    DelayForTCY();
    PCFSetEN(0);
    DelayLCD();
    PCFSetDB(0b0011); // Function set cmd(4-bit interface)
    PCFSetEN(1);
    DelayForTCY();
    PCFSetEN(0);
    DelayForTCY10();
    PCFSetDB(0b0011); // Function set cmd(4-bit interface)
    PCFSetEN(1);
    DelayForTCY();
    PCFSetEN(0);
    DelayForTCY10();
    PCFSetDB(0b0010); // Function set cmd(4-bit interface)
    PCFSetEN(1);
    DelayForTCY();
    PCFSetEN(0);
    DelayForTCY10();
    PCFSetLCD(mode);  // Function set 2l 5x7
    PCFSetLCD(DOFF);  // Clear display
    PCFSetLCD(CLEAR); // Turn the display on then off
    PCFSetLCD(DON & CURSOR_OFF & BLINK_OFF);// Display ON/Blink ON
    PCFSetLCD(0b00000100);//Cursor Shift
    PCFSetLCD(0b00000110); //Cursor Inc
    PCFSetLCD(HOME);
}
void PCFSetLCD(char cmd)
{
    DelayLCD();
    PCFSetRS(0); //Command Mode
    PCFSetDB(cmd >> 4); //MSB nibble                   
    PCFSetEN(1);                    
    DelayForTCY();
    PCFSetEN(0);                   
    PCFSetDB(cmd); //LSB nibble                   
    PCFSetEN(1);                    
    DelayForTCY();
    PCFSetEN(0);      
}
void PCFWritecharLCD(char data)
{
    DelayLCD();
    PCFSetRS(1); //Data Mode
    PCFSetDB(data >> 4); //MSB nibble                   
    PCFSetEN(1);
    DelayForTCY();
    PCFSetEN(0);
    PCFSetDB(data); //LSB nibble                   
    PCFSetEN(1);                    
    DelayForTCY();
    PCFSetEN(0);
}
void PCFGotoLCD(char DDaddr)
{
    DelayLCD();
    PCFSetRS(0); //Command Mode
    PCFSetDB((DDaddr | 0b10000000) >> 4);
    PCFSetEN(1);
    DelayForTCY();
    PCFSetEN(0);
    PCFSetDB(DDaddr);
    PCFSetEN(1);
    DelayForTCY();
    PCFSetEN(0);
}
void PCFGotolnLCD(char line)
{
    char DDaddr = 0;
    switch(line)
    {
        case 0: DDaddr = 0x00;
            break;
        case 1: DDaddr = 0x40;
            break;
        case 2: DDaddr = 0x14;
            break;
        case 3: DDaddr = 0x54;
    }
    DelayLCD();
    PCFSetRS(0); //Command Mode
    PCFSetDB((DDaddr | 0b10000000) >> 4);
    PCFSetEN(1);
    DelayForTCY();
    PCFSetEN(0);
    PCFSetDB(DDaddr);
    PCFSetEN(1);
    DelayForTCY();
    PCFSetEN(0);
}
void PCFWriteLCD(char *str)
{
    while(*str)                  // Write data to LCD up to null
    {
    	PCFWritecharLCD(*str); // Write character to LCD
        str++;               // Increment buffer
    }
}
void PCFPutsLCD(const char *str)
{
    while(*str)                  // Write data to LCD up to null
    {
    	PCFWritecharLCD(*str); // Write character to LCD
        str++;               // Increment buffer
    }
}


