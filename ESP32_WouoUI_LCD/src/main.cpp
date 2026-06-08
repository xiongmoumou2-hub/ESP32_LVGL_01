#include <Arduino.h>
#include "TFT_ST7735/st7735.h"
#include "botton.h"

// --- 解决 String 命名冲突的宏定义魔法 ---
#define String Wouo_String
#include "WouoUI/WouoUI.h"
#include "WouoUI/WouoUI_user.h"
#undef String
// ----------------------------------------

uint32_t last_ui_time = 0;

void setup()
{
  // 1. 开启串口，波特率设置与电脑端的串口监视器一致 (比如 115200)
  // 这相当于作者代码里的 USART2_Init(115200);
  Serial.begin(115200);

  // 2. 屏幕底层与硬件初始化
  ST7735_Hardware_Init();
  ST7735_Init();
  ST7735_FillScreen(0x0000); // 清黑屏

  // 3. WouoUI 初始化与绑定
  WouoUI_SelectDefaultUI();
  WouoUI_AttachSendBuffFun(WouoUI_To_ST7735_Wrapper);
  TestUI_Init(); // 初始化所有的测试页面

  // 3. 绑定 OneButton 的回调函数
  btn_up.attachClick(click_up);
  btn_down.attachClick(click_down);
  btn_left.attachClick(click_left);
  btn_right.attachClick(click_right);
  btn_click.attachClick(click_click);
  btn_return.attachClick(click_return);
  btn_return.attachDoubleClick(double_click_return);

  Serial.println("System Initialized!");
}

void loop()
{
  btn_up.tick();
  btn_down.tick();
  btn_left.tick();
  btn_right.tick();
  btn_click.tick();
  btn_return.tick();

  if (millis() - last_ui_time >= 5)
  {
    last_ui_time = millis();
    WouoUI_Proc(5);
  }
}