/**
	Programme du controleur d'un aéroglisseur
	author *********************************************************
	author ***             VINCENT AUDETTE			     ***
	author ***           	FELIX CANTIN		             ***
	author ***            JEROME GRENIER		             ***
	author ***             GABRIEL TREMBLAY                      ***
	author ***	        IKRAM KOULMI	   		     ***
	author *********************************************************
	date 2018-02-01
*/

#include "lcd.h"
#include "uart.h"
#include "fifo.h"
#include "driver.h"
#include <avr/io.h>
#include <util/delay.h>

//Initialisation des sous programmes
void prog_led(int etat, int i);
void print_lcd(uint8_t horizontal, uint8_t vertical, uint8_t slider, float voltage, int bouton);
void to_uart(uint8_t horizontal, uint8_t vertical, uint8_t sust);
int sustentation(int button_state, uint8_t vertical, int i);

int main(void)
{		
	DDRD = clear_bit(DDRD, PD3); // Mettre la broche du bouton du joystick en entrée
	PORTD = set_bit(PORTD, PD3);	// On doit actier la "pull-up" interne de la broche pour
									// forcer un état haut quand le bouton n'est pas enfoncé
	DDRB = set_bits(DDRB, 0b00011111);	// Mettre les cinq premiers bits du port des DELs en
										// en sortie
		
	uart_init();				//Initialisation du UART
	SREG = set_bit(SREG, 7);	//Initialisation du port de communication
	lcd_init();					//Initialisation du LCD
	int button_state = 0;		// Variable qui contiendra l'état du bouton
	uint8_t horizontal = 0;		//Initialisation de l'horizontal
	uint8_t vertical = 0;		//Initialisation de la vertical
	uint8_t slider = 50;		//Initialisation du slider
	float voltage = 0;		//Initialisation du voltage
	int i = 0;					//Initialisation de i qui sert pour la gestion des "deux modes"
	adc_init();
	
	OSCCAL = OSCCAL + 7;
	DDRD = set_bit(DDRD, PD2);

	PORTD = clear_bit(PORTD, PD2);
	_delay_ms(500);
	
	PORTD = set_bit(PORTD, PD2);
	_delay_ms(1000);
	_delay_ms(1000);
	_delay_ms(1000);
	_delay_ms(1000);
	_delay_ms(1000);
	
	uart_put_string("AT+CIPMODE=1\r\n");
	uart_flush();
	_delay_ms(250);

	uart_put_string("AT+CIPSTART=\"UDP\",\"192.168.4.1\",100,123\r\n");
	uart_flush();
	_delay_ms(250);
	uart_put_string("AT+CIPSEND\r\n");
	uart_flush();

		
	while(1)
	{		
		//Pour déterminer le voltage de la batterie
		voltage = ((adc_read(PA3)*0.01294)/0.232558);
		
		
		
		if ((read_bit(PIND, PD3) == FALSE) && ((adc_read(PA2) == 250)||(i >= 2)) && (button_state == 0)){	//Lorsquon appui sur le joystick et que le bouton est a 0
																											//et que le slider est a 5 ou i >=2
			i++;	//Incrémentation de i à chaque clic qui respecte les conditions
			while(read_bit(PIND, PD3) == FALSE);	//Tant que le bouton nest pas relache
		}
		
		
		if (i >= 2)		//Si i est plus grand que 2 et que le slider est a 5 (pour accéder au "deuxième mode")
		{
			if (i>=4){	//Après 4 clics, on réinitialise la valeur de i
				i = 0;
			}
			
			horizontal = adc_read(PA1);			//Défini la lecture de la position horizontale du Joystick
			slider = adc_read(PA2);				//Défini la lecture du slider pour la sustentation
			slider = ((slider - 255)*-1);		//Inversion de la valeur envoyé par le UART
			vertical = (adc_read(PA0)-255)*-1;			//Défini la lecture du joystick pour la vertical
			if(vertical >= 0 && vertical < 130){		//Mise a l'echelle de la valeur verticale
				vertical = 0;
			}
			else{
				vertical = ((2*vertical)-255);
			}
						
			to_uart(horizontal, vertical, slider);
			print_lcd(horizontal, vertical, slider, voltage, button_state);
			prog_led(button_state, i);
		}
				
		else
		{				
			horizontal = adc_read(PA1);			//Défini la lecture de la position horizontale du Joystick
			vertical = adc_read(PA2);			//Défini la lecture de la position vertical du Joystick à PA2(slider)
			vertical = ((vertical - 255)*-1);	//Inversion de la valeur envoyé par le UART
			button_state = sustentation(button_state, vertical, i);				//Définie la valeur de button_state selon la sous fonction sustentation
			
			if(button_state == 1)
			{
				to_uart(horizontal, vertical, slider);				//Envoie l'information au uart
			}else if(button_state == 0){
				to_uart(horizontal, 0, 0);				//Envoie l'information au uart pour mode statique
			}
			print_lcd(horizontal, vertical, slider, voltage, button_state);		//Imprime les informations sur le LCD
		}
		
		_delay_ms(50);
	}//Fin while
}//Fin main

