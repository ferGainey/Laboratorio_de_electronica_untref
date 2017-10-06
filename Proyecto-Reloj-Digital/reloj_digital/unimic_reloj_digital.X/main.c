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
char* array_seleccionar[] = {"Ndia", " Dia", " Mes", "Anio", "Hora", " Min", " Seg", "Cafe"};
char* array_boton_seleccionar_tipo_cafe[] = {"Cortado", "Solo   ", "C/Leche"};
char* array_boton_seleccionar_tamanio_cafe[] = {"Chico   ", "Mediano ", "Grande  "};
char* array_boton_seleccionar_accion_cafe[] = {"Pedir   ", "Terminar"};

//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/// Funcion Caratula
/// Display presentation day hour
/// variable lecture diasem, anio, dia, hora, etc
//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[

/* Eliminar
void caratula(void) {
    lcd_comand(0b00001100); //Enciende display sin cursor y sin blink  
    sprintf(buffer2, "%02u/%02u/%02u", dia, mes, anio);
    lcd_gotoxy(9, 1);
    lcd_putrs(buffer2);
    sprintf(buffer2, "%02u:%02u:%02u", hora, minuto, segundo);
    lcd_gotoxy(1, 2);
    lcd_putrs(buffer2);
}
 */

void setDiaSemana(void) {
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
}

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
    lcd_gotoxy(1, 2);
    lcd_putrs(buffer2);
}

void setRelojDigital(void) {
    setDiaSemana();
    setDiaMesAnio();
    setHoraMinutoSegundo();
}

//Funcion Setup

void setup(void) {
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
    ANCON1 = 0b10010111; // Config AN11 Analog Port
    ADCON0 = 0b00111111; // Control AN11 Analog Port
    ADCON1 = 0b11111111; // Config Analog Port
    //RTCCFGbits.RTCEN=1; No se utilizan
    //RTCCFGbits.RTCWREN=1; No se utilizan
    T1CONbits.T1OSCEN = 1;
    //Write_RTC();
    lcd_init();
    lcd_comand(0b00001100); //Display=on / Cursor=off / Blink=off

    setRelojDigital();
    Write_RTC();
}

// Funcion main, funcion principal del programa

int main(void) {
    setup();
    /*
     * Estado del boton centro
     * 0. Habilitado
     * 1. Deshabilitado
     */
    int boton_centro_estado = 0;

    /**
     * Boton Seleccionar
     * 0. Dia de semana
     * 1. Dia
     * 2. Mes
     * 3. Año
     * 4. Hora
     * 5. Minuto
     * 6. Segundo
     */
    int boton_seleccionar = 0;

    while (1) {

        Read_RTC();

        if (switch_Center == 0 && boton_centro_estado == 0) {
            boton_centro_estado = 1;
            lcd_gotoxy(10, 2);
            lcd_putrs("Edt");
            __delay_ms(98);
        }

        if (switch_Center == 0 && boton_centro_estado == 1) {
            boton_centro_estado = 0;
            lcd_gotoxy(10, 2);
            lcd_putrs("        ");
            __delay_ms(98);
        }

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
                                // Guardar variables globales para los tipos/tamaños de cafe
                                break;
                            }
                            
                            if (accion == 1) {
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


        }

        setRelojDigital();
        __delay_ms(98); // 100ms retardo maximo para esta funcion

    }
    return 0;
}
