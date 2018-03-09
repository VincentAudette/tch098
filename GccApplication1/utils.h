#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

/*
	 __ ___  __
	|_   |  (_
	|__  |  __)
	MIT License
	Copyright (c) 2018	�cole de technologie sup�rieure
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify and/or merge copies of the Software, and to permit persons
	to whom the Software is furnished to do so, subject to the following conditions:
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
*/
/**
	\file
	\brief Header de fonctions utiles � tout projet
	\author Iouri Savard Colbert
	\date 13 Mars 2012
	\attention Le fait d'inclure ce header dans un projet inclu automatiquement
	<stdint.h> ce qui rend sont inclusion redondante
*/

/* ----------------------------------------------------------------------------
Includes
---------------------------------------------------------------------------- */

#include <stdint.h>


/* ----------------------------------------------------------------------------
Typedefs
---------------------------------------------------------------------------- */

/**
    \brief d�fini un type bool�en
    En C le type boolen n'existe pas. Il n'a seulement �t� ajout� en C++. Bien
    que les compilateurs sont la plupart du temps gentils, la vraie mani�re
    stable et officielle de faire les choses est la suivante
*/
typedef enum {
    FALSE = 0,
    TRUE = !(0)
}bool;


/**
    \brief La seule d�finition qui nous manque vraiment de stdlib.h c'est celle-ci
*/
#ifndef NULL
    #define NULL 0
#endif


/* ----------------------------------------------------------------------------
Macros
---------------------------------------------------------------------------- */

/* bit operation ----------------------------------------------------------- */

/**
    \brief Pour mettre � 1 un seul bit � l'int�rieur d'un byte
	\param[in]  byte    le byte � modifier
    \param[in]  bit     le numr�o du bit � modifier � partir de LSB = 0
    \return le byte modifi�
    \code
    uint8_t mon_byte= 0b00001111;
    mon_byte = set_bit(mon_byte, 6);
    \endcode
    Produit le r�sultat suivant :
        mon_byte = 0b01001111
*/
#define set_bit(byte, bit)              ((byte) | (1 << (bit)))


/**
    \brief Pour mettre � 0 un seul bit � l'int�rieur d'un byte
	\param[in]  byte    le byte � modifier
    \param[in]  bit     le numr�o du bit � modifier � partir de LSB = 0
    \return le byte modifi�
    \code
    uint8_t mon_byte= 0b00001111;
    mon_byte = clear_bit(mon_byte, 1);
    \endcode
    Produit le r�sultat suivant :
        mon_byte = 0b00001101
*/
 #define clear_bit(byte, bit)            ((byte) & ~(1 << (bit)))


/**
    \brief Pour togller un seul bit � l'int�rieur d'un byte
	\param[in]  byte    le byte � modifier
    \param[in]  bit     le numr�o du bit � modifier � partir de LSB = 0
    \return le byte modifi�
    \code
    uint8_t mon_byte= 0b00001111;
    mon_byte = toggle_bit(mon_byte, 1);
    \endcode
    Produit le r�sultat suivant :
        mon_byte = 0b00001101
*/
#define toggle_bit(byte, bit)           ((byte) ^ (1 << (bit)))


/**
    \brief Pour mettre � une certaine valeur un seul bit � l'int�rieur d'un byte
	\param[in]  byte    le byte � modifier
    \param[in]  bit     le numr�o du bit � modifier � partir de LSB = 0
    \param[in]  value   la valeur � appliquer (ne peut �tre que 0 ou 1)
    \return le byte modifi�
    \code
    uint8_t mon_byte= 0b00001111;
    mon_byte = write_bit(mon_byte, 1, 0);
    \endcode
    Produit le r�sultat suivant :
        mon_byte = 0b00001101
*/
#define write_bit(byte, bit, value)     (((byte) & ~(1 << (bit))) | ((value) << (bit)))


