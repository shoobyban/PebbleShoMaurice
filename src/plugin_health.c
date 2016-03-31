#include "config.h"

TextLayer *steps_layer;
static char s_steps_buffer[20];
int health_loaded = 0;

void health_load(Layer * window_layer, GRect rect) {
    if (show_steps && !health_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "health_load");
        steps_layer = text_layer_create(rect);
        text_layer_set_background_color(steps_layer, GColorClear);
        text_layer_set_overflow_mode(steps_layer, GTextOverflowModeWordWrap);
        text_layer_set_text_color(steps_layer, text_color);
        text_layer_set_text_alignment(steps_layer, GTextAlignmentRight);
        text_layer_set_font(steps_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));

        layer_add_child(window_layer, text_layer_get_layer(steps_layer));
        health_loaded = 1;
        register_plugin((load_fn)health_load,(void_fn)health_redraw,(void_fn)health_unload);
    }
}

void health_redraw() {
    if (health_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "health_redraw");
        HealthMetric metric = HealthMetricStepCount;
        time_t start = time_start_of_today();
        time_t end = time(NULL);

        // Check the metric has data available for today
        HealthServiceAccessibilityMask mask = health_service_metric_accessible(metric, start, end);
        
        if (mask & HealthServiceAccessibilityMaskAvailable) {
            snprintf(s_steps_buffer, sizeof (s_steps_buffer), "%d steps", (int) health_service_sum_today(metric));
            text_layer_set_background_color(steps_layer, bg_color);
            text_layer_set_text(steps_layer, s_steps_buffer);
        }
    }
}

void health_unload() {
    if (health_loaded) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "health_unload");
        text_layer_destroy(steps_layer);
        health_loaded = 0;
    }
}