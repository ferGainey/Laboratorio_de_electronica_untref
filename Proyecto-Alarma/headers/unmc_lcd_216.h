/*********************************************************************
 *
 *                Unimic 01 Version 1.1   10 September 2015
 *
 ***********************************************************************
 * FileName:        unmc_lcd_01.h
 * Dependencies:
 * Processor:       PIC18F26J50
 * Compiler:        XC8 1.34
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * THIS SOFTWARE IS PROVIDED IN AN ï¿½AS ISï¿½ CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 ***********************************************************************/
#include <p18cxxx.h>
#include <delays.h>

#ifndef _FLEX_LCD_H
#define _FLEX_LCD_H


#define FCY 8000000UL
#define _delay_100us() Delay100TCYx(FCY/4000000UL)
#define _delay_5ms() Delay1KTCYx(FCY/800000UL)

/* ** Definicion de tipo de variables **/
typedef unsigned char       int8;
typedef unsigned int		int16;
typedef unsigned long	    int32;
/* ** No olvidarse de short long de 24-bits ** */


/* ** Para no utilizar el PIN RW comentar la siguiente definición ** */
//#define USE_RW //modificamos (para la student)


#define DATA_PIN_7 	LATBbits.LATB7 //modificamos desde aca
#define DATA_PIN_6 	LATBbits.LATB6
#define DATA_PIN_5 	LATBbits.LATB5
#define DATA_PIN_4 	LATBbits.LATB4
	
#define READ_PIN_7	PORTBbits.RB7
#define READ_PIN_6	PORTBbits.RB6
#define READ_PIN_5	PORTBbits.RB5
#define READ_PIN_4	PORTBbits.RB4
	
	
#define TRIS_DATA_PIN_7       TRISBbits.TRISB7
#define TRIS_DATA_PIN_6       TRISBbits.TRISB6
#define TRIS_DATA_PIN_5       TRISBbits.TRISB5
#define TRIS_DATA_PIN_4       TRISBbits.TRISB4

//#define TRISBbits.TRISB2=0;
//#define TRISBbits.TRISB3=0;
	
#define EPIN	LATBbits.LATB2 //modificamos
#define RSPIN	LATBbits.LATB3 //modificamos
#ifdef USE_RW
	#define RWPIN	LATDbits.LATD5 
#endif
	
#define TRISEN	TRISBbits.TRISB2 //modificamos
#define TRISRS	TRISBbits.TRISB3 //modificamos
#ifdef USE_RW 
	#define TRISRW	TRISDbits.TRISD5 
#endif 


#define lcd_type 2 // 0=5x7, 1=5x10, 2=2 lines
#define lcd_line_two 0x40 // LCD RAM address for the 2nd line

/*;--- Modos de Entrada
; Incrementa Direccion, Display fijo	0x06
; Decrementa Direccion, Display fijo	0x04
; Incrementa Direccion, Cursor fijo		0x07
; Decrementa Direccion, Cursor fijo		0x05
;--- Corriemiento Cursor
; Cursor a la Izquierda					0x10
; Cursor a la Derecha					0x14
; Display a la Izquierda				0x18
; Display a la Derecha					0x1C
;--- Fijar Sistema
; Bus 8 bits, 1 línea, 5x7				0x30
; Bus 8 bits, 1 linea, 5x10				0x34
; Bus 8 bits, 2 líneas, 5x7				0x38
; Bus 4 bits, 1 línea, 5x7				0x20	
; Bus 4 bits, 1 linea, 5x10				0x24
; Bus 4 bits, 2 líneas, 5x7 			0x28
*/
#endif

void lcd_send_nibble(int8 nibble);//internas (no se usan
int8 lcd_read_nibble(void);
int8 lcd_read_byte(void);
void lcd_send_byte(int8 address, int8 n);

void lcd_init(void);//inicializa el lcd
void lcd_gotoxy(int8 x, int8 y);//posiciona caracter,linea
void lcd_putc(int8 c);  //escribe 1 caracter en el cursor
void lcd_comand(int8 d); //comando
void lcd_setcursor_vb(int8 visible, int8 blink);
void lcd_puts(char *ptr);//envia un string de la RAM al lcd
void lcd_putrs(const char *ptr);//envia un string de la ROM al lcd




