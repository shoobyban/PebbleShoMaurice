#include "config.h"
#include "analog_points.h"
#include "plugin_digital.h"
#include "gbitmap_color_palette_manipulator.h"

BitmapLayer *s_background_layer;
GBitmap * s_background_bitmap;
static GPath *s_minute_hand, *s_hour_hand, *s_hour_in_hand, *s_sec_hand;
Layer *s_hands_layer, *s_sec_layer;
Layer *window_layer_backup;
int analog_loaded = 0;

void date_update_proc(Layer *layer, GContext *ctx);
void sec_update_proc(Layer *layer, GContext *ctx);
void hands_update_proc(Layer *layer, GContext *ctx);

void analog_load(Layer *window_layer) {
    if (!analog_loaded) {
        window_layer_backup = window_layer;
        GRect bounds = layer_get_bounds(window_layer);
        GPoint center = grect_center_point(&bounds);

        // init hand paths
        s_minute_hand = gpath_create(&MINUTE_HAND_POINTS);
        gpath_move_to(s_minute_hand, center);
        s_hour_hand = gpath_create(&HOUR_HAND_POINTS);
        gpath_move_to(s_hour_hand, center);
        s_hour_in_hand = gpath_create(&HOUR_IN_POINTS);
        gpath_move_to(s_hour_in_hand, center);
        s_sec_hand = gpath_create(&SEC_POINTS);
        gpath_move_to(s_sec_hand, center);

        s_background_layer = bitmap_layer_create(layer_get_frame(window_layer));
    #ifdef PBL_COLOR
        s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_TIME);
        if (customcolor) {
            replace_gbitmap_color(GColorBlack, bg_color, s_background_bitmap, s_background_layer);
            replace_gbitmap_color(GColorWhite, text_color, s_background_bitmap, s_background_layer);
        }
    #else
        s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
    #endif
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
        layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

        digital_load(window_layer,GRect(40, 144, 64, 28));

        battp_load(window_layer, GRect(40, 30, 30, 14));

        if (calmode != CALMODE_MBATT) {
            battimg_load(window_layer, GRect(83, 35, 16, 10));
        }

        bt_load(window_layer, GRect(64, 33, 13, 13));

        switch (calmode) {
            case CALMODE_FULL: 
                fulldate_load(window_layer,GRect(40, 114, 68, 20));
                break;
            case CALMODE_NORMAL:
                day_load(window_layer, GRect(73, 114, 27, 20)); 
                num_load(window_layer, GRect(46, 114, 18, 20));
                break;
            case CALMODE_CAL:
                day_load(window_layer, GRect(60, 114, 27, 20)); 
                cal_load(window_layer, GRect(1, 138, 40, 80));
                break;
            case CALMODE_MBATT:
                month_load(window_layer, GRect(80, 28, 27, 18));        
                break;
        }

        health_load(window_layer, GRect(104, 138, 39, 80));

        s_hands_layer = layer_create(bounds);
        layer_set_update_proc(s_hands_layer, hands_update_proc);
        layer_add_child(window_layer, s_hands_layer);

        s_sec_layer = layer_create(bounds);
        layer_set_update_proc(s_sec_layer, sec_update_proc);
        layer_add_child(window_layer, s_sec_layer);

        analog_loaded = 1;
    }
}

void analog_redraw() {
    if (analog_loaded) {
        #ifdef PBL_COLOR
        s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_TIME);
        if (customcolor) { // no need if not custom color 
            replace_gbitmap_color(GColorBlack, bg_color, s_background_bitmap, s_background_layer);
            replace_gbitmap_color(GColorWhite, text_color, s_background_bitmap, s_background_layer);
        }
        #else
        s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
        #endif
        bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
    }
}

/**
 * Minute and hour hand update
 */
void hands_update_proc(Layer *layer, GContext *ctx) {
    if (analog_loaded) {
    #ifdef PBL_COLOR
        graphics_context_set_antialiased(ctx, 1);
    #endif

        GRect bounds = layer_get_bounds(layer);
        GPoint center = GPoint(bounds.size.w / 2, (bounds.size.h / 2));

        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        int32_t hour_angle = (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6);
        int32_t min_angle = TRIG_MAX_ANGLE * t->tm_min / 60;

        // minute/hour hand
        gpath_rotate_to(s_hour_hand, hour_angle);

    #ifdef PBL_COLOR
        graphics_context_set_fill_color(ctx, hourin_color);
        graphics_context_set_stroke_color(ctx, hour_color);
        graphics_context_set_stroke_width(ctx, 3);
    #else
        graphics_context_set_fill_color(ctx, hour_color);
        graphics_context_set_stroke_color(ctx, hourin_color);
    #endif
        gpath_draw_filled(ctx, s_hour_hand);
        gpath_draw_outline(ctx, s_hour_hand);

    #ifndef PBL_COLOR
        gpath_rotate_to(s_hour_in_hand, hour_angle);
        graphics_context_set_fill_color(ctx, hourin_color);
        gpath_draw_filled(ctx, s_hour_in_hand);
    #else
        graphics_context_set_stroke_color(ctx, minute_color);
        graphics_context_set_stroke_width(ctx, 1);
    #endif

        // minute hand
        graphics_context_set_fill_color(ctx, minute_color);
        gpath_rotate_to(s_minute_hand, min_angle);
        gpath_draw_filled(ctx, s_minute_hand);
        gpath_draw_outline(ctx, s_minute_hand);

        // center dot
        graphics_fill_circle(ctx, center, 7);
        graphics_context_set_fill_color(ctx, hourin_color);
        graphics_fill_circle(ctx, center, 4);

        if (show_digital) {
            digital_refresh(ctx, t);
        }

    #ifdef PBL_HEALTH
        if (show_steps) {
            health_redraw(ctx);
        }
    #endif
        date_update_proc(layer,ctx);
    }
}

/**
 * Second hand update
 */
void sec_update_proc(Layer *layer, GContext *ctx) {
    if (show_second && analog_loaded) {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        int32_t sec_angle = TRIG_MAX_ANGLE * t->tm_sec / 60;
        gpath_rotate_to(s_sec_hand, sec_angle);
#ifdef PBL_COLOR
        graphics_context_set_stroke_width(ctx, 3);
        graphics_context_set_stroke_color(ctx, sec_color);
#else
        graphics_context_set_stroke_color(ctx, GColorWhite);
#endif
        gpath_draw_outline(ctx, s_sec_hand);
    }
}

/**
 * Date layer update
 */
void date_update_proc(Layer *layer, GContext *ctx) {
    if (analog_loaded) {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        switch (calmode) { 
            case CALMODE_MBATT:
                month_refresh(t);
                break;
            case CALMODE_FULL:
                fulldate_refresh(t);
                break;
            case CALMODE_NORMAL:
                day_refresh(t);
                num_refresh(t);
                break;
            case CALMODE_CAL:
                cal_refresh(t);
                day_refresh(t);
                break;
        }
    }
}

void analog_refresh() {
    layer_mark_dirty(s_hands_layer);
}

void analog_refresh_sec() {
    layer_mark_dirty(s_sec_layer);
}

void analog_unload() {
    if (analog_loaded) {
        gpath_destroy(s_minute_hand);
        gpath_destroy(s_hour_hand);
        gpath_destroy(s_hour_in_hand);

        bitmap_layer_destroy(s_background_layer);

        call_plugins_unload();

        layer_destroy(s_hands_layer);
    }
}

void analog_reload() {
    analog_unload();
    analog_load(window_layer_backup);
}