! Inform / INFSP 6


!!==============================================================================
!!
!!	TYPES
!!	Rutinas biplataforma para imprimir texto en diferentes estilos
!!
!!==============================================================================
!!
!!	File:			types.h
!!	Author(s):		J. Francisco Martín (jfm.lisaso@gmail.com)
!!	Language:		ES (Castellano)
!!	System:			Inform/INFSP 6
!!	Platform:		Máquina-Z / Glulx
!!	Version:		3.3
!!	Released:		2012/06/01
!!
!!------------------------------------------------------------------------------
!!
!!	Copyright (c) 2012, J. Francisco Martín
!!
!!	Este programa es software libre: usted puede redistribuirlo y/o 
!!	modificarlo bajo los términos de la Licencia Pública General GNU 
!!	publicada por la Fundación para el Software Libre, ya sea la versión 
!!	3 de la Licencia, o (a su elección) cualquier versión posterior.
!!
!!	Este programa se distribuye con la esperanza de que sea útil, pero 
!!	SIN GARANTÍA ALGUNA; ni siquiera la garantía implícita MERCANTIL o 
!!	de APTITUD PARA UN PROPÓSITO DETERMINADO. Consulte los detalles de 
!!	la Licencia Pública General GNU para más información.
!!
!!	Debería haber recibido una copia de la Licencia Pública General GNU 
!!	junto a este programa. En caso contrario, consulte
!!	<http://www.gnu.org/licenses/>.
!!
!!------------------------------------------------------------------------------
System_file;

Constant _RST_	= 0;
Constant _IST_	= 1;
Constant _BST_	= 3;
Constant _PST_	= 2;

Default	PARSERST_TYPE _RST_;
Default	PARSERST_MSG1 "";
Default	PARSERST_MSG2 "";

!!==============================================================================
!! Seleccionar estilo de texto
!!------------------------------------------------------------------------------

#Ifdef	TARGET_ZCODE;
[ roman_style;	font on; style roman; ];	! Seleccionar estilo - romana
[ italic_style;	style underline; ];			! Seleccionar estilo - cursiva
[ bold_style;	style bold; ];				! Seleccionar estilo - negrita
[ proportional_style;	font off; ];		! Seleccionar estilo - proporcional
#Ifnot;
[ roman_style;	glk($0086, 0); ];			! Seleccionar estilo - romana
[ italic_style;	glk($0086, 1); ];			! Seleccionar estilo - cursiva
[ bold_style;	glk($0086, 3); ];			! Seleccionar estilo - negrita
[ proportional_style;	glk($0086, 2); ];	! Seleccionar estilo - proporcional
#Endif;

[ parser_style;								! Para mensajes del 'parser'
	switch ( PARSERST_TYPE ) {
		_RST_:	roman_style();
		_IST_:	italic_style();
		_BST_:	bold_style();
		_PST_:	proportional_style();
	}
];
[ start_parser_style;
	parser_style();
	if ( PARSERST_MSG1 ~= 0 ) print (string) PARSERST_MSG1;
];
[ end_parser_style;
	if ( PARSERST_MSG2 ~= 0 ) print (string) PARSERST_MSG2;
	roman_style();
];

!!==============================================================================
!! Imprimir texto en diferentes estilos
!!------------------------------------------------------------------------------

#Ifdef	TARGET_ZCODE;
[ italic_type text;
    if (text==0) return false;
    style underline;
    print (string) text;
    style roman;
    return true;
];
[ bold_type text;
    if (text==0) return false;
    style bold;
    print (string) text;
    style roman;
    return true;
];
[ proportional_type text;
    if (text==0) return false;
    font off;
    print (string) text;
    font on;
    return true;
];
#Ifnot;	!! TARGET_GLULX
[ italic_type text;
    if (text==0) return false;
    glk($0086, 1);
    print (string) text;
    glk($0086, 0);
    return true;
];
[ bold_type text;
    if (text==0) return false;
    glk($0086, 3);
    print (string) text;
    glk($0086, 0);
    return true;
];
[ proportional_type text;
    if (text==0) return false;
    glk($0086, 2);
    print (string) text;
    glk($0086, 0);
    return true;
];
#Endif;

[ parser_type text;
	if (text==0) return false;
	start_parser_style();
	print (string) text;
	end_parser_style();
	return true;
];

!! Abreviaciones para las rutinas anteriores
[ ity text; return italic_type(text); ];
[ bty text; return bold_type(text); ];
[ pty text; return proportional_type(text); ];
[ aty text; return parser_type(text); ];


