#include "config.h"

Window *window;

void handle_battery(BatteryChargeState charge_state) {
    battp_refresh(charge_state);

    if (calmode != CALMODE_MBATT) {
        battimg_refresh(charge_state);
    }
}

void handle_bluetooth(bool connected) {
    bt_refresh(connected);
}

static void window_load(Window *window) {

    window_layer = window_get_root_layer(window);

    analog_load(window_layer);

    if (show_digital) {
    }

    battery_state_service_subscribe(&handle_battery);
    handle_battery(battery_state_service_peek());
    bluetooth_connection_service_subscribe(&handle_bluetooth);
    handle_bluetooth(bluetooth_connection_service_peek());

    if (show_second) {
        tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
    } else {
        tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
    }

    handle_minute_tick(0, 1);
    appstarted = 1;

}

static void window_unload(Window *window) {
    analog_unload();
}

static void init() {
    customcolor = 0;
    appstarted = 0;

    config_load();

    app_message_register_inbox_received(config_received_handler);
    app_message_open(200, 200);

    window = window_create();

    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    window_stack_push(window, true);
}

static void deinit() {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "deinit");
    tick_timer_service_unsubscribe();
    window_destroy(window);
}

int main() {
    init();
    app_event_loop();
    deinit();
}
