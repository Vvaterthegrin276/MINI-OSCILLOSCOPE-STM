# 🎯 Hướng dẫn hiệu chỉnh (Calibration)

> Sau khi mạch chạy, số đo có thể lệch vài phần trăm do sai số trở, sai số cảm biến. Hiệu chỉnh giúp số trên màn hình khớp với VOM. Thời gian: ~15 phút. Cần: **VOM tin cậy** làm chuẩn.

---

## 1. Hiệu chỉnh đo điện áp

### Nguyên lý
Firmware tính: `V_hiển thị = V_PA0 × HỆ_SỐ_CHIA` với `HỆ_SỐ_CHIA = (10k + 1.2k) / 1.2k = 9.3333`.

Trở thật có sai số ±5%, nên hệ số thật của bạn có thể là 9.1 hoặc 9.6. Ta đo ngược lại hệ số thật rồi ghi vào firmware.

### Các bước
1. Mở firmware, tìm khối `THÔNG SỐ HIỆU CHỈNH` ở đầu file:
   ```cpp
   const float DIV_RATIO = 9.3333f;   // hệ số cầu phân áp
   ```
2. Vặn chiết áp Volt ra **3 mốc**: ~5V, ~12V, ~20V (đo bằng VOM).
3. Tại mỗi mốc, chép lại: `V_vom` (VOM đo tại cọc Đỏ–Đen) và `V_màn` (màn hình hiển thị).
4. Tính hệ số thực của từng mốc:
   ```
   hệ số thực = V_vom / V_màn × DIV_RATIO_hiện_tại
   ```
   Ví dụ: VOM = 12.05V, màn = 12.40V, hệ số hiện tại 9.3333:
   `hệ số thực = 12.05 / 12.40 × 9.3333 = 9.0695`
5. Lấy **trung bình 3 mốc** (nếu 3 mốc lệch nhau quá 3% → kiểm tra lại trở 10k/1.2k, có thể một con sai giá trị).
6. Sửa `DIV_RATIO` bằng giá trị vừa tính, nạp lại code (chỉ mất 2 phút).
7. Kiểm tra lại 3 mốc — sai số còn lại nên dưới ±1%.

## 2. Hiệu chỉnh đo dòng

### Nguyên lý
ACS712-05B xuất 2.5V tại 0A và thay đổi 185mV mỗi ampe (dương hoặc âm theo chiều dòng). Firmware tính:
```cpp
I = (V_PA1 × I_DIV − ACS_VOFFSET) / ACS_SENS
```
Hai hằng số cần chỉnh: `ACS_VOFFSET` (điểm 0A) và đôi khi cả `I_DIV` (hệ số ngược cầu chia 10k/20k).

### Các bước — chỉnh điểm 0A trước
1. **Không nối tải** (dòng = 0 thật).
2. Vào màn Dashboard, xem số dòng. Nếu hiển thị 0.00A → hoàn hảo, bỏ qua.
3. Nếu lệch: đo VOM tại chân OUT của ACS712 (so với GND), ví dụ được 2.48V. Sửa:
   ```cpp
   const float ACS_VOFFSET = 2.48f;   // thay 2.5 bằng giá trị đo được
   ```
4. Nạp lại, kiểm tra 0A hiển thị ≤ 0.02A.

### Các bước — chỉnh độ dốc (sensitivity)
1. Nối một tải biết dòng rõ ràng: ví dụ **bóng đèn 12V/21W** → dòng ~1.75A; hoặc trở công suất 10Ω/10W với ngõ ra 12V → ~1.2A.
2. So `I_màn` với `I_thực` (tính từ P/U hoặc đo VOM nối tiếp).
3. Nếu lệch đều theo tỉ số k, sửa:
   ```cpp
   const float ACS_SENS = 0.185f * k;   // k = I_thực / I_màn
   ```
   Ví dụ: tải thực 1.75A nhưng màn hiển thị 1.62A → k = 1.75/1.62 = 1.08 → `ACS_SENS = 0.1998`.
