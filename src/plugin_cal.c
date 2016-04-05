#include "config.h"

static char s_cal_buffer[12];
TextLayer *s_cal_label;
int cal_loaded = 0;

void cal_load(Layer * window_layer, GRect rect) {
    if (!cal_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "cal_load");
        s_cal_buffer[0] = '\0';
        s_cal_label = text_layer_create(rect);
        text_layer_set_text(s_cal_label, s_cal_buffer);
        text_layer_set_background_color(s_cal_label, bg_color);
        text_layer_set_text_color(s_cal_label, text_color);
        text_layer_set_overflow_mode(s_cal_label, GTextOverflowModeWordWrap);
        text_layer_set_text_alignment(s_cal_label, GTextAlignmentCenter);
        text_layer_set_font(s_cal_label, fonts_get_system_font(FONT_KEY_GOTHIC_14));
        layer_add_child(window_layer, text_layer_get_layer(s_cal_label));
        cal_loaded = 1;
        register_plugin((load_fn)cal_load,(void_fn)cal_redraw,(void_fn)cal_unload);
    }
}

void cal_redraw() {
    if (cal_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "cal_redraw");
        text_layer_set_background_color(s_cal_label, bg_color);
        text_layer_set_text_color(s_cal_label, text_color);
    }
}

void cal_refresh(struct tm * t) {
    if (cal_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "cal_refresh");
        strftime(s_cal_buffer, sizeof (s_cal_buffer), "%d\n%b", t);
        text_layer_set_text(s_cal_label, s_cal_buffer);
    }
}

void cal_unload() {
    if (cal_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "cal_unload");
        text_layer_destroy(s_cal_label);
        cal_loaded = 0;
    }
}

