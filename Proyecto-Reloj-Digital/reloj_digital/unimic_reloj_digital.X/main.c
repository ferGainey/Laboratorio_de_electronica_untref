#include <xc.h>
#include <delays.h>
#include <stdio.h>
#include <unmc_lcd_216.h>
#include <unmc_rtcc_01.h>
#include <unmc_config_01.h>
#include <unmc_inout_03.h>

#include<plib/rtcc.h>

rtccTimeDate RtccTimeDate, RtccAlrmTimeDate, Rtcc_read_TimeDate;

rtccTime RtccTime; // Inicializa la estructura de tiempo
rtccTime RtccTimeVal;
rtccDate RtccDate; //Inicializa la estructura de Fecha

//Funcion Setup

void Setup(void) {
    OSCTUNEbits.INTSRC = 1; //setea el oscilador de 32768 para el RTC
    OSCTUNEbits.PLLEN = 0; //desactiva PLL
    OSCCONbits.IRCF0 = 1; //selecciona el clock en 8MHz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS0 = 0; //oscilator INTRC
    OSCCONbits.SCS1 = 0;
    TRISA = 0b11111111; //0b00010111;
    TRISB = 0;
    TRISC = 0b00000111;
    INTCON2bits.RBPU = 1;
    ANCON0 = 0b11111111; // Config AN7 to AN0 Digital Ports
    ANCON1 = 0b11111111; // Config AN11 Analog Port
    ADCON0 = 0b11111111; // Control AN11 Analog Port
    ADCON1 = 0b11111111; // Config Analog Port
    //RTCCFGbits.RTCEN=1; No se utilizan
    //RTCCFGbits.RTCWREN=1; No se utilizan
    T1CONbits.T1OSCEN = 1;
    //Write_RTC();
    lcd_init();
    lcd_comand(0b00001100); //Display=on / Cursor=off / Blink=off

}

void write_in_lcd(char* key) {
    sprintf(buffer2, "%01u", key);
    lcd_putrs(buffer2);
}

// Funcion main, funcion principal del programa

int main(void) {
    Setup();
    
    int boton_centro_estado = 0;
    int boton_seleccionar = 0;//el boton seleccionar no tiene 3 estados (nombre del dia, fecha, hora) como ahora, sino que hay que agregarle numero de dia, numero de mes, numero de año; hora, minuto, segundo

    while (1) {

        if (switch_Center == 0 && boton_centro_estado == 0) {
            boton_centro_estado = 1;
            lcd_gotoxy(14, 2);
            lcd_putrs("1");
            __delay_ms(98);
        }

        if (switch_Center == 0 && boton_centro_estado == 1) {
            boton_centro_estado = 0;
            lcd_gotoxy(14, 2);
            lcd_putrs("0");
            __delay_ms(98);
        }

        if (boton_centro_estado == 1) {

            if (switch_Right == 0) {
                if (boton_seleccionar == 2) {
                    boton_seleccionar = 0;
                    lcd_gotoxy(15, 2);
                    lcd_putrs("0");
                } else {
                    boton_seleccionar++;
                }
            }

            if (switch_Left == 0) {
                if (boton_seleccionar == 0) {
                    boton_seleccionar = 2;
                } else {
                    boton_seleccionar--;
                }
            }

            if (boton_seleccionar == 0) {
                if (switch_Up == 0) {
                    if (diasem == 6) {
                        diasem = 0;
                        Write_RTC();
                    } else {
                        diasem++;
                        Write_RTC();
                    }
                }

                if (switch_Down == 0) {
                    if (diasem == 0) {
                        diasem = 6;
                        Write_RTC();
                    } else {
                        diasem--;
                        Write_RTC();
                    }
                }
            }
        }

        if (diasem == 0) {
            lcd_gotoxy(1, 1);
            lcd_putrs("Domingo");
            __delay_ms(50);
        } else if (diasem == 1) {
            lcd_gotoxy(1, 1);
            lcd_putrs("Lunes  ");
            __delay_ms(50);
        } else if (diasem == 2) {
            lcd_gotoxy(1, 1);
            lcd_putrs("Martes ");
            __delay_ms(50);
        } else if (diasem == 3) {
            lcd_gotoxy(1, 1);
            lcd_putrs("Miercol");
            __delay_ms(50);
        } else if (diasem == 4) {
            lcd_gotoxy(1, 1);
            lcd_putrs("Jueves ");
            __delay_ms(50);
        } else if (diasem == 5) {
            lcd_gotoxy(1, 1);
            lcd_putrs("Viernes");
            __delay_ms(50);
        } else if (diasem == 6) {
            lcd_gotoxy(1, 1);
            lcd_putrs("Sabado ");
            __delay_ms(50);
        }

        __delay_ms(98); // 100ms retardo maximo para esta funcion

    }
    return 0;
}
