#pragma once
#ifndef PLUGIN_CAL_H
#define PLUGIN_CAL_H

#define CALMODE_NORMAL 1
#define CALMODE_MBATT  2
#define CALMODE_FULL   3
#define CALMODE_CAL    4

void cal_load(Layer * window_layer, GRect rect);
void cal_refresh(struct tm * t);
void cal_redraw();
void cal_unload();

#endif /* PLUGIN_CAL_H */

