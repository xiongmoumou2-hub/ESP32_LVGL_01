#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <OneButton.h>
#include <WiFi.h>

TFT_eSPI tft = TFT_eSPI();

const char *ssid = "ESP32_S3_AP";
const char *password = "12345678";

// 引脚定义
#define PIN_INPUT1 21
#define PIN_INPUT2 18
#define PIN_INPUT3 14
#define PIN_INPUT4 17
#define PIN_INPUT5 13
#define PIN_INPUT6 16

OneButton button1(PIN_INPUT1, true);
OneButton button2(PIN_INPUT2, true);
OneButton button3(PIN_INPUT3, true);
OneButton button4(PIN_INPUT4, true);
OneButton button5(PIN_INPUT5, true);
OneButton button6(PIN_INPUT6, true);

int counter = 0;
bool isWifiStarted = false;   // 核心标志位：记录 Wi-Fi 是否已经启动
unsigned long lastUpdate = 0; // 用于替代 delay 的时间戳

// ==========================================
// 提前声明和定义按键处理逻辑，避免编译报错
// ==========================================

// 启动 Wi-Fi 的专属函数
void startWiFi()
{
  isWifiStarted = true; // 锁上标志位，防止重复启动

  tft.fillRect(2, 28, 156, 60, TFT_BLACK); // 清除屏幕中间的提示文字
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Starting AP...", 10, 35, 2);

  WiFi.setTxPower(WIFI_POWER_5dBm);
  WiFi.softAP(ssid, password);

  // Wi-Fi 开启后，更新屏幕显示账号密码
  tft.fillRect(2, 28, 156, 60, TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("SSID: " + String(ssid), 10, 35, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("PWD: " + String(password), 10, 55, 2);
}

// 通用的按键处理函数
void handleButton(int btnNum)
{
  Serial.printf("Button %d clicked.\n", btnNum);

  // 在屏幕的一个空闲区域显示刚才按了哪个键 (带有黑色背景，防止重影)
  tft.fillRect(10, 75, 140, 16, TFT_BLACK);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.drawString("Btn " + String(btnNum) + " Clicked!", 10, 75, 2);

  // 核心逻辑：如果 Wi-Fi 还没启动，就立刻启动它！
  if (!isWifiStarted)
  {
    startWiFi();
  }
}

// 绑定每个按键的具体动作
void click1() { handleButton(1); }
void click2() { handleButton(2); }
void click3() { handleButton(3); }
void click4() { handleButton(4); }
void click5() { handleButton(5); }
void click6() { handleButton(6); }

// ==========================================
// 主程序
// ==========================================

void setup()
{
  Serial.begin(115200);

  // 绑定单击事件
  button1.attachClick(click1);
  button2.attachClick(click2);
  button3.attachClick(click3);
  button4.attachClick(click4);
  button5.attachClick(click5);
  button6.attachClick(click6);

  // 如果你之前需要 SPI.begin() 才能亮屏，请在这里补上
  // SPI.begin(12, 13, 11, -1);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // 绘制静态的 UI 框架
  tft.drawRect(0, 0, 160, 128, TFT_CYAN);
  tft.fillRect(2, 2, 156, 25, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.drawCentreString("Wi-Fi Base Station", 80, 6, 2);

  tft.drawLine(10, 95, 150, 95, TFT_DARKGREY);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Clients:", 10, 105, 2);

  // 开机引导提示
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Press ANY button", 10, 40, 2);
  tft.drawString("to start Wi-Fi...", 10, 60, 2);
}

void loop()
{
  // 1. 无阻塞地疯狂扫描按键状态
  button1.tick();
  button2.tick();
  button3.tick();
  button4.tick();
  button5.tick();
  button6.tick();

  // 2. 替代 delay() 的时间戳大法：每 1000 毫秒执行一次里面的代码
  if (millis() - lastUpdate >= 1000)
  {
    lastUpdate = millis();

    // 只有在 Wi-Fi 已经启动的情况下，才去查连接数量
    if (isWifiStarted)
    {
      int connectedDevices = WiFi.softAPgetStationNum();
      if (connectedDevices > 0)
      {
        tft.setTextColor(TFT_RED, TFT_BLACK);
      }
      else
      {
        tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
      }
      tft.drawNumber(connectedDevices, 75, 105, 2);
    }

    // 动态刷新右下角的运行证明
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.drawNumber(counter % 10, 140, 105, 2);
    counter++;
  }
}