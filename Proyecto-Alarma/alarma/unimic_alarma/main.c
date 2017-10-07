#include <xc.h>
#include <delays.h>
#include <stdio.h>
#include <unmc_lcd_216.h>
#include <unmc_rtcc_01.h>
#include <unmc_config_01.h>
#include <unmc_inout_02.h>
#include <unmc_inout_03.h>

#include<plib/rtcc.h>

rtccTimeDate RtccTimeDate, RtccAlrmTimeDate, Rtcc_read_TimeDate;

rtccTime RtccTime; // Inicializa la estructura de tiempo
rtccTime RtccTimeVal;
rtccDate RtccDate; //Inicializa la estructura de Fecha
char* array_seleccionar[] = {" Dia", " Mes", "Anio", "Hora", " Min", " Seg"};
int array_contrasenia_ingresada[4];
int array_contrasenia[] = {1, 2, 3, 4};
int numeros_ingresados = 0;

int txt_insert_pass = 0;

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

void setRelojDigital(void) {
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

void ingreso_numero(int* num) {
    array_contrasenia_ingresada[numeros_ingresados] = num;
    lcd_gotoxy(numeros_ingresados + 1, 2);
    sprintf(buffer2, "%01u", num);
    lcd_putrs(buffer2);
    numeros_ingresados++;
    while (column1 == 1) {
        __delay_ms(40);
    }
}

void guarda_numero_ingresado(void) {
    row1 = 1;
    row2 = 0;
    row3 = 0;
    row4 = 0;
    {
        if (column1 == 1 && row1 == 1) {
            key = 1;
            ingreso_numero(key);
        }
        if (column2 == 1 && row1 == 1) {
            key = 2;
            ingreso_numero(key);
        }
        if (column3 == 1 && row1 == 1) {
            key = 3;
            ingreso_numero(key);
        }
    }
    row1 = 0;
    row2 = 1;
    row3 = 0;
    row4 = 0;
    {
        if (column1 == 1 && row2 == 1) {
            key = 4;
            ingreso_numero(key);
        }
        if (column2 == 1 && row2 == 1) {
            key = 5;
            ingreso_numero(key);
        }
        if (column3 == 1 && row2 == 1) {
            key = 6;
            ingreso_numero(key);
        }
    }
    row1 = 0;
    row2 = 0;
    row3 = 1;
    row4 = 0;
    {
        if (column1 == 1 && row3 == 1) {
            key = 7;
            ingreso_numero(key);
        }
        if (column2 == 1 && row3 == 1) {
            key = 8;
            ingreso_numero(key);
        }
        if (column3 == 1 && row3 == 1) {
            key = 9;
            ingreso_numero(key);
        }
    }
    row1 = 0;
    row2 = 0;
    row3 = 0;
    row4 = 1;

    if (column2 == 1 && row4 == 1) {
        key = 0;
        ingreso_numero(key);
    }

}

void pantalla_contrasenia(void) {

    lcd_gotoxy(1, 1);
    lcd_putrs("Insert Password ");
    lcd_gotoxy(1, 2);
    lcd_putrs("                ");

    while (1) {
        if (numeros_ingresados < 4) {
            guarda_numero_ingresado();
            row1 = 0;
            row2 = 0;
            row3 = 0;
            row4 = 1;

            if (column1 == 1 && row4 == 1) {
                setRelojDigital();
                break;
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

            pantalla_contrasenia();
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

            pantalla_contrasenia();
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

            pantalla_contrasenia();
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

            pantalla_contrasenia();
        }
    }
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

    setRelojDigital();
    Write_RTC();
}

int main(void) {
    setup();

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
    int boton_centro_estado = 0;

    while (1) {
        //lo saque de los del profe
        lcd_comand(0b00000010);
        LED_2_Toggle;
        LED_3_Toggle
        __delay_ms(98);

        Read_RTC();

        //esto de aca abajo se usaria solo si quiero editar
        //cambiar los botones switch y eso con lo respectivo de la alarma

        titila_texto_insert_password();
        si_apreta_numero_va_a_pantalla_contrasenia();

        if (boton_centro_estado == 1) {

            if (switch_Right == 0) {
                if (boton_seleccionar == 7) {
                    boton_seleccionar = 0;
                } else {
                    boton_seleccionar++;
                }
                while (switch_Right == 0);
            }

            if (switch_Left == 0) {
                if (boton_seleccionar == 0) {
                    boton_seleccionar = 7;
                } else {
                    boton_seleccionar--;
                }
                while (switch_Left == 0);
            }
            //imprime lo que estamos editando
            sprintf(buffer2, "%s", array_seleccionar[boton_seleccionar]);
            lcd_gotoxy(13, 2);
            lcd_putrs(buffer2);

            // Modificar Dia semana
            if (boton_seleccionar == 0) {

                if (switch_Up == 0) {
                    if (diasem == 6) {
                        diasem = 0;
                        Write_RTC();
                    } else {
                        diasem++;
                        Write_RTC();
                    }
                    while (switch_Up == 0);
                }

                if (switch_Down == 0) {
                    if (diasem == 0) {
                        diasem = 6;
                        Write_RTC();
                    } else {
                        diasem--;
                        Write_RTC();
                    }
                    while (switch_Down == 0);
                }
            }

            //Modificar dia
            if (boton_seleccionar == 1) {

                if (switch_Up == 0) {
                    if (dia == 31) {
                        dia = 1;
                        Write_RTC();
                    } else {
                        dia++;
                        Write_RTC();
                    }
                    while (switch_Up == 0);
                }

                if (switch_Down == 0) {
                    if (dia == 1) {
                        dia = 31;
                        Write_RTC();
                    } else {
                        dia--;
                        Write_RTC();
                    }
                    while (switch_Down == 0);
                }

                __delay_ms(50);
            }

            //Modificar mes
            if (boton_seleccionar == 2) {

                if (switch_Up == 0) {
                    if (mes == 12) {
                        mes = 1;
                        Write_RTC();
                    } else {
                        mes++;
                        Write_RTC();
                    }
                    while (switch_Up == 0);
                }

                if (switch_Down == 0) {
                    if (mes == 1) {
                        mes = 12;
                        Write_RTC();
                    } else {
                        mes--;
                        Write_RTC();
                    }
                    while (switch_Down == 0);
                }

                __delay_ms(50);
            }

            //Modificar anio
            if (boton_seleccionar == 3) {

                if (switch_Up == 0) {
                    if (anio == 99) {
                        anio = 0;
                        Write_RTC();
                    } else {
                        anio++;
                        Write_RTC();
                    }
                    while (switch_Up == 0);
                }

                if (switch_Down == 0) {
                    if (anio == 0) {
                        anio = 99;
                        Write_RTC();
                    } else {
                        anio--;
                        Write_RTC();
                    }
                    while (switch_Down == 0);
                }

                __delay_ms(50);
            }

            //Modificar hora
            if (boton_seleccionar == 4) {

                if (switch_Up == 0) {
                    if (hora == 59) {
                        hora = 0;
                        Write_RTC();
                    } else {
                        hora++;
                        Write_RTC();
                    }
                    while (switch_Up == 0);
                }

                if (switch_Down == 0) {
                    if (hora == 0) {
                        hora = 59;
                        Write_RTC();
                    } else {
                        hora--;
                        Write_RTC();
                    }
                    while (switch_Down == 0);
                }

                __delay_ms(50);
            }

            //Modificar minuto
            if (boton_seleccionar == 5) {

                if (switch_Up == 0) {
                    if (minuto == 59) {
                        minuto = 0;
                        Write_RTC();
                    } else {
                        minuto++;
                        Write_RTC();
                    }
                    while (switch_Up == 0);
                }

                if (switch_Down == 0) {
                    if (minuto == 0) {
                        minuto = 59;
                        Write_RTC();
                    } else {
                        minuto--;
                        Write_RTC();
                    }
                    while (switch_Down == 0);
                }

                __delay_ms(50);
            }

            //Modificar segundo
            if (boton_seleccionar == 6) {

                if (switch_Up == 0) {
                    if (segundo == 59) {
                        segundo = 0;
                        Write_RTC();
                    } else {
                        segundo++;
                        Write_RTC();
                    }
                    while (switch_Up == 0);
                }

                if (switch_Down == 0) {
                    if (segundo == 0) {
                        segundo = 59;
                        Write_RTC();
                    } else {
                        segundo--;
                        Write_RTC();
                    }
                    while (switch_Down == 0);
                }

                __delay_ms(50);
            }

            //parte del cafe
            /**
            if (boton_seleccionar == 7) {
                
                boton_seleccionar = 0;
                int boton_seleccionar_cafe = 0;
                int tipo = 0; //0=cortado, 1=solo, 2=c/leche
                int tamanio = 0; //0=chico, 1=mediano, 2=grande
                int accion = 0; //0=pedir, 1=terminar
                
                while (1) {
                    
                    if (switch_Right == 0) {
                        if (boton_seleccionar_cafe == 2) {
                            boton_seleccionar_cafe = 0;
                        } else {
                            boton_seleccionar_cafe++;
                        }
                        while (switch_Right == 0);
                    }

                    if (switch_Left == 0) {
                        if (boton_seleccionar_cafe == 0) {
                            boton_seleccionar_cafe = 2;
                        } else {
                            boton_seleccionar_cafe--;
                        }
                        while (switch_Left == 0);
                    }
                    
                    if (boton_seleccionar_cafe == 0) {

                        if (switch_Up == 0) {
                            if (tipo == 2) {
                                tipo = 0;
                            } else {
                                tipo++;
                                //ToDo = escribir en pantalla
                            }
                            while (switch_Up == 0);
                        }


                        if (switch_Down == 0) {
                            if (tipo == 0) {
                                tipo = 2;
                                //ToDo = escribir en pantalla
                            } else {
                                tipo--;
                                //ToDo = escribir en pantalla
                            }
                            while (switch_Down == 0);
                        }
                    }
                    
                    if (boton_seleccionar_cafe == 1) {

                        if (switch_Up == 0) {
                            if (tamanio == 2) {
                                tamanio = 0;
                            } else {
                                tamanio++;
                            }
                            while (switch_Up == 0);
                        }

                        if (switch_Down == 0) {
                            if (tamanio == 0) {
                                tamanio = 2;
                            } else {
                                tamanio--;
                            }
                            while (switch_Down == 0);
                        }
                    }
                    
                    if (boton_seleccionar_cafe == 2) {
                        if (switch_Up == 0) {
                            if (accion == 1) {
                                accion = 0;
                            } else {
                                accion++;
                            }
                            while (switch_Up == 0);
                        }

                        if (switch_Down == 0) {
                            if (accion == 0) {
                                accion = 1;
                            } else {
                                accion--;
                            }
                            while (switch_Down == 0);
                        }
                        
                        if (switch_Center == 0) {
                            if (accion == 0) {
                                // Guarda variables globales para los tipos/tamaños de cafe
                                reporte_cafe[tipo][tamanio] ++;
                               
                            }
                            
                            if (accion == 1) {
                                int a = 0;
                                sprintf(buffer2, "%01u", reporte_cafe[0][0]);
                                lcd_gotoxy(13, 2);
                                lcd_putrs(buffer2);                                
                                //break;
                            }
                            
                            if (accion == 2) {
                                break;
                            }
                            
                            while (switch_Center == 1);
                        }
                    }

                    //aca abajo imprimimos lo del cafe
                    sprintf(buffer3, "%s", array_boton_seleccionar_tipo_cafe[tipo]);
                    lcd_gotoxy(1, 1);
                    lcd_putrs(buffer3);

                    sprintf(buffer4, "%s", array_boton_seleccionar_tamanio_cafe[tamanio]);
                    lcd_gotoxy(9, 1);
                    lcd_putrs(buffer4);

                    sprintf(buffer5, "%s", array_boton_seleccionar_accion_cafe[accion]);
                    lcd_gotoxy(1, 2);
                    lcd_putrs(buffer5);

                }
             
            }
             */
        }

        setRelojDigital();
        __delay_ms(98); // 100ms retardo maximo para esta funcion

    }
    return 0;
}
