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
void DHTSetup()
{
    DHTtris = 1;  //Input mode for ZImp. pull-ups
    DHTpin = 1;   //Logic 0 set pin to High on Input mode
}
char DHTReadbyte()
{
    char bitn = 8, res = 0, cnt;
	DHTtris = 1; // TRIS is input(1)
	while(bitn)
    {
		cnt = 0;
#if _XTAL_FREQ > 4000000        
        while(!DHTpin)//Next: wait until pin goes high max 55uS
        {
            NOP();
            if(cnt++ > 50) //min 40uS max55uS, Estimated DHT22
                return 0;
        }
        __delay_us(35);//Normal Tcy 26uS=0 logic 70uS=1 logic, Estimated DHT22
#else
        while(!DHTpin); 
        __delay_us(25);//Low Tcy correction 
#endif
       if(DHTpin)
            res |= 0x01;
		bitn --;
        if(bitn) res <<= 1;
        cnt = 0;
        while (DHTpin) //Wait for end
        {
            NOP();
            if(cnt++ > 99) //min 40uS max55uS, Estimated DHT22 
                return 0;
        }
	}
    return res;
}

char DHTUpdate(dhtstruct *dht)
{
    char cnt, chk, urh, lrh, ute, lte;
    DHTtris = 0;    //Output mode
    DHTpin = 0;     //Line to 0 for Host Start
    if(DHTTYPE == 22)
        __delay_ms(4); //DHT22 require 1ms 
    if(DHTTYPE == 11)
        __delay_ms(18);  //DHT11 require 18ms
    DHTpin = 1;     //Line to 1
    __delay_us(20); //Release line for 20uS
    DHTtris = 1;    //Input mode
    cnt = 0;
    while(DHTpin)   //Wait for sensor response max 40uS
    {
        __delay_us(1);
        if(cnt++ > 40) //min 20uS max40uS, Estimated DHT22
            return 0;
    }
    cnt = 0;
    while(!DHTpin) //wait in pull-down state edge max 80uS
    {
        __delay_us(1);
        if(cnt++ > 80)  //max 80uS, Estimated
            return 0;
    }
    cnt = 0;
    while(DHTpin) //wait in pull-up state max 80uS
    {
        __delay_us(1);
        if(cnt++ > 80) //max 80uS, Estimated wait in up level
            return 0;
    }
    //Read 5byte data <HighRH><LowRH><HIGHTE><LOWTE><CHK>
    urh = DHTReadbyte();
    lrh = DHTReadbyte();
    ute = DHTReadbyte();
    lte = DHTReadbyte();
    chk = DHTReadbyte(); //Checksum
    cnt = urh + lrh + ute + lte;
    if(chk != cnt)
        return 0;
    dht->rawhum = urh << 8;  
    dht->rawhum |= lrh;
    dht->rawtem = ute << 8;
    dht->rawtem |= lte;
    return 1;   
}
