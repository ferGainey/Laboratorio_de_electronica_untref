#include <xc.h>
#include <delays.h>
#include <stdio.h>
#include <unmc_lcd_216.h>
#include <unmc_rtcc_01.h>
#include <unmc_config_01.h>
#include <unmc_inout_02.h>
#include <unmc_inout_03.h>
#include <math.h>

#include<plib/rtcc.h>

rtccTimeDate RtccTimeDate, RtccAlrmTimeDate, Rtcc_read_TimeDate;

rtccTime RtccTime; // Inicializa la estructura de tiempo
rtccTime RtccTimeVal;
rtccDate RtccDate; //Inicializa la estructura de Fecha
char* array_seleccionar[] = {" Dia", " Mes", "Anio", "Hora", " Min", " Seg"};
int array_contrasenia_ingresada[4];
int array_contrasenia[] = {1, 2, 3, 4};
int numeros_ingresados = 0;
int motivo_ingreso_de_numeros = 0; //0=contrasenia, 1=edicion hora o fecha. Por defecto va a ser 0
int numeros_para_fecha[2];

/**
 * Boton Seleccionar
 * 0. Dia
 * 1. Mes
 * 2. Año
 * 3. Hora
 * 4. Minuto
 * 5. Segundo
 */
int boton_seleccionar = 0;
//int boton_centro_estado = 0;

int txt_insert_pass = 0;

//Pongo las firmas de los metodos
void setDiaMesAnio(void);
void setHoraMinutoSegundo(void);
void set_reloj_digital(void);
void titila_texto_insert_password(void);
void guarda_numero_ingresado(int* num);
void ingreso_numero(void);
void mostrar_presionar_a(void);
int verificar_contrasenia();
int covertir_valores_de_array_a_escala_decimal();
void editar_fecha_reloj_digital(void);
void editar_hora_reloj_digital(void);
void resetear_pantalla_ingresar_contrasenia(void);
void ir_a_pantalla_ingresar_contrasenia(void);
void cambiar_opcion(void);
void seleccionar_opcion(void);
void ir_a_pantalla_editar_contrasenia(void);
void ir_a_pantalla_edicion(void);
void ir_a_pantalla_inicial(void);
void si_apreta_numero_va_a_pantalla_contrasenia(void);
void activar_alarma(void);
void setup(void);

void setDiaMesAnio(void) {
    if (dia > 31) {
        dia = 1;
        Write_RTC();
    }

    if (mes > 12) {
        mes = 1;
        Write_RTC();
    }

    sprintf(buffer2, "%02u/%02u/%02u", dia, mes, anio);
    lcd_gotoxy(9, 1);
    lcd_putrs(buffer2);
}

void setHoraMinutoSegundo(void) {
    if (hora >= 24) {
        hora = 0;
        minuto = 0;
        segundo = 0;
        Write_RTC();
    }

    sprintf(buffer2, "%02u:%02u:%02u", hora, minuto, segundo);
    lcd_gotoxy(9, 2);
    lcd_putrs(buffer2);
}

void set_reloj_digital(void) {
    setDiaMesAnio();
    setHoraMinutoSegundo();
}

void titila_texto_insert_password(void) {
    if (txt_insert_pass == 0) {
        txt_insert_pass = 1;
        lcd_gotoxy(1, 1);
        lcd_putrs("Insert");
        lcd_gotoxy(1, 2);
        lcd_putrs("Password");
        __delay_ms(80);
    }

    if (txt_insert_pass == 1) {
        txt_insert_pass = 0;
        lcd_gotoxy(1, 1);
        lcd_putrs("        ");
        lcd_gotoxy(1, 2);
        lcd_putrs("        ");
        __delay_ms(60);
    }
}

void guarda_numero_ingresado(int* num) {
    if (motivo_ingreso_de_numeros == 0) {
        array_contrasenia_ingresada[numeros_ingresados] = num;
        lcd_gotoxy(numeros_ingresados + 1, 2);
        sprintf(buffer2, "%01u", num);
        lcd_putrs(buffer2);
        numeros_ingresados++;
    } else {
        numeros_para_fecha[numeros_ingresados] = num;
        numeros_ingresados++;
    }
}

