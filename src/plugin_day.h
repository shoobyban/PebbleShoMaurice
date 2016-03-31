#pragma once

#ifndef PLUGIN_DAY_H
#define PLUGIN_DAY_H

void day_load(Layer * window_layer, GRect rect);
void day_refresh(struct tm * t);
void day_redraw();
void day_unload();

#endif /* PLUGIN_DAY_H */

