#include "config.h"

static char s_month_buffer[10];
TextLayer *s_month_label;
int _month_loaded = 0;

void month_load(Layer * window_layer, GRect rect) {
    if (!_month_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "month_load");
        s_month_buffer[0] = '\0';
        s_month_label = text_layer_create(rect);
        text_layer_set_text(s_month_label, s_month_buffer);
        text_layer_set_background_color(s_month_label, GColorClear);
        text_layer_set_text_color(s_month_label, text_color);
        text_layer_set_font(s_month_label, fonts_get_system_font(FONT_KEY_GOTHIC_18));

        layer_add_child(window_layer, text_layer_get_layer(s_month_label));
        _month_loaded = 1;
        register_plugin((load_fn)month_load,(void_fn)month_redraw,(void_fn)month_unload);
    }
}

void month_redraw() {
    if (_month_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "month_redraw");
        text_layer_set_background_color(s_month_label, bg_color);
        text_layer_set_text_color(s_month_label, text_color);
    }
}

void month_refresh(struct tm * t) {
    if (_month_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "month_refresh");
        text_layer_set_background_color(s_month_label, bg_color);
        strftime(s_month_buffer, sizeof ( s_month_buffer), "%b", t);
        text_layer_set_text(s_month_label, s_month_buffer);
    }
}

void month_unload() {
    if (_month_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "month_unload");
        text_layer_destroy(s_month_label);
        _month_loaded = 0;
    }
}

int month_loaded() {
    return _month_loaded;
}