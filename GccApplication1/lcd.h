#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

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
	\file lcd.h
	\brief driver pour un affichage LCD pilot� par un HD44780
	\author Iouri Savard Colbert
	\date 28 avril 2014
	Ce module est divis� en deux sous-modules suivants :
    - hd44780
    - lcd
    hd44780 :
    Ce sous- module fournis des fonctions pour accomplir les fonctionnalit�s
    mat�rielle fournies par le circuit du LCD.  Autrement dit, elles permettent � un
    utilisateur � ne pas avoir g�n�rer manuellement des codes binaires pour faire
    fonctionner ce dernier.  Les fonctions de ce module sont tr�s rapides
    et peu gourmandes en m�moire.  Par contre elles sont limit�es en
    fonctionnalit�.  Les deux limitations majeures sont l'absence de relation
    avec l'affichage r�el et l'impossibilit� de conna�tre la position actuelle
    du curseur.  Ces fonctionnalit�s sont ajout�e par le module englobant lcd.
    lcd :
    Permet d'accomplir les m�mes t�ches que le pilote directement, mais palie
    par logiciel aux limitations majeurs de celui-ci.  Cet ajout
	de fonctionnalit� se fait en �change d'un l�ger co�t en temps de traitement
	et en utilisation de m�moire.  Ceci dit, ces deux derniers restent minimes.
	Toute personne voulant contr�ller l'affichage de LCD devrait passer par ce
	sous-module et non pas par le pilote de hd44780.
    Utilisation des sous modules :
    De fa�on � fonctionner correctement, les fonctions des sous modules ne
    peuvent �tre interm�lang�es.  Donc, un code qui n'aurait besoin d'une
    seule fois d'une foctionnalit� du msous-module LCD text doit quand m�me utilsier
	LCD pour tous les autres op�rations, aussi simples soient-elles.
*/

/**
    \brief Switch qui permet l'utilisation de caract�res japonais
    Si la switch est d�finie, les caract�res peuvent �tre utilis�s
*/
#define ENABLE_JAPANESE_CHAR

/* ----------------------------------------------------------------------------
Includes
---------------------------------------------------------------------------- */

#include "utils.h"


/* ----------------------------------------------------------------------------
Defines et typedef
---------------------------------------------------------------------------- */

/**
    \brief D�fini quel port est utilis� pour le data du LCD
*/
#define DATA_PORT   PORTC

/**
    \brief D�fini le registre pour contr�ller la direction du data du LCD
*/
#define DATA_DDR    DDRC

/**
    \brief D�fini quel port est utilis� pour le contr�lle du LCD
*/
#define CTRL_PORT   PORTA

/**
    \brief D�fini le registre pour contr�ller la direction du contr�lle du LCD
*/
#define CTRL_DDR    DDRA

/**
    \brief D�fini le num�ro de la broche qui joue le r�le de enable
*/
#define E_PIN       7

/**
    \brief D�fini le num�ro de la broche qui joue le r�le de read/write
*/
#define RW_PIN      6

/**
    \brief D�fini le num�ro de la broche qui joue le r�le de register select
*/
#define RS_PIN      5


/**
    \brief D�fini le bombre de colonnes du LCD
*/
#define LCD_NB_COL 16

/**
    \brief D�fini le bombre de rang�es du LCD
*/
#define LCD_NB_ROW 2

/**
    \sa hd44780_shift_cursor(hd44780_shift_e shift)
*/
typedef enum{

    HD44780_SHIFT_RIGHT,
    HD44780_SHIFT_LEFT,

}hd44780_shift_e;

/**
    \sa lcd_shift_cursor(lcd_shift_e shift)
*/
typedef enum{

    LCD_SHIFT_RIGHT,
    LCD_SHIFT_LEFT,
    LCD_SHIFT_UP,
    LCD_SHIFT_DOWN,
	LCD_SHIFT_END,
	LCD_SHIFT_START,
	LCD_SHIFT_TOP,
	LCD_SHIFT_BOTTOM

}lcd_shift_e;

/**
    \brief D�fini le caract�re repr�sentant un fl�che vers la droite
    \sa lcd_write_char(char character)
*/
#define CHAR_RIGHT_ARROW 0x7E

/**
    \brief D�fini le caract�re repr�sentant un fl�che vers la gauche
    \sa lcd_write_char(char character)
*/
#define CHAR_LEFT_ARROW 0x7F



/* ----------------------------------------------------------------------------
Prototypes
---------------------------------------------------------------------------- */

/* HD44780 ----------------------------------------------------------------- */