void ingreso_numero(void) {
    row1 = 1;
    row2 = 0;
    row3 = 0;
    row4 = 0;
    {
        if (column1 == 1 && row1 == 1) {
            key = 1;
            guarda_numero_ingresado(key);
            while (column1 == 1 && row1 == 1) {
                __delay_ms(40);
            }
        }
        if (column2 == 1 && row1 == 1) {
            key = 2;
            guarda_numero_ingresado(key);
            while (column2 == 1 && row1 == 1) {
                __delay_ms(40);
            }
        }
        if (column3 == 1 && row1 == 1) {
            key = 3;
            guarda_numero_ingresado(key);
            while (column3 == 1 && row1 == 1) {
                __delay_ms(40);
            }
        }
    }
    row1 = 0;
    row2 = 1;
    row3 = 0;
    row4 = 0;
    {
        if (column1 == 1 && row2 == 1) {
            key = 4;
            guarda_numero_ingresado(key);
            while (column1 == 1 && row2 == 1) {
                __delay_ms(40);
            }
        }
        if (column2 == 1 && row2 == 1) {
            key = 5;
            guarda_numero_ingresado(key);
            while (column2 == 1 && row2 == 1) {
                __delay_ms(40);
            }
        }
        if (column3 == 1 && row2 == 1) {
            key = 6;
            guarda_numero_ingresado(key);
            while (column3 == 1 && row2 == 1) {
                __delay_ms(40);
            }
        }
    }
    row1 = 0;
    row2 = 0;
    row3 = 1;
    row4 = 0;
    {
        if (column1 == 1 && row3 == 1) {
            key = 7;
            guarda_numero_ingresado(key);
            while (column1 == 1 && row3 == 1) {
                __delay_ms(40);
            }
        }
        if (column2 == 1 && row3 == 1) {
            key = 8;
            guarda_numero_ingresado(key);
            while (column2 == 1 && row3 == 1) {
                __delay_ms(40);
            }
        }
        if (column3 == 1 && row3 == 1) {
            key = 9;
            guarda_numero_ingresado(key);
            while (column3 == 1 && row3 == 1) {
                __delay_ms(40);
            }
        }
    }
    row1 = 0;
    row2 = 0;
    row3 = 0;
    row4 = 1;

    if (column2 == 1 && row4 == 1) {
        key = 0;
        guarda_numero_ingresado(key);
        while (column2 == 1 && row4 == 1) {
            __delay_ms(40);
        }
    }
}

void mostrar_presionar_a(void) {
    lcd_gotoxy(7, 2);
    lcd_putrs("Press A");
}

void resetear_pantalla_ingresar_contrasenia(void) {
    lcd_gotoxy(1, 2);
    lcd_putrs("              ");

    for (int i = 0; i < 0; i++) {
        array_contrasenia_ingresada[i] = 0;
    }

    numeros_ingresados = 0;
}

void cambiar_opcion(void) {
    row1 = 0;
    row2 = 0;
    row3 = 1;
    row4 = 0;
    if (column4 == 1 && row3 == 1) {
        if (boton_seleccionar == 3) {
            boton_seleccionar = 0;
        } else {
            boton_seleccionar++;
        }
        while (column4 == 1 && row3 == 1) {
            __delay_ms(40);
        }
    }

    row1 = 0;
    row2 = 0;
    row3 = 0;
    row4 = 1;
    if (column4 == 1 && row4 == 1) {
        if (boton_seleccionar == 0) {
            boton_seleccionar = 3;
        } else {
            boton_seleccionar--;
        }
        while (column4 == 1 && row4 == 1) {
            __delay_ms(40);
        }
    }

    sprintf(buffer2, "%01u", boton_seleccionar);
    lcd_gotoxy(8, 1);
    lcd_putrs(buffer2);
}

void activar_alarma(void) {
    numeros_ingresados = 0;
    lcd_gotoxy(1, 1);
    lcd_putrs("Alarm activated ");
    lcd_gotoxy(1, 2);
    lcd_putrs("                ");

    while (1) {
        //se tiene que quedar escuchando los sensores y el ingreso de numero
        break;
    }

    //Si se activa un sensor, pasa esto
    if (1 != 1) {
        //lo saque de los del profe
        lcd_comand(0b00000010);
        LED_2_Toggle;
        LED_3_Toggle
        __delay_ms(98);
    }


}

void seleccionar_opcion(void) {
    row1 = 1;
    row2 = 0;
    row3 = 0;
    row4 = 0;
    if (column4 == 1 && row1 == 1) {
        if (boton_seleccionar == 0) {
            activar_alarma();
        }
        if (boton_seleccionar == 1) {
            ir_a_pantalla_editar_contrasenia();
        }
        if (boton_seleccionar == 2) {
            editar_fecha_reloj_digital();
        }
        if (boton_seleccionar == 3) {
            editar_hora_reloj_digital();
        }
    }
}

void ir_a_pantalla_edicion(void) {
    while (1) {
        lcd_gotoxy(1, 1);
        lcd_putrs("Active ");
        lcd_gotoxy(1, 2);
        lcd_putrs("Edt Pass");

        Read_RTC();

        set_reloj_digital();
        cambiar_opcion();
        seleccionar_opcion();
    }
}

