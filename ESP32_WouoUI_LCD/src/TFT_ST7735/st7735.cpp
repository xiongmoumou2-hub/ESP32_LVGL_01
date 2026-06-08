#include "st7735.h"
#include <SPI.h>

// ==========================================
// 屏幕分辨率定义 (严格匹配横屏模式)
// ==========================================
#define ST7735_TFTWIDTH 160
#define ST7735_TFTHEIGHT 128

// ==========================================
// 内部驱动：底层通信
// ==========================================
inline void SPI_WriteByte(uint8_t data)
{
    SPI.transfer(data);
}

static void ST7735_WriteCmd(uint8_t cmd)
{
    ST7735_DC_CLR;
    ST7735_CS_CLR;
    SPI_WriteByte(cmd);
    ST7735_CS_SET;
}

static void ST7735_WriteData(uint8_t data)
{
    ST7735_DC_SET;
    ST7735_CS_CLR;
    SPI_WriteByte(data);
    ST7735_CS_SET;
}

// ==========================================
// 硬件与 SPI 初始化
// ==========================================
void ST7735_Hardware_Init(void)
{
    pinMode(TFT_CS, OUTPUT);
    pinMode(TFT_DC, OUTPUT);
    pinMode(TFT_RST, OUTPUT);
    pinMode(TFT_BL, OUTPUT);

    ST7735_CS_SET;
    digitalWrite(TFT_BL, HIGH); // 点亮背光

    // 初始化 ESP32-S3 硬件 SPI (时钟, MISO空着, MOSI, CS空着)
    SPI.begin(TFT_SCLK, -1, TFT_MOSI, -1);

    SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));
}

// ==========================================
// 屏幕寄存器初始化
// ==========================================
void ST7735_Init(void)
{
    // 硬件复位
    ST7735_RES_SET;
    delay(1);
    ST7735_RES_CLR;
    delay(10);
    ST7735_RES_SET;
    delay(120);

    ST7735_WriteCmd(0x11); // 退出休眠
    delay(120);

    // 基础配置
    ST7735_WriteCmd(0xB1);
    ST7735_WriteData(0x01);
    ST7735_WriteData(0x2C);
    ST7735_WriteData(0x2D);
    ST7735_WriteCmd(0xB2);
    ST7735_WriteData(0x01);
    ST7735_WriteData(0x2C);
    ST7735_WriteData(0x2D);
    ST7735_WriteCmd(0xB3);
    ST7735_WriteData(0x01);
    ST7735_WriteData(0x2C);
    ST7735_WriteData(0x2D);
    ST7735_WriteData(0x01);
    ST7735_WriteData(0x2C);
    ST7735_WriteData(0x2D);

    // 电源与色彩控制
    ST7735_WriteCmd(0xB4);
    ST7735_WriteData(0x07);
    ST7735_WriteCmd(0xC0);
    ST7735_WriteData(0xA2);
    ST7735_WriteData(0x02);
    ST7735_WriteData(0x84);
    ST7735_WriteCmd(0xC1);
    ST7735_WriteData(0xC5);
    ST7735_WriteCmd(0xC2);
    ST7735_WriteData(0x0A);
    ST7735_WriteData(0x00);
    ST7735_WriteCmd(0xC3);
    ST7735_WriteData(0x8A);
    ST7735_WriteData(0x2A);
    ST7735_WriteCmd(0xC4);
    ST7735_WriteData(0x8A);
    ST7735_WriteData(0xEE);
    ST7735_WriteCmd(0xC5);
    ST7735_WriteData(0x0E);

    // 关键：方向与色彩格式
    ST7735_WriteCmd(0x20); // 颜色反转关 (若画面颜色全反，请改回 0x21)

    // 0x60 = 横屏模式，匹配你的 160宽 x 128高
    // ST7735_WriteCmd(0x36);
    // ST7735_WriteData(0x60);

    // 0xA0 = 横屏模式（旋转180度）
    ST7735_WriteCmd(0x36);
    ST7735_WriteData(0xA0);

    ST7735_WriteCmd(0x3A);
    ST7735_WriteData(0x05); // 16位 RGB565

    ST7735_WriteCmd(0x29); // 开启显示
    delay(100);
}

