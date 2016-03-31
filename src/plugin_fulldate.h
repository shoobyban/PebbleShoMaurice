#pragma once

#ifndef PLUGIN_FULLDATE_H
#define PLUGIN_FULLDATE_H

void fulldate_load(Layer * window_layer, GRect rect);
void fulldate_refresh(struct tm * t);
void fulldate_redraw();
void fulldate_unload();

#endif /* PLUGIN_FULLDATE_H */

