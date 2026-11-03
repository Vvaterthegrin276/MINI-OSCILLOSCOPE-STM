# 🔧 Hướng dẫn lắp ráp — 14 bước hàn ("Đọc tên là biết bốc con gì")

> Quy tắc làm việc: **mỗi bước xong đều phải kiểm tra bằng VOM trước khi sang bước tiếp theo.** Đi đúng thứ tự sẽ không bao giờ phải dò lại toàn mạch. Trước khi bắt đầu, đọc [docs/01-an-toan.md](01-an-toan.md).

---

## Bước 0 — Kiểm tra module TRƯỚC KHI HÀN

Không hàn gì cả cho đến khi làm xong 3 việc sau:

1. **Chụp mặt sau màn hình TFT** → xem thứ tự chân in trên mạch. Module 2.4" phổ biến in: `3.3V, GND, CS, RESET, DC, MOSI, SCK, LED` (tên gọi tương đương: VCC, GND, CS, RST, DC, SDA, SCL, BLK). Mỗi hãng in thứ tự khác nhau — ảnh chụp là "chứng cứ" để hàn đúng.
2. **Đo VOM 3 lỗ chân biến trở xanh trên XL4016** → xác định chân giữa (chân có trở kháng thay đổi khi vặn biến trở). Đánh dấu lại bằng giấy.
3. **Ghi ra giấy** thứ tự chân TFT và chân chiết áp để lát nữa hàn đúng ngay lần đầu.

> Chiết áp 10 vòng WXD3 có 3 chân: khi quay trục theo chiều kim đồng hồ, trở giữa hai mép không đổi nhưng trở giữa **chân giữa** và **chân bên trái** giảm. Chân giữa là chân "ra tín hiệu" (wiper). Chiết áp thường 10k cũng có 3 chân nối y hệt — chỉ khác là vặn 1 vòng thay vì 10 vòng.

## Bước 1 — Hàn phần AC 220V ⚠️ (phần nguy hiểm duy nhất)

| Từ | Đi đến |
|---|---|
| Dây **L** (Lửa) từ ổ cắm | Công tắc → Cầu chì 2A → Cọc **L** trên nguồn 24V |
| Dây **N** (Nguội) từ ổ cắm | Cọc **N** trên nguồn 24V |
| Dây **FG** (Tiếp địa) | Vỏ nhôm nguồn 24V |

- Bọc co nhiệt (hoặc băng keo điện) **kín 3 mối** này.
- **Kiểm tra:** VOM để chế độ kêu ( continuity ) — đo hai điểm L–N: phải **hở** (máy không kêu). Bật công tắc: vẫn phải hở (cầu chì chưa đứt, đường chưa nối tải nào).

## Bước 2 — Cấp 24V vào 2 module

| Từ | Đi đến |
|---|---|
| Cọc V+ nguồn 24V | Chân **IN+** XL4016 • Chân **IN+** LM2596 • Chân (+) tụ hóa 470µF/50V thứ nhất |
| Cọc V− nguồn 24V | Chân **IN−** XL4016 • Chân **IN−** LM2596 • Chân (−) tụ hóa 470µF/50V thứ nhất |

- **Kiểm tra:** cắm điện, đo OUT+/OUT− của LM2596, vặn biến trở nó ra **5.0V**. Rút phích mới làm tiếp.

## Bước 3 — Hàn chiết áp ra XL4016

1. **Tháo 2 biến trở xanh (CV và CC) khỏi board XL4016** — vị trí trống của chúng chính là nơi hàn 3 dây dẫn ra chiết áp.
2. Hàn 3 dây từ vị trí **CV** (Volt) ra 3 chân **Chiết áp 10 vòng WXD3** (chỉnh Volt — vặn 10 vòng nên chỉnh mịn).
3. Hàn 3 dây từ vị trí **CC** (Dòng) ra 3 chân **Chiết áp thường 10k** (chỉnh Dòng — giới hạn dòng cứng, 1 vòng là đủ).
4. **Kiểm tra:** cắm điện (không tải), vặn chiết áp Volt — đo OUT của XL4016 phải thay đổi mượt từ **gần 0V đến ~23V**, không giật cục.

> Chiết áp WXD3 thay biến trở CV (chỉnh Volt mịn), chiết áp thường 10k thay biến trở CC (giới hạn dòng) — nhờ vậy bạn vừa chỉnh được điện áp đầu ra, vừa giới hạn được dòng cứng bằng tay.

## Bước 4 — Tụ hóa đầu ra

| Từ | Đi đến |
|---|---|
| Chân (+) tụ hóa 470µF/50V thứ hai | Chân **OUT+** XL4016 |
| Chân (−) tụ hóa 470µF/50V thứ hai | Chân **OUT−** XL4016 |

Tụ này giữ đầu ra "mềm" khi tải thay đổi đột ngột và giảm gợn trước khi tín hiệu vào cầu đo Volt.

## Bước 5 — Cảm biến dòng ACS712 và cầu đo Dòng

Đường công suất (dây **18AWG to**):