4. Nạp lại và kiểm tra ở 2–3 mức dòng khác nhau.

> ⚠️ Dòng lớn kéo dài làm ACS712 và dây nóng, số đo trôi nhẹ. Chỉnh sensitivity ở mức dòng ≤ 3A cho ổn định.

## 3. Chỉnh kênh Gợn AC (vặn biến trở trên module LM358)

Kênh này dùng **module LM358 2 tầng** nên không có hằng số hiệu chỉnh trong firmware — "chỉnh" ở đây là **vặn 2 biến trở xanh** ngay trên module để có dạng sóng đẹp:

1. **Lần đầu cấp điện:** trước khi cắm dây 02 → PA2, đo VOM tại chân 02 so với mass. Vặn cả 2 biến trở **ngược chiều kim đồng hồ** (Gain nhỏ nhất) — chân 02 phải cho mức DC xác định, **không được vượt quá 3.3V**. Đạt rồi mới cắm dây vào PA2 (chân ADC của STM32 chỉ chịu được tối đa 3.3V).
2. Vào màn Oscilloscope, kênh Gợn AC. Chưa nối tải: đường sóng phải là một đường thẳng ổn định, không nhảy loạn.
3. Nối tải (đèn xe máy 12V là tải gây gợn đẹp): đường sóng phải "thở" lên xuống quanh trục giữa. Sóng quá nhỏ → vặn từ từ biến trở **theo chiều kim đồng hồ** để tăng gain. Sóng dẹt bị cắt đầu (clipping) → vặn ngược lại để giảm gain.
4. Gain quá lớn làm sóng tràn dải 3.3V và bị cắt đầu — đây là hiện tượng bình thường, không phải lỗi mạch. Chỉnh đến khi sóng đầy nhịp mà không chạm mép trên/mép dưới màn hình là đẹp nhất.
5. Nếu vặn biến trở mà chân 02 vẫn kẹt cứng ở 0V hoặc 5V: rút điện, kiểm tra lại 2 dây nguồn 5V và 2 dây G của module (Bước 7 lắp ráp) — module chưa có nguồn đủ thì khuếch đại không chạy.

## 4. Bảng hằng số hiệu chỉnh trong firmware

| Hằng số | Ý nghĩa | Giá trị mặc định | Khi nào chỉnh |
|---|---|---|---|
| `VREF` | Điện áp chuẩn ADC | 3.3 | Nếu đo 3V3 thực tế bằng VOM ≠ 3.30 (ví dụ 3.28) |
| `DIV_RATIO` | Hệ số cầu phân áp Volt | 9.3333 | Sau bước 1 ở trên |
| `ACS_VOFFSET` | Điện áp 0A của ACS712 | 2.5 | Sau bước chỉnh điểm 0A |
| `ACS_SENS` | Độ nhạy ACS712 (V/A) | 0.185 | Sau bước chỉnh sensitivity |
| `I_DIV` | Hệ số ngược cầu chia dòng | 1.5 | Chỉ khi cầu 10k/20k bị thay bằng giá trị khác |
| *(không có)* | Kênh Gợn AC — gain do module LM358 2 tầng quyết định | — | Không chỉnh trong firmware; vặn 2 biến trở xanh trên module (xem mục 3) |

## 5. Mẹo đo sạch

- Đo số liệu **sau khi quạt đã quay và mạch chạy 5 phút** — linh kiện cần ổn định nhiệt.
- Dùng dây đầu ra **18AWG ngắn** — dây dài mỏng gây sụt áp, số dòng "ảo".
- Khi so VOM và màn hình, **đọc 2 số liệu cùng lúc** (có thể nhờ bạn bấm HOLD để đóng băng màn hình rồi mới đọc VOM).
- Ghi lại các giá trị hiệu chỉnh vào [CHANGELOG.md](../CHANGELOG.md) nếu bạn thay trở — sau này mua trở mới thì nhớ vì sao số khác.
