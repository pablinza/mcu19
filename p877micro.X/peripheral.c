/************************************************************************
* Libreria de rutinas basicas para los PIC16F87xA                       * 
*    Company: Universidad Evangelica Boliviana 							*
*     Author: Pablo Zarate A. pablinza@me.com							*
*    Version: Dec  2018 V18.12  										*
*    Summary: Es una libreria de funciones y procedimientos de uso base *
*			  para los PIC serie 16F87x empleados en la materia ELT-436.*
************************************************************************/
#ifndef USART_LIB
void USARTSetup(unsigned int baud)
{
    unsigned int brg;
    TXSTAbits.BRGH = 1; //0=FREQ/(64*Baud) 1=FREQ/(16*Baud)
    brg = _XTAL_FREQ/(16*(baud + 1));
    SPBRG = brg;
    TRISCbits.TRISC6 = 1; //TXD HighZ
    TRISCbits.TRISC7 = 1; //RXD HighZ
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;
}
void USARTCheck()
{
    if(RCSTAbits.OERR)
    {
        RCSTAbits.CREN = 0;
        NOP();
        RCSTAbits.CREN = 1;
    }	
}
#endif
void putch(char byte)
{
	while(PIR1bits.TXIF == 0);
	TXREG = byte;
}
char getch()
{
	while(PIR1bits.RCIF == 0);
	return RCREG;
}
void EEWrite(char addr, char data)
{
    char GIEbit;
    EEADR = addr;
    EEDATA = data;
    EECON1bits.EEPGD = 0;
    EECON1bits.WREN = 1;
    GIEbit = INTCONbits.GIE;
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    while(EECON1bits.WR);
    INTCONbits.GIE = GIEbit;
    EECON1bits.WREN = 0;
}
char EERead(char addr)
{
    EEADR = addr;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;
    return EEDATA;
}

void ADCSetup(char mode)
{
    ADCON0bits.ADCS0 = 1; //Ajusta el TAD a FRC
    ADCON0bits.ADCS1 = 1; //Ajusta el TAD a FRC
#if defined(_16F877A) && !defined(_16F876A) && !defined(_16F874A) && !defined(_16F873A)
    ADCON1bits.ADCS2 = 0; //Ajusta el TAD a FRC
#endif
    ADCON1bits.PCFG = mode;
    ADCON0bits.ADON = 1;
}
void ADCStart(char ch)
{
    ADCON0bits.CHS = ch;
    __delay_us(20); //2TAD + TCQ
    ADCON0bits.GO = 1;
}
unsigned int ADCRead()
{
    unsigned int value;
    while(ADCON0bits.GO);
    value = ADRESH;
    value = (value << 8) & 0xFF00;
    value = value | ADRESL;
    value = value >> 6;
    return value;
}
void TMR0Setup(char cs, char pre)
{
    OPTION_REGbits.T0CS = cs;
    OPTION_REGbits.T0SE = 0; //Counter 0=rising 1=falling
    if(pre < 8)
    {    
        OPTION_REGbits.PSA = 0;
        OPTION_REGbits.PS = pre;
    }
    TMR0 = 0;
    INTCONbits.T0IF = 0;
}
char TMR0Getval()
{
    return TMR0;
}
void TMR1Setup(char cs, char pre)
{
    T1CONbits.TMR1CS = cs;
    T1CONbits.T1CKPS = pre;
    TMR1H = 0;
    TMR1L = 0;
    PIR1bits.TMR1IF = 0;
}
unsigned int TMR1Getval()
{
    unsigned int value;
    value = TMR1H;
    value = value << 8;
    value |= TMR1L; 
    return value;
}
void TMR2Setup(char pre, char post)
{
    T2CONbits.T2CKPS = pre;
    T2CONbits.TOUTPS = post;
    PIR1bits.TMR2IF = 0;
}
char TMR2Getval()
{
    return(TMR2);
}