| Từ | Đi đến |
|---|---|
| Chân OUT+ XL4016 | Chân **IP+** ACS712 |
| Chân **IP−** ACS712 | **Cọc Đỏ (+)** đầu ra |

Đường tín hiệu và nuôi chip (dây 22AWG):

| Từ | Đi đến |
|---|---|
| VCC ACS712 | Nguồn 5V (OUT+ LM2596) |
| GND ACS712 | Mass chung |
| OUT ACS712 | Chân 1 trở **10kΩ** (đo Dòng) |
| Chân 2 trở 10kΩ | Chân **PA1** STM32 |
| Chân 1 trở **20kΩ** | Chân PA1 STM32 |
| Chân 2 trở 20kΩ | Mass chung |
| Tụ gốm 104 (thứ nhất) | Chân PA1 ↔ Mass (hàn **sát chân chip**) |

- **Kiểm tra:** đo OUT ACS712 so với GND phải được **2.5V ± 0.05** khi chưa có tải.

## Bước 6 — Cầu đo Volt

| Từ | Đi đến |
|---|---|
| **Cọc Đỏ (+)** | Chân 1 trở **10kΩ** (đo Volt) |
| Chân 2 trở 10kΩ | Chân **PA0** STM32 |
| Chân 1 trở **1.2kΩ** | Chân PA0 STM32 |
| Chân 2 trở 1.2kΩ | Mass chung |
| Tụ gốm 104 (thứ hai) | Chân PA0 ↔ Mass (hàn sát chân chip) |

- **Kiểm tra:** vặn XL4016 ra 12.00V (đo bằng VOM), đo tại PA0 phải được **~1.29V**. Nếu lệch nhiều, kiểm tra lại giá trị trở (10k và 1.2k).

## Bước 7 — Mạch đo gợn AC (Module LM358 2 tầng)

> ⚠️ **Trước khi hàn:** vặn **cả 2 biến trở xanh** trên module **ngược chiều kim đồng hồ hết hành trình** (mức Gain nhỏ nhất). Xác định tên chân bằng cách nhìn vào **MẶT SAU của module** — nơi in chữ — đừng đoán theo mặt trước!

**Cụm bên PHẢI — đầu vào (chữ in V, IN, G từ trên xuống):**

| Từ | Đi đến |
|---|---|
| Chân **V** (cụm phải) | Nguồn 5V (OUT+ LM2596) |
| Chân **IN** (cụm phải) | **Cọc Đỏ (+)** đầu ra |
| Chân **G** (cụm phải) | Mass chung |

**Cụm bên TRÁI — đầu ra (chữ in G, 02, V từ trên xuống):**

| Từ | Đi đến |
|---|---|
| Chân **G** (cụm trái) | Mass chung |
| Chân **02** (cụm trái) | Chân **PA2** STM32 |
| Chân **V** (cụm trái) | Nguồn 5V |

**Cụm ở GIỮA (chữ in G, 01):** bỏ trống, không đấu dây gì — đây là ngõ ra tầng 1, ta chỉ lấy tín hiệu sau cả 2 tầng ở chân 02.

- **Kiểm tra:** cắm điện, đo chân 02 so với mass phải có mức DC xác định (không kẹt cứng ở 0V hay 5V) và **không vượt quá 3.3V** trước khi cắm dây vào PA2. Chi tiết xem [docs/06-hieu-chinh.md](06-hieu-chinh.md) mục 3.

## Bước 8 — Nguồn 5V nuôi mạch

| Từ | Đi đến |
|---|---|
| OUT+ LM2596 | Chân **5V** STM32 |
| OUT+ LM2596 | Chân (+) tụ hóa **100µF/16V** • Dây đỏ (+) **quạt 5V** |
| OUT− LM2596 | Mass chung |
| Chân (−) tụ hóa 100µF | Mass chung |
| Dây đen (−) quạt | Mass chung |

> ⚠️ **Riêng với board Black Pill (STM32F411):** nhận nguồn 5V tại chân **5V** (hoặc VIN) và tự hạ áp xuống 3.3V bên trong. **Không cấp nguồn 5V trực tiếp vào chân 3.3V của board.** Kiểm tra luôn **jumper BOOT0 đang ở vị trí 0 (GND)** — nếu để ở 1, board có điện nhưng không chạy firmware.

## Bước 9 — Màn hình TFT

Hàn 8 chân màn theo **đúng thứ tự đã xác định ở Bước 0** — dùng chữ in trên module làm chuẩn (đối chiếu bảng "Chữ in trên module" trong docs/03):

| Chữ in trên module | Nối đến | Ghi chú |
|---|---|---|
| `3.3V` (VCC) | **3V3** STM32 | ❗ Không được cắm 5V — cháy màn ngay lập tức |
| `GND` | Mass chung | |
| `CS` | PA4 | |
| `DC` | PA6 | |
| `RESET` (RST) | PB0 | |
| `SCK` (SCL) | PA5 | |
| `MOSI` (SDA) | PA7 | |
| `LED` (BLK) | 3V3 STM32 | Đèn nền luôn bật |

