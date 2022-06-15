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

float volts = 0;
int contador = 0, bits = 0, temp = 0, j = 0;
char valor[5], bts[6], rec[3];
__bit recep = 0;
void Read_a2d(void);
void escreve(void);
void envia(void);
void recebe(void);
void trata(char a);

 int enviaAberto;
  int enviaFechado;
  
  int abrirportao = 0;
  int fecharportao = 0;

void main(void) {
  config();
  lcd_init();
  lcd_puts("    PARADO..");
  
 
  
  PORTB = 0;
  PORTD = 0;
  PORTE = 0;
  TMR0 = 6;
  //    output_high(led_amarelo);
  led_amarelo = 1;
  while (1) {
    __delay_ms(30);  
    if (!botao1 && !fecharportao) {  // ACIONANDO BOTAO 1
        
      __delay_ms(5);
      lcd_init();
      
      lcd_puts(" PORTAO ABERTO!");
//      led_vermelho1 = 0;
      led_amarelo = 1;
      led_verde = 0;
      while (!TXIF) {
  }
      if (!enviaAberto) {
          TXREG = '1';
          enviaAberto = 1; 
      }

    }
    
    __delay_ms(30);
    if (!botao2 && !abrirportao) {  // ACIONANDO BOTAO 2
      __delay_ms(5);

      lcd_init();
      lcd_puts(" PORTAO FECHADO!");
      led_vermelho1 = 0;
      led_amarelo = 1;
//      led_verde = 0;
      while (!TXIF) {
  }
  if (!enviaFechado) {
          TXREG = '2';
          enviaFechado = 1; 
      }
    }

    if (!botao3) {  // ACIONANDO BOTAO 3
      __delay_ms(5);
      if (!botao3) {
        while (!botao3)
          ;

        led_vermelho1 = 0;
        led_amarelo = 1;
        led_verde = 0;
        lcd_init();
        lcd_puts("    PARADO..");
      }
    }
  }
}

void __interrupt(high_priority) tmr(void) {
  if (TMR0IF) {
    TMR0IF = 0;
    contador++;
    TMR0 = 6;
  }

  if (RCIF) {
    RCIF = 0;
    trata(RCREG);
    lcd_init();

    
    if (RCREG ==
        0x31)  // RECONHE QUANDO JAVA MANDA ALGO, NO CASO ENVIANDO 1 >> ABRIR
    {
 
      __delay_ms(30);

      if (!botao1) {
        lcd_init();
        lcd_puts("PORTAO JA ESTA");
        lcd_goto(40);
        lcd_puts("    ABERTO!");
        __delay_ms(3000);
      } else {
        lcd_init();
        lcd_puts("   ABRINDO..");
        led_vermelho1 = 0;
        led_amarelo = 0;
        led_verde = 1;
        RCREG = 0;
        abrirportao = 1;
        fecharportao = 0;
        
        enviaFechado = 0;
      }
    }

    if (RCREG == 0x32)  // RECONHE QUANDO JAVA MANDA ALGO, NO CASO ENVIANDO 2
    {
     
      __delay_ms(30);
      if (!botao2) {
        lcd_init();
        lcd_puts("PORTAO JA ESTA");
        lcd_goto(40);
        __delay_ms(3000);
        lcd_puts("   FECHADO!");
      } else {
        lcd_init();
        lcd_puts("    FECHANDO..");
        led_vermelho1 = 1;
        led_amarelo = 0;
        led_verde = 0;
        RCREG = 0;
        fecharportao = 1;
        abrirportao = 0;
        
        enviaAberto = 0;
      }
    }

    if (RCREG == 0x33)  // RECONHE QUANDO JAVA MANDA ALGO, NO CASO ENVIANDO 3
    {
 
      //printf("1gfg\n");
      __delay_ms(30);
      lcd_init();
      lcd_puts("   PARADO..");
      led_vermelho1 = 0;
//      led_amarelo = 1;
      led_verde = 0;
      RCREG = 0;
    }
  }
}

void Read_a2d(void) {
  GODONE = 1;
  while (GODONE)
    ;

  volts = (ADRESL + ADRESH * 256);
  bits = volts;
  volts = volts * 5 / 1023;
  //    sprintf (bts, "%i", bits);
  //    sprintf (valor, "%.2f", volts);
  escreve();
}

void escreve(void) {
  lcd_clear();
  lcd_goto(0);
  lcd_puts(bts);
  lcd_puts(" INICIANDO..");
  lcd_goto(40);
  lcd_puts(valor);
  //    lcd_puts (" volts");
}

void envia(void)  // MAS PRECISA CHAMAR O METODO
{
  //    sprintf (bts, "ENVIANDO%c",0x42);
  while (!TXIF) {
  }
  TXREG = 'B';
}

void trata(char a) {
  if (a == '#') {
    recep = 1;
    j = 0;
  } else if (a == '*') {
    recep = 0;
    escreve();
  } else if (recep && a != '#') {
    rec[j] = a;
    j++;
  }
}