/* 
 * File:   voltimetro.c
 * Author: marcos
 *
 * Created on 27 de Setembro de 2019, 09:33
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pic18f4520.h>
#include "configura.h"
#include "lcd.h"


#define _XTAL_FREQ 4000000
#define botao1 PORTCbits.RC1
#define botao2 PORTCbits.RC2
#define botao3 PORTCbits.RC3
#define botao4 PORTCbits.RC0

#define led_verde PORTBbits.RB0
#define led_amarelo PORTBbits.RB5
#define led_vermelho1 PORTBbits.RB6
#define led_vermelho2 PORTBbits.RB7

float volts=0;
int contador = 0, bits=0, temp=0;
char valor[5], bts[6];

void Read_a2d(void);
void escreve(void);
void envia(void);
void recebe(void);
    
void main(void) {
   
    config();
    lcd_init();
    lcd_puts ( " INICIANDO..");
    
//    led_amarelo != led_amarelo;
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    TMR0=6;
    while (1) {

       
        
        if (!botao1) {  // ACIONANDO BOTAO 3
            __delay_ms(5);
            if (!botao1) {
                while (!botao1);
                lcd_init();
                lcd_puts ( " BOTAO 1 APERTADO..");
//                LATA5 = !LATA5; // APAGA LUZ LCD
                envia();
            }
        }
        
        if (!botao2) {  // ACIONANDO BOTAO 3
            __delay_ms(5);
            if (!botao2) {
                while (!botao2);
                lcd_init();
                lcd_puts ( " BOTAO 2 APERTADO..");
//                LATA5 = !LATA5; // APAGA LUZ LCD
                envia();
            }
        }
        
        if (!botao3) {  // ACIONANDO BOTAO 3
            __delay_ms(5);
            if (!botao3) {
                while (!botao3);
                lcd_init();
                lcd_puts ( " BOTAO 3 APERTADO..");
//                LATA5 = !LATA5; // APAGA LUZ LCD
                envia();
            }
        }
        
        if (!botao4) {  // ACIONANDO BOTAO 3
            __delay_ms(5);
            if (!botao4) {
                while (!botao4);
                lcd_init();
                lcd_puts ( " BOTAO 4 APERTADO..");
//                LATA5 = !LATA5; // APAGA LUZ LCD
                envia();
            }
        }
        
        
        if (RCREG==0x42) // RECONHE QUANDO JAVA MANDA ALGO, NO CASO ENVIANDO B
        {
            RCIF=0;
            __delay_ms(300);
            led_amarelo=!led_amarelo;
            lcd_puts ( " RECEBENDO");
            RCREG=0;
        }

    }
}

void __interrupt(high_priority) tmr (void) // interrupcao
{
    
    if (TMR0IF) 
    {
        TMR0IF=0;
        contador++;
//        if(contador==250) // FICA PISCANDO LED VERDE >> EXEMPLO INTERRUPCAO
//        {
//            led_verde=!led_verde;
//            led_amarelo =! led_verde;
//            
//            contador=0;
////          Read_a2d();
//            
//        }
        TMR0=6;
    }
}

void Read_a2d(void)
{
    GODONE =1;
    while (GODONE);
        
    
    volts =(ADRESL + ADRESH*256);
    bits=volts;
    volts=volts*5/1023;
//    sprintf (bts, "%i", bits);
//    sprintf (valor, "%.2f", volts);
    escreve();
       
}

void escreve(void)
{
    
    lcd_clear();
    lcd_goto (0);
    lcd_puts (bts);
    lcd_puts ( " INICIANDO..");
    lcd_goto (40);
    lcd_puts (valor);
//    lcd_puts (" volts");
    
    
}

void envia(void) // MAS PRECISA CHAMAR O METODO 
{
//    sprintf (bts, "ENVIANDO%c",0x42);
    while(!TXIF)
    {}
    TXREG = 0x42;
}

