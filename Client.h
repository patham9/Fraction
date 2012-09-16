#ifndef Client_H
#define Client_H

#include "Game.h"
#include "Hamlib/Hgui.h"

void *client_thread(void *arg);
void client_send_mouse_down(EventArgs *e);
void client_send_key_up(EventArgs *e);
void client_send_button(int i);
int client_client();
int client_player();
int get_step();

#endif
