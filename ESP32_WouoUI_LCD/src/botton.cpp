#include "botton.h"

OneButton btn_up(PIN_BTN_UP, true, true);
OneButton btn_down(PIN_BTN_DOWN, true, true);
OneButton btn_left(PIN_BTN_LEFT, true, true);
OneButton btn_right(PIN_BTN_RIGHT, true, true);
OneButton btn_click(PIN_BTN_CLICK, true, true);
OneButton btn_return(PIN_BTN_RETURN, true, true);

// -- 单击事件 --
void click_up() { WOUOUI_MSG_QUE_SEND(msg_up); }
void click_down() { WOUOUI_MSG_QUE_SEND(msg_down); }
void click_left() { WOUOUI_MSG_QUE_SEND(msg_left); }
void click_right() { WOUOUI_MSG_QUE_SEND(msg_right); }
void click_click() { WOUOUI_MSG_QUE_SEND(msg_click); }
void click_return() { WOUOUI_MSG_QUE_SEND(msg_return); }
//双击事件
void double_click_return() { WOUOUI_MSG_QUE_SEND(msg_home); }

