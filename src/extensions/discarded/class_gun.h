! Inform / INFSP 6


!!==============================================================================
!!
!!	GUNS
!!	Armas de fuego simples
!!
!!==============================================================================
!!
!!	File:			class_gun.h
!!	Author(s):		J. Francisco Martín (jfm.lisaso@gmail.com)
!!	Language:		ES (Castellano)
!!	System:			Inform/INFSP 6
!!	Platform:		Máquina-Z / Glulx
!!	Version:		1.0
!!	Released:		2010/04/05
!!
!!------------------------------------------------------------------------------
!!
!!	Copyright (c) 2010, J. Francisco Martín
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
!!
!! Calibres de pistola ordenados por rango (mm. y pulgadas)
!!		- 2 mm (.079+ caliber) pequeño
!!		- 3 mm (.118+ caliber)
!!		- 4 mm (.157+ caliber)
!!		- 5 mm (.197+ caliber)
!!		- 6 mm (.236+ caliber) medio
!!		- 7 mm (.276+ caliber)
!!		- 8 mm (.315+ caliber)
!!		- 9 mm (.354+ caliber)
!!		- 10 mm (.394+ caliber) grande
!!		- 11 mm (.433+ caliber)
!!		- 12 mm (.472+ caliber)
!!		- 13 mm (.511+ caliber)
!! [Fuente: Wikipedia]
!!
!!------------------------------------------------------------------------------
System_file;

Class	Gun
 with	caliber 9,	!! Calibre medio normal de 9mm.
		ammo 0;		!! Munición

[ Firearm obj x;
	if ( obj == nothing ) objectloop( x in player ) if (x ofclass Gun) obj = x;
	if ( obj == nothing ) {
		print "Necesitas un arma de fuego para hacer eso.^";
		return false;
	}
	if ( obj ofclass Gun ) {
		if ( obj.ammo > 0 ) {
			second = obj;
			if ( noun == u_obj or d_obj ) {
				second.ammo--;
				print "Apuntas hacia ";
				if ( noun == u_obj ) print (name) u_obj;
				else print (name) d_obj;
				print " con ", (the) second ," y disparas: ¡PANG!^";
				return false;
			}
			return true;
		}
		print "No te quedan balas en ", (the) obj ,".^";
		return false;
	}
	print "Sólo puedes hacer eso con armas de fuego.^";
	return false;
];

!!------------------------------------------------------------------------------

Verb	'dispara'
	*								-> Shoot
	* noun							-> Shoot
	* noun 'con' noun				-> Shoot
	* noun 'a//' noun				-> Shoot reverse
	* noun 'al' noun				-> Shoot
	* 'a//' noun					-> Shoot
	* 'a//' noun 'con' noun			-> Shoot reverse
	* 'al' noun						-> Shoot
	* 'al' noun 'con' noun			-> Shoot reverse
	* 'hacia' noun					-> Shoot
	* 'hacia' noun 'con' noun		-> Shoot reverse
;

Verb	'apunta' 'amartilla' 'encañona'
	*								-> ShootHelp
	* noun							-> ShootHelp
	* 'a//' noun					-> ShootHelp
	* 'a//' noun 'con' noun			-> ShootHelp
	* 'al' noun						-> ShootHelp
	* 'al' noun 'con' noun			-> ShootHelp
	* noun 'con' noun				-> ShootHelp
	* noun 'a//' noun				-> ShootHelp
	* noun 'al' noun				-> ShootHelp
;

!!------------------------------------------------------------------------------

Default NO_SHOOT "Mejor no.";

[ ShootSub;
	if ( noun == nothing ) noun = u_obj;
	if ( Firearm( second ) == true ) print (string) NO_SHOOT;
	return true;
];

[ ShootHelpSub;
	print_ret "Para usar un arma de fuego, teclea simplemente 
	DISPARA (a) OBJETIVO (con ARMA).";
];