/**
    \brief Pour lire un seul bit � l'int�rieur d'un byte
	\param[in]  byte    le byte � lire
    \param[in]  bit     le numr�o du bit � lire � partir de LSB = 0
    \return le bit lu (ne peut �tre que 0 ou 1 )
    \code
    uint8_t mon_byte= 0b00001111;
    unit8_t val_retour;
    val_retour = read_bit(mon_byte, 1);
    \endcode
    Produit le r�sultat suivant :
        val_retour = 1;
*/
#define read_bit(byte, bit)             (((byte) >> (bit)) & 1)

/**
    \brief Pour mettre � 1 plusieurs bit � l'int�rieur d'un byte
	\param[in]  byte    le byte � modifier
    \param[in]  mask    le masque des bits qui seront mis � 1
    \return le byte modifi�
    \code
    uint8_t mon_byte= 0b00001111;
    mon_byte = set_bits(mon_byte, 0b00111100);
    \endcode
    Produit le r�sultat suivant :
        mon_byte = 0b00111111;
*/
#define set_bits(byte, mask)            ((byte) | (mask))


/**
    \brief Pour mettre � 0 plusieurs bit � l'int�rieur d'un byte
	\param[in]  byte    le byte � modifier
    \param[in]  mask    le masque des bits qui seront mis � 0
    \return le byte modifi�
    \code
    uint8_t mon_byte= 0b00001111;
    mon_byte = clear_bits(mon_byte, 0b00111100);
    \endcode
    Produit le r�sultat suivant :
        mon_byte = 0b00000011;
*/
#define clear_bits(byte, mask)          ((byte) & ~(mask))


/**
    \brief Pour toggler plusieurs bit � l'int�rieur d'un byte
    \param[in]  byte    le byte � modifier
    \param[in]  mask    le masque des bits qui seront bascul�s
    \return le byte modifi�
    \code
    uint8_t mon_byte= 0b00001111;
    mon_byte = toggle_bits(mon_byte, 0b00111100);
    \endcode
    Produit le r�sultat suivant :
        mon_byte = 0b00110011;
*/
#define toggle_bits(byte, mask)         ((byte) ^ (mask))


/**
    \brief Pour mettre � une certaine valeur plusieurs bit � l'int�rieur d'un byte
    \param[in]  byte    le byte � modifier
    \param[in]  mask    le masque des bits qui seront modifi�es
    \param[in]  value   la valeur � appliquer
    \return le byte modifi�
    \code
    uint8_t mon_byte= 0b00001111;
    mon_byte = write_bits(mon_byte, 0b00111100, 0b01010101);
    \endcode
    Produit le r�sultat suivant :
        mon_byte = 0b00010111;
*/
#define write_bits(byte, mask, value)   (((byte) & ~(mask)) | ((value) & (mask)))


/**
    \brief Pour lire la valeur de plusieurs bit � l'int�rieur d'un byte avec les bits
    non int�ressants masqu�s � 0
	\param[in]  byte    le byte � lire
    \param[in]  mask    le masque des bits qui seront r��lement lus
    \return la valeur masqu�e
    \code
    uint8_t mon_byte= 0b00001111;
    unit8_t val_retour;
    val_retour = read_bits(mon_byte, 0b00111100);
    \endcode
    Produit le r�sultat suivant :
        val_retour = 0b00001100;
*/
#define read_bits(byte, mask)           ((byte) & (mask))


/* Math *********************************************************************/

/**
    \brief Retourne la valeur absolue d'un nombre
	\param[in]  value    Le nombre � rendre positif
    \return le nombre en valeur absolue
*/
#define abs(value)                          ((value) < 0? -(value) : (value))



/* ----------------------------------------------------------------------------
Prototypes
---------------------------------------------------------------------------- */

/* Memory management **********************************************************/

/**
    \brief Copie un bloc de m�moire � un autre endroit
    \param[out] destination Pointeur sur la destination de la copie
    \param[in]  source  Pointeur sur la source de la copie
    \param[in]  num     le nombre de byte � copier
    \attention num c'est le nombre de bytes et non pas le nombte d'�l�ment d'un
    tableau.  Pour copier un tableau qui conteint autre chose que des bytes,
    l'argument num devrait avoir la forme suivante :
        nb_elements * sizeof(element)
*/
void mem_copy(void * destination, const void * source, uint8_t num );


/* String stuff *************************************************************/

