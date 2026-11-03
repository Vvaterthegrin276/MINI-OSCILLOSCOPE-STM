/*
 * ============================================================================
 *  NGUỒN ĐA NĂNG (Multi-Function DC Power Supply) — Firmware
 * ============================================================================
 *  Board    : STM32F411CEU6 (Black Pill)
 *  Màn hình : TFT ST7789V 2.4" 240x320, giao tiếp SPI
 *  License  : MIT
 *
 *  Chức năng:
 *   - Dashboard     : hiển thị Điện áp / Dòng / Công suất bằng số lớn
 *   - Oscilloscope  : vẽ dạng sóng mini (Gợn AC / Volt / Dòng), 5 thước thời gian
 *   - Thông tin     : trang giới thiệu + hướng dẫn nút bấm
 *   - 4 nút         : MODE (đổi màn), CHANNEL (đổi kênh sóng),
 *                     TIME/DIV (đổi thước thời gian), HOLD (đóng băng)
 *
 *  GHI CHÚ QUAN TRỌNG: chữ in lên màn hình KHÔNG DẤU (vd "DIEN AP" thay vì
 *  "Điện áp") vì font 5x7 có sẵn của TFT_eSPI không hỗ trợ dấu tiếng Việt.
 *
 *  Nối chân xem chi tiết tại docs/03-bang-chan-stm32.md
 *  Cài IDE & nạp code xem tại docs/05-huong-dan-nap-code.md
 * ============================================================================
 */

#include <TFT_eSPI.h>          // Thư viện màn hình (driver ST7789 + đồ họa)
                               // Cấu hình chân nằm trong platformio.ini (build_flags)
                               // hoặc firmware/User_Setup.h (Arduino IDE)

/* ============================================================================
 *  1. ĐỊNH NGHĨA CHÂN (khớp 100% với docs/03-bang-chan-stm32.md)
 * ============================================================================ */
#define TFT_CS    PA4    // SPI Chip Select
#define TFT_DC    PA6    // Data/Command
#define TFT_RST   PB0    // Reset màn
// PA5 = SCK và PA7 = MOSI là SPI1 phần cứng — cấu hình trong platformio.ini (build_flags)
// hoặc firmware/User_Setup.h (Arduino IDE), không khai báo lại ở đây

#define BTN_MODE  PA3    // Nút MODE    (chân kia của nút xuống GND, dùng pull-up nội bộ)
#define BTN_CH    PB10   // Nút CHANNEL
#define BTN_TIME  PB1    // Nút TIME/DIV — LƯU Ý: Black Pill KHÔNG có chân PB11 trên header,
                         // nên nút này dùng PB1 (xem docs/03)
#define BTN_HOLD  PB12   // Nút HOLD

#define PIN_V     PA0    // ADC đo ĐIỆN ÁP (từ cầu phân áp 10k/1.2k)
#define PIN_I     PA1    // ADC đo DÒNG    (từ ACS712 qua cầu 10k/20k)
#define PIN_AC    PA2    // ADC đo GỢN AC  (từ chân 02 của module LM358 2 tầng)

/* ============================================================================
 *  2. THÔNG SỐ HIỆU CHỈNH — sửa các số này sau khi làm docs/06-hieu-chinh.md
 * ============================================================================ */
const float VREF        = 3.30f;    // Điện áp chuẩn ADC. Nếu VOM đo chân 3V3 được 3.28 thì sửa 3.28
const float DIV_RATIO   = 9.3333f;  // Hệ số cầu phân áp Volt: (10k + 1.2k) / 1.2k
const float I_DIV       = 1.5f;     // Hệ số ngược cầu chia dòng: (10k + 20k) / 20k
const float ACS_VOFFSET = 2.50f;    // Điện áp ngõ ra ACS712 tại 0A (đo thực tế rồi thay)
const float ACS_SENS    = 0.185f;   // Độ nhạy ACS712-05B: 0.185 V/A (bản 20A = 0.100, 30A = 0.066)

/* ============================================================================
 *  3. CẤU HÌNH MÀN HÌNH
 * ============================================================================ */
#define SCREEN_W  240
#define SCREEN_H  320
TFT_eSPI tft = TFT_eSPI();     // chân CS/DC/RST/SCL/SDA lấy từ cấu hình TFT_eSPI

// Bảng màu RGB565 (rút gọn 16bit: 5bit đỏ, 6bit xanh lá, 5bit xanh dương)
#define C_BG      0x0000   // đen
#define C_TEXT    0xFFFF   // trắng
#define C_YELLOW  0xFFE0   // vàng    — số điện áp
#define C_GREEN   0x07E0   // xanh lá — số dòng
#define C_CYAN    0x07FF   // xanh lơ — số công suất
#define C_ORANGE  0xFD20   // cam     — đường sóng
#define C_GRAY    0x7BEF   // xám     — nhãn phụ
#define C_GRID    0x2104   // xanh xám rất tối — lưới scope
#define C_RED     0xF800   // đỏ      — cờ HOLD

