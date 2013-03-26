

!!==============================================================================
!!
!!	TYPES
!!	Rutinas biplataforma (Z/GLULX) para imprimir texto con diferentes estilos
!!
!!==============================================================================
!!
!!	File:			types.h
!!	Author(s):		J. Francisco Mart�n <jfm.lisaso@gmail.com>
!!	Language:		ES (Castellano)
!!	System:			Inform, INFSP 6
!!	Platform:		Z-Machine / Glulx
!!	Version:		4.0
!!	Released:		2013/03/25
!!
!!------------------------------------------------------------------------------
!!
!!	Copyright (c) 2013, J. Francisco Mart�n
!!
!!	Este programa es software libre: usted puede redistribuirlo y/o 
!!	modificarlo bajo los t�rminos de la Licencia P�blica General GNU 
!!	publicada por la Fundaci�n para el Software Libre, ya sea la versi�n 
!!	3 de la Licencia, o (a su elecci�n) cualquier versi�n posterior.
!!
!!	Este programa se distribuye con la esperanza de que sea �til, pero 
!!	SIN GARANT�A ALGUNA; ni siquiera la garant�a impl�cita MERCANTIL o 
!!	de APTITUD PARA UN PROP�SITO DETERMINADO. Consulte los detalles de 
!!	la Licencia P�blica General GNU para m�s informaci�n.
!!
!!	Deber�a haber recibido una copia de la Licencia P�blica General GNU 
!!	junto a este programa. En caso contrario, consulte
!!	<http://www.gnu.org/licenses/>.
!!
!!------------------------------------------------------------------------------
System_file;

Constant _RST_	= 0;
Constant _IST_	= 1;
Constant _MST_	= 2;
Constant _BST_	= 3;

Default	EMPHASIS_STYLE	_IST_;
Default	STRONG_STYLE	_BST_;
Default	CODE_STYLE		_MST_;
Default PARSER_STYLE	_RST_;
Default	PARSER_MSG1		"";
Default PARSER_MSG2		"";

!!==============================================================================
!!	Selecci�n de estilo de texto
!!------------------------------------------------------------------------------

#Ifdef	TARGET_ZCODE;
[ roman_style;	font on; style roman; ];		! Estilo: romana
[ italic_style;	font on; style underline; ];	! Estilo: italica
[ bold_style;	font on; style bold; ];			! Estilo: negrita
[ monospaced_style; font off; ];				! Estilo: monoespaciada
#Ifnot;	! TARGET_GLULX;
[ roman_style;	glk($0086, 0); ];				! Estilo: romana
[ italic_style;	glk($0086, 1); ];				! Estilo: italica
[ bold_style;	glk($0086, 3); ];				! Estilo: negrita
[ monospaced_style; glk($0086, 2); ];			! Estilo: monoespaciada
#Endif;

!! Estilo de mensajes del parser:

[ start_parser_style;
	switch (PARSER_STYLE) {
	_RST_:	roman_style();
	_IST_:	italic_style();
	_BST_:	bold_style();
	_MST_:	monospaced_style();
	}
	if (PARSER_MSG1 ~= 0) print (string) PARSER_MSG1;
];

[ end_parser_style;
	if (PARSER_MSG2 ~= 0) print (string) PARSER_MSG2;
	roman_style();
];

!!==============================================================================
!!	Imprimir texto en diferentes estilos
!!------------------------------------------------------------------------------

[ emph text;
	if (text == 0) return false;
	switch (EMPHASIS_STYLE) {
	_RST_:	roman_style();
	_IST_:	italic_style();
	_BST_:	bold_style();
	_MST_:	monospaced_style();
	}
	print (string) text;
	roman_style();
	return true;
];

[ strong text;
	if (text == 0) return false;
	switch (STRONG_STYLE) {
	_RST_:	roman_style();
	_IST_:	italic_style();
	_BST_:	bold_style();
	_MST_:	monospaced_style();
	}
	print (string) text;
	roman_style();
	return true;
];

[ code text;
	if (text == 0) return false;
	switch (CODE_STYLE) {
	_RST_:	roman_style();
	_IST_:	italic_style();
	_BST_:	bold_style();
	_MST_:	monospaced_style();
	}
	print (string) text;
	roman_style();
	return true;
];

[ parser text;
	if (text == 0) return false;
	start_parser_style();
	print (string) text;
	end_parser_style();
	return true;
];

!! Alias:

[ em text; return emph(text); ];


