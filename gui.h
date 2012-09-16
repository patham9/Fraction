#ifndef GUI_H
#define GUI_H

#include "Hamlib/Hauto.h"
#include "Hamlib/Hutil.h" 
#include "Hamlib/Hinput.h"
#include "Hamlib/Hgui.h" 

void gui_mouse_down(int player,EventArgs *e);
void gui_key_up(int player,EventArgs *e);
void gui_button(int player,int i);

#endif
