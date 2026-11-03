// ============================================================================
//  User_Setup.h — Cấu hình TFT_eSPI cho dự án "Nguồn DC đa năng"
//  Màn hình: TFT ST7789V 2.4" (240x320), giao tiếp SPI
//  Board   : STM32F411CEU6 (Black Pill)
// ============================================================================
//  File này dành cho ai NẠP CODE BẰNG ARDUINO IDE:
//    1. Cài thư viện "TFT_eSPI" của Bodmer qua Library Manager
//    2. COPY ĐÈ file này vào:  Documents/Arduino/libraries/TFT_eSPI/User_Setup.h
//       (thay thế toàn bộ nội dung file cũ)
//    3. Đó là tất cả — các chân đã trùng với bảng nối docs/03-bang-chan-stm32.md
//
//  Ai dùng VS Code + PlatformIO KHÔNG cần file này: cấu hình tương đương
//  đã nằm trong build_flags của firmware/platformio.ini.
//  Khi đổi chân nào ở đây, nhớ sửa đồng bộ trong platformio.ini.
// ============================================================================

#define USER_SETUP_INFO "Nguon DC da nang - BlackPill F411 + ST7789 2.4in"

// ---- Driver màn hình --------------------------------------------------------
#define ST7789_DRIVER

#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// ---- Chân nối (khớp 100% với docs/03-bang-chan-stm32.md) -------------------
#define TFT_MISO -1      // không dùng MISO — PA6 được làm chân DC
#define TFT_MOSI PA7     // SDA màn hình
#define TFT_SCLK PA5     // SCL màn hình
#define TFT_CS   PA4
#define TFT_DC   PA6
#define TFT_RST  PB0

#define LOAD_GLCD        // font 5x7 mặc định dùng trong firmware

// ---- Tần số SPI -------------------------------------------------------------
// 27MHz an toàn cho dây nối trên bo đục lỗ. Nếu màn vẽ chậm muốn nhanh hơn,
// thử 40000000 — nếu màn hình bắt đầu "nhiễu nét" thì trả về 27MHz.
#define SPI_FREQUENCY  27000000

// ---- Màu màn ----------------------------------------------------------------
// ST7789 cần bật inversion để màu đúng. Nếu thấy màu đỏ/xanh bị đảo,
// xóa dòng này đi (một số ít module bán ngược màu).
#define TFT_INVERSION_ON
