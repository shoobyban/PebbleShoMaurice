#pragma once 
#ifndef PLUGIN_NUM_H
#define PLUGIN_NUM_H

void num_load(Layer * window_layer, GRect rect);
void num_refresh(struct tm * t);
void num_redraw();
void num_unload();

#endif /* PLUGIN_NUM_H */

