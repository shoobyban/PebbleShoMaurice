#include "config.h"

static char s_fulldate_buffer[20];
TextLayer *s_fulldate_label;
int _fulldate_loaded = 0;

void fulldate_load(Layer * window_layer, GRect rect) {
    if (!_fulldate_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "fulldate_load");
        s_fulldate_buffer[0] = '\0';
        s_fulldate_label = text_layer_create(rect);
        text_layer_set_text(s_fulldate_label, s_fulldate_buffer);
        text_layer_set_background_color(s_fulldate_label, bg_color);
        text_layer_set_text_color(s_fulldate_label, text_color);
        text_layer_set_font(s_fulldate_label, fonts_get_system_font(FONT_KEY_GOTHIC_18));
        layer_add_child(window_layer, text_layer_get_layer(s_fulldate_label));
        _fulldate_loaded = 1;
        register_plugin((load_fn)fulldate_load,(void_fn)fulldate_redraw,(void_fn)fulldate_unload);
    }
}

void fulldate_redraw() {
    if (_fulldate_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "fulldate_redraw");
        text_layer_set_background_color(s_fulldate_label, bg_color);
        text_layer_set_text_color(s_fulldate_label, text_color);
    }
}

void fulldate_refresh(struct tm * t) {
    if (_fulldate_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "fulldate_refresh");
        strftime(s_fulldate_buffer, sizeof (s_fulldate_buffer), "%a %d %b", t);
        text_layer_set_text(s_fulldate_label, s_fulldate_buffer);
    }
}

void fulldate_unload() {
    if (_fulldate_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "fulldate_unload");
        text_layer_destroy(s_fulldate_label);
        _fulldate_loaded = 0;
    }
}

int fulldate_loaded() {
    return _fulldate_loaded;
}
