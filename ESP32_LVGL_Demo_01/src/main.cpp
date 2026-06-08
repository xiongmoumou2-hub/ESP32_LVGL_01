/**
 * @file main.cpp
 * @brief ESP32 + LVGL 8.4.0 多级菜单测试工程
 * @note  适用于 160x128 屏幕及物理按键 (Keypad) 交互
 */

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "ui.h"

/* ==========================================
 * 主程序入口 (Main Application)
 * ========================================== */
void setup()
{
  Serial.begin(115200);
  Serial.println("[APP] System starting...");

  system_init();
  ui_menu_create();
  //ui_test_create();
  Serial.println("[APP] LVGL Setup Complete!");
}

void loop()
{
  lvgl_ui_clock();
}
