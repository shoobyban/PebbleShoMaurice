#pragma once
#include <pebble.h>

void analog_init(Layer *window_layer);
void analog_load(Layer *window_layer);
void analog_redraw();
void analog_refresh();
void analog_refresh_sec();
void analog_unload();
void redisplay_plugins();