/**
    \brief Retourne la longueur d'une string (array termin� par '\0')
    \param[in]  string  Pointeur sur la string
    \return la longueur de la string sans le caract�re nul
    \code
    uint8_t ret_val;
    ret_val = string_length("Hello World");
    \endcode
    Produira la r�sultat suivant :
    > ret_val = 11
*/
uint8_t string_length(const char* string);

/**
    \brief Retourne la longueur d'une string (array termin� par '\0')
    \param[out] out_string  Pointeur sur la string de destination
    \param[in]  in_string   Pointeur sur la string de source
    \return Le nombre de caract�res qui ont �t� copi�s, sans compter le '\0'
*/
uint8_t string_copy(char* out_string, const char* in_string);


/* Conversion text to number ************************************************/

/**
    \brief Converti un caract�re repr�sentant un chiffre en sa valeur
    \param[in]  character   Le caract�re en question
    \return La valeur ou z�ro si le caract�re n'avait pas rapport
*/
uint8_t char_to_uint(char character);


/**
    \brief Converti une string repr�sentant un nombre en sa valeur
    \param[in]  string  La string en question
    \return La valeur ou de la cochonnerie si la string n'avait pas rapport
*/
uint32_t string_to_uint(const char* string);


/**
    \brief Converti un array de char repr�sentant un nombre en sa valeur
    \param[in]  char_array  Le array en question
    \param[in]  size    La grosseur int�ressante du array puisque celui n'est pas
    oblig� de se terminer par la caract�re nul
    \return La valeur ou de la cochonnerie si le array n'avait pas rapport
*/
uint32_t char_array_to_uint(const char* char_array, uint8_t size);


/**
    \brief Converti un caract�re repr�sentant un chiffre hexad�cimal en sa valeur
    \param[in]  character   Le caract�re en question
    \return La valeur ou z�ro si le caract�re n'avait pas rapport
*/
uint8_t hex_char_to_uint(char character);


/**
    \brief Converti une string repr�sentant un nombre hexad�cimal en sa valeur
    \param[in]  string  La string en question
    \return La valeur ou de la cochonnerie si la string n'avait pas rapport
*/
uint32_t hex_string_to_uint(const char* string);


/**
    \brief Converti un array de char repr�sentant un nombre hexad�cimal en sa valeur
    \param[in]  char_array  Le array en question
    \param[in]  size    La grosseur int�ressante du array puisque celui n'est pas
    oblig� de se terminer par la caract�re nul
    \return La valeur ou de la cochonnerie si le array n'avait pas rapport
*/
uint32_t hex_char_array_to_uint(const char* char_array, uint8_t size);


/* Conversion number to text ************************************************/

/**
    \brief Converti un nombre en le caract�re ASCII qui le repr�sente
    \param[in]  digit   Le nombre entre 0 et 9
    \return Le caract�re ASCII qui rep�sente le param�tre
    \warning C'est une erreur d'appeler cette fonction avec un param�tre qui est n�gatif
    ou plus grand que 9
*/
char uint_to_char(uint8_t digit);

/**
    \brief Converti un nombre hexd�cimal en le caract�re ASCII qui le repr�sente
    \param[in]  hex_digit   Le nombre entre 0 et 15
    \return Le caract�re ASCII qui rep�sente le param�tre
    \warning C'est une erreur d'appeler cette fonction avec un param�tre qui est n�gatif
    ou plus grand que 15
    Pour les nombre entre 10 et 15, la sortie est une lettre majuscule entre A et F
*/
char uint_to_hex_char(uint8_t hex_digit);

/**
    \brief Converti un entier non sign� de 8 bits en une string
    \param[out] out_string  La string de destination
    \param[in]  number      Le nombre � convertir
    \return     Le nombre de caract�res ajout�s � la string sans compter le '\0'
    \warning    La string doit �tre assez longue pour contenir la conversion. Dans le
    cas d'un 8 bits, �a prend une string qui fait minimalement 4 caract�res de long.
    La string de sortie a toujours 3 caract�res et ce peu importe la longueur du nombre.
    Par cons�quent, si le nombre est plus petit que 100, le r�sultat sera "padd�" de 0
    Le retour de la fonction permet de faire des appels successifs et de progressivement
    construire une string complexe
    \code
    char string[16];
    uint8_t string_index;
    string_index = uint8_to_string(string, 42);
    string[string_index] = '!';
    string[string_index + 1] = '\0';
    \endcode
    produira la string suivante :
        042!
*/
uint8_t uint8_to_string(char* out_string, uint8_t number);

