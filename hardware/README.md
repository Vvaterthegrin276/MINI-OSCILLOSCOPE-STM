# Phần cứng (Hardware)

Dự án này **KHÔNG dùng mạch in (PCB)** — toàn bộ mạch được hàn tay trên **bo đục lỗ (bo cấn) 5×7cm**, các mối nối dây hàn phía dưới bo. Đây là cách làm phù hợp với sinh viên: không cần phần mềm thiết kế, không cần đặt sản xuất, bo chỉ vài nghìn đồng, và khi muốn sửa mạch thì hàn lại dễ dàng.

Board vi điều khiển dùng là **STM32F411CEU6 (Black Pill)** — cắm lên 2 dãy lỗ của bo đục lỗ bằng 2 hàng header đã hàn sẵn. Để trống 2 chân **PA13/PA14** (chỉ dành cho ST-Link nạp code qua SWD). Nhớ đặt **jumper BOOT0 về 0 (GND)** ngay khi cắm board lên bo.

Thư mục này chứa các ghi chú liên quan đến phần cứng của dự án.

## Hiện có

| Tài nguyên | Vị trí | Trạng thái |
|---|---|---|
| Sơ đồ khối hệ thống | [images/so-do-khoi.png](../images/so-do-khoi.png) | ✅ Hoàn thành |
| Sơ đồ nguyên lý mạch | [images/so-do-nguyen-ly.png](../images/so-do-nguyen-ly.png) | ✅ Hoàn thành |
| Sơ đồ chân STM32 | [images/so-do-chan-stm32.png](../images/so-do-chan-stm32.png) | ✅ Hoàn thành |
| Bảng nối dây chi tiết | [docs/03-bang-chan-stm32.md](../docs/03-bang-chan-stm32.md) | ✅ Hoàn thành |
| Ảnh linh kiện thật | [README.md](../README.md) mục "Linh kiện thật" | ✅ Hoàn thành |
| Ảnh mạch thật trên bo đục lỗ | — | 📷 Bổ sung sau khi hàn xong |

## 3 lưu ý khi hàn trên bo đục lỗ

1. **ACS712 nằm NỖI TIẾP trên đường công suất** (OUT+ XL4016 → IP+ → IP− → cọc Đỏ), không phải một nhánh rẽ. Đường này dùng dây 18AWG — khoét lỗ bo to một chút cho dây lọt.
2. **Module LM358 2 tầng** là khối riêng có 3 cụm chân: đầu vào (V–IN–G), đầu ra (G–02–V), giữa (G–01) — coi nó như một khối đen có 2 biến trở chỉnh gain: đầu vào IN lấy từ Cọc Đỏ (+), đầu ra chân 02 về PA2, cụm giữa bỏ trống.
3. **Tụ gốm 104 của PA0/PA1 phải hàn SÁT chân vi điều khiển** — đây là vị trí hàn thật trên mạch, hàn xa là mất ý nghĩa lọc nhiễu.

> 💡 **Mẹo đi dây dưới bo:** dây công suất (18AWG) đi thẳng, ngắn nhất có thể; dây tín hiệu (22AWG) có thể đi vòng. Cố gắng gom tất cả các điểm mass về 1 điểm duy nhất trên bo (xem [docs/03-bang-chan-stm32.md](../docs/03-bang-chan-stm32.md) mục 5) — đây là cách rẻ nhất để số đo sạch trên mạch đục lỗ.