/**
    \brief Fait l'initialisation de base du contr�leur LCD.  C'est-�-dire
    l'initialisation, r�gler la config pour 16 caract�res X 2 rang�es, effacer
    l'�cran et mettre le curseur au d�but.
    \param[in]  increment   Si == TRUE, le texte avance vers la droite
    \param[in]  cursor      Si == TRUE, un curseur est affich�
    \param[in]  blink       Si == TRUE, la case du curseur clignote
    \return Rien
*/
void hd44780_init(bool increment, bool cursor, bool blink);

/**
    \brief Efface tous les caract�res sur l'�cran et remet le curseur au d�but
    \return Rien
*/
void hd44780_clear_display();

/**
    \brief D�fini le sens dans lequel les caract�res sont ins�r�.  � moins
    d'�tre bien sp�cial dans sa t�te, en Am�rique du nord le sens normal est
    incr�mental et ne devrait pas changer souvent.  Cette fonction est utilis�e
    par la fonction qui insert des caract�re.
    \param[in]  increment   Si == TRUE, le exte avance vers la droite
    \return Rien
*/
void hd44780_set_entry_mode(bool increment);

/**
    \brief Permet de contr�ler certain param�tres de l'affichage
    \param[in]  display Si == TRUE, l'affichage est allum�
    \param[in]  cursor  Si == TRUE, un curseur est affich�
    \param[in]  blink   Si == TRUE, la case du curseur clignote
    \return Rien
*/
void hd44780_set_display_control(bool display, bool cursor, bool blink);

/**
    \brief Permet de d�placer le curseur dans la m�moire du HD44780.
    \param[in]  col La colone de 0 � 39
    \param[in]  row La rang� de 0 � 1
    \return Rien
    Cette fonction permet de faire abstraction des adresses en m�moire qui sont
    un peu batardes et d'y aller directement une position dans l'affichage.
    Rien n'emp�che d'aller mettre un caract�re � l'ext�rieur de la zone
    affichable, en autant que les limites des param�tres soient respect�es
*/
void hd44780_set_cursor_position(uint8_t col, uint8_t row);

/**
    \brief Permet de d�placer le curseur d'une position � gauche ou � droite.
    \param[in]  shift   La direction du shift
    \return Rien
*/
void hd44780_shift_cursor(hd44780_shift_e shift);

/**
    \brief �crit un seul caract�re � l'endroit actuel du curseur.
    \param[in]  character   Le caract�re ASCII � afficher.
    \return Rien
    Au retour, le curseur aura �t� d�plac� d'une position dans le sens d�termin� � l'aide
    de hd44780_set_entry_mode.  Si le caract�re n'est pas affichable, une boule
    pas rapport sera affich� � la place.
*/
void hd44780_write_char(char character);



/* LCD --------------------------------------------------------------------- */

/**
    \brief Fait l'initialisation du LCD.
    \return Rien
	Cette fonction doit pr�alablement �tre appel�e avant d'utiliser les autres
	fonctions du module.
*/
void lcd_init(void);

/**
    \brief Efface l'�cran du LCD et retourne le curseur � la position 0,0
	Il n'est pas r�element possible "d'effacer" l'�cran du LCD. Bien que
	la fonctionnalit� soit offerte par le hd44780, en r�alit� la fiche technique
	nous apprend que le LCD ne fait que remplacer tous les caract�res de l'�cran
	par des espaces. C'est pour cette raison que cette fonction est relativement
	lente. Il serait une mauvaise id.e de faire un appel � celle-ci dans une boucle
	avec de d�lais critiques.
*/
void lcd_clear_display(void);

/**
    \brief D�place le curseur � un endroit pr�cis dans l'�cran
    \param[in]  col La colonne (0 � 15)
    \param[in]  row La range� (0 � 2)
	Par exemple, lcd_set_cursor_position(3, 1) : am�ne le curseur
	� la position du X
        0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        | | | | | | | | | | | | | | | | | 0
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        | | | |X| | | | | | | | | | | | | 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	Il y a 16 colonnes et 2 rang�es. Si les param�tres d�passent les dimensions
	de l'�cran, la demande est ignor�e.
*/
void lcd_set_cursor_position(uint8_t col, uint8_t row);