/**
    \brief Converti un entier non sign� de 16 bits en une string
    \param[out] out_string  La string de destination
    \param[in]  number      Le nombre � convertir
    \return     Le nombre de caract�res ajout�s � la string sans compter le '\0'
    \warning    La string doit �tre assez longue pour contenir la conversion. Dans le
    cas d'un 16 bits, �a prend une string qui fait minimalement 6 caract�res de long.
    La string de sortie a toujours 5 caract�res et ce peu importe la longueur du nombre.
    Par cons�quent, si le nombre est plus petit que 10 000, le r�sultat sera "padd�" de 0
    Le retour de la fonction permet de faire des appels successifs et de progressivement
    construire une string complexe
    \code
    char string[16];
    uint8_t string_index;
    string_index = uint16_to_string(string, 42);
    string[string_index] = '!';
    string[string_index + 1] = '\0';
    \endcode
    produira la string suivante :
        00042!
*/
uint8_t uint16_to_string(char* out_string, uint16_t number);

/**
    \brief Converti un entier non sign� de 32 bits en une string
    \param[out] out_string  La string de destination
    \param[in]  number      Le nombre � convertir
    \return     Le nombre de caract�res ajout�s � la string sans compter le '\0'
    \warning    La string doit �tre assez longue pour contenir la conversion. Dans le
    cas d'un 32 bits, �a prend une string qui fait minimalement 11 caract�res de long.
    La string de sortie a toujours 10 caract�res et ce peu importe la longueur du nombre.
    Par cons�quent, si le nombre est plus petit que 1 000 000 000, le r�sultat sera "padd�" de 0
    Le retour de la fonction permet de faire des appels successifs et de progressivement
    construire une string complexe
    \code
    char string[16];
    uint8_t string_index;
    string_index = uint32_to_string(string, 42);
    string[string_index] = '!';
    string[string_index + 1] = '\0';
    \endcode
    produira la string suivante :
        0000000042!
*/
uint8_t uint32_to_string(char* out_string, uint32_t number);

/**
    \brief Converti un entier hexad�cimal de 8 bits en une string
    \param[out] out_string  La string de destination
    \param[in]  number      Le nombre � convertir
    \return     Le nombre de caract�res ajout�s � la string sans compter le '\0'
    \warning    La string doit �tre assez longue pour contenir la conversion. Dans le
    cas d'un 8 bits, �a prend une string qui fait minimalement 3 caract�res de long.
    Le retour de la fonction permet de faire des appels successifs et de progressivement
    construire une string complexe
    \code
    char string[16];
    uint8_t string_index;
    string_index = uint8_to_hex_string(string, 0x2A);
    string[string_index] = '!';
    string[string_index + 1] = '\0';
    \endcode
    produira la string suivante :
        2A!
*/
uint8_t uint8_to_hex_string(char* out_string, uint8_t number);

/**
    \brief Converti un entier hexad�cimal de 16 bits en une string
    \param[out] out_string  La string de destination
    \param[in]  number      Le nombre � convertir
    \return     Le nombre de caract�res ajout�s � la string sans compter le '\0'
    \warning    La string doit �tre assez longue pour contenir la conversion. Dans le
    cas d'un 16 bits, �a prend une string qui fait minimalement 5 caract�res de long.
    Le retour de la fonction permet de faire des appels successifs et de progressivement
    construire une string complexe
    \code
    char string[16];
    uint8_t string_index;
    string_index = uint16_to_hex_string(string, 0x002A);
    string[string_index] = '!';
    string[string_index + 1] = '\0';
    \endcode
    produira la string suivante :
        002A!
*/
uint8_t uint16_to_hex_string(char* out_string, uint16_t number);

