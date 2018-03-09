#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

/**
     __   __                 __     __
    |__) /  \ \_/  /\  |  | |  \ | /  \
    |  \ \__/ / \ /~~\ \__/ |__/ | \__/
    Copyright (c) Roxaudio 2012. All rights reserved.
    This Source Code is the Property of Roxaudio inc. and can only be
    used in accordance with Roxaudio's Source Code License Agreement.
	\file uart.h
	\brief G�re la communication bidirectionelle avec le port s�rie
	\author Iouri Savard Colbert
	\date 13 Mars 2012
*/

/******************************************************************************
Includes
******************************************************************************/

#include "utils.h"

/******************************************************************************
Defines
******************************************************************************/

#define UART_RX_BUFFER_SIZE 64	//Certaines r�ponses du ESP8266 prennent jusqu'� 60 caract�res
#define UART_TX_BUFFER_SIZE 64


typedef enum{

    BAUDRATE_2400 = 0,
    BAUDRATE_4800,
    BAUDRATE_9600,
    BAUDRATE_19200,
    BAUDRATE_38400,
    BAUDRATE_57600,
    BAUDRATE_115200,
    BAUDRATE_230400,
    BAUDRATE_250000,

}baudrate_e;

#define DEFAULT_BAUDRATE BAUDRATE_9600

/******************************************************************************
Prototypes
******************************************************************************/

/**
    \brief Fait l'initialisation du UART
*/
void uart_init(void);


/**
    \brief D�finit le badrate du port choisit
*/
void uart_set_baudrate(baudrate_e baudrate);


/**
    \brief Ajoute un byte au rolling buffer � envoyer par le UART
    \param byte le byte � ajouter
*/
void uart_put_byte(uint8_t byte);


/**
    \brief Ajoute la string (par copie) au rolling buffer � envoyer par le UART.
    \param un pointeur sur le premier char de la string
	
	La copie s'arr�te au premier \0. Ce dernier n'est pas copi�. Si la string est plus
	longue que l'espace qui est libre dans le buffer, la fonction va patiement attendre
	que de l'espace se lib�re. Dans cette situation, cette fonction peut �tre tr�s longue
	� retourner, ce qui pourrait briser des timmings critiques dans le code.
*/
void uart_put_string(char* string);

/**
    \brief Retire un byte au rolling buffer re�u par le UART.
    \return le byte re�u
    Si le buffer est vide cette fonction retourne '\0'.  Si le caract�re � retourner
    est '\0' la fonction retourne aussi cette valeur.  Pour faire la diff�rence entre
    les deux il faut pr�alablement v�rifier si le buffer est vide avec la fonction
    uart_is_rx_buffer_empty()
*/
uint8_t uart_get_byte(void);

/**
    \brief Retourne tout ce que que le buffer de r�ception contient sous la forme d'une string
    Pour que cette fonction puisse fonctionner le buffer qui lui est pass� doit minimalement
	avoir deux bytes de long. Un byte pour retourner au moins un caract�re et un autre pour
	le \0. Si le buffer est plus petit que �a, cette fonction va corrompre la m�moire sans
	pr�venir.
	
	D�s que le buffer de r�ception est vide, la fonction termine la string avec un \0 et
	retourne. Comme le UART est relativement tr�s lent par rapport au processeur, il faut
	attendre un peu avant d'appeler cette fonction.
	
	Si le buffer est vide au moment d'appeler cette fonction, celle-ci retourne une string
	d'un seul caract�re; \0
	
	Si le uart est utilis� pour communiquer autre chose que des strings, par exemple
	des donn�es binaires, il se peut tr�s bien que la valeur 0 soit pr�sente dans le buffer.
	Dans ce cas il est impossible de faire la distinction entre une valeur 0 et un \0.
	
	Si le buffer contient plus de bytes que buffer_length - 1, la fonction va copier 
	buffer - 1 bytes et utiliser le dernier byte pour le \0. Par cons�quent, le buffer de
	r�ception ne sera totalement vid�. Pour savoir si cette situation s'est produite, un
	appel � la fonction uart_is_rx_buffer_empty() retournera FALSE.
	
	Pour garantir que cette situation ne se produira jamais, il suffit de passer un
	buffer plus gros que UART_RX_BUFFER_SIZE
*/
void uart_get_string(char* out_buffer, uint8_t buffer_length);


/**
    \brief Vide le buffer de r�ception
	
	Dans cette situation les bytes re�u sont juste effac�s et perdus � jamais.
*/
void uart_clean_rx_buffer(void);


/**
    \brief Attend que tous les caract�res dans le buffer soient envoy�s. Cette
	fonction bloque l'�xc�cution du code.
*/
void uart_flush(void);


/**
    \brief Indique si le buffer de r�ception est vide.
    \param TRUE si il est vide, FALSE s'il contient 1 byte ou plus
*/
bool uart_is_rx_buffer_empty(void);

/**
    \brief Indique si le buffer de transmission est vide.
    \param TRUE si il est vide, FALSE s'il contient 1 byte ou plus
*/
bool uart_is_tx_buffer_empty(void);


#endif // UART_H_INCLUDED