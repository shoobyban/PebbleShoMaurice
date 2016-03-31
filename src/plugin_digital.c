#include "plugin_digital.h"

static TextLayer *time_layer;
int digital_loaded = 0;

void digital_load(Layer * window_layer, GRect rect) {
    if (show_digital && !digital_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "digital_load");
        GFont font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_22));
        
        time_layer = text_layer_create(rect);
        text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
        text_layer_set_text_color(time_layer, text_color);
        text_layer_set_background_color(time_layer, GColorClear);
        text_layer_set_font(time_layer, font);
        layer_add_child(window_layer, text_layer_get_layer(time_layer));
        digital_loaded = 1;
        register_plugin((load_fn)digital_load,(void_fn)digital_redraw,(void_fn)digital_unload);
    }
}

void digital_refresh(GContext * ctx, struct tm *t) {
    if (digital_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "digital_refresh");
        text_layer_set_text_color(time_layer, text_color);
        text_layer_set_background_color(time_layer, bg_color);
        static char time_text[] = "00:00";
        if (digital24) {
            strftime(time_text, sizeof (time_text), "%R", t);
        } else {
            strftime(time_text, sizeof (time_text), "%l:%M", t);
        }
        text_layer_set_text(time_layer, time_text);
    }
}

void digital_redraw() {
    if (digital_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "digital_redraw");
        text_layer_set_text_color(time_layer, text_color);
        text_layer_set_background_color(time_layer, bg_color);    
    }
}

void digital_unload() {
    if (digital_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "digital_unload");
        text_layer_destroy(time_layer);
        digital_loaded = 0;
    }
}
