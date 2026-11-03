# 📍 Bảng chân STM32F411CEU6 (Black Pill)

> Bảng này là "bản đồ" nối dây cho toàn bộ dự án. Cột **"Nối trực tiếp đến"** ghi rõ linh kiện gì — không cần suy luận thêm. Sơ đồ trực quan: xem `images/so-do-chan-stm32.png` (bảng chân) và `images/so-do-nguyen-ly.png` (sơ đồ nguyên lý toàn mạch).
>
> 💡 Về dây nối: so với Blue Pill (F103), Black Pill giữ **nguyên vị trí các chân PA0–PA7, PB0, PB10, PB12** — chỉ khác board là STM32F411CEU6 (RAM 128KB, ADC 12 bit, nhanh hơn) và cách cấp nguồn/jumper BOOT0 ghi ở ghi chú dưới bảng nguồn.
>
> ⚠️ **Khác nhau DUY NHẤT phải đi lại dây:** Black Pill **không đưa chân PB11 ra header** (vị trí đó trên board là PB2, và hàng chân đó là `B10 • B2 • B1 • B0`). Vì vậy nút **TIME/DIV chuyển từ PB11 sang PB1** — xem bảng nút nhấn ở mục 4. Đây là lỗi hay gặp nhất khi ai đó vẫn theo bảng chân cũ của Blue Pill.

---

## 1. Nhóm nguồn

| Chân STM32 | Nối trực tiếp đến | Chức năng |
|---|---|---|
| **5V** | Chân OUT+ của LM2596 | Nguồn nuôi toàn board — Black Pill tự ổn áp xuống 3.3V bên trong |
| **3V3** | Chân VCC màn hình TFT • Chân BLK đèn nền TFT | Nguồn 3.3V cho màn hình |
| **GND** | Tất cả các mass — xem nhóm "BUS GND" bên dưới | Điểm mass chung |

> ⚠️ **Riêng với board Black Pill (F411), nhớ 2 điều:**
>
> 1. **Chỉ cấp 5V vào chân 5V** (board tự hạ xuống 3.3V bên trong). Tuyệt đối **không cấp 5V vào chân 3V3** — cháy board ngay lập tức.
> 2. **Jumper BOOT0 phải về vị trí 0 (GND)** để chip khởi động từ Flash. Nếu BOOT0 = 1, board có điện nhưng màn hình không chạy firmware.

## 2. Nhóm đo (ADC — chân analog)

| Chân STM32 | Nối trực tiếp đến | Chức năng |
|---|---|---|
| **PA0** | Chân 2 trở 10kΩ (đo Volt) • Chân 1 trở 1.2kΩ • Tụ gốm 104 (đầu kia xuống GND) | ADC đo **Điện áp** đầu ra |
| **PA1** | Chân 2 trở 10kΩ (đo Dòng) • Chân 1 trở 20kΩ (đầu kia xuống GND) • Tụ gốm 104 | ADC đo **Dòng điện** qua ACS712 |
| **PA2** | Chân **02** của module LM358 2 tầng (cụm đầu ra) | ADC đo **Gợn sóng AC** (oscilloscope mini) |

### Giải thích cầu đo Volt (PA0)
Dòng ra tải đi qua cầu phân áp gồm trở **10kΩ nối tiếp 1.2kΩ** hạ xuống GND. Điểm giữa hai trở nối vào PA0. Hệ số chia = (10k + 1.2k) / 1.2k = **9.333**. Vậy đầu ra 23V tương ứng ~2.46V tại PA0 — nằm gọn trong dải đo 3.3V của STM32. Tụ gốm 104 hàn **sát chân chip** giúp lọc nhiễu, số đo không nhảy loạn.

### Giải thích cầu đo Dòng (PA1)
ACS712 xuất tín hiệu quanh mức 2.5V (0A). Cầu chia 10kΩ/20kΩ nhân thêm hệ số **2/3** để tín hiệu nằm an toàn trong 0–3.3V. Khi tính dòng trong firmware, ta nhân ngược lại hệ số 1.5 rồi lấy (V − 2.5V) / 0.185 (V/A với bản 5A).

### Giải thích kênh Gợn AC (PA2)
Kênh này không dùng cầu trở rời nữa mà dùng hẳn **module khuếch đại LM358 2 tầng** — trên module có sẵn 2 biến trở xanh để chỉnh hệ số khuếch đại của từng tầng. Tín hiệu lấy **trực tiếp từ Cọc Đỏ (+)** nối vào chân IN của cụm đầu vào (V–IN–G); module khuếch đại qua 2 tầng rồi đưa ra chân **02** thuộc cụm đầu ra (G–02–V), nối thẳng vào PA2. Cụm chân ở giữa (chữ in G–01) là ngõ ra tầng 1 — **bỏ trống**, không đấu gì cả. Nhờ gain chỉnh được bằng tay, bạn vặn lớn để soi rõ gợn nhỏ, vặn nhỏ khi sóng bị dẹt cắt đầu — chi tiết xem [docs/06-hieu-chinh.md](06-hieu-chinh.md) mục 3.

