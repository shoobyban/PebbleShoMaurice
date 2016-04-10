#include "config.h"

static char s_day_buffer[6];
TextLayer *s_day_label;
int _cal_loaded = 0;

void cal_load(Layer * window_layer, GRect rect) {
    if (!_day_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "cal_load");
        s_day_buffer[0] = '\0';
        s_day_label = text_layer_create(rect);
        text_layer_set_text(s_day_label, s_day_buffer);
        text_layer_set_background_color(s_day_label, bg_color);
        text_layer_set_text_color(s_day_label, text_color);
        text_layer_set_overflow_mode(steps_layer, GTextOverflowModeWordWrap);
        text_layer_set_font(s_day_label, fonts_get_system_font(FONT_KEY_GOTHIC_18));
        layer_add_child(window_layer, text_layer_get_layer(s_day_label));
        _cal_loaded = 1;
        register_plugin((load_fn)cal_load,(void_fn)cal_redraw,(void_fn)cal_unload);
    }
}

void cal_redraw() {
    if (_cal_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "day_redraw");
        text_layer_set_background_color(s_day_label, bg_color);
        text_layer_set_text_color(s_day_label, text_color);
    }
}

void cal_refresh(struct tm * t) {
    if (_cal_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "cal_refresh");
        strftime(s_day_buffer, sizeof (s_day_buffer), "%a %b", t);
        text_layer_set_text(s_day_label, s_day_buffer);
    }
}

void cal_unload() {
    if (_cal_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "cal_unload");
        text_layer_destroy(s_day_label);
        _cal_loaded = 0;
    }
}

