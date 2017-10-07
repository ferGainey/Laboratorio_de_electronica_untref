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

#define _XTAL_FREQ 8000000
#pragma config CPUDIV=OSC1
#pragma config CP0=OFF
#pragma config WDTEN=OFF
#pragma config PLLDIV=1
#pragma config STVREN=ON
#pragma config XINST=OFF
#pragma config XINST=OFF
#pragma config OSC=INTOSC
#pragma config T1DIG=ON
#pragma config LPT1OSC=OFF
#pragma config FCMEN=OFF
#pragma config IESO=OFF
#pragma config WDTPS=32768
#pragma config DSWDTOSC=INTOSCREF
#pragma config RTCOSC=T1OSCREF      // RTCC Clock Select (RTCC uses T1OSC/T1CKI)
#pragma config DSBOREN=OFF
#pragma config DSWDTEN=OFF
#pragma config DSWDTPS=G2
#pragma config IOL1WAY=OFF
#pragma config MSSP7B_EN=MSK7
#pragma config WPCFG=OFF
#pragma config WPDIS=OFF
