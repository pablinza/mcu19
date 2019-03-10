/************************************************************************
* Libreria de rutinas basicas para los Recursos/Perifericos de los MCU	*
* 				PIC16F687/F689/F690 			   				* 
*    Company: Universidad Evangelica Boliviana 							*
*     Author: Pablo Zarate A. pablinza@me.com							*
*    Version: Dec 2019 V19.03  										*
*    Summary: Es una libreria de funciones y procedimientos de uso base *
*			  para los PIC serie 16F68x empleados en la materia ELT-436.*
************************************************************************/
void OSCSetup()
{
#if _XTAL_FREQ == 8000000
    OSCCONbits.IRCF = 7;
#elif _XTAL_FREQ == 4000000
    OSCCONbits.IRCF = 6;
#elif _XTAL_FREQ == 2000000
    OSCCONbits.IRCF = 5;
#elif _XTAL_FREQ == 1000000
    OSCCONbits.IRCF = 4;
#elif _XTAL_FREQ == 500000
    OSCCONbits.IRCF = 3;
#elif _XTAL_FREQ == 250000
    OSCCONbits.IRCF = 2;
#elif _XTAL_FREQ == 125000
    OSCCONbits.IRCF = 1;
#elif _XTAL_FREQ == 31000
    OSCCONbits.IRCF = 0;
#else 
    #warning "Oscilador Interno no ajustado a Frecuencia"
#endif
#if _XTAL_FREQ < 125000
    while(!OSCCONbits.LTS);
#else
    while(!OSCCONbits.HTS);   
#endif
}

#ifndef USART_LIB
void USARTSetup(unsigned int baud)
{
    unsigned int brg;
    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 1;
    brg = _XTAL_FREQ/(4*(baud + 1));
    SPBRG = brg;
    SPBRGH = brg >> 8;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;
    RCSTAbits.SPEN = 1;
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
    EEDAT = data;
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
    EECON1bits.RD = 1;
    return EEDAT;
}
void ADCSetup()
{
    ADCON1bits.ADCS = 0b11; //Ajusta el TAD a FRC
    ADCON0bits.VCFG = 0; //0=VDD    1=RA1/Vref
    ADCON0bits.ADON = 1;
}
void ADCStart(char ch)
{
    ADCON0bits.CHS = ch;
    __delay_us(20); //2TAD + TACQ
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
#ifdef _P16F690
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
#endif