// ==========================================
// 内部驱动：设置显示窗口
// ==========================================
static void ST7735_SetWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
    ST7735_WriteCmd(0x2A);
    ST7735_WriteData(xStart >> 8);
    ST7735_WriteData(xStart & 0xFF);
    ST7735_WriteData(xEnd >> 8);
    ST7735_WriteData(xEnd & 0xFF);

    ST7735_WriteCmd(0x2B);
    ST7735_WriteData(yStart >> 8);
    ST7735_WriteData(yStart & 0xFF);
    ST7735_WriteData(yEnd >> 8);
    ST7735_WriteData(yEnd & 0xFF);

    ST7735_WriteCmd(0x2C);
}

// ==========================================
// 快速刷单色全屏
// ==========================================
void ST7735_FillScreen(uint16_t color)
{
    uint32_t pixelCount = ST7735_TFTWIDTH * ST7735_TFTHEIGHT;
    uint8_t color_hi = color >> 8;
    uint8_t color_lo = color & 0xFF;

    ST7735_SetWindow(0, 0, ST7735_TFTWIDTH - 1, ST7735_TFTHEIGHT - 1);
    ST7735_DC_SET;
    ST7735_CS_CLR;

    for (uint32_t i = 0; i < pixelCount; i++)
    {
        SPI_WriteByte(color_hi);
        SPI_WriteByte(color_lo);
    }
    ST7735_CS_SET;
}

// ==========================================
// 局部开窗推流 (为其他彩色 GUI 框架预留)
// ==========================================
void ST7735_SendBuffer(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t *color_buff)
{
    uint32_t pixelCount = (xEnd - xStart + 1) * (yEnd - yStart + 1);
    ST7735_SetWindow(xStart, yStart, xEnd, yEnd);

    ST7735_DC_SET;
    ST7735_CS_CLR;
    for (uint32_t i = 0; i < pixelCount; i++)
    {
        SPI_WriteByte(color_buff[i] >> 8);
        SPI_WriteByte(color_buff[i] & 0xFF);
    }
    ST7735_CS_SET;
}


// ==========================================
// WouoUI 到 ST7735 的桥接函数
// ==========================================
void WouoUI_To_ST7735_Wrapper(uint8_t buff[][160])
{
    ST7735_SetWindow(0, 0, 159, 127);

    ST7735_DC_SET;
    ST7735_CS_CLR;

    // 【性能大杀器】：开辟一行像素的内存缓存 (160个像素 * 2字节 = 320字节)
    uint8_t line_buffer[320];

    // 逐行扫描
    for (uint8_t y = 0; y < 128; y++)
    {
        uint8_t page = y / 8;
        uint8_t bit_mask = 1 << (y % 8);

        // 1. 先在内存中高速计算出这一行的所有颜色 (极其快速)
        for (uint8_t x = 0; x < 160; x++)
        {
            if (buff[page][x] & bit_mask)
            {
                line_buffer[x * 2] = 0xFF;     // 白点高位
                line_buffer[x * 2 + 1] = 0xFF; // 白点低位
            }
            else
            {
                line_buffer[x * 2] = 0x00;     // 黑底高位
                line_buffer[x * 2 + 1] = 0x00; // 黑底低位
            }
        }

        // 2. 调用 Arduino ESP32 底层的块传输函数
        // 将一整行 320 个字节一次性轰击给屏幕！
        // 这样一来，一帧画面的函数调用次数从 40000 次骤降到 128 次！
        SPI.writeBytes(line_buffer, 320);
    }

    ST7735_CS_SET;
}