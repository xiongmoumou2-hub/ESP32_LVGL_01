#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <freertos/FreeRTOS.h>
#include "ui.h"
#include "GUI/generated/gui_guider.h"
#include "GUI/generated/events_init.h"

// 声明函数
void lvgl_task(void *pt);
void adc_task(void *pt); // 预留给你的 ADC 任务

lv_ui guider_ui;

void setup()
{
  Serial.begin(115200);
  Serial.println("[APP] System starting...");

  // 1. 初始化底层硬件驱动（屏幕、触摸屏、LVGL核心）
  system_init();

  // 2. 在 setup 中**仅创建一次**任务
  // 将 LVGL 任务固定在 Core 1，分配 8KB 栈空间，优先级设为 2
  xTaskCreatePinnedToCore(lvgl_task, "lvgl_task", 1024 * 8, NULL, 2, NULL, 1);

  // 你可以在这里创建你刚才提到的 ADC 任务 (比如放 Core 0)
  // xTaskCreatePinnedToCore(adc_task, "adc_task", 1024 * 4, NULL, 1, NULL, 0);

  Serial.println("[APP] Setup complete, RTOS running!");
}

void loop()
{
  // 在使用了 FreeRTOS 任务接管核心逻辑后，Arduino 的 loop() 就可以空闲下来了。
  // 放一个较长的 delay，防止触发看门狗，也可以在这里处理一些不耗时的业务逻辑。
  vTaskDelay(pdMS_TO_TICKS(1000));
}

/* ==========================================
 * LVGL 专属任务：负责 UI 的创建和后续的持续刷新
 * ========================================== */
void lvgl_task(void *pt)
{
  Serial.println("[LVGL] Task started, creating UI...");

  // 1. 第一步：在任务开始时，先同步初始化 UI
  // 因为此时还在死循环外部，lvgl_ui_clock 还没开始工作，所以创建 UI 绝对安全！
  setup_ui(&guider_ui);
  events_init(&guider_ui);

  Serial.println("[LVGL] UI created successfully!");

  // 2. 第二步：进入任务死循环，接管 LVGL 的心跳
  while (1)
  {
    lvgl_ui_clock();              // 处理 LVGL 的定时器和事件
  }

  // FreeRTOS 任务理论上不应该运行到这里。如果 break 了，必须删除自己。
  vTaskDelete(NULL);
}