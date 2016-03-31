#include <pebble.h>
#include "config.h"

int battp_loaded = 0;
TextLayer *battp_layer;

void battp_load(Layer * window_layer, GRect rect) {
    if (!battp_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "battp_load");
        battp_layer = text_layer_create(rect);
        text_layer_set_text_color(battp_layer, text_color);
        text_layer_set_background_color(battp_layer, bg_color);
        text_layer_set_font(battp_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
        layer_add_child(window_layer, text_layer_get_layer(battp_layer));
        battp_loaded = 1;
        register_plugin((load_fn)battp_load,(void_fn)battp_redraw,(void_fn)battp_unload);
    }
}

void battp_redraw() {
    if (battp_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "battp_redraw");
        text_layer_set_text_color(battp_layer, text_color);
        text_layer_set_background_color(battp_layer, bg_color);
    }
}

void battp_refresh(BatteryChargeState charge_state) {
    if (battp_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "battp_refresh");
        static char battp_buffer [] = "+100%";
        snprintf(battp_buffer, sizeof (battp_buffer), "%s%d%%", charge_state.is_charging ? "+" : "", charge_state.charge_percent);
        text_layer_set_text(battp_layer, battp_buffer);
    }
}

void battp_unload() {
    if (battp_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "battp_unload");
        text_layer_destroy(battp_layer);
        battp_loaded = 0;
    }
}