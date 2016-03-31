#pragma once 
#ifndef PLUGIN_BATTIMG_H
#define PLUGIN_BATTIMG_H

void battimg_load(Layer * window_layer, GRect rect);
void battimg_redraw();
void battimg_refresh(BatteryChargeState charge_state);
void battimg_unload();

#endif /* PLUGIN_BATTIMG_H */

