#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include <lvgl.h>
#include "lvgl_config/lv_port_indev.h"
#include "lvgl_config/lv_port_disp.h"

extern lv_indev_t *indev_keypad;

void lvgl_ui_clock(void);
void system_init(void);
void ui_menu_create(void);

void ui_test_create(void);
void btn_event_cb(lv_event_t *e);

#endif /* UI_H */