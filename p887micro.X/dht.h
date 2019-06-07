/****************************************************************
 *  Libreria DHT para los sensores DHT11/22 1-Wire				*
 *  Autor: Pablo Zarate email:pablo@digital-bo.com				*
 *  facebook group: ELECTRONICA MCU Version:1906				*
 *  Material de uso para aprendizaje en ELT436 U.E.B.			*
 * 	Requiere libreria periphera.h								*
 * #define DHTTYPE <modelo> //11 = DHT11 o 22 = DHT22			*
 * #define DHTpin PORTXbits.RXX //Pin para los datos I/O		*
 * #define DHTtris TRISXbits.TRISXX //Control I/O				*
 ****************************************************************/
#if _XTAL_FREQ < 4000000  
#error "Fosc bajo para sensor rutinas DHT"
#endif

#define DHT11 1 //40 bits <8b:intHU><8b:decHU><8b:intTE><8b:decTE><8b:chk>
#define DHT22 2 //40 bits <16:HU><16b:TE><8b:chk>
#if !defined(DHTpin) && !defined(DHTtris)
	#error "Lib DHT debe definir DHTpin/DHTtris"
#endif
#if !defined(DHTTYPE)
	#error "Lib DHT modelo DHTTYPE no especificado"
#endif
typedef struct
{
    unsigned int rawhum;
    unsigned int rawtem;
} dhtstruct;
void DHTSetup();
char DHTReadbyte();
char DHTUpdate(dhtstruct *dht);
#include "dht.c"