/* ============================================================================
 *  4. BIẾN TRẠNG THÁI
 * ============================================================================ */
enum Screen : uint8_t { SCR_DASH = 0, SCR_SCOPE = 1, SCR_INFO = 2 };
Screen screen = SCR_DASH;                  // Màn hình đang hiển thị

enum ScopeCh : uint8_t { CH_RIPPLE = 0, CH_VOLT = 1, CH_CURR = 2 };
ScopeCh scopeCh = CH_RIPPLE;               // Kênh oscilloscope đang chọn

// Thước thời gian: vi-giây chèn thêm giữa 2 mẫu (0 = lấy nhanh nhất)
const uint16_t TIMEBASE_US[]  = { 0, 100, 500, 2000, 10000 };
const char*    TIMEBASE_NAME[] = { "FAST", "100us", "500us", "2ms", "10ms" };
const uint8_t  N_TIMEBASE = sizeof(TIMEBASE_US) / sizeof(TIMEBASE_US[0]);
uint8_t timebaseIdx = 0;

bool holding    = false;                   // true = đóng băng màn hình
bool needRedraw = true;                    // cờ "phải vẽ lại toàn bộ khung"

// Bộ lọc trung bình trượt cho Dashboard
const uint8_t N_AVG = 16;
uint32_t sumV = 0, sumI = 0;
uint8_t  cntAvg = 0;
float vAvg = 0, iAvg = 0;

/* ============================================================================
 *  5. KHỞI ĐỘNG — chạy 1 lần khi có điện
 * ============================================================================ */
void setup() {
  Serial.begin(115200);                    // Mở Serial Monitor để debug cũng được

  // ---- 4 nút nhấn: INPUT_PULLUP nên KHÔNG cần trở ngoài ----
  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_CH,   INPUT_PULLUP);
  pinMode(BTN_TIME, INPUT_PULLUP);
  pinMode(BTN_HOLD, INPUT_PULLUP);

  // ---- ADC 12 bit: giá trị đọc về 0..4095 ----
  analogReadResolution(12);

  // ---- Màn hình (kích thước 240x320 đã khai báo trong cấu hình TFT_eSPI) ----
  tft.init();
  tft.setRotation(0);                      // màn dọc, gốc trên-trái
  tft.fillScreen(C_BG);

  // ---- Màn chào mừng ----
  tft.setTextColor(C_TEXT);
  tft.setTextSize(2);
  tft.setCursor(40, 120); tft.print("NGUON DA NANG");
  tft.setTextSize(1);
  tft.setTextColor(C_GRAY);
  tft.setCursor(58, 150); tft.print("STM32F411CEU6  |  Black Pill");
  delay(1200);
  tft.fillScreen(C_BG);
}

/* ============================================================================
 *  6. VÒNG LẶP CHÍNH
 * ============================================================================ */
void loop() {
  handleButtons();        // 1. Đọc 4 nút (có lọc rung)
  readMeasurements();     // 2. Lấy mẫu ADC + lọc trung bình

  if (screen == SCR_INFO) return;          // Trang thông tin là trang tĩnh

  if (!holding) {                          // HOLD thì không vẽ gì mới (đóng băng)
    if (screen == SCR_DASH) drawDashboard();
    else                    drawScope();
  }

  updateHoldBadge();                       // 3. Vẽ/xóa cờ HOLD góc phải trên
}

/* ============================================================================
 *  7. ĐO & LỌC SỐ LIỆU
 * ============================================================================ */
float adcToVolt(uint16_t raw) {
  return raw * (VREF / 4095.0f);
}

void readMeasurements() {
  sumV += analogRead(PIN_V);
  sumI += analogRead(PIN_I);
  if (++cntAvg >= N_AVG) {                                   // đủ 16 lần lấy mẫu
    float vPA0 = adcToVolt(sumV / N_AVG);
    float vPA1 = adcToVolt(sumI / N_AVG);

    // Điện áp đầu ra = điện áp tại PA0 nhân hệ số cầu phân áp
    vAvg = vPA0 * DIV_RATIO;

    // Dòng: khôi phục điện áp gốc ACS712 (nhân ngược cầu chia 10k/20k),
    // trừ mức 2.5V của 0A, rồi chia cho độ nhạy 0.185 V/A
    iAvg = (vPA1 * I_DIV - ACS_VOFFSET) / ACS_SENS;

    sumV = sumI = 0;
    cntAvg = 0;
  }
}

/* ============================================================================
 *  8. MÀN HÌNH DASHBOARD (3 số lớn: V / A / W)
 * ============================================================================ */
