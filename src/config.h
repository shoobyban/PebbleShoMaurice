#pragma once
#include <pebble.h>
#include "analog.h"
#include "plugin.h"
#include "plugin_digital.h"
#include "plugin_health.h"
#include "plugin_battp.h"
#include "plugin_battimg.h"
#include "plugin_bt.h"
#include "plugin_day.h"
#include "plugin_num.h"
#include "plugin_month.h"
#include "plugin_fulldate.h"
#include "plugin_cal.h"
#include "gbitmap_color_palette_manipulator.h"

#ifndef MAURICE_CONFIG
#define MAURICE_CONFIG

#define KEY_BG_COLOR 0
#define KEY_H_COLOR  1
#define KEY_HI_COLOR 2
#define KEY_M_COLOR  3
#define KEY_T_COLOR  4
#define KEY_RESET    5
#define KEY_S_COLOR  6
#define KEY_SECOND   7
#define KEY_DIGITAL  8
#define KEY_24       9
#define KEY_STEPS    10
#define KEY_BTVIBE   11
#define KEY_CALMODE  12

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

#ifdef PBL_COLOR
GColor8 bg_color;
GColor8 hour_color;
GColor8 hourin_color;
GColor8 minute_color;
GColor8 sec_color;
GColor8 text_color;
#else
GColor bg_color;
GColor hour_color;
GColor hourin_color;
GColor minute_color;
GColor sec_color;
GColor text_color;
#endif

int customcolor;
int show_second;
int show_digital;
int digital24;
int show_steps;
int bt_vibe;
int appstarted;
int calmode;

#endif

void config_load();
void config_received_handler(DictionaryIterator *iter, void *ctx);
void redraw_bg();
void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed);
void handle_second_tick(struct tm *tick_time, TimeUnits units_changed);