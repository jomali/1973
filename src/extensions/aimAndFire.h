

!!==============================================================================
!!
!!	AIM AND FIRE
!!	Sistema de apuntado y disparo QTE (Quick Time Event)
!!
!!==============================================================================
!!
!!	File:			aimAndFire.h
!!	Author(s):		J. Francisco Martín <jfm.lisaso@gmail.com>
!!	Language:		ES (Castellano)
!!	System:			Inform-INFSP 6
!!	Platform:		Máquina-Z / GLULX
!!	Version:		3.1
!!	Released:		2014/02/06
!!
!!------------------------------------------------------------------------------
!!
!!	# HISTORIAL DE VERSIONES
!!
!!	3.1: 2014/02/06	Modo de depuración.
!!	3.0: 2012/07/05	Versión preliminar de la rutina updateGridPosition_SHM() 
!!					para mover la retícula con las ecuaciones del Movimiento 
!!					Armónico Simple (Simple Harmonic Motion). Ver NOTA #1.
!!	2.0: 2012		Refactorización del código, correcciones a la lógica y 
!!					adición de comentarios.
!!	1.0: 2012		Versión original de la librería.
!!
!!------------------------------------------------------------------------------
!!
!!	Copyright (c) 2012, 2014, J. Francisco Martín
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
!!	# INSTRUCCIONES DE INSTALACIÓN
!!
!!	Para utilizar las funciones de tiempo real de la librería se debe incluir 
!!	el siguiente punto de entrada glk en el código del relato interactivo (las 
!!	funciones de tiempo real funcionan únicamente en GLULX):
!!
!!		[ HandleGlkEvent ev; 
!!			if (ev-->0 == 1) ! evtype_Timer
!!				 AimingManager.run();
!!		];
!!
!!------------------------------------------------------------------------------
System_file;

!! Descomentar para obtener información de depuración:
!Constant DEBUG_AIMANDFIRE;


!!==============================================================================
!!	Objeto gestor del sistema de apuntado.
!!------------------------------------------------------------------------------

#Ifdef	TARGET_GLULX;
Object	AimingManager
 with	!!----------------------------------------------------------------------
		!! Atributos:
		!!----------------------------------------------------------------------
		timer_frequency	10,		! Freq. de los eventos del timer glk
		delay			100,	! Retraso tras detener la animación
		counter			0,		! Contador
		end_flag		false,	! Indica el final de la operación de apuntado
		width			0,		! Ancho de la línea de apuntado
		grid			"[+]",	! Retícula de apuntado
		grid_length		3,		! Longitud (en caracteres) de la retícula
		grid_position	0,		! Guarda la posición de la retícula
		grid_direction	0,		! Guarda el sentido de movimiento de la retícula
		activado		false,
		!!----------------------------------------------------------------------
		!! Operación principal del controlador. Invocada en cada ciclo del 
		!! reloj glk. Imprime la animación de la retícula de apuntado y su 
		!! posición final una vez detenida.
		!!----------------------------------------------------------------------
		run [;
			if (~~self.activado) return false;
			!! Obtiene el ancho de la ventana de estado:
			glk($0025, statusBar.winid, gg_arguments, gg_arguments+4);
			self.width = gg_arguments-->0;
			!! Apuntado finalizado. Muestra el resultado durante unos instantes:
			if (self.end_flag) self.finalDelay(); 
			!! Calcula posición de retícula e imprime la animación:
			else {
				self.updateGridPosition_LM(); ! Linear Motion
				self.drawAimingLine();
			}
		],
		!!----------------------------------------------------------------------
		!! Inicializa los atributos y lanza los eventos de reloj glk. Se se 
		!! invoca sin el parámetro flag (con con flag==false), la animación de 
		!! de la retícula comenzará desde el punto y en el sentido en que fue 
		!! detenida la última vez. En caso contrario, la animación comenzará 
		!! desde el extremo izquierdo de la ventana de estado y moviéndose 
		!! hacia la derecha.
		!!	@param flag
		!!----------------------------------------------------------------------
		initialize [ flag;
			if (flag) {
				self.grid_position  = 0;
				self.grid_direction = 0;
			}
			self.counter = 0;
			self.end_flag = false;
			self.activado = true;
			!! Lanza los eventos de reloj glk
			glk(214, self.timer_frequency); ! glk_request_timer_events(t_freq);
		],
		!!----------------------------------------------------------------------
		!! Retorna la posición de la retícula.
		!!	@return int grid_position
		!!----------------------------------------------------------------------
		getGridPosition [; return self.grid_position; ],
		!!----------------------------------------------------------------------
		!! Retorna el ancho de la barra de apuntado.
		!!	@return int width
		!!----------------------------------------------------------------------
		getAimingLineWidth [; return self.width; ],
		!!----------------------------------------------------------------------
		!! Indica el final de la operación de apuntado.
		!!----------------------------------------------------------------------
		aimingEnded [; self.end_flag = true; ],
		!!----------------------------------------------------------------------
		!! Calcula la posición de la retícula (Movimiento Rectilíneo Uniforme)
		!!----------------------------------------------------------------------
		updateGridPosition_LM [; ! Linear Motion
			!! Sentido del movimiento: hacia la derecha
			if (self.grid_direction == 0) {
				if (self.grid_position < self.width - (self.grid_length+1)) {
					self.grid_position++;
				}
				else {
					self.grid_direction = 1;
					self.grid_position--;
				}
			}
			!! Sentido del movimiento: hacia la izquierda
			else {
				if (self.grid_position > 1) {
					self.grid_position--;
				}
				else {
					self.grid_direction = 0;
					self.grid_position++;
				}
			}
		],
		!!----------------------------------------------------------------------
		!! Calcula la posición de la retícula (Movimiento Armónico Simple). 
		!! XXX - NOTA #1: En el estado actual, la animación no se imprime 
		!! correctamente, por eso la rutina está comentada.
		!!----------------------------------------------------------------------
