#ifndef __ST7735_H__
#define __ST7735_H__

#include <Arduino.h>
#include <stdint.h>

// ==========================================
// 1. 引脚定义 (请根据你的 ESP32-S3 实际接线确认)
// ==========================================
#define TFT_MOSI 11 // SPI 数据线
#define TFT_SCLK 12 // SPI 时钟线
#define TFT_CS 10   // 片选引脚
#define TFT_DC 9    // 数据/命令控制引脚
#define TFT_RST 8   // 复位引脚
#define TFT_BL 7    // 背光控制引脚

// ==========================================
// 2. 底层控制宏定义
// ==========================================
#define ST7735_CS_CLR digitalWrite(TFT_CS, LOW)
#define ST7735_CS_SET digitalWrite(TFT_CS, HIGH)

#define ST7735_DC_CLR digitalWrite(TFT_DC, LOW)
#define ST7735_DC_SET digitalWrite(TFT_DC, HIGH)

#define ST7735_RES_CLR digitalWrite(TFT_RST, LOW)
#define ST7735_RES_SET digitalWrite(TFT_RST, HIGH)

// ==========================================
// 3. 常用 RGB565 颜色定义
// ==========================================
#define ST7735_BLACK 0x0000
#define ST7735_WHITE 0xFFFF
#define ST7735_RED 0xF800
#define ST7735_GREEN 0x07E0
#define ST7735_BLUE 0x001F

// ==========================================
// 4. 对外开放的 API 函数声明
// ==========================================
void ST7735_Hardware_Init(void);
void ST7735_Init(void);
void ST7735_FillScreen(uint16_t color);
void ST7735_SendBuffer(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t *color_buff);

// UI 桥接器：已完全适配 WouoUI 的 160x128 分辨率
void WouoUI_To_ST7735_Wrapper(uint8_t buff[][160]);

#endif // __ST7735_H__