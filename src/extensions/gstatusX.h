! gstatusX.h -- Status window module for GWindows
!
! GStatusWinX crea una linea de estado que imprime el nombre de la localidad 
! actual del protagonista, centrado.
!
! Inherits from: TextGrid 
!
!--------------------------------------------------------------------------

Ifndef GW_GSTATUS_H;
system_file;
Constant GW_GSTATUS_H;
Class GStatusXWin
 class TextGrid,
 with
 redraw [; self.update(); ], ! CORREGIDO POR ELIUK BLAU
 update [ pos;
    if (location == 0 || player == 0 || parent(player) == 0) return;
    glk_set_window(self.winid);
	pos = (self.width - 
		PrintToBuffer(_AlmacenDeTexto_, 160, PrintLocationName)) / 2;
    glk_window_clear(self.winid);
    glk_window_move_cursor(self.winid, pos, 0);
	PrintLocationName();
 ],
 split 1,
 split_dir winmethod_Above,
 stylehints
  style_Normal stylehint_Reversecolor 1,
 has abssplit;

Endif;
