/*********************************************************
 *  Libreria I2C para el modulo MSSP/SSP PIC16F
 *  Autor: Pablo Zarate email:pablo@digital-bo.com
 *  facebook group: ELECTRONICA MCU 
 *  Material de uso para aprendizaje en ELT436 U.E.B.
 *  V18.1105
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
void I2CSetup(unsigned int rate)
{
  SSPCON = 0; //Reset Values
  SSPCON2 = 0; //Reset Values
  SSPCONbits.SSPM = 0b1000; //Master Mode Fosc/(4*(SPADD+1))
  SSPSTATbits.SMP = 1; //Slew rate 1=off(100KHz/1MHz) 0=On(400KHz)
  if(rate == I2CFAST) SSPSTATbits.SMP = 0; //Slew rate enabled 400KHz
  SSPADD = (_XTAL_FREQ / (4000L * rate))-1; //Setting Clock Speed
  SSPSTATbits.CKE = 0; //some models(CKE = 1 for SMBUS PIC16F87x)
  SSPCONbits.SSPEN = 1;
  TRISCbits.TRISC3 = 1; //SCL Input Open Drain Slave Mode
  TRISCbits.TRISC4 = 1; //SDA Input Open Drain Slave Mode
    
}
void I2CIdle()  //Wait for idle
{
    while((SSPCON2 & 0x1F)||(SSPSTAT & 0x04)); 
}
void I2CAck()   //Send ACK
{
    SSPCON2bits.ACKDT = 0;  //ACK
    SSPCON2bits.ACKEN = 1;  //Initiate Ack Sequence
    while (SSPCON2bits.ACKEN); // wait until ACK sequence is over
}
void I2CNotAck()//Send NACK
{
    SSPCON2bits.ACKDT = 1;  //No ACK
    SSPCON2bits.ACKEN = 1;  //Initiate Ack Sequence
    while (SSPCON2bits.ACKEN); // wait until ACK sequence is over
}
void I2CStop()  //Send STOP
{
    I2CIdle();
    SSPCON2bits.PEN = 1;
    while(SSPCON2bits.PEN);
}
void I2CStart() //Send START
{
    I2CIdle();
    SSPCON2bits.SEN = 1;
    while(SSPCON2bits.SEN); //Wait to Start is sended
}
void I2CRestart()//Send RESTART
{
    I2CIdle();
    SSPCON2bits.RSEN = 1;
    while(SSPCON2bits.RSEN);
}
char I2CRead()
{	
    I2CIdle();
    SSPCON2bits.RCEN = 1;   // enable master for 1 byte reception
	while(!SSPSTATbits.BF); // wait until byte received
	return(SSPBUF);         // return with read byte 
}
char I2CWrite(char data)
{	
    I2CIdle();
    SSPBUF = data;                  // write single byte to SSPBUF
  	if(SSPCONbits.WCOL) return (0); //if WCOL Collision
	while(SSPSTATbits.BF);          //wait until write cycle is complete
	return (!SSPCON2bits.ACKSTAT);  //function returns '1' if transmission is acknowledged
}