void drawDashboard() {
  static float lastV = -999, lastI = -999, lastP = -999;
  static bool  frameDone = false;

  // ---- Vẽ lại khung khi đổi màn hình ----
  if (needRedraw || !frameDone) {
    tft.fillScreen(C_BG);
    tft.setTextColor(C_GRAY);
    tft.setTextSize(1);
    tft.setCursor(20, 40);  tft.print("DIEN AP (VOLT)");
    tft.setCursor(20, 110); tft.print("DONG DIEN (CURRENT)");
    tft.setCursor(20, 180); tft.print("CONG SUAT (POWER)");
    tft.setCursor(48, 292); tft.print("[MODE] doi man hinh");
    lastV = lastI = lastP = -999;          // ép vẽ lại cả 3 số
    frameDone = true;
    needRedraw = false;
  }

  float p = vAvg * iAvg;
  if (p < 0) p = 0;

  // ---- Chỉ vẽ lại số khi giá trị thay đổi đủ lớn (tránh nhấp nháy) ----
  if (fabs(vAvg - lastV) > 0.04f) {
    tft.fillRect(20, 58, 200, 32, C_BG);
    tft.setTextColor(C_YELLOW); tft.setTextSize(4);
    tft.setCursor(20, 58); tft.print(vAvg, 2); tft.print(" V");
    lastV = vAvg;
  }
  if (fabs(iAvg - lastI) > 0.02f) {
    tft.fillRect(20, 128, 200, 32, C_BG);
    tft.setTextColor(C_GREEN); tft.setTextSize(4);
    tft.setCursor(20, 128); tft.print(fabs(iAvg), 2); tft.print(" A");
    lastI = iAvg;
  }
  if (fabs(p - lastP) > 0.4f) {
    tft.fillRect(20, 198, 200, 32, C_BG);
    tft.setTextColor(C_CYAN); tft.setTextSize(4);
    tft.setCursor(20, 198); tft.print(p, 1); tft.print(" W");
    lastP = p;
  }
}

/* ============================================================================
 *  9. MÀN HÌNH OSCILLOSCOPE MINI
 * ============================================================================ */
const uint8_t SCOPE_N = 120;               // 120 cột x 2px = 240px chiều ngang
uint16_t waveBuf[SCOPE_N];                 // bộ đệm 1 khung sóng

void drawScope() {
  static bool frameDone = false;

  // ---- Vẽ lại khung lưới + tiêu đề khi đổi màn / đổi kênh / đổi thước ----
  if (needRedraw || !frameDone) {
    tft.fillScreen(C_BG);
    tft.setTextColor(C_GRAY); tft.setTextSize(1);
    tft.setCursor(8, 10); tft.print("SCOPE: ");
    tft.setTextColor(C_TEXT);
    tft.print(scopeChName());
    tft.setTextColor(C_GRAY);
    tft.setCursor(140, 10); tft.print("t/cot: ");
    tft.print(TIMEBASE_NAME[timebaseIdx]);

    // Lưới nền
    for (int x = 0; x <= SCREEN_W; x += 24) tft.drawFastVLine(x, 30, 200, C_GRID);
    for (int y = 30; y <= 230; y += 25)     tft.drawFastHLine(0, y, SCREEN_W, C_GRID);
    // Trục giữa (tương ứng mức 0 "sinh lý" của sóng)
    tft.drawFastHLine(0, 130, SCREEN_W, C_GRAY);

    tft.setTextColor(C_GRAY);
    tft.setCursor(8, 292); tft.print("[CH] doi kenh  [T/DIV] thoi gian");
    frameDone = true;
    needRedraw = false;
  }

  captureWave();                           // 1. lấy 120 mẫu theo thước thời gian
  renderWave();                            // 2. vẽ đường sóng màu cam
}

const char* scopeChName() {
  switch (scopeCh) {
    case CH_RIPPLE: return "Gon AC";
    case CH_VOLT:   return "Volt";
    case CH_CURR:   return "Dong";
  }
  return "?";
}

void captureWave() {
  // Chọn chân ADC theo kênh đang hiển thị
  uint8_t pin;
  if (scopeCh == CH_RIPPLE) pin = PIN_AC;
  else if (scopeCh == CH_VOLT)  pin = PIN_V;
  else                          pin = PIN_I;

  uint16_t dt = TIMEBASE_US[timebaseIdx];

  for (uint8_t i = 0; i < SCOPE_N; i++) {
    uint32_t s = 0;
    for (uint8_t k = 0; k < 4; k++) {      // trung bình 4 mẫu mỗi cột cho sóng mượt
      s += analogRead(pin);
    }
    waveBuf[i] = s / 4;
    if (dt) delayMicroseconds(dt);         // giãn thời gian theo thước đã chọn
  }
}