//Programme qui gère la sustantation lors du clic du joystick
int sustentation(int button_state, uint8_t vertical, int i){
	// Si le bouton est enfoncé (il ne faut pas oublier qu'en appuyant
	// sur le bouton, on connecte la broche du microcontrôleur à la masse,
	// donc on génère un 0 (ou FALSE))
	if(vertical <= 0){					//On empêche d'activer ou de désactiver la sustentation si le véhicule est en marche
		if(read_bit(PIND, PD3) == 0){	//Si le bouton est appuyé
			if (button_state == 0){
				button_state = 1;		//Modifie d'état à 1 si l'état est à 0
			}
			else {
				button_state = 0;		//Modifie d'état à 0 si l'état est à 1
			}
			while(read_bit(PIND, PD3) == 0);	//Tant que le bouton nest pas relache
			prog_led(button_state, i);			//Allume les leds selon l'état
		}
	}
	return button_state;	//Renvoi l'état du bouton 1 ou 0
}

//Fonction qui envoie des strings et des bytes dans le uart
void to_uart(uint8_t horizontal, uint8_t vertical, uint8_t sust){
	char stringhor[4];				//Initialise le string qui sera imprimé sur le LCD et envoyé dans le UART
	char stringver[4];
	char stringsus[4];
	uart_put_string("[");	//String de départ
	
	uint8_to_string(stringhor, horizontal);
	uart_put_string(stringhor);	//Envoie le string dans le port UART
	
	uint8_to_string(stringver, vertical);	//Converti vertical en string
	uart_put_string(stringver);	//Envoie le string dans le port UART
	
	uint8_to_string(stringsus, sust);	//Converti sust en string
	uart_put_string(stringsus);	//Envoie le string dans le port UART
		
	uart_put_string("]");	//String de fin dans le UART
	}

//Fonction qui affiche sur le LCD
void print_lcd(uint8_t horizontal, uint8_t vertical, uint8_t slider, float voltage, int bouton){
	char stringhor[4];				//Initialise les strings qui seront imprimés sur le LCD
	char stringver[4];
	char stringsus[4];
	char stringvolt[1];
	char string_mV[2];
	
	uint8_t volt;
	uint8_t mV;
	volt = (uint8_t)voltage;
	mV=(voltage-volt)*100;
	
	
	
	lcd_clear_display();	//On efface l'écran LCD avant de le réimprimer
	
	uint8_to_string(stringhor, horizontal);	//COnverti horizontal en string
	lcd_write_string ("DIR:");	//Imprime DIR, représentant la direction horizontale
	lcd_write_string (stringhor);	//Affiche la position horizontale du Joystick sur le LCD
	
	//Afficher le voltage
	
	uint8_to_string(stringvolt, volt);	//Converti voltage en string
	lcd_write_char(stringvolt);
	//lcd_write_string("V");
	uint8_to_string(string_mV, mV);
	lcd_write_string(string_mV);	//Imprime voltage
		
	////Afficher l'état du bouton poussoir 1 ou 0
	//if (bouton == 1){				//Si la sustentation est activée
		//lcd_write_string(" S1");
	//}
	//else {							//Si la sustentation est désactivée
		//lcd_write_string(" S0");
	//}
	

	lcd_write_string ("\n\r");	//Passe une ligne sur le LCD et ramène le texte à gauche
	lcd_write_string ("GAZ:");	//Imprime GAZ, représentant la vitesse d'avancement
	uint8_to_string(stringver, vertical);	//Converti vertical en string
	lcd_write_string(stringver);	//Imprime vertical
	
	lcd_write_string (" SUS:");	//Imprime SUS, représentant la sustentation
	uint8_to_string(stringsus, slider);	//Converti slider en string
	lcd_write_string(stringsus);	//Imprime slider
}

//Fonction qui allume ou éteint les led en fonction de l'état du bouton du joystick
void prog_led(int etat, int i){
	if (etat == 1){		//Si la sustentation est active
		PORTB = set_bits(PORTB, 0b00010101);	//On met en fonction les DELs
	}
	else {
		PORTB = clear_bits(PORTB, 0b00010101);	//On met hors fonction les DELs
		
		if (i>=2){		//Lorsque i plus grand ou égal à 2 (dans le deuxième mode)
			PORTB = set_bits(PORTB, 0b00000100);	//On met en fonction les DELs
		}
		else{
			PORTB = clear_bits(PORTB, 0b00000100);	//On met hors fonction les DELs
		}
	}
}

