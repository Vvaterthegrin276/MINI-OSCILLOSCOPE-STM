# 🤝 Hướng dẫn đóng góp (CONTRIBUTING)

Cảm ơn bạn quan tâm đến dự án! Dù bạn là học sinh, sinh viên hay kỹ sư, mọi đóng góp — từ sửa 1 lỗi chính tả đến thêm 1 tính năng lớn — đều được ghi nhận. Tài liệu này nói rõ cách đóng góp cho "đẹp đôi cả hai bên".

## 1. Các kiểu đóng góp

| Kiểu | Ví dụ | Độ khó |
|---|---|---|
| 🐛 Báo lỗi | "Màn hình không sáng khi nối đúng hướng dẫn" | Dễ — chỉ cần tạo Issue |
| 📖 Cải thiện tài liệu | Sửa câu khó hiểu, thêm ảnh thực tế, dịch sang tiếng khác | Dễ |
| 🔧 Chia sẻ kinh nghiệm lắp ráp | Thêm mẹo hàn, ảnh của bạn vào gallery | Trung bình |
| 💡 Đề xuất tính năng | "Thêm đo Vpp tự động đi anh ơi" | Trung bình |
| 💻 Viết code | Thêm tính năng, tối ưu firmware | Khó hơn một chút |

## 2. Báo lỗi (Bug Report) — làm như sau

1. Vào tab **Issues** của repo → nút **New issue** → chọn mẫu **🐛 Báo lỗi**.
2. Điền theo mẫu: bạn làm bước nào, mong đợi gì, thực tế thấy gì, ảnh chụp nếu có.
3. **Ghi rõ bản firmware đang dùng** (xem dòng version ở đầu file `.ino` hoặc [CHANGELOG.md](CHANGELOG.md)) và ảnh mạch của bạn nếu liên quan.

Trước khi tạo Issue mới, **tìm trong Issues cũ** — có thể ai đó đã gặp và có sẵn lời giải.

## 3. Đề xuất tính năng (Feature Request)

Tạo Issue với mẫu **💡 Đề xuất tính năng**. Nêu rõ: (1) bạn muốn gì, (2) giải quyết vấn đề nào, (3) ý tưởng cài đặt (nếu có). Người duy trì repo sẽ gắn nhãn và cân nhắc khi cập nhật dự án.

## 4. Gửi code (Pull Request) — cho ai đã quen GitHub

1. **Fork** repo → tạo nhánh mới từ `main`:
   ```bash
   git checkout -b ten-tinh-nang-cua-ban
   ```
2. Chỉnh sửa. Với code firmware, giữ nguyên phong cách hiện tại: comment tiếng Việt, chia khối đánh số, in chữ lên màn hình **không dấu**.
3. Commit với thông điệp rõ ràng, ví dụ `Thêm tính đo Vpp trên màn hình scope`.
4. Push lên fork → mở **Pull Request** vào nhánh `main`, mô tả: đổi gì, vì sao, test thế nào.

## 5. Quy tắc nội bộ

- Ngôn ngữ chính của repo là **tiếng Việt**; các file hướng tới bạn bè quốc tế (README_EN.md) dùng tiếng Anh.
- Với phần **an toàn điện (220V)**: tuyệt đối không thêm nội dung đi ngược quy tắc trong `docs/01-an-toan.md`. Mọi PR đụng vào phần này sẽ được soi kỹ hơn bình thường — vì an toàn là ưu tiên số một.
- Firmware giữ chuẩn **Arduino + STM32duino** — build được bằng cả **VS Code + PlatformIO** (cấu hình sẵn trong `firmware/platformio.ini`) lẫn **Arduino IDE**. Thư viện màn hình là **TFT_eSPI**: cấu hình chân nằm trong `build_flags` của `platformio.ini` (PlatformIO) và `firmware/User_Setup.h` (Arduino IDE) — đổi chân màn phải sửa đồng bộ cả 2 nơi. Nếu PR thêm thư viện mới, nhớ khai báo vào mục `lib_deps` của `platformio.ini` để người dùng PlatformIO không phải cài tay.
- Bình luận lịch sự, tập trung vào vấn đề. Repo này là sân học tập — sai sót là chuyện bình thường.

## 6. Sau khi PR được gộp

Tên bạn sẽ xuất hiện trong [CHANGELOG.md](CHANGELOG.md) của phiên bản kế tiếp. Đó là cách dự án nói lời cảm ơn!