/**
    \brief Converti un entier hexad�cimal de 32 bits en une string
    \param[out] out_string  La string de destination
    \param[in]  number      Le nombre � convertir
    \return     Le nombre de caract�res ajout�s � la string sans compter le '\0'
    \warning    La string doit �tre assez longue pour contenir la conversion. Dans le
    cas d'un 16 bits, �a prend une string qui fait minimalement 9 caract�res de long.
    Le retour de la fonction permet de faire des appels successifs et de progressivement
    construire une string complexe
    \code
    char string[16];
    uint8_t string_index;
    string_index = uint32_to_hex_string(string, 0x0000002A);
    string[string_index] = '!';
    string[string_index + 1] = '\0';
    \endcode
    produira la string suivante :
        0000002A!
*/
uint8_t uint32_to_hex_string(char* out_string, uint32_t number);

/**
    \brief Converti un entier sign� de 8 bits en une string
    \param[out] out_string  La string de destination
    \param[in]  number      Le nombre � convertir
    \return     Le nombre de caract�res ajout�s � la string sans compter le '\0'
    \warning    La string doit �tre assez longue pour contenir la conversion. Dans le
    cas d'un 8 bits, �a prend une string qui fait minimalement 4 caract�res de long.
    La string de sortie aura le signe comme pr�fixe suivit de 3 caract�res rep�sentant
    la valeur absolue du nombre. Le pr�fixe peu �tre '+' ou '-'. Si le nombre est plus
    petit que 100 et plus grand que -100, le r�sultat sera "padd�" de 0
    Le retour de la fonction permet de faire des appels successifs et de progressivement
    construire une string complexe
    \code
    char string[16];
    uint8_t string_index;
    string_index = int8_to_string(string, -42);
    string[string_index] = '!';
    string[string_index + 1] = '\0';
    \endcode
    produira la string suivante :
        -042!
*/
uint8_t int8_to_string(char* out_string, int8_t number);

/**
    \brief Converti un entier sign� de 16 bits en une string
    \param[out] out_string  La string de destination
    \param[in]  number      Le nombre � convertir
    \return     Le nombre de caract�res ajout�s � la string sans compter le '\0'
    \warning    La string doit �tre assez longue pour contenir la conversion. Dans le
    cas d'un 16 bits, �a prend une string qui fait minimalement 7 caract�res de long.
    La string de sortie aura le signe comme pr�fixe suivit de 3 caract�res rep�sentant
    la valeur absolue du nombre. Le pr�fixe peu �tre '+' ou '-'. Si le nombre est plus
    petit que 10000 et plus grand que -10000, le r�sultat sera "padd�" de 0
    Le retour de la fonction permet de faire des appels successifs et de progressivement
    construire une string complexe
    \code
    char string[16];
    uint8_t string_index;
    string_index = int16_to_string(string, -42);
    string[string_index] = '!';
    string[string_index + 1] = '\0';
    \endcode
    produira la string suivante :
        -00042!
*/
uint8_t int16_to_string(char* out_string, int16_t number);

/**
    \brief Converti un entier sign� de 32 bits en une string
    \param[out] out_string  La string de destination
    \param[in]  number      Le nombre � convertir
    \return     Le nombre de caract�res ajout�s � la string sans compter le '\0'
    \warning    La string doit �tre assez longue pour contenir la conversion. Dans le
    cas d'un 32 bits, �a prend une string qui fait minimalement 12 caract�res de long.
    La string de sortie aura le signe comme pr�fixe suivit de 3 caract�res rep�sentant
    la valeur absolue du nombre. Le pr�fixe peu �tre '+' ou '-'. Si le nombre est plus
    petit que 1 000 000 000 et plus grand que -1 000 000 000, le r�sultat sera "padd�" de 0
    Le retour de la fonction permet de faire des appels successifs et de progressivement
    construire une string complexe
    \code
    char string[16];
    uint8_t string_index;
    string_index = int32_to_string(string, -42);
    string[string_index] = '!';
    string[string_index + 1] = '\0';
    \endcode
    produira la string suivante :
        -0000000042!
*/
uint8_t int32_to_string(char* out_string, int32_t number);


#endif // UTILS_H_INCLUDED