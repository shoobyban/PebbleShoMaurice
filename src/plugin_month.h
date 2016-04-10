#pragma once 

#ifndef PLUGIN_MONTH_H
#define PLUGIN_MONTH_H

void month_load(Layer * window_layer, GRect rect);
void month_refresh(struct tm * t);
void month_redraw();
void month_unload();
int month_loaded();

#endif /* PLUGIN_MONTH_H */

