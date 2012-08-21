#ifndef Draw_H
#define Draw_H

#include "Hamlib/Hauto.h"
#include "Hamlib/Hrend.h"

//very mighty for "visual debugging" the cellular automat! but slows rendering down!
#define debugdraw_enabled_default 0
#define debugdraw_property_r op_max(0,(1.0-((float)c->lastchange)/10.0))
#define debugdraw_property_b c->wateramount/10
#define debugdraw_property_g c->forest_distance/worldsize

void draw();
void Draw_Set_HeightmapChanged();
void Draw_Set_DebugDraw(int s);
int Draw_Get_DebugDraw();

#endif
