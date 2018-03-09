/*
* LesGlisseux.c
*
* Created: 2018-01-19 14:34:26
*  Author: AP12270
*/


// Programme d'initiation :
//Ce programme reçoit des strings envoyer du controlleur et l'

#include <avr/io.h>
#include <util/delay.h>
#include "driver.h"
#include "utils.h"
#include "lcd.h"
#include "driver.h"
#include "uart.h"

#define PREMIER_BIT 0
#define HORIZONTAL_BIT_1 1
#define HORIZONTAL_BIT_2 2
#define HORIZONTAL_BIT_3 3
#define VERTICAL_BIT_1 4
#define VERTICAL_BIT_2 5
#define VERTICAL_BIT_3 6
#define SUSTENTATION_BIT_1 7
#define SUSTENTATION_BIT_2 8
#define SUSTENTATION_BIT_3 9
#define DERNIER_BIT 10


int main(void)
{
		
	char mon_byte;
	char hor[4];
	char ver[4];
	char sus[4];
	uint8_t horizontal=0;
	uint8_t vertical=0;
	uint8_t sustentation=0;
	
	
	DDRD = clear_bit(DDRD, PD3); // Mettre la broche du bouton du joystick en entrée
	
	PORTD = set_bit(PORTD, PD3); /* On doit activer la "pull-up" interne de la broche pour
	forcer un état haut quand le bouton n'est pas enfoncé*/
	
	DDRB = set_bits(DDRB, 0b00011111); /* Mettre les cinq premiers bits du port des DELs en
	en sortie */
	
	lcd_init(); // Faire l'initialisation du LCD
	adc_init();
	servo_init();
	uart_init();
	pwm_init(TRUE,FALSE);
	
	int comp =0;
	SREG = set_bit(SREG,7);

	//RECEIVEUR
	
	// Calibration de la fréquence du microcontrôleur
	
		OSCCAL += 10;
	
	// Configurer la broche de RESET de l’ESP8266 en sortie
	
		DDRD = set_bit(DDRD, PD2);
	
	// Faire un reset de l’ESP8266 en mettant la broche de sortie à 0 
	//puis faire un délai de 500ms.
	
		PORTD = clear_bit(PORTD, PD2);
		_delay_ms(500);
	
	// Redémarrer l’ESP8266 en arrêtant de faire un reset, c’est-à-dire 
	// en mettant la broche de sortie à 1 puis faire un délai d’au moins 5 secondes.
	
		PORTD = set_bit(PORTD, PD2);
		_delay_ms(1000);
		_delay_ms(1000);
		_delay_ms(1000);
		_delay_ms(1000);
		_delay_ms(1000);
	
	// Envoyer la commande pour le mode Passthrough, attendre que la commande soit 
	// complètement envoyée puis faire un délai de 250ms.
	
		uart_put_string("AT+CIPMODE=1\r\n");
		uart_flush();
		_delay_ms(250);
	
	// Envoyer la commande pour établir la transmission UDP, attendre que la commande
	// soit complètement envoyée puis faire un délai de 250ms.
	
		uart_put_string("AT+CIPSTART=\"UDP\",\"0.0.0.0\",123,100\r\n");
		uart_flush();
		_delay_ms(250);
	
	while (1)
	{
		
		//lcd_clear_display();
		while(uart_is_rx_buffer_empty() == FALSE)
		{
			mon_byte = uart_get_byte();	
	
			switch(comp)
			{
				// Déclaration du premier byte envoyé dans le string
				case PREMIER_BIT:
					if(mon_byte == '['){
						comp++;
					}
				break;
		
				//Premier byte Horizontal envoyé
				case HORIZONTAL_BIT_1:
				hor[0]=mon_byte;
				comp++;
				break;
				
				//Deuxième byte Horizontal envoyé
				case HORIZONTAL_BIT_2:
				hor[1]=mon_byte;
				comp++;
				break;
				
				//Troisième byte Horizontal envoyé
				case HORIZONTAL_BIT_3:
				hor[2]=mon_byte;
				hor[3]='\0';
				comp++;
				break;
			
				//Premier byte Vertical envoyé
				case VERTICAL_BIT_1:
				ver[0]=mon_byte;
				comp++;
				break;
				
				//Deuxième byte Vertical envoyé
				case VERTICAL_BIT_2:
				ver[1]=mon_byte;
				comp++;
				break;
				
				//Troisième byte Vertical envoyé
				case VERTICAL_BIT_3:
				ver[2]=mon_byte;
				ver[3]='\0';
				comp++;
				break;
				
				//Premier byte Sustentation envoyé
				case SUSTENTATION_BIT_1:
				sus[0]=mon_byte;
				comp++;
				break;
								
				//Deuxième byte Sustentation envoyé
				case SUSTENTATION_BIT_2:
				sus[1]=mon_byte;
				comp++;
				break;
								
				//Troisième byte Sustentation envoyé
				case SUSTENTATION_BIT_3:
				sus[2]=mon_byte;
				sus[3]='\0';
				comp++;
				break;
				
				// Déclaration du dernier bit envoyé dans le string
				case DERNIER_BIT:
				if(mon_byte == ']'){
					//end string received
					comp++;
				}
				break;
			
				case 11:
				
					//Impression des bytes Horizontal
					lcd_clear_display();
					lcd_write_string("Hor:");
					lcd_write_string(hor);
					
					lcd_write_string("\n\r");
					
					//Impression des bytes Vertical
					lcd_write_string("Ver:");
					lcd_write_string(ver);
					
					//Impression des bytes Sustentation
					lcd_write_string(" SUS:");
					lcd_write_string(sus);
					
					
					comp=0;
				break;
			}//END STATE MACHINE
		}
		
		horizontal = char_array_to_uint(hor, 3);
		vertical = char_array_to_uint(ver, 3);
		sustentation = char_array_to_uint(sus, 3);
		
		pwm_set_a(vertical);
		pwm_set_b(sustentation);
		
		servo_set_a(horizontal);		
		_delay_ms(100);
	}
}