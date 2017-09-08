/*********************************************************************
 *
 *                Unimic XC8  Version 1.1   10 September 2015
 *
 *********************************************************************
 * FileName:        unmc_config_01.h
 * Dependencies:
 * Processor:       PIC18F26J50
 * Compiler:        XC8 1.34
 * Company:         Microchip Technology, Inc.
 *
 *********************************************************************/

OSCTUNEbits.INTSRC=1; //setea el oscilador de 32768 para el RTC
    OSCTUNEbits.PLLEN=0; //desactiva PLL
    OSCCONbits.IRCF0=1; //selecciona el clock en 8MHz
    OSCCONbits.IRCF1=1;
    OSCCONbits.IRCF2=1;
    OSCCONbits.SCS0=0; //oscilador INTRC
    OSCCONbits.SCS1=0;
    ANCON0=0b11111101;          // habilita AN1
    TRISCbits.TRISC0=1;
    TRISCbits.TRISC1=1;
    TRISCbits.TRISC2=0;
    TRISCbits.TRISC6=1;
    TRISCbits.TRISC7=1;
    TRISAbits.TRISA0=1;
    TRISAbits.TRISA1=1;
    TRISAbits.TRISA2=1;
    TRISAbits.TRISA3=1;
    TRISAbits.TRISA5=1;
    TRISAbits.TRISA6=1;
    TRISAbits.TRISA7=1;
    RTCCFGbits.RTCEN=1;
    RTCCFGbits.RTCWREN=1;
    T1CONbits.T1OSCEN=1;
    Write_RTC();
    lcd_init();
    lcd_comand(0b00001100);