!		updateGridPosition_SHM [ i ini sini aux gp; ! Simple Harmonic Motion
!			aux = WIN_WIDTH/2;
!			@numtof aux ini; ! ini: centro de la ventana
!			@numtof GRID_COUNTER i; ! i: contador
!			@sin i sini; ! sini: seno de GRID_COUNTER
!			@fmul ini sini aux; ! aux: (WIN_WIDTH/2)*sin(GRID_COUNTER)
!
!			if (GRID_MOVEMENT == 0) @fadd ini aux gp;
!			else @fsub ini aux gp;
!			@ftonumn gp GRID_POS;
!
!			GRID_COUNTER++;
!			if (GRID_COUNTER == 180)
!			{	GRID_COUNTER = 0;
!				if (GRID_MOVEMENT == 0) GRID_MOVEMENT = 1;
!				else GRID_MOVEMENT = 0;
!			}
!		],
		!!----------------------------------------------------------------------
		!! Dibuja la animación de apuntado
		!!----------------------------------------------------------------------
		drawAimingLine [;
			! If we have no status window, we must not try to redraw it.
			if (statusBar.winid == 0) {
				print "** AimingManager error. No hay ventana de estado. **^";
				return;
			}
			glk($002F, statusBar.winid); ! set_window
			StatusLineHeight(gg_statuswin_size);
			glk($002A, statusBar.winid); ! window_clear
			! Se imprimen las guías:
			glk($002B, statusBar.winid, (self.width/2)-3, 0); ! window_move_cursor
			print "·";
			glk($002B, statusBar.winid, (self.width/2)+3, 0); ! window_move_cursor
			print "·";
			! Se imprime la retícula:
			glk($002B, statusBar.winid, self.getGridPosition(), 0);
			print (string) self.grid;
			glk($002F, gg_mainwin); ! set_window
		],
		!!----------------------------------------------------------------------
		!! Una vez detenida la animación de apuntado aún se muestra la posición 
		!! final de la retícula durante unos momentos (tiempo establecido por 
		!! el atributo delay). Al cabo, se detienen los eventos del timer y se 
		!! imprime la barra de estado normal.
		!!----------------------------------------------------------------------
		finalDelay [;
			self.drawAimingLine();
			self.counter++;
			if (self.counter > self.delay) {
				glk(214, 0); ! glk_request_timer_events(0);
				self.activado = false;
				DrawStatusLine();
			}
		],
;
#Endif;	! TARGET_GLULX;


!!==============================================================================
!!	Lanza el sistema de apuntado y disparo:
!!	@return int: 
!!		-2, (respuesta del sistema en Máquina-Z), 
!!		-1, (fallo),
!!		result (distancia entre la retícula y el centro).
!!------------------------------------------------------------------------------
!! TODO: controlar errores relativos a distintos anchos de ventana

[ AimAndFire grid_pos width result;
	#Ifdef	TARGET_ZCODE;

	grid_pos = 0; width = 0; result = 0; ! Suprime advertencias del compilador
	#Ifdef DEBUG_AIMANDFIRE;
	print "** AimAndFire: Apuntado y disparo para máquina-Z. **^";
	#Endif; ! DEBUG_AIMANDFIRE;
	return -2;

	#Ifnot;	! TARGET_GLULX;

	AimingManager.initialize();
	KeyCharPrimitive(); ! Pulsación de tecla. Fin de la animación
	AimingManager.aimingEnded();
	grid_pos = AimingManager.getGridPosition() + 1;
	width	 = AimingManager.getAimingLineWidth();
	#Ifdef DEBUG_AIMANDFIRE;
	print "** Posición de la retícula: ", grid_pos, "/", width, " **^";
	print "** ", grid_pos, " - ", (width/2), ", = ", grid_pos-(width/2), " **^";
	#Endif; ! DEBUG_AIMANDFIRE;
	result = grid_pos - (width/2);
	if (result < 0 ) return -result;
	else return result;

	#Endif;	! TARGET_
];