/**
    \brief D�place le curseur d'une seule case dans la position sp�cifi�e
    \param[in]  shift   Une des valeurs de la liste suivante
	Les valeurs possibles pour shift sont :
    - LCD_SHIFT_RIGHT	: D�place le curseur d'une case � droite
    - LCD_SHIFT_LEFT	: D�place le curseur d'une case � gauche
    - LCD_SHIFT_UP		: D�place le curseur d'une case en haut
    - LCD_SHIFT_DOWN	: D�place le curseur d'une case en bas
    - LCD_SHIFT_END		: D�place le curseur � la derni�re colonne de la ligne (mais reste sur la m�me rang�)
    - LCD_SHIFT_START	: D�place le curseur � la premi�re colonne de la ligne (mais reste sur la m�me rang�)
    - LCD_SHIFT_TOP		: D�place le curseur sur la premi�re ligne (mais reste dans la m�me colonne)
    - LCD_SHIFT_BOTTOM	: D�place le curseur sur la derni�re ligne (mais reste dans la m�me colonne)
	Les 4 premi�res options se comportent comme si l'�cran du LCD �tait une boucle dans les deux directions.
	Par exemple :
	Si le curseur est � la position du X
         0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        | | | | | | | | | | | | | | | |X| 0
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        | | | | | | | | | | | | | | | | | 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	Et que l'appel suivant est fait : `lcd_shift_cursor(LCD_SHIFT_RIGHT);`
	Le curseur va se retrouver � la position suivante:
         0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |X| | | | | | | | | | | | | | | | 0
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        | | | | | | | | | | | | | | | | | 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void lcd_shift_cursor(lcd_shift_e shift);

/**
    \brief �crit un seul caract�re ASCII � la position du curseur sur le LCD
    \param[in]  character Le caract�re � afficher
	Le LCD ne g�re pas les accents et les caract�res un peu sp�ciaux. L'exception
	� la r�gle sont deux fl�ches. C'est deux caract�res ne font pas parti du code
	ASCII. Pour les afficher, il est possible d'appeler la fonction avec l'une des
	deux d�finitions suivantes :
    - CHAR_RIGHT_ARROW
    - CHAR_LEFT_ARROW
	Cette fonction ne g�re pas non plus les caract�re de contr�le tel que :
		"\n", "\r", "\0", "\b", ...
*/
void lcd_write_char(char character);

/**
    \brief �crit une string � la position du curseur sur le LCD.
    \param[in] string La string � afficher
	Pour que cette fonction produise le r�sultat attendu, il faut absolument
	que la string soit termin�e par un caract�re nul ('\0'). Le caract�re nul
	n'est pas envoy� au LCD.
	Par exemple :
	`lcd_write_string("Hello World");` produira le r�sultat suivant :
         0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |H|e|l|l|o| |W|o|r|l|d| | | | | | 0
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        | | | | | | | | | | | | | | | | | 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	La fonction d�place automatiquement le curseur au d�but de la ligne suivante
	si la string � afficher est trop longue.
	Par exemple :
	`lcd_write_string("Hello World ABCDEF");` produira le r�sultat suivant :
         0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |H|e|l|l|o| |W|o|r|l|d| |A|B|C|D| 0
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |E|F| | | | | | | | | | | | | | | 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	La fonction sait g�rer deux caract�re de contr�le; "\n" et "\r"
	"\n" D�place le curseur d'une ligne vers le bas. Son comportement est identique
	� faire l'appel suivant : lcd_shift_cursor(LCD_SHIFT_DOWN);
	Par exemple :
	`lcd_write_string("Hello\nWorld");` produira le r�sultat suivant :
         0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |H|e|l|l|o| | | | | | | | | | | | 0
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        | | | | | |W|o|r|l|d| | | | | | | 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	"\r" Renvois le curseur au d�but de la ligne en cours. Son comportement est identique
	� faire l'appel suivant : lcd_shift_cursor(LCD_SHIFT_START);
	Par exemple :
	`lcd_write_string("Hello World\rABC");` produira le r�sultat suivant :
         0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |A|B|C|l|o| |W|o|r|l|d| | | | | | 0
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        | | | | | | | | | | | | | | | | | 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	Pour produire l'effet d'un retour de chariot, il faut combiner les deux caract�res
	de contr�le ensemble. Ceux qui viennent du monde UNIX ne trouveront rien de d�rangeant
	ici. Par contre, ceux qui viennent de Windows sont d'habitu�s de n'utiliser que "\n"
	pour produire cet effet. Premi�rement Windows est un syst�me d'exploitation sans saveur
	et mal fait. Deuxi�mement, la bonne mani�re c'est "\n\r" point final.
	Par exemple :
	`lcd_write_string("Hello\n\rWorld");` produira le r�sultat suivant :
         0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1
         0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |H|e|l|l|o| | | | | | | | | | | | 0
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |W|o|r|l|d| | | | | | | | | | | | 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void lcd_write_string(const char* string);


#endif // LCD_H_INCLUDED