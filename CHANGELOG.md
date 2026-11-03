# 📜 CHANGELOG — Lịch sử phiên bản

Toàn bộ thay đổi đáng chú ý của dự án được ghi lại trong file này.
Format dựa trên [Keep a Changelog](https://keepachangelog.com/) — phiên bản theo dạng `MAJOR.MINOR.PATCH`.

## [5.3] — 2026-11-02

### Bản hiện tại — "Đổi vi điều khiển sang STM32F411 (Black Pill)"

#### Thay đổi (Changed)
- Đổi board từ STM32F103C8T6 (Blue Pill) sang **STM32F411CEU6 (Black Pill)** — ADC 12 bit, RAM 128KB, phép tính float nhanh hơn; chân nối giữ nguyên ở PA0–PA7, PB0, PB10, PB12, 3V3/5V/GND — **riêng nút TIME/DIV chuyển từ PB11 sang PB1** vì Black Pill không đưa chân PB11 ra header (lỗi biên dịch `PB11 was not declared` với core STM32duino nếu dùng PB11)
- Firmware đổi sang thư viện màn hình **TFT_eSPI (Bodmer)** — vẽ nhanh hơn bộ Adafruit GFX + ST7789; cấu hình chân màn (CS=PA4, DC=PA6, RST=PB0, SCL=PA5, SDA=PA7) khai báo trong `build_flags` của `platformio.ini`, không phải sửa file thư viện
- Thêm **`firmware/User_Setup.h`** — file cấu hình TFT_eSPI sẵn sàng cho ai dùng Arduino IDE (copy đè vào thư mục thư viện)
- **Sửa lỗi biên dịch với core STM32duino 4.x**: `'ltoa' was not declared in this scope` trong TFT_eSPI — thêm `-include api/itoa.h` vào `build_flags` của `platformio.ini`; docs/05 thêm dòng xử lý lỗi tương ứng cho Cách B (Arduino IDE)
- Firmware build thành công với board `blackpill_f411ce` (kiểm tra thực tế: Flash 29.6KB / 512KB, RAM 2KB / 128KB); sửa lỗi chính tả 2 chuỗi in trên màn hình ("thoc thoi gian" → "thoi gian")
- Docs đồng bộ: bảng chân thêm cột **chữ in thật trên module TFT** (`3.3V/GND/CS/RESET/DC/MOSI/SCK/LED`) và ghi chú riêng cho Black Pill (5V vào chân 5V, jumper BOOT0 về 0); Bước 0/8/9/12 lắp ráp cập nhật theo module TFT thật; nạp code đổi sang board `blackpill_f411ce` / Generic STM32F4 series, thêm 2 trường hợp cấp nguồn khi nạp và lỗi BOOT0 trong bảng xử lý lỗi
- BOM: board STM32F411CEU6 (Black Pill), chi phí ước tính cập nhật (~750.000đ – 1.000.000đ)

#### Sơ đồ (Diagrams)
- **Vẽ lại cả 4 sơ đồ PNG** theo phong cách schematic kỹ thuật (nền trắng, khung vuông nét mực, phân màu dây kiểu in ấn) — bỏ card bo tròn, gradient, badge tròn; trông như sơ đồ tự vẽ chứ không phải ảnh infographic
- Sơ đồ nguyên lý: **bổ sung 2 tụ gốm 104** sát PA0/PA1 (mạch đo thật thiếu 2 tụ này); **tách màu đường ra 0–23V** khỏi bus 24V để không nhầm; nhãn chip đổi sang STM32F411CEU6 (Black Pill) kèm ghi chú BOOT0

## [5.2] — 2026-10-27

### Bản hiện tại — "Đổi sang module LM358 2 tầng"

#### Thêm mới (Added)
- Hỗ trợ **VS Code + PlatformIO**: thêm `firmware/platformio.ini` cấu hình sẵn (board `bluepill_f103c8`, framework Arduino, nạp qua ST-Link, `lib_deps` tự tải Adafruit GFX + ST7735/ST7789) — mở thư mục `firmware/` bằng VS Code là build + nạp được ngay; Arduino IDE vẫn dùng như cũ

#### Thay đổi (Changed)
- Thay toàn bộ mạch khuếch đại gợn AC bằng linh kiện rời (LM358 + trở 10k/20k + tụ 1µF/104, gain ×2 cố định, bias 1.65V) bằng **module LM358 2 tầng** — gain chỉnh bằng 2 biến trở xanh ngay trên module, không cần hàn thêm linh kiện
- Nối dây Bước 7 mới theo module: cụm đầu vào (V–IN–G) lấy tín hiệu từ **Cọc Đỏ (+)**, cụm đầu ra (G–02–V) đưa chân **02** về **PA2**, cụm giữa (G–01) bỏ trống
- Bước 3 lắp ráp: **tháo hẳn 2 biến trở xanh CV/CC** khỏi board XL4016 và hàn dây ra chiết áp ngoài (Volt: WXD3 10 vòng • Dòng: chiết áp thường 10k)
- Firmware nâng cấp lên v5.2 — chỉ đổi version và ghi chú, không đổi logic đo

#### Tài liệu (Docs)
- Viết lại **docs/05-huong-dan-nap-code.md**: tách 2 lộ trình nạp code — Cách A: **VS Code + PlatformIO** (khuyên dùng), Cách B: Arduino IDE; thêm bảng xử lý lỗi riêng cho PlatformIO; đồng bộ badge IDE, firmware README, CONTRIBUTING và BOM (docs/02) theo 2 cách build
- README thêm dòng **Open Project — môn Linh Kiện Mạch Điện Tử** và mục **Nhóm thực hiện** (4 thành viên + giảng viên hướng dẫn, dùng placeholder để nhóm tự điền); README_EN đồng bộ mục Team
- Vẽ **sơ đồ nguyên lý mạch** (images/so-do-nguyen-ly.png) — schematic chi tiết mọi nối dây thật, khớp bảng chân docs/03 và 14 bước hàn docs/04; nhúng vào cả 2 README + hardware
- BOM (docs/02) gọn hơn: bỏ 1 trở 10kΩ (bias), 1 trở 20kΩ (hồi tiếp), tụ hóa 1µF, tụ gốm 100nF — còn đúng 2× 10kΩ, 1× 1.2kΩ, 1× 20kΩ
- Cập nhật bảng chân (docs/03), quy trình hàn Bước 3/7/11/13 (docs/04), hiệu chỉnh mục 3 (docs/06), hardware README, firmware README
- Vẽ lại 2 sơ đồ PNG (so-do-khoi, so-do-chan-stm32) theo module LM358
- README: bỏ badge phiên bản, chỉnh thông số **0 – 23V / tối đa 5A** (giới hạn bởi nguồn 24V/5A), hướng đến sinh viên năm 1–2 + người đam mê chế đồ, thêm mục **"Linh kiện thật"** (6 ảnh chụp thực tế), bỏ mục Lộ trình (sẽ thêm lại sau), viết lại mục Đóng góp ngắn gọn hơn
- BOM: chỉ dùng **1 chiết áp 10 vòng WXD3-12/13 (Volt)** + **1 chiết áp thường 10k B10K (Dòng)**; nguồn 24V "tổ ong" vỏ nhôm hiệu suất 90%
- Dự án **không dùng PCB** — hàn tay trên bo đục lỗ 5×7cm: hardware README viết lại, bỏ mọi kế hoạch KiCad/PCB

## [5.1] — 2026-10-23

### Bản hiện tại — "Bản dành cho người hàn"

#### Thêm mới (Added)
- Sơ đồ khối hệ thống + sơ đồ chân STM32 trực quan (thư mục `images/`)
- Bộ tài liệu 6 chương: an toàn, BOM, bảng chân, 14 bước lắp ráp, nạp code, hiệu chỉnh
- Firmware Arduino hoàn chỉnh: Dashboard (V/A/W), oscilloscope mini 3 kênh 5 thước thời gian, 4 nút điều khiển, cờ HOLD
- GitHub templates: mẫu báo lỗi, mẫu đề xuất tính năng, mẫu pull request

#### Đã biết (Known issues)
- Chữ trên màn hình không dấu (font 5x7 của Adafruit GFX không hỗ trợ ký tự tiếng Việt)
- Đo dòng dưới 100mA chưa chính xác do nhiễu ACS712 — hẹn bản v6 dùng trở shunt + op-amp tốt hơn

## [5.0] — 2026-10 (chưa công khai)

- Đổi từ Arduino Uno sang STM32F103C8T6 — ADC 12 bit chính xác hơn, màn vẽ mượt hơn
- Thêm kênh đo gợn AC qua LM358 (bias 1.65V, gain ×2) — *(từ v5.2 đã đổi sang module LM358 2 tầng)*
- Thêm oscilloscope mini thay cho chỉ số Vpp dạng text

## [1.x–4.x] — 2026 (giai đoạn thí nghiệm)

- 1.x: Bản đầu tiên dùng Arduino Nano + LCD 16x2, chỉ đo áp
- 2.x: Thêm ACS712 đo dòng, thêm quạt tản nhiệt
- 3.x: Chuyển sang TFT SPI màu, thử ST7735 1.8"
- 4.x: Thử nghiệm STM32, đụng độ nhiều lỗi SPI — đẻ ra v5.0

## Định dạng ghi chép

- **Thêm mới (Added)**: tính năng, tài liệu, file mới
- **Thay đổi (Changed)**: thay đổi làm ảnh hưởng cách dùng cũ
- **Sửa lỗi (Fixed)**: sửa lỗi
- **Đã biết (Known issues)**: hạn chế đang tồn tại

> Khi bạn đóng góp được gộp vào repo, tên bạn sẽ xuất hiện ở phiên bản kế tiếp!