## 3. Nhóm màn hình TFT (SPI)

| Chân TFT | Chữ in trên module | Chân STM32 | Chức năng |
|---|---|---|---|
| VCC | `3.3V` | 3V3 | Nguồn màn — **TUYỆT ĐỐI không cắm 5V** |
| GND | `GND` | GND | Mass |
| CS | `CS` | PA4 | Chip Select — chọn màn hình trên bus SPI |
| SCL | `SCK` | PA5 | SPI Clock (trùng SCK/SPI1 mặc định của STM32) |
| DC | `DC` | PA6 | Data/Command — phân biệt lệnh hay dữ liệu |
| SDA | `MOSI` | PA7 | SPI Data (trùng MOSI/SPI1 mặc định của STM32) |
| RST | `RESET` | PB0 | Reset màn hình |
| BLK | `LED` | 3V3 | Đèn nền (backlight), luôn bật |

> 💡 Module TFT 2.4" phổ biến ở VN in tên chân theo kiểu `3.3V / GND / CS / RESET / DC / MOSI / SCK / LED` — đó chính là các chân VCC / GND / CS / RST / DC / SDA / SCL / BLK trong bảng trên (xem lại ảnh chụp mặt sau màn hình ở Bước 0 của docs/04). Module còn có các chân thẻ SD (SD_CS…) và cảm ứng (T_CS…) — **dự án này không dùng**, bỏ trống.

> 💡 Vì PA5/PA7 chính là chân SPI1 phần cứng của STM32 (cả F103 lẫn F411 đều như vậy), firmware dùng hardware SPI qua thư viện **TFT_eSPI** — tốc độ vẽ màn nhanh và code gọn.

## 4. Nhóm nút nhấn (đầu vào số)

| Nút | Chân 1 nối đến | Chân 2 nối đến | Chức năng trong firmware |
|---|---|---|---|
| **MODE** | PA3 | GND | Chuyển màn hình: Dashboard → Oscilloscope → Thông tin |
| **CHANNEL** | PB10 | GND | Đổi kênh oscilloscope: Gợn AC → Volt → Dòng |
| **TIME/DIV** | PB1 | GND | Đổi thước thời gian (timebase) của oscilloscope — dùng PB1 vì Black Pill không có PB11 trên header |
| **HOLD** | PB12 | GND | Đóng băng màn hình để đọc số / soi dạng sóng |

> Firmware cấu hình các chân này ở chế độ `INPUT_PULLUP` (kéo lên nội bộ), nên nút chỉ cần nối 2 chân: một chân vào pin STM32, một chân xuống GND — không cần trở ngoài.

## 5. BUS GND — điểm mass trung tâm

Chọn **1 lỗ trên bo 5×7cm làm cọc mass trung tâm**, rồi hàn tất cả các điểm sau về cọc này:

- Cọc V− nguồn 24V • IN− và OUT− của XL4016 • IN− và OUT− của LM2596
- GND ACS712 • Chân G của module LM358 (cả 2 cụm) • GND STM32 • GND TFT • Dây (−) quạt
- Âm (−) của 3 tụ hóa: 470µF, 470µF, 100µF
- Chân 2 các trở: 1.2kΩ, 20kΩ • Chân 2 các tụ gốm 104
- Chân 2 của cả 4 nút nhấn • **Cọc Đen (−) đầu ra**

> ⚠️ **Vì sao phải gom mass về 1 điểm?** Nếu mass nối lộn xộn thành vòng dây, dòng công suất đi qua sẽ tạo áp rơi trên dây mass, khiến số đo trên STM32 nhảy loạn (hiện tượng "mass loop"). Gom về 1 điểm (single-point ground) là cách rẻ nhất để có số đo sạch.

## 6. Bảng tóm tắt cho người hàn nhanh

| Nhóm | Chân dùng | Đếm |
|---|---|---|
| Nguồn | 3V3, 5V, GND | 3 |
| ADC đo | PA0, PA1, PA2 | 3 |
| Nút nhấn | PA3, PB10, PB1, PB12 | 4 |
| TFT SPI | PA4, PA5, PA6, PA7, PB0 | 5 |
| **Tổng** | | **15 chân** |
