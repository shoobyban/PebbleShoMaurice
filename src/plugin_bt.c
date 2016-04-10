#include <pebble.h>
#include "config.h"

BitmapLayer * bluetooth_layer;
GBitmap *bluetooth_images[4];
bool bt_connected = false;
int _bt_loaded = 0;

void bt_load(Layer * window_layer, GRect rect) {
    if (!_bt_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "bt_load");
        for (int i = 0; i < 2; i++)
            bluetooth_images[i] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTH_OFF + i);
        bluetooth_layer = bitmap_layer_create(rect);
        layer_add_child(window_layer, bitmap_layer_get_layer(bluetooth_layer));
        _bt_loaded = 1;
        bt_redraw();
        register_plugin((load_fn)bt_load,(void_fn)bt_redraw,(void_fn)bt_unload);
    }
}

void bt_redraw() {
    if (_bt_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "bt_redraw");
        GBitmap * bmp = bluetooth_images[bt_connected ? 1 : 0];
        if (customcolor) {
            #ifdef PBL_COLOR
            replace_gbitmap_color(GColorBlack, bg_color, bmp, bluetooth_layer);
            replace_gbitmap_color(GColorWhite, text_color, bmp, bluetooth_layer);
            #endif
        }
        bitmap_layer_set_bitmap(bluetooth_layer, bmp);
    }
}

void bt_refresh(bool connected) {
    if (_bt_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "bt_refresh");
        if (appstarted && !connected && bt_vibe) {
            vibes_long_pulse();
        }
        bt_connected = connected;
        bt_redraw();
    }
}

void bt_unload() {
    if (_bt_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "bt_unload");
        bitmap_layer_destroy(bluetooth_layer);
        for (int i = 0; i < 2; i++) {
            gbitmap_destroy(bluetooth_images[i]);
        }
        _bt_loaded = 0;
    }
}

int bt_loaded() {
    return _bt_loaded;
}