void renderWave() {
  static uint8_t prevY[SCOPE_N] = {0};
  static bool    prevValid = false;

  // Quy đổi: giá trị ADC 0..4095 → tọa độ y 230..30 (y nhỏ = giá trị cao)
  uint8_t newY[SCOPE_N];
  for (uint8_t x = 0; x < SCOPE_N; x++) {
    newY[x] = 230 - (uint32_t)waveBuf[x] * 200 / 4095;
  }

  // Xóa đường sóng cũ (vẽ đè màu nền)
  if (prevValid) {
    for (uint8_t x = 1; x < SCOPE_N; x++) {
      tft.drawLine((x - 1) * 2, prevY[x - 1], x * 2, prevY[x], C_BG);
      tft.drawLine((x - 1) * 2 + 1, prevY[x - 1], x * 2 + 1, prevY[x], C_BG);
    }
  }

  // Vẽ đường sóng mới (mỗi cột rộng 2px cho khớp 240px màn hình)
  for (uint8_t x = 1; x < SCOPE_N; x++) {
    tft.drawLine((x - 1) * 2, newY[x - 1], x * 2, newY[x], C_ORANGE);
    tft.drawLine((x - 1) * 2 + 1, newY[x - 1], x * 2 + 1, newY[x], C_ORANGE);
  }

  memcpy(prevY, newY, SCOPE_N);
  prevValid = true;
}

/* ============================================================================
 * 10. CỜ HOLD (đóng băng) — vẽ/xóa góc phải trên
 * ============================================================================ */
void updateHoldBadge() {
  static bool lastHolding = false;
  if (holding == lastHolding) return;      // trạng thái chưa đổi thì thôi
  lastHolding = holding;

  if (holding) {
    tft.fillRect(168, 4, 64, 22, C_RED);
    tft.setTextSize(2);
    tft.setTextColor(C_BG, C_RED);
    tft.setCursor(174, 6); tft.print("HOLD");
  } else {
    tft.fillRect(168, 4, 64, 22, C_BG);
    needRedraw = true;                     // khung bị xóa một phần → vẽ lại cả màn
  }
}

/* ============================================================================
 * 11. ĐỌC NÚT NHẤN — có lọc rung (debounce) bằng millis + chờ nhả nút
 * ============================================================================ */
bool pressed(uint8_t pin, uint32_t &lastMs) {
  if (digitalRead(pin) == LOW && millis() - lastMs > 250) {  // chống rung 250ms
    lastMs = millis();
    while (digitalRead(pin) == LOW) { delay(10); }           // chờ người dùng nhả
    return true;
  }
  return false;
}

void handleButtons() {
  static uint32_t tMode = 0, tCh = 0, tTime = 0, tHold = 0;

  if (pressed(BTN_MODE, tMode)) {                       // ---- MODE: đổi màn
    screen = (Screen)((screen + 1) % 3);
    needRedraw = true;
    if (screen == SCR_INFO) drawInfo();
  }
  if (pressed(BTN_CH, tCh) && screen == SCR_SCOPE) {    // ---- CHANNEL: đổi kênh
    scopeCh = (ScopeCh)((scopeCh + 1) % 3);
    needRedraw = true;
  }
  if (pressed(BTN_TIME, tTime) && screen == SCR_SCOPE) { // ---- TIME/DIV: đổi thước
    timebaseIdx = (timebaseIdx + 1) % N_TIMEBASE;
    needRedraw = true;
  }
  if (pressed(BTN_HOLD, tHold)) {                       // ---- HOLD: đóng băng
    holding = !holding;
  }
}

/* ============================================================================
 * 12. MÀN HÌNH THÔNG TIN
 * ============================================================================ */
void drawInfo() {
  tft.fillScreen(C_BG);
  tft.setTextColor(C_TEXT);
  tft.setTextSize(2);
  tft.setCursor(56, 60); tft.print("NGUON DA NANG");

  tft.setTextSize(1);
  tft.setTextColor(C_GRAY);
  tft.setCursor(50, 95);  tft.print("Ban lap rap module tren bo duc lo");
  tft.setCursor(50, 115); tft.print("STM32F411CEU6 + TFT ST7789V 2.4in");

  tft.setTextColor(C_TEXT);
  tft.setCursor(50, 150); tft.print("NUT BAM:");
  tft.setTextColor(C_GRAY);
  tft.setCursor(50, 170); tft.print("MODE     : doi man hinh");
  tft.setCursor(50, 188); tft.print("CHANNEL  : doi kenh scope");
  tft.setCursor(50, 206); tft.print("TIME/DIV : doi thoi gian");
  tft.setCursor(50, 224); tft.print("HOLD     : dong bang man hinh");

  tft.setTextColor(C_YELLOW);
  tft.setCursor(50, 262); tft.print("github.com/<ban>/nguon-dc-da-nang");
}
