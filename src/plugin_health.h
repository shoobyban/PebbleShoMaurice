#pragma once

#ifndef PLUGIN_HEALTH_H
#define PLUGIN_HEALTH_H

void health_load(Layer * window_layer, GRect rect);
void health_redraw();
void health_unload();
int health_loaded();

#endif /* PLUGIN_HEALTH_H */
