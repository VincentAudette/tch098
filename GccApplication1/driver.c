/**
	\file  driver.c
	\brief Code source de fonctions qui pilotent directement du matériel
	
	\author *********************************************************
	\author ***             VINCENT AUDETTE						  ***
	\author ***           	FELIX CANTIN		                  ***
	\author ***            JEROME GRENIER						  ***
	\author ***             GABRIEL TREMBLAY                      ***
	\author ***				IKRAM KOULMI					      ***
	\author *********************************************************
	\date 2018-02-01
*/

/******************************************************************************
Includes
******************************************************************************/

#include <avr/io.h>
#include "driver.h"
#include "lcd.h"
#include "utils.h"


/******************************************************************************
Définitions de fonctions
******************************************************************************/

void adc_init(void){
	
	// Configuration des broches utilisées du port A en entrée (Entre PA0 et PA7)	
	DDRA = clear_bit(DDRA, PA1);//Servo
	
	DDRA = clear_bit(DDRA, PA0);//Moteur

	// Sélectionner la référence de tension: la tension d'alimentation (AVCC)
	ADMUX = clear_bit(ADMUX, REFS1);
	ADMUX = set_bit(ADMUX, REFS0);
	
	// Choisir le format du résultat de conversion: shift à gauche pour que
	// les 8 MSB se retrouvent dans le registre ADCH
	ADMUX = set_bit(ADMUX, ADLAR);

	// Choisir un facteur de division d'horloge (64) afin que l'horloge ait
	// une fréquence entre 50kHz et 200kHz. Donc 8MHz/64 = 125kHz.
	ADCSRA = set_bit(ADCSRA, ADPS1);
	ADCSRA = set_bit(ADCSRA, ADPS2);
	ADCSRA = set_bit(ADCSRA, ADPS0); //RAJOUTÉ POUR RENDRE LE SERVO PLUS COOL

	// Activer le CAN
	ADCSRA = set_bit(ADCSRA, ADEN);
}

uint8_t adc_read(uint8_t pin_name){
	
	// Choisir l'entrée analogique (broche) à convertir
	ADMUX = set_bit(ADMUX, MUX0);//Servo
	ADMUX = clear_bit(ADMUX, MUX1);
	ADMUX = clear_bit(ADMUX, MUX2);
	ADMUX = clear_bit(ADMUX, MUX3);
	ADMUX = clear_bit(ADMUX, MUX4);
	
	ADMUX = write_bits(ADMUX, 0b0001111, pin_name);//Moteur

	// Démarrage d'une conversion 
	ADCSRA = set_bit(ADCSRA, ADSC);
	
	// Attente de la fin de la conversion
	while(read_bit(ADCSRA, ADSC) != 0){
		
	}

	// Lecture et renvoie du résultat
	return ADCH;
}

void servo_init(void){
	
	// Configuration des broches de sortie
	DDRD = set_bit(DDRD, PD5);

	// Configuration du compteur et du comparateur
	TCCR1A = set_bit(TCCR1A, COM1A1);
	TCCR1A = set_bit(TCCR1A, COM1B1);
	TCCR1A = set_bit(TCCR1A, WGM11);
	
	TCCR1B = set_bit(TCCR1B, WGM13);
	TCCR1B = set_bit(TCCR1B, WGM12);

	// Configuration de la valeur maximale du compteur (top) à 20000
	ICR1 = 20000;
	
	// Initialiser la valeur du compteur à 0
	TCNT1 = 0;

	// Démarrer le compteur et fixer un facteur de division de fréquence à 8
	TCCR1B = set_bit(TCCR1B, CS11);
	
}

void servo_set_a(uint8_t angle){
	OCR1A = (7.058*angle)+600;
	
	// Modification du rapport cyclique du PWM du servomoteur. Min = 600, Max = 2550
	
}

void servo_set_b(uint8_t angle){
	
	// Modification du rapport cyclique du PWM du servomoteur. Min = 600, Max = 2550

}

void pwm_init(bool init_a, bool init_b){
	
		// Configuration des broches de sortie
		DDRB = set_bit(DDRB, PB3);
		DDRD = set_bit(DDRD, PD7);
		
		// Configuration du compteur et du comparateur
		
		//Premier moteur
		TCCR0= set_bit(TCCR0, WGM00);
		TCCR0= set_bit(TCCR0, WGM01);
		
		//Deuxième moteur
		TCCR2= set_bit(TCCR2, WGM20);
		TCCR2= set_bit(TCCR2, WGM21);

		// Démarrer le compteur et fixer un facteur de division de fréquence à 1024
		
		//Premier moteur
		TCCR0 = clear_bit(TCCR0, CS02);
		TCCR0= set_bit(TCCR0, CS01);
		TCCR0= set_bit(TCCR0, CS00);
		
		//Deuxième moteur
		TCCR2 = clear_bit(TCCR2, CS22);
		TCCR2= set_bit(TCCR2, CS21);
		TCCR2= set_bit(TCCR2, CS20);
	
}

void pwm_set_a(uint8_t duty){
	
	// Pour avoir un duty de 0, il faut éteindre le PWM et directement piloter la sortie à 0
	if(duty == 0){
		
		//Mettre 0 dans la broche PD7 (OC2) du port
		PORTD = clear_bit(PORTD, PD7);
		
		//Désactive le comparateur
		TCCR0 = clear_bit(TCCR0, COM01);
	}
	
	else{
		// Modification du rapport cyclique du PWM
		if(duty > 0 && duty < 130){
			OCR0 =0;
		}
		else{
			OCR0 = (2*duty)-255;
		}		
		//Active le comparateur
		TCCR0 = set_bit(TCCR0, COM01);
	}
}


void pwm_set_b(uint8_t duty){

	// Pour avoir un duty de 0, il faut éteindre le PWM et directement piloter la sortie à 0
	if(duty == 0){
		
		//Mettre 0 dans la broche PB3 (OC0) du port
		PORTB = clear_bit(PORTB, PB3);
		
		//Désactive le comparateur
		TCCR2 = clear_bit(TCCR2, COM21);
	}
	
	else{
		// Modification du rapport cyclique du PWM
		OCR2 = (2*duty)-255;
		
		//Active le comparateur
		TCCR2 = set_bit(TCCR2, COM21);
	}
}

void joystick_button_init(void){
	
}


bool joystick_button_read(void){
return FALSE;
}