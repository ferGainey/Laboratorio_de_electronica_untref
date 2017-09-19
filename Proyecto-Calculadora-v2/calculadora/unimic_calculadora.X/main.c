#include <xc.h>
#include <delays.h>
#include <stdio.h>
#include <unmc_lcd_216.h>
#include <unmc_rtcc_01.h>
#include <unmc_config_01.h>
#include <unmc_inout_04.h>
#include <math.h>


int flag_sentido_escritura = 0; // se reinicia con el boton C, cambia a 1 cuando ingresas el signo
int contador_numeros_ingresados_izq = 0; // se reinicia con el boton C, cambia a 0 cuando ingresas un signo
int contador_numeros_ingresados_der = 0;
int array_lcd_izq[6];
int array_lcd_der[6];
char* operador = "";



//Funcion Setup

void Setup(void) {
    OSCTUNEbits.INTSRC = 1; //setea el oscilador de 32768 para el RTC
    OSCTUNEbits.PLLEN = 0; //desactiva PLL
    OSCCONbits.IRCF0 = 1; //selecciona el clock en 8MHz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS0 = 0; //oscilator INTRC
    OSCCONbits.SCS1 = 0;
    TRISA = 0b00010111;
    TRISB = 0;
    TRISC = 0b00000111;
    INTCON2bits.RBPU = 1;
    ANCON0 = 0b11111111; // Config AN7 to AN0 Digital Ports
    ANCON0 = 0b10010111; // Config AN11 Analog Port
    ADCON0 = 0b00101101; // Control AN11 Analog Port
    ADCON1 = 0b00010000; // Config Analog Port
    //RTCCFGbits.RTCEN=1; No se utilizan
    //RTCCFGbits.RTCWREN=1; No se utilizan
    T1CONbits.T1OSCEN = 1;
    //Write_RTC();
    lcd_init();
    lcd_comand(0b00001100); //Display=on / Cursor=off / Blink=off
    LED_2_Off;
    LED_3_Off;
}

void write_in_lcd(int* key) { 
    sprintf(buffer2, "%u", key);//sprintf(buffer2, "%01u", key);
    lcd_putrs(buffer2);
}

void escribir_numero(int* num) {

    if (flag_sentido_escritura == 0) {
        if (contador_numeros_ingresados_izq < 6) {
            array_lcd_izq[contador_numeros_ingresados_izq] = num;
            contador_numeros_ingresados_izq++;
            lcd_gotoxy(7 - contador_numeros_ingresados_izq, 1);
            for (int i = 0; i < contador_numeros_ingresados_izq; i++) {
                write_in_lcd(array_lcd_izq[i]);
            };
        };
    } else {
        if (contador_numeros_ingresados_der < 6) {
            array_lcd_der[contador_numeros_ingresados_der] = num;
            contador_numeros_ingresados_der++;
            lcd_gotoxy(10, 1);
            for (int i = 0; i < contador_numeros_ingresados_der; i++) {
                write_in_lcd(array_lcd_der[i]);
            };
        };
    };
}

// Funcion main, funcion principal del programa

int main(void) {
    Setup();

    while (1) {

        // Numeros 7 8 9 %
        row1 = 1;
        row2 = 0;
        row3 = 0;
        row4 = 0;

        if (column1 == 1) {
            escribir_numero(7);
            while (column1 == 1) {
            };
        };

        if (column2 == 1) {
            escribir_numero(8);
            while (column2 == 1) {
            };
        };

        if (column3 == 1) {
            escribir_numero(9);
            while (column3 == 1) {
            };
        };

        if (column4 == 1) {
            lcd_gotoxy(8, 1);
            lcd_putrs("%");
            flag_sentido_escritura = 1;
            operador = "%";
            while (column4 == 1) {
            };
        };


        // Numeros 4 5 6 x
        row1 = 0;
        row2 = 1;
        row3 = 0;
        row4 = 0;

        if (column1 == 1) {
            escribir_numero(4);
            while (column1 == 1) {
            };
        };

        if (column2 == 1) {
            escribir_numero(5);
            while (column2 == 1) {
            };
        };

        if (column3 == 1) {
            escribir_numero(6);
            while (column3 == 1) {
            };
        };

        if (column4 == 1) {
            lcd_gotoxy(8, 1);
            lcd_putrs("x");
            flag_sentido_escritura = 1;
            operador = "x";
            while (column4 == 1) {
            };
        };


        // Numeros 1 2 3 -
        row1 = 0;
        row2 = 0;
        row3 = 1;
        row4 = 0;

        if (column1 == 1) {
            escribir_numero(1);
            while (column1 == 1) {
            };
        };

        if (column2 == 1) {
            escribir_numero(2);
            while (column2 == 1) {
            };
        };

        if (column3 == 1) {
            escribir_numero(3);
            while (column3 == 1) {
            };
        };

        if (column4 == 1) {
            lcd_gotoxy(8, 1);
            lcd_putrs("-");
            flag_sentido_escritura = 1;
            operador = "-";
            while (column4 == 1) {
            };
        };

        // Numeros c 0 = +
        row1 = 0;
        row2 = 0;
        row3 = 0;
        row4 = 1;

        if (column1 == 1) {
            //write_in_lcd(".");
            lcd_init();
            Setup();
            flag_sentido_escritura = 0;
            operador = "";
            contador_numeros_ingresados_der = 0;
            contador_numeros_ingresados_izq = 0;
            while (column1 == 1) {
            };
        };

        if (column2 == 1) {
            escribir_numero(0);
            while (column2 == 1) {
            };
        };

        if (column3 == 1) {
            int num_izq = 0;
                for (int i = contador_numeros_ingresados_izq - 1; i >= 0; i--) {
                    num_izq = num_izq + array_lcd_izq[i] * (int) pow(10, (contador_numeros_ingresados_izq - 1) - i);
                };
                int num_der = 0;//char num_der = "";
                for (int i = contador_numeros_ingresados_der - 1; i >= 0; i--) {
                    num_der = num_der + array_lcd_der[i] * (int) pow(10, (contador_numeros_ingresados_der - 1) - i);
                };
                lcd_gotoxy(1, 2);
            if (operador == "+") {           
                int resultado_suma = 0;
                resultado_suma = resultado_suma + num_izq + num_der;
                write_in_lcd(resultado_suma);
            };
            if (operador == "x") {
                int resultado_multiplicacion = 0;
                resultado_multiplicacion = resultado_multiplicacion + (int) (num_izq * num_der);
                write_in_lcd(resultado_multiplicacion);
            };
            if (operador == "%") {
                int resultado_division = 0;
                resultado_division = resultado_division + (num_izq / num_der);
                write_in_lcd(resultado_division);
            };
            if (operador == "-") {
                int resultado_resta = 0;
                if (num_der > num_izq) {
                    resultado_resta = num_der - num_izq;
                    lcd_puts("-");
                    lcd_gotoxy(2, 2);
                    write_in_lcd(resultado_resta);
                }
                else {
                    resultado_resta = resultado_resta + (num_izq - num_der);
                    write_in_lcd(resultado_resta);
                };
                
                
            };
            while (column3 == 1) {
            };
        };

        if (column4 == 1) {
            // Los operadores siempre se imprimen en la 
            //posicion (7,1) (Columna, Fila)
            lcd_gotoxy(8, 1);
            lcd_putrs("+");
            flag_sentido_escritura = 1;
            operador = "+";
            while (column4 == 1) {
            };
        };

        LED_2_Toggle;
        LED_3_Toggle;
        __delay_ms(98); // 100ms retardo maximo para esta funcion

    }
    return 0;
}
