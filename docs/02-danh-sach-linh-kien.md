# 🛒 Danh sách linh kiện (BOM — Bill of Materials)

> Toàn bộ linh kiện đều mua được ở các cửa hàng điện tử (Vũ Công Phạt, Ngọc Dân...) hoặc trên Shopee/Lazada với từ khóa gợi ý ở cột cuối. Tổng chi phí ước tính: **~750.000đ – 1.000.000đ**.

---

## 1. Module & Board (trụ cột của dự án)

| # | Linh kiện | Thông số cần mua | Số lượng | Từ khóa tìm kiếm |
|---|---|---|---|---|
| 1 | Nguồn 24V "tổ ong" vỏ nhôm | 24V — 5A (120W), hiệu suất 90% | 1 | `nguon 24V 5A vỏ nhôm` |
| 2 | Module hạ áp XL4016 | Module chịu 8A nhưng nguồn vào chỉ 24V/5A nên đầu ra thực tế **tối đa ~5A**; **có 2 biến trở xanh** (CV và CC) | 1 | `module XL4016 8A` |
| 3 | Module hạ áp LM2596 | Đầu vào tối đa 35V, đầu ra chỉnh được (hạ 24V → 5V) | 1 | `module LM2596` |
| 4 | Cảm biến dòng ACS712 | Loại **5A** (mã 05B) — đừng mua nhầm 20A/30A | 1 | `module ACS712 5A` |
| 5 | Module khuếch đại LM358 **2 tầng** | Board xanh có **2 biến trở xanh chỉnh Gain**; 3 cụm chân: vào (V–IN–G), ra (G–02–V), giữa (G–01) | 1 | `module LM358 2 tầng` |
| 6 | Board STM32F411CEU6 | "Black Pill", cần chip có chữ F411 trên mặt — ADC 12 bit, RAM 128KB | 1 | `STM32F411CEU6 Black Pill` |
| 7 | Màn hình TFT ST7789V | 2.4 inch, **giao tiếp SPI**, 240×320, 8 chân | 1 | `TFT 2.4 ST7789 SPI` |
| 8 | Mạch nạp ST-Link V2 | Kèm 4 dây: 3.3V, GND, SWDIO, SWCLK | 1 | `ST-Link V2 mini` |

> ⚠️ **Lưu ý quan trọng khi mua TFT:** màn hình 2.4" có nhiều bản điều khiển khác nhau (ILI9341, ST7789V...). Cần đúng **ST7789V**. Nếu mua nhầm, hình sẽ ngược màu hoặc không hiển thị — cách nhận biết nhanh là chụp mặt sau màng hình gửi cho người bán xác nhận.

## 2. Điện trở (toàn bộ loại 1/4W, giá vài trăm đồng/con)

| # | Giá trị | Số lượng | Vai trò trong mạch |
|---|---|---|---|
| 1 | **10kΩ** (nâu-đen-cam) | 2 | Đo Volt (phân áp trên) • Đo Dòng (div ACS712) |
| 2 | **1.2kΩ** (nâu-đỏ-đỏ) | 1 | Đo Volt (phân áp dưới) — với 10k tạo hệ số chia 9.33 |
| 3 | **20kΩ** (đỏ-đen-cam) | 1 | Đo Dòng (div dưới với 10k tạo hệ số 2/3) |
| 4 | **47Ω** (vàng-tím-đen) | 1 | Dự phòng cho đèn nền TFT (chỉ dùng nếu màn của bạn không có trở bảo vệ sẵn) |

## 3. Tụ điện

| # | Loại | Giá trị | Số lượng | Vai trò |
|---|---|---|---|---|
| 1 | Tụ hóa | **470µF / 50V** | 2 | Lọc nguồn 24V vào + Lọc đầu ra XL4016 |
| 2 | Tụ hóa | **100µF / 16V** | 1 | Lọc nguồn 5V nuôi mạch |
| 3 | Tụ gốm 104 (100nF) | — | 2 | Lọc nhiễu tần số cao cho kênh đo Volt và kênh đo Dòng (hàn SÁT chân chip STM32) |

## 4. Cơ khí & Phụ kiện

| # | Linh kiện | Thông số | Số lượng | Ghi chú |
|---|---|---|---|---|
| 1 | Chiết áp 10 vòng | **WXD3-12/13 (10kΩ)** | 1 | Chỉnh **Volt** — vặn 10 vòng nên chỉnh mịn, kèm núm vặn số |
| 2 | Chiết áp thường | **10kΩ (loại B10K)** | 1 | Chỉnh **Dòng** (giới hạn dòng cứng) — chiết áp đơn 1 vòng là đủ |
| 3 | Nút nhấn | 6×6mm (thuận hoặc có nắp) | 4 | MODE / CHANNEL / TIME/DIV / HOLD |
| 4 | Quạt tản nhiệt | 5V, cỡ 40×40mm | 1 | Thổi mát vào tản nhôm của XL4016 |
| 5 | Ổ cắm AC 3 chân | Có công tắc + ổ cầu chì | 1 | Kèm **2 viên cầu chì 2A** (1 lắp + 1 dự phòng) |
| 6 | Cọc bắp chuối | 1 đỏ + 1 đen | 2 | Đầu ra V+ và GND cho tải |
| 7 | Hộp nhựa | 180×115×65mm | 1 | Vỏ chứa toàn bộ mạch |
| 8 | Bo lỗ (bo cấn) | 5×7cm | 1 | Tấm hàn cầu đo và linh kiện nhỏ — dự án KHÔNG dùng PCB |
| 9 | Dây silicon | Lõi đồng 18AWG | ~1m | Đi dây công suất (IN/OUT XL4016, ACS712, cọc ra) |
| 10 | Dây tín hiệu | 22AWG | ~2m | Đi dây tín hiệu, TFT, nút nhấn, chiết áp |
| 11 | Ống co nhiệt | Đường kính 2–5mm | ~0.5m | Bọc mối hàn, bắt buộc với 3 mối 220V |

## 5. Dụng cụ cần có (nếu chưa có)

| Dụng cụ | Ghi chú |
|---|---|
| Mỏ hàn 40–60W + thiếc | Không cần loại đắt, loại 150.000đ là đủ cho dự án này |
| VOM (đồng hồ vạn năng) | Bắt buộc — mọi điểm kiểm tra đều dùng VOM |
| Kìm bấm chân, kìm nhíp | Bấm chân linh kiện gọn trước khi hàn |
| ST-Link V2 + cáp USB | Nạp code cho STM32 (ở mục 1) |
| Máy tính có cổng USB | Cài VS Code + PlatformIO (hoặc Arduino IDE) theo docs/05-huong-dan-nap-code.md |

## 6. Chi phí ước tính

| Khoản | Chi phí |
|---|---|
| Nguồn 24V 5A vỏ nhôm | 200.000đ – 280.000đ |
| STM32 Black Pill + ST-Link | 220.000đ – 300.000đ |
| Màn hình TFT 2.4" | 120.000đ – 180.000đ |
| Các module còn lại (XL4016, LM2596, ACS712, LM358 2 tầng) | 90.000đ – 130.000đ |
| Linh kiện nhỏ (trở, tụ, chiết áp, nút, cọc, hộp, bo, dây...) | 140.000đ – 190.000đ |
| **Tổng** | **~750.000đ – 1.000.000đ** |
