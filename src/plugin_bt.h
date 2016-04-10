#pragma once

#ifndef PLUGIN_BT_H
#define PLUGIN_BT_H

void bt_load(Layer * window_layer, GRect rect);
void bt_redraw();
void bt_refresh(bool connected);
void bt_unload();
int bt_loaded();

#endif /* PLUGIN_BT_H */

