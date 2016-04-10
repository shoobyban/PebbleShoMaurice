#include "config.h"

static char s_day_buffer[6];
TextLayer *s_day_label;
int _day_loaded = 0;

void day_load(Layer * window_layer, GRect rect) {
    if (!_day_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "day_load");
        s_day_buffer[0] = '\0';
        s_day_label = text_layer_create(rect);
        text_layer_set_text(s_day_label, s_day_buffer);
        text_layer_set_background_color(s_day_label, bg_color);
        text_layer_set_text_color(s_day_label, text_color);
        text_layer_set_font(s_day_label, fonts_get_system_font(FONT_KEY_GOTHIC_18));
        layer_add_child(window_layer, text_layer_get_layer(s_day_label));
        _day_loaded = 1;
        register_plugin((load_fn)day_load,(void_fn)day_redraw,(void_fn)day_unload);
    }
}

void day_redraw() {
    if (_day_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "day_redraw");
        text_layer_set_background_color(s_day_label, bg_color);
        text_layer_set_text_color(s_day_label, text_color);
    }
}

void day_refresh(struct tm * t) {
    if (_day_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "day_refresh");
        strftime(s_day_buffer, sizeof (s_day_buffer), "%a", t);
        text_layer_set_text(s_day_label, s_day_buffer);
    }
}

void day_unload() {
    if (_day_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "day_unload");
        text_layer_destroy(s_day_label);
        _day_loaded = 0;
    }
}

int day_loaded() {
    return _day_loaded;
}