> Các chân còn lại của module (thẻ SD: `SD_CS`, `SD_MOSI`…; cảm ứng: `T_CS`, `T_DIN`…) — dự án không dùng, bỏ trống.

## Bước 10 — 4 nút nhấn

| Nút | Chân 1 → | Chân 2 → |
|---|---|---|
| **MODE** | PA3 | Mass chung |
| **CHANNEL** | PB10 | Mass chung |
| **TIME/DIV** | PB1 | Mass chung |
| **HOLD** | PB12 | Mass chung |

Không cần trở ngoài — firmware dùng pull-up nội bộ.

> ⚠️ Lưu ý riêng cho Black Pill: nút TIME/DIV dùng **PB1**, không phải PB11 — Black Pill không đưa chân PB11 ra header như Blue Pill (hàng chân đó là `B10 • B2 • B1 • B0`). Giải thích chi tiết ở docs/03.

## Bước 11 — Nối mass tổng (BUS GND)

Chọn **1 lỗ trên bo 5×7cm làm cọc mass trung tâm**. Hàn tất cả các điểm sau về cọc này (danh sách đầy đủ xem [docs/03-bang-chan-stm32.md](03-bang-chan-stm32.md) mục 5):

> Cọc V− nguồn 24V • IN−/OUT− XL4016 • IN−/OUT− LM2596 • GND ACS712 • chân G của module LM358 (cả 2 cụm) • GND STM32 • GND TFT • (−) quạt • âm 3 tụ hóa (470µF, 470µF, 100µF) • chân 2 các trở (1.2k, 20k) • chân 2 các tụ gốm 104 • chân 2 cả 4 nút nhấn • **Cọc Đen (−)**.

## Bước 12 — Cố định vào hộp

1. Cố định nguồn 24V, XL4016, LM2596 vào **đáy hộp** (keo nến hoặc vít nhỏ).
2. Gắn màn hình TFT, 4 nút nhấn, 2 chiết áp, 2 cọc bắp chuối lên **mặt trước hộp**.
3. Gắn **quạt thổi mát** hướng vào tản nhôm XL4016. Khoét lỗ thoát khí đối diện quạt — không khí nóng có chỗ thoát thì quạt mới có tác dụng.
4. Kiểm tra **jumper BOOT0** trên board STM32 đang ở vị trí **0 (GND)** — nhét sẵn ngay từ bây giờ để khỏi quên khi nạp code.

## Bước 13 — Kiểm tra lần cuối (8 điểm đo)

Cắm điện và đo lần lượt 8 điểm sau bằng VOM (chế độ DCV):

| # | Điểm đo | Giá trị đúng |
|---|---|---|
| 1 | Cọc V+ nguồn 24V | 24V ± 1 |
| 2 | OUT LM2596 | 5.0V ± 0.1 |
| 3 | Chân 3V3 STM32 | 3.3V ± 0.05 |
| 4 | OUT XL4016 (vặn chiết áp Volt) | 0 → ~23V |
| 5 | OUT ACS712 (không tải) | 2.5V ± 0.05 |
| 6 | Chân PA0 (XL4016 ra 12V) | ~1.29V |
| 7 | Chân PA2 (module LM358 để gain nhỏ nhất) | Mức DC xác định, nằm trong 0–3.3V |
| 8 | Vỏ nhôm nguồn 24V ↔ đất | Không bao giờ có điện áp |

Đạt cả 8 điểm → chuyển sang Bước 14. Không đạt → xem bảng xử lý sự cố cuối file này.

## Bước 14 — Nạp code STM32

Làm theo [docs/05-huong-dan-nap-code.md](05-huong-dan-nap-code.md). Sau khi nạp xong, màn hình hiển thị Dashboard với số đo điện áp — chúc mừng, bộ nguồn của bạn đã chạy! 🎉

---

## Xử lý sự cố thường gặp

| Triệu chứng | Nguyên nhân hay gặp | Cách xử lý |
|---|---|---|
| Cắm điện, cầu chì nổ ngay | Mối 220V hàn chồng nhau; tụ 470µF đảo cực | Rút phích, đo L–N, soi lại Bước 1–2 |
| STM32 không lên nguồn | OUT LM2596 chưa ra 5V; dây 5V hàn lệch chân; jumper BOOT0 chưa về 0 | Đo lại Bước 2 và 8; kiểm tra jumper BOOT0 = 0 (GND) |
| Màn hình trắng / không hiển thị | Cắm nhầm 5V vào VCC TFT; thứ tự chân sai so với Bước 0 | Kiểm tra ảnh chụp mặt sau màn |
| Số đo Volt nhảy loạn | Thiếu tụ 104 sát chân PA0; mass không gom về 1 điểm | Làm lại Bước 6 và 11 |
| Đo dòng luôn 0 hoặc âm | Cầu chia 10k/20k sai vị trí; ACS712 đo ngược chiều | Kiểm tra lại Bước 5 |
| Oscilloscope không có sóng | Module LM358 thiếu 5V; dây 02–PA2 lỏng; gain vặn quá nhỏ/quá lớn | Đo lại Bước 7, vặn lại 2 biến trở trên module |
