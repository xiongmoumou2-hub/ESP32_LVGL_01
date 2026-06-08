#ifndef __BOTTON_H
#define __BOTTON_H

#define String Wouo_String
#include "WouoUI/WouoUI.h"
#include "WouoUI/WouoUI_user.h"
#undef String

#include "OneButton.h"

// ==========================================
// 1. 定义按键引脚 (请根据你的 ESP32-S3 实际接线修改)
// ==========================================
#define PIN_BTN_UP 21
#define PIN_BTN_DOWN 14
#define PIN_BTN_LEFT 18
#define PIN_BTN_RIGHT 17
#define PIN_BTN_CLICK 6
#define PIN_BTN_RETURN 7

// ==========================================
// 2. 实例化 OneButton 对象
// ==========================================
extern OneButton btn_up;
extern OneButton btn_down;
extern OneButton btn_left;
extern OneButton btn_right;
extern OneButton btn_click;
extern OneButton btn_return;

// ==========================================
// 3. 定义按键回调函数 (将按键动作翻译给 WouoUI)
// ==========================================
void click_up();
void click_down();
void click_left();
void click_right();
void click_return();
void click_click();

void double_click_return();

#endif // BOTTON_H