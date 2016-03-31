#include "config.h"

/**
 * Configuration init
 */
void config_load() {
#ifdef PBL_COLOR
    if (
            persist_read_int(KEY_BG_COLOR) < 256 &&
            persist_read_int(KEY_H_COLOR) < 256 &&
            persist_read_int(KEY_HI_COLOR) < 256 &&
            persist_read_int(KEY_M_COLOR) < 256 &&
            persist_read_int(KEY_S_COLOR) < 256 &&
            persist_read_int(KEY_T_COLOR)) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "OLD SETTINGS BUG, DELETING");
        persist_delete(KEY_BG_COLOR);
        persist_delete(KEY_H_COLOR);
        persist_delete(KEY_HI_COLOR);
        persist_delete(KEY_M_COLOR);
        persist_delete(KEY_S_COLOR);
        persist_delete(KEY_T_COLOR);
    }
    if (!persist_exists(KEY_BG_COLOR)) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "NO COLOR, DEFAULT");
        persist_write_int(KEY_BG_COLOR, 0x000000);
        persist_write_int(KEY_H_COLOR, 0xffffff);
        persist_write_int(KEY_HI_COLOR, 0x99353f);
        persist_write_int(KEY_M_COLOR, 0xffffff);
        persist_write_int(KEY_S_COLOR, 0xffffff);
        persist_write_int(KEY_T_COLOR, 0xffffff);
    }

    APP_LOG(APP_LOG_LEVEL_DEBUG, "SETTING FROM bg: %ld h: %ld hi: %ld m: %ld s: %ld t: %ld",
            (long int) persist_read_int(KEY_BG_COLOR),
            (long int) persist_read_int(KEY_H_COLOR),
            (long int) persist_read_int(KEY_HI_COLOR),
            (long int) persist_read_int(KEY_M_COLOR),
            (long int) persist_read_int(KEY_S_COLOR),
            (long int) persist_read_int(KEY_T_COLOR)
            );

    if (persist_read_int(KEY_BG_COLOR) == 0x000000 &&
            persist_read_int(KEY_T_COLOR) == 0xffffff) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "NO CUSTOM COLOR");
        customcolor = 0;
    } else {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "CUSTOM COLOR");
        customcolor = 1;
    }

    if (persist_exists(KEY_BG_COLOR)) {
        bg_color = GColorFromHEX(persist_read_int(KEY_BG_COLOR));
        hour_color = GColorFromHEX(persist_read_int(KEY_H_COLOR));
        hourin_color = GColorFromHEX(persist_read_int(KEY_HI_COLOR));
        minute_color = GColorFromHEX(persist_read_int(KEY_M_COLOR));
        sec_color = GColorFromHEX(persist_read_int(KEY_S_COLOR));
        text_color = GColorFromHEX(persist_read_int(KEY_T_COLOR));
    }
#else
    bg_color = GColorBlack;
    hour_color = GColorWhite;
    hourin_color = GColorBlack;
    minute_color = GColorWhite;
    sec_color = GColorWhite;
    text_color = GColorWhite;
#endif

    show_second = persist_read_int(KEY_SECOND);
    show_steps = persist_read_int(KEY_STEPS);
    mbatt = persist_read_int(KEY_MBATT);
    show_digital = persist_read_int(KEY_DIGITAL);
    digital24 = persist_read_int(KEY_24);
    bt_vibe = persist_read_int(KEY_BTVIBE);
    fulldate = persist_read_int(KEY_FULLDATE);

    APP_LOG(APP_LOG_LEVEL_DEBUG, "sec: %d st: %d mb: %d d: %d v: %d f: %d",
            (int) show_second, (int) show_steps, (int) mbatt, (int) show_digital, 
            (int) bt_vibe, (int) fulldate);

}

void config_received_handler(DictionaryIterator *iter, void *ctx) {
    Tuple *sec_t = dict_find(iter, KEY_SECOND);
    Tuple *show_digital_t = dict_find(iter, KEY_DIGITAL);
    Tuple *digital24_t = dict_find(iter, KEY_24);
    Tuple *monthbatt_t = dict_find(iter, KEY_MBATT);
    Tuple *steps_t = dict_find(iter, KEY_STEPS);
    Tuple *btvibe_t = dict_find(iter, KEY_BTVIBE);
    Tuple *fulldate_t = dict_find(iter, KEY_FULLDATE);

#ifdef PBL_COLOR
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Size: %d", (int) dict_size(iter));

    Tuple *reset_t = dict_find(iter, KEY_RESET);
    Tuple *background_color_t = dict_find(iter, KEY_BG_COLOR);
    Tuple *hour_color_t = dict_find(iter, KEY_H_COLOR);
    Tuple *hour_in_color_t = dict_find(iter, KEY_HI_COLOR);
    Tuple *minute_color_t = dict_find(iter, KEY_M_COLOR);
    Tuple *text_color_t = dict_find(iter, KEY_T_COLOR);
    Tuple *sec_color_t = dict_find(iter, KEY_S_COLOR);

    APP_LOG(APP_LOG_LEVEL_DEBUG, "#####");

    int change = 0;

    if (reset_t) {
        int reset = reset_t->value->int8;
        if (reset) {
            persist_delete(KEY_BG_COLOR);
            persist_delete(KEY_H_COLOR);
            persist_delete(KEY_HI_COLOR);
            persist_delete(KEY_M_COLOR);
            persist_delete(KEY_S_COLOR);
            persist_delete(KEY_T_COLOR);
            change = 1;
        }
    }

    if (background_color_t) {
        int color = background_color_t->value->int32;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "@@in bg: %x", color);

        persist_write_int(KEY_BG_COLOR, color);
        bg_color = GColorFromHEX(color);
        change++;
    }

    if (hour_color_t) {
        int color = hour_color_t->value->int32;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "@@in h: %x", color);
        persist_write_int(KEY_H_COLOR, color);
        hour_color = GColorFromHEX(color);
        change++;
    }

    if (hour_in_color_t) {
        int color = hour_in_color_t->value->int32;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "@@in hi: %x", color);
        persist_write_int(KEY_HI_COLOR, color);
        hourin_color = GColorFromHEX(color);
        change++;
    }

    if (minute_color_t) {
        int color = minute_color_t->value->int32;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "@@in m: %x", color);
        persist_write_int(KEY_M_COLOR, color);
        minute_color = GColorFromHEX(color);
        change++;
    }

    if (sec_color_t) {
        int color = sec_color_t->value->int32;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "@@in s: %x", color);
        persist_write_int(KEY_S_COLOR, color);
        sec_color = GColorFromHEX(color);
    }

    if (text_color_t) {
        int color = text_color_t->value->int32;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "@@in t: %x", color);
        persist_write_int(KEY_T_COLOR, color);
        text_color = GColorFromHEX(color);
        change++;
    }

    if (change) {
        call_plugins_redraw();
    } else {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "@@@ UNSUCCESSFUL SETTINGS @@@");
    }
#endif
    if (sec_t) {
        if (sec_t->value->int8 != show_second) {
            tick_timer_service_unsubscribe();
            show_second = sec_t->value->int8;
            persist_write_int(KEY_SECOND, show_second);
            analog_refresh();
            if (show_second) {
                tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
            } else {
                tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
            }
            analog_refresh();
        }
    }

    if (show_digital_t) {
        show_digital = show_digital_t->value->int8;
        persist_write_int(KEY_DIGITAL, show_digital);
    }

    if (digital24_t) {
        digital24 = digital24_t->value->int8;
        persist_write_int(KEY_24, digital24);
    }

    if (monthbatt_t) {
        mbatt = monthbatt_t->value->int8;
        persist_write_int(KEY_MBATT, mbatt);
    }

    if (steps_t) {
        show_steps = steps_t->value->int8;
        persist_write_int(KEY_STEPS, show_steps);
    }

    if (btvibe_t) {
        bt_vibe = btvibe_t->value->int8;
        persist_write_int(KEY_BTVIBE, bt_vibe);
    }

    if (fulldate_t) {
        fulldate = fulldate_t->value->int8;
        persist_write_int(KEY_BTVIBE, fulldate);
    }

    //  window_stack_push(window,false);
    //  window_stack_pop_all(false);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "got: sec: %d st: %d mb: %d d: %d v: %d f: %d",
            (int) show_second, (int) show_steps, (int) mbatt, (int) show_digital,
            (int) bt_vibe, (int) fulldate);

}

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
    analog_refresh();
}

void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
    analog_refresh_sec();
    if (tick_time->tm_sec == 0) {
        analog_refresh();
    }
}
