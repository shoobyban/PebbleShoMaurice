#pragma once
#include <pebble.h>
#include "config.h"

void digital_load(Layer * window_layer, GRect rect);
void digital_refresh();
void digital_redraw();
void digital_unload();
int digital_loaded();
