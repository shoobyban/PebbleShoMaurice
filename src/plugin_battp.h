#pragma once
#ifndef PLUGIN_BATTP_H
#define PLUGIN_BATTP_H

void battp_load(Layer * window_layer,GRect rect);
void battp_redraw();
void battp_refresh(BatteryChargeState charge_state);
void battp_unload();

#endif /* PLUGIN_BATTP_H */

