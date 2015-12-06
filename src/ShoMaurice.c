#include <pebble.h>

#define NUM_CLOCK_TICKS 11
#define DOTS_SIZE    4


const GPathInfo HOUR_HAND_POINTS = {
  6,
  (GPoint []) {
    { 7,  0},
    { 5,-25},
    { 3,-36},
    {-3,-36},
    {-5,-25},
    {-7,  0},
  }
};
const GPathInfo HOUR_IN_POINTS = {
  6,
  (GPoint []) {
    { 5,  0},
    { 3,-20},
    { 2,-31},
    {-2,-31},
    {-3,-20},
    {-5,  0},
  }
};

const GPathInfo MINUTE_HAND_POINTS = {
  6,
  (GPoint []) {
    { 5,  0},
    { 4,-27},
    { 3,-50},
    {-3,-50},
    {-4,-27},
    {-5,  0},
  }
};

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

static GBitmap *battery_images[22];
static BitmapLayer *battery_layer;

static GBitmap *bluetooth_images[4];
static BitmapLayer *bluetooth_layer;

static Window *window;
static Layer *s_date_layer, *s_hands_layer;
static TextLayer *s_day_label, *s_num_label, *battp_layer;
static BitmapLayer *s_background_layer;

GBitmap * s_background_bitmap;

static GPath *s_minute_hand, *s_hour_hand, *s_hour_in_hand;
static char s_num_buffer[4], s_day_buffer[6];


static void handle_bluetooth(bool connected) {
  bitmap_layer_set_bitmap(bluetooth_layer, bluetooth_images[connected ? 1 : 0]);
}

static void handle_battery(BatteryChargeState charge_state) {
  static char battp_buffer [] = "+100%";
  snprintf(battp_buffer, sizeof(battp_buffer), "%s%d%%",  charge_state.is_charging ? "+" : "", charge_state.charge_percent);
  text_layer_set_text(battp_layer, battp_buffer);
  bitmap_layer_set_bitmap(battery_layer, battery_images[
    (charge_state.is_charging ? 11 : 0) + min(charge_state.charge_percent / 10, 10)]);
}

static void hands_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GPoint center = GPoint(bounds.size.w / 2, (bounds.size.h / 2));

  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  int32_t hour_angle = (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6);
  int32_t min_angle = TRIG_MAX_ANGLE * t->tm_min / 60;

  // minute/hour hand
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_stroke_color(ctx, GColorBlack);

  gpath_rotate_to(s_hour_hand, hour_angle);
  gpath_draw_filled(ctx, s_hour_hand);
  gpath_draw_outline(ctx, s_hour_hand);
  
  gpath_rotate_to(s_hour_in_hand, hour_angle);
  graphics_context_set_fill_color(ctx, GColorBlack);
  gpath_draw_filled(ctx, s_hour_in_hand);

  graphics_context_set_fill_color(ctx, GColorWhite);

  gpath_rotate_to(s_minute_hand, min_angle);
  gpath_draw_filled(ctx, s_minute_hand);
  gpath_draw_outline(ctx, s_minute_hand);

  // center dot
  graphics_fill_circle(ctx, center, DOTS_SIZE+3);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, center, DOTS_SIZE);

}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
    layer_mark_dirty(s_hands_layer);
}

static void date_update_proc(Layer *layer, GContext *ctx) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  strftime(s_day_buffer, sizeof(s_day_buffer), "%a", t);
  text_layer_set_text(s_day_label, s_day_buffer);

  strftime(s_num_buffer, sizeof(s_num_buffer), "%d", t);
  text_layer_set_text(s_num_label, s_num_buffer);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  s_background_layer = bitmap_layer_create(layer_get_frame(window_layer));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

  battp_layer = text_layer_create(GRect(40, 30, 30, 14));
  text_layer_set_text_color(battp_layer, GColorWhite);
  text_layer_set_background_color(battp_layer, GColorBlack);
  text_layer_set_font(battp_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  layer_add_child(window_layer, text_layer_get_layer(battp_layer));

  for (int i = 0; i < 22; i++) {
    battery_images[i] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_0 + i);  
  }
  battery_layer = bitmap_layer_create(GRect(83, 35, 16, 10));
  layer_add_child(window_layer, bitmap_layer_get_layer(battery_layer));

  for (int i = 0; i < 2; i++)
    bluetooth_images[i] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTH_OFF + i);  
  bluetooth_layer = bitmap_layer_create(GRect(64, 33, 13, 13));
  layer_add_child(window_layer, bitmap_layer_get_layer(bluetooth_layer));

  s_date_layer = layer_create(bounds);
  layer_set_update_proc(s_date_layer, date_update_proc);
  layer_add_child(window_layer, s_date_layer);

  s_day_label = text_layer_create(GRect(73, 114, 27, 20));
  text_layer_set_text(s_day_label, s_day_buffer);
  text_layer_set_background_color(s_day_label, GColorBlack);
  text_layer_set_text_color(s_day_label, GColorWhite);
  text_layer_set_font(s_day_label, fonts_get_system_font(FONT_KEY_GOTHIC_18));

  layer_add_child(s_date_layer, text_layer_get_layer(s_day_label));

  s_num_label = text_layer_create(GRect(46, 114, 18, 20));
  text_layer_set_text(s_num_label, s_num_buffer);
  text_layer_set_background_color(s_num_label, GColorBlack);
  text_layer_set_text_color(s_num_label, GColorWhite);
  text_layer_set_font(s_num_label, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));

  layer_add_child(s_date_layer, text_layer_get_layer(s_num_label));

  s_hands_layer = layer_create(bounds);
  layer_set_update_proc(s_hands_layer, hands_update_proc);
  layer_add_child(window_layer, s_hands_layer);

  battery_state_service_subscribe(&handle_battery);
  handle_battery(battery_state_service_peek());
  bluetooth_connection_service_subscribe(&handle_bluetooth);
  handle_bluetooth(bluetooth_connection_service_peek());
  
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  handle_minute_tick(0,1);
  
}

static void window_unload(Window *window) {
  layer_destroy(s_date_layer);

  text_layer_destroy(s_day_label);
  text_layer_destroy(s_num_label);

  layer_destroy(s_hands_layer);

  bitmap_layer_destroy(battery_layer);
  for (int i = 0; i < 22; i++)
    gbitmap_destroy(battery_images[i]);
  bitmap_layer_destroy(bluetooth_layer);
  for (int i = 0; i < 2; i++)
    gbitmap_destroy(bluetooth_images[i]);

}

static void init() {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);

  s_day_buffer[0] = '\0';
  s_num_buffer[0] = '\0';

  // init hand paths
  s_minute_hand = gpath_create(&MINUTE_HAND_POINTS);
  s_hour_hand = gpath_create(&HOUR_HAND_POINTS);
  s_hour_in_hand = gpath_create(&HOUR_IN_POINTS);

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = grect_center_point(&bounds);
  gpath_move_to(s_minute_hand, center);
  gpath_move_to(s_hour_hand, center);
  gpath_move_to(s_hour_in_hand, center);

}

static void deinit() {
  gpath_destroy(s_minute_hand);
  gpath_destroy(s_hour_hand);
  gpath_destroy(s_hour_in_hand);

  tick_timer_service_unsubscribe();
  window_destroy(window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
