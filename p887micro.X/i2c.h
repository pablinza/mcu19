/*********************************************************
 *  Libreria I2C para el modulo MSSP/SSP PIC16F
 *  Autor: Pablo Zarate email:pablo@digital-bo.com
 *  facebook group: ELECTRONICA MCU 
 *  Material de uso para aprendizaje en ELT436 U.E.B.
 *  V19.0211
 *******************************************************************************/
#ifndef  I2C_LIB
#define  I2C_LIB

#ifndef _XTAL_FREQ
    #error "Debe Definir la Frecuencia de Operacion"
#endif
#define I2CLOW 31
#define I2CSTD 100
#define I2CFAST 400
#define I2CHIGH 1000
void I2CSetup(unsigned int rate);
void I2CIdle();  //Test if I2C1 module is in idle
void I2CAck();   //Send ACK
void I2CNotAck();//Send NACK
void I2CStop();  //Send STOP
void I2CStart(); //Send START
void I2CRestart();//Send RESTART
char I2CRead();
char I2CWrite(char data);
#include "i2c.c"

#endif

