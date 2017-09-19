/*********************************************************************
 *
 *                Unimic 01 Version 1.1   10 September 2015
 *
 ***********************************************************************
 * FileName:        unmc_rtcc_01.h
 * Dependencies:    
 * Processor:       PIC18F26J50
 * Compiler:        XC8 1.34
 * Company:         Microchip Technology, Inc.
 *
 ***********************************************************************/

#include<plib/rtcc.h>

rtccTimeDate RtccTimeDate ,RtccAlrmTimeDate, Rtcc_read_TimeDate ;

char segundo_u;         //variables BCD donde se guradan o leen los
char segundo_d;         //datos del RTCC
char minuto_u;
char minuto_d;
char hora_u;
char hora_d;
char fecha_u;
char fecha_d;
char mes_u;
char mes_d;
char dia_semana;
char anio_u;
char anio_d;
bit flag=0;
int i=0;
int j=0;
int a=0;
int b=0;
int c=0;
int k=0;
int anio=15;                //año 2013 -> 00001110
int mes=9;                  //mes enero-> 00000001 (1) .... dici-> 00001100 (12)
int diasem=3;               //0 Dom , 1 Lun , 2 Mar......, 6 Sab
int dia=8;                  //dia 01 -> 00000001.....31 -> 00011111
int hora=18;                 //hora 00 -> 00000000.....24 -> 00011000
int minuto=30;               //variables de numeros enteros que permiten
int segundo=0;              //representar los valores en el LCD


//---Global structures used in deep sleep library---
//rtccTimeDate, RtccTimeDate ,RtccAlrmTimeDate, Rtcc_read_TimeDate ;

rtccTime RtccTime; // Inicializa la estructura de tiempo
rtccTime RtccTimeVal;
rtccDate RtccDate;//Inicializa la estructura de Fecha

unsigned char buffer2[20];

void DelayFor18TCY(void)
{
    Delay10TCYx(10);        // 50us
    //__delay_us(50);
}

void DelayPORXLCD(void)
{
    Delay1KTCYx(20);        // 10ms
    //__delay_ms(10);
}

//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
// Funcion Write_RTC
// Permite capturar los datos del RTC y cargarlos en los registros correspondientes
// (diasem, anio, dia, hora, etc)
//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[

    void Write_RTC(void)
        {
           RtccWrOn();                              //write enable the rtcc registers
           //mRtccSetClockOe(1);                    //saca hacia afuera el pulso(ver patita)RTCC output pin
           PIE3bits.RTCCIE=1;
           segundo_d=segundo/10;
           segundo_u=segundo-segundo_d*10;
           RtccTime.f.sec =segundo_d*16+segundo_u;  //guarda segundo en rtcc
           minuto_d=minuto/10;
           minuto_u=minuto-minuto_d*10;
           RtccTime.f.min =minuto_d*16+minuto_u;    //guarda minuto en rtcc
           hora_d=hora/10;
           hora_u=hora-hora_d*10;
           RtccTime.f.hour=hora_d*16+hora_u;        //guarda hora en rtcc
           anio_d=anio/10;
           anio_u=anio-anio_d*10;
           RtccDate.f.year=anio_d*16+anio_u;        //guarda año en rtcc
           mes_d=mes/10;
           mes_u=mes-mes_d*10;
           RtccDate.f.mon=mes_d*16+mes_u;           //guarda mes en rtcc
           fecha_d=dia/10;
           fecha_u=dia-fecha_d*10;
           RtccDate.f.mday=fecha_d*16+fecha_u;      //guarda dia (numero)en rtcc
           dia_semana=diasem;
           RtccDate.f.wday =diasem;                 //guarda dia (domingo ... sabado)
           RtccWriteTime(&RtccTime,1);              //write into registers
           RtccWriteDate(&RtccDate,1);              //write into registers
           mRtccOn();                               //habilita rtcc
        }


//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
// Funcion Read_RTC
// Permite capturar los datos del RTC y cargarlos en los registros correspondientes
// (diasem, anio, dia, hora, etc)
//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[

void Read_RTC(void)//los digitos del RTCC estan en formato BCD hay que particionar
                    //la lectura de la estructura RtccTime (cada elemento en 2 nibles)
   {
    RtccReadTime(&RtccTime);                //Lee estructura de la hora del RTCC
    segundo_u=(RtccTime.f.sec & 0x0F);      //lee la parte baja en BCD
    segundo_d=((RtccTime.f.sec & 0xF0)>>4); //lee la parte alta en BCD
    minuto_u=(RtccTime.f.min & 0x0F);
    minuto_d=((RtccTime.f.min & 0xF0)>>4);
    hora_u=(RtccTime.f.hour & 0x0F);
    hora_d=((RtccTime.f.hour & 0xF0)>>4);
    RtccReadDate(&RtccDate);                //Lee estructura del la Fecha del RTCC
    fecha_u=(RtccDate.f.mday & 0x0F);
    fecha_d=((RtccDate.f.mday & 0xF0)>>4);
    mes_u=(RtccDate.f.mon & 0x0F);
    mes_d=((RtccDate.f.mon & 0xF0)>>4);
    dia_semana=RtccDate.f.wday;
    anio_u=(RtccDate.f.year & 0x0F);
    anio_d=((RtccDate.f.year & 0xF0)>>4);

    segundo=segundo_d*10+segundo_u;
            minuto=minuto_d*10+minuto_u;
            hora=hora_d*10+hora_u;
            dia=fecha_d*10+fecha_u;
            diasem=dia_semana;
            mes=mes_d*10+mes_u;
            anio=anio_d*10+anio_u;
    }