void ir_a_pantalla_ingresar_contrasenia(void) {

    numeros_ingresados = 0;
    lcd_gotoxy(1, 1);
    lcd_putrs("Insert Password ");
    lcd_gotoxy(1, 2);
    lcd_putrs("                ");

    while (1) {
        if (numeros_ingresados < 4) {
            ingreso_numero();
        } else {
            mostrar_presionar_a();
            if (verificar_contrasenia() == 1) {
                ir_a_pantalla_edicion();
            } else {
                resetear_pantalla_ingresar_contrasenia();
            }
        }
    }
}

void si_apreta_numero_va_a_pantalla_contrasenia(void) {
    row1 = 1;
    row2 = 0;
    row3 = 0;
    row4 = 0;
    {
        if ((column1 == 1 && row1 == 1) ||
                (column2 == 1 && row1 == 1) ||
                (column3 == 1 && row1 == 1)) {

            ir_a_pantalla_ingresar_contrasenia();
        }
    }
    row1 = 0;
    row2 = 1;
    row3 = 0;
    row4 = 0;
    {
        if ((column1 == 1 && row2 == 1) ||
                (column2 == 1 && row2 == 1) ||
                (column3 == 1 && row2 == 1)) {

            ir_a_pantalla_ingresar_contrasenia();
        }
    }
    row1 = 0;
    row2 = 0;
    row3 = 1;
    row4 = 0;
    {
        if ((column1 == 1 && row3 == 1) ||
                (column2 == 1 && row3 == 1) ||
                (column3 == 1 && row3 == 1)) {

            ir_a_pantalla_ingresar_contrasenia();
        }
    }
    row1 = 0;
    row2 = 0;
    row3 = 0;
    row4 = 1;
    {
        if ((column1 == 1 && row4 == 1) ||
                (column2 == 1 && row4 == 1) ||
                (column3 == 1 && row4 == 1)) {

            ir_a_pantalla_ingresar_contrasenia();
        }
    }
}

void ir_a_pantalla_inicial() {
    set_reloj_digital();
    titila_texto_insert_password();
    si_apreta_numero_va_a_pantalla_contrasenia();
}

void ir_a_pantalla_editar_contrasenia(void) {
    numeros_ingresados = 0;
    lcd_gotoxy(1, 1);
    lcd_putrs("Insert new Password ");
    lcd_gotoxy(1, 2);
    lcd_putrs("                ");
    while (1) {
        if (numeros_ingresados < 4) {
            ingreso_numero();
        } else {
            for (int i = 0; i < 4; i++) {
                array_contrasenia[i] = array_contrasenia_ingresada[i];
            }
            break;
        }
    }
}

int verificar_contrasenia() {
    int equals = 0;
    for (int i = 0; i < 4; i++) {
        if (array_contrasenia[i] == array_contrasenia_ingresada[i]) {
            equals = 1;
        } else {
            equals = 0;
        }
    }
    return equals;
}

int covertir_valores_de_array_a_escala_decimal() {
    int numero = 0;
    for (int i = 2 - 1; i >= 0; i--) {
        numero = numero + numeros_para_fecha[i] * (int) pow(10, (2 - 1) - i);
    };
    sprintf(buffer2, "%02u", numero);
    lcd_gotoxy(8, 2);
    lcd_putrs(buffer2);
    return numero;
}

void editar_fecha_reloj_digital(void) {
    int posicion = 0;
    //int numeros_para_fecha[2]; //lo saque para afuera así reutilizo codigo de ingreso de numeros
    motivo_ingreso_de_numeros = 1;

    //Modificar dia, se puede limpiar la posicion
    if (posicion == 0) {
        numeros_ingresados = 0;
        int dia_ingresado = 0;
        while (numeros_ingresados < 3) {
            //numeros_para_fecha[numeros_ingresados] = 1; //puse 1 por defecto, despues hay que tomar lo que ingrese
            ingreso_numero();
            //numeros_ingresados++;
            if (numeros_ingresados == 2) {
                dia_ingresado = covertir_valores_de_array_a_escala_decimal(); 
                if ((0 < dia_ingresado) && (dia_ingresado <= 31)) {
                    dia = dia_ingresado;
                    numeros_ingresados++;
                } else {
                    numeros_ingresados = 0;
                }
            }
        }
        //Write_RTC();
        setDiaMesAnio();
        posicion++;
        __delay_ms(50);
    }

    //Modificar mes
    if (posicion == 1) {
        numeros_ingresados = 0;
        int mes_ingresado = 0;
        while (numeros_ingresados < 3) {
            ingreso_numero();
            if (numeros_ingresados == 2) {
                mes_ingresado = covertir_valores_de_array_a_escala_decimal(); 
                if ((0 < mes_ingresado) && (mes_ingresado <= 12)) {
                    mes = mes_ingresado;
                    numeros_ingresados++;
                } else {
                    numeros_ingresados = 0;
                }
            }
        }
        //Write_RTC();
        setDiaMesAnio();
        posicion++;
        __delay_ms(50);
    }

    //Modificar anio
    if (posicion == 2) {
        numeros_ingresados = 0;
        int anio_ingresado = 0;
        while (numeros_ingresados < 3) {
            ingreso_numero();
            if (numeros_ingresados == 2) {
                anio_ingresado = covertir_valores_de_array_a_escala_decimal();
                if ((0 <= anio_ingresado) && (anio_ingresado <= 99)) {
                    anio = anio_ingresado;
                    numeros_ingresados++;
                } else {
                    numeros_ingresados = 0;
                }
            }
        }
        //Write_RTC();
        setDiaMesAnio();
        posicion++;
        __delay_ms(50);
    }
    Write_RTC();
    motivo_ingreso_de_numeros = 0; //vuelve al motivo por defecto que es el de la contrasenia
    numeros_ingresados = 0;
}

