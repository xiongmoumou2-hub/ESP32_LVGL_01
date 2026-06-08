#include "ui.h"

void lvgl_ui_clock(void)
{
    static uint32_t last_tick = 0;
    uint32_t current_tick = millis();
    lv_tick_inc(current_tick - last_tick);
    last_tick = current_tick;
    lv_timer_handler();
    delay(5);
}

void system_init(void)
{
    /* 1. 初始化 LVGL 核心 */
    lv_init();

    /* 2. 初始化显示与输入驱动 */
    lv_port_disp_init();
    lv_port_indev_init();

    /* 3. 配置全局焦点组 (用于物理按键导航) */
    lv_group_t *g = lv_group_create();
    if (g != NULL)
    {
        lv_group_set_default(g);             /* 设为系统默认组 */
        lv_indev_set_group(indev_keypad, g); /* 绑定键盘设备到该组 */
    }
    else
    {
        Serial.println("[ERR] Failed to create LVGL group!");
    }
}

/**
 * @brief 创建多级菜单界面
 */
void ui_menu_create(void)
{
    /* 获取默认组，用于修复菜单项不响应物理按键的问题 */
    lv_group_t *g = lv_group_get_default();

    /* 1. 创建主菜单对象 */
    lv_obj_t *menu = lv_menu_create(lv_scr_act());
    lv_obj_set_size(menu, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
    lv_obj_center(menu);

    /* 2. 配置顶部 Header 及返回按钮 */
    lv_obj_t *back_btn = lv_menu_get_main_header_back_btn(menu);
    lv_obj_t *back_btn_label = lv_label_create(back_btn);
    lv_label_set_text(back_btn_label, "Back");
    lv_group_add_obj(g, back_btn); /* 将返回按钮加入焦点组 */

    /* 临时变量，用于复用 */
    lv_obj_t *cont;
    lv_obj_t *label;

    /* ----------------------------------
     * 创建子页面 (Sub Pages)
     * ---------------------------------- */
    lv_obj_t *sub_1_page = lv_menu_page_create(menu, "Page 1");
    cont = lv_menu_cont_create(sub_1_page);
    lv_group_add_obj(g, cont);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Welcome to P1!");

    lv_obj_t *sub_2_page = lv_menu_page_create(menu, "Page 2");
    cont = lv_menu_cont_create(sub_2_page);
    lv_group_add_obj(g, cont);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Welcome to P2!");

    lv_obj_t *sub_3_page = lv_menu_page_create(menu, "Page 3");
    cont = lv_menu_cont_create(sub_3_page);
    lv_group_add_obj(g, cont);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Welcome to P3!");

    /* ----------------------------------
     * 创建主页面 (Main Page)
     * ---------------------------------- */
    lv_obj_t *main_page = lv_menu_page_create(menu, NULL);

    cont = lv_menu_cont_create(main_page);
    lv_group_add_obj(g, cont);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Item 1");
    lv_menu_set_load_page_event(menu, cont, sub_1_page);

    cont = lv_menu_cont_create(main_page);
    lv_group_add_obj(g, cont);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Item 2");
    lv_menu_set_load_page_event(menu, cont, sub_2_page);

    cont = lv_menu_cont_create(main_page);
    lv_group_add_obj(g, cont);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Item 3");
    lv_menu_set_load_page_event(menu, cont, sub_3_page);

    /* 3. 设置默认显示主页面 */
    lv_menu_set_page(menu, main_page);
}

/**
 * @brief 创建基础测试界面 (滑块、按钮、开关)
 */
void ui_test_create(void)
{
    /* 设置屏幕弹性布局 */
    lv_obj_set_flex_flow(lv_scr_act(), LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(lv_scr_act(), LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /* 1. 滑块 (Slider) */
    lv_obj_t *slider = lv_slider_create(lv_scr_act());
    lv_obj_set_width(slider, 120);

    /* 2. 按钮 (Button) */
    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
    lv_obj_t *label = lv_label_create(btn1);
    lv_label_set_text(label, "Function 1");
    lv_obj_add_event_cb(btn1, btn_event_cb, LV_EVENT_CLICKED, NULL); /* 仅监听点击件，提升效率 */

    /* 3. 开关 (Switch) */
    lv_obj_t *sw = lv_switch_create(lv_scr_act());
}

/**
 * @brief 按钮点击事件回调
 */
void btn_event_cb(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_target(e);

    Serial.println("[UI] Button clicked!");

    if (lv_obj_get_child_cnt(btn) > 0)
    {
        lv_obj_t *label = lv_obj_get_child(btn, 0);
        if (label != NULL)
        {
            lv_label_set_text(label, "I was clicked!");
        }
    }
}