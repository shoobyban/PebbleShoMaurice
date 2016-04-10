#include <pebble.h>
#include "config.h"

int _battimg_loaded = 0;
BitmapLayer *battimg_layer;
GBitmap *battery_images[22];
BatteryChargeState battery_state;

void battimg_load(Layer * window_layer, GRect rect) {
    if (!_battimg_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "battimg_load");
        for (int i = 0; i < 22; i++) {
            battery_images[i] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_0 + i);
        }
        battimg_layer = bitmap_layer_create(rect);
        layer_add_child(window_layer, bitmap_layer_get_layer(battimg_layer));
        _battimg_loaded = 1;
        register_plugin((load_fn)battimg_load,(void_fn)battimg_redraw,(void_fn)battimg_unload);
    }
}

void battimg_redraw() {
    if (_battimg_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "battimg_redraw");
        GBitmap * bmp = battery_images[(battery_state.is_charging ? 11 : 0) + min(battery_state.charge_percent / 10, 10)];
        #ifdef PBL_COLOR
        if (customcolor) {
            replace_gbitmap_color(GColorBlack, bg_color, bmp, battimg_layer);
            replace_gbitmap_color(GColorWhite, text_color, bmp, battimg_layer);
        }
        #endif
        bitmap_layer_set_bitmap(battimg_layer, bmp);
    }
}

void battimg_refresh(BatteryChargeState charge_state) {
    if (_battimg_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "battimg_refresh");
        battery_state = charge_state;
        battimg_redraw();
    }
}

void battimg_unload() {
    if (_battimg_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "battimg_unload");
        bitmap_layer_destroy(battimg_layer);
        for (int i = 0; i < 22; i++) {
            gbitmap_destroy(battery_images[i]);
        }
        _battimg_loaded = 0;
    }
}

int battimg_loaded() {
    return _battimg_loaded;
}
