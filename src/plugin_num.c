#include "config.h"

static char s_num_buffer[4];
TextLayer *s_num_label;
int _num_loaded = 0;

void num_load(Layer * window_layer, GRect rect) {
    if (!_num_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "num_load");
        s_num_buffer[0] = '\0';
        s_num_label = text_layer_create(rect);
        text_layer_set_text(s_num_label, s_num_buffer);
        text_layer_set_background_color(s_num_label, bg_color);
        text_layer_set_text_color(s_num_label, text_color);
        text_layer_set_font(s_num_label, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));

        layer_add_child(window_layer, text_layer_get_layer(s_num_label));
        _num_loaded = 1;
        register_plugin((load_fn)num_load,(void_fn)num_redraw,(void_fn)num_unload);
    }
}

void num_refresh(struct tm * t) {
    if (_num_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "num_refresh");
        strftime(s_num_buffer, sizeof(s_num_buffer), "%d", t);
        text_layer_set_text(s_num_label, s_num_buffer);
    }
}

void num_redraw() {
    if (_num_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "num_redraw");
        text_layer_set_background_color(s_num_label, bg_color);
        text_layer_set_text_color(s_num_label, text_color);
    }
}

void num_unload() {
    if (_num_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "num_unload");
        text_layer_destroy(s_num_label);
        _num_loaded = 0;
    }
}

int num_loaded() {
    return _num_loaded;
}