void editar_hora_reloj_digital(void) {
    int posicion = 0;
    motivo_ingreso_de_numeros = 1;
    
    //Modificar hora
    if (posicion == 0) {
        numeros_ingresados = 0;
        int hora_ingresada = 0;
        while(numeros_ingresados < 3){
            ingreso_numero();
          if(numeros_ingresados == 2){
            hora_ingresada = covertir_valores_de_array_a_escala_decimal();
            if((0 <= hora_ingresada) && (hora_ingresada <= 23)){
                hora=hora_ingresada;
                numeros_ingresados++;
            }
            else{
                numeros_ingresados = 0;
            }
          }
        } 
        //Write_RTC();
        setHoraMinutoSegundo();
        posicion++;
        __delay_ms(50);
    }

    //Modificar minuto
    if (posicion == 1) {
        numeros_ingresados = 0;
        int minuto_ingresado = 0;
        while(numeros_ingresados < 3){
            ingreso_numero();
          if(numeros_ingresados == 2){
            minuto_ingresado = covertir_valores_de_array_a_escala_decimal();
            if((0 <= minuto_ingresado) && (minuto_ingresado <= 59)){
                minuto = minuto_ingresado;
                numeros_ingresados++;
            }
            else{
                numeros_ingresados = 0;
            }
          }
        } 
        //Write_RTC();
        setHoraMinutoSegundo();
        posicion++;
        __delay_ms(50);
    }

    //Modificar segundo
    if (posicion == 2) {
        numeros_ingresados = 0;
        int segundo_ingresado = 0;
        while(numeros_ingresados < 3){
            ingreso_numero();
          if(numeros_ingresados == 2){
            segundo_ingresado = covertir_valores_de_array_a_escala_decimal();
            if((0 <= segundo_ingresado) && (segundo_ingresado <= 59)){
                segundo=segundo_ingresado;
                numeros_ingresados++;
            }
            else{
                numeros_ingresados = 0;
            }
          }
        } 
        //Write_RTC();
        setHoraMinutoSegundo();
        posicion++;
        __delay_ms(50);
    }
    Write_RTC();
    motivo_ingreso_de_numeros = 0; //vuelve al motivo por defecto que es el de la contrasenia
    numeros_ingresados = 0;
}

void setup(void) {
    OSCTUNEbits.INTSRC = 1; //setea el oscilador de 32768 para el RTC
    OSCTUNEbits.PLLEN = 0; //desactiva PLL
    OSCCONbits.IRCF0 = 1; //selecciona el clock en 8MHz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS0 = 0; //oscilator INTRC
    OSCCONbits.SCS1 = 0;
    TRISA = 0b11110000;
    TRISB = 0;
    TRISC = 0b00000111;
    //TRISAbits.TRISA0=1;
    //TRISBbits.TRISB0=0;
    //TRISCbits.TRISC0=0;
    ANCON0 = 0b11111111; // Config AN7 to AN0 Digital Ports
    //ANCON1=0b10010111;            // Config AN11 Analog Port
    ANCON1 = 0b11111111; // Config AN12 to AN8 Digital Ports
    //ADCON0=0b00101101;
    //ADCON1=0b00010000;            Sin Conversor AD
    RTCCFGbits.RTCEN = 1;
    RTCCFGbits.RTCWREN = 1;
    T1CONbits.T1OSCEN = 1;
    Write_RTC();
    lcd_init();
    lcd_comand(0b00001100); //Display=on / Cursor=off / Blink=off
    LED_1_Off;
    LED_2_Off;
    LED_3_Off;

    set_reloj_digital();
    Write_RTC();
}

int main(void) {
    setup();

    while (1) {

        Read_RTC();

        ir_a_pantalla_inicial();
    }

    __delay_ms(98); // 100ms retardo maximo para esta funcion
    return 0;
}
