# ⚠️ Quy tắc An toàn Điện — Đọc đầu tiên!

> **Tài liệu này bắt buộc phải đọc trước khi bạn cầm cây hàn hoặc cắm phích điện.**
> Dự án này làm việc với điện lưới **220V AC** — điện áp có thể gây tử vong. Tin tốt là: chỉ cần bạn làm đúng quy tắc dưới đây, toàn bộ phần còn lại của mạch (24V, 5V, 3.3V) là an toàn tuyệt đối.

---

## 1. Phần nào nguy hiểm, phần nào an toàn?

| Phần mạch | Điện áp | Mức nguy hiểm |
|---|---|---|
| Ổ cắm → công tắc → cầu chì → nguồn 24V | **220V AC** | ☠️ **NGUY HIỂM CHẾT NGƯỜI** — phần nguy hiểm duy nhất |
| Đầu ra nguồn 24V (vỏ nhôm) | 24V DC | Khá an toàn, chỉ hơi rát nếu chạm |
| Đầu ra XL4016, cọc bắp chuối | 0 – 23V DC | An toàn |
| Mạch điều khiển STM32, TFT | 5V / 3.3V DC | Hoàn toàn an toàn |

**Kết luận:** Bạn chỉ cần cẩn thận tuyệt đối với 3 mối nối L – N – FG ở Bước 1 của hướng dẫn lắp ráp. Từ Bước 2 trở đi, rút phích điện rồi làm thoải mái.

## 2. Ba việc BẮT BUỘC trước khi cắm điện lần đầu

### ✅ Việc 1 — Lắp cầu chì 2A vào ổ cắm
Dùng loại **ổ cắm AC 3 chân có sẵn công tắc và ổ cầu chì** (mua kèm 2 viên cầu chì 2A, một viên lắp sẵn, một viên dự phòng). Cầu chì này là "vật hy sinh": nếu bạn hàn chồng mạch hoặc nguồn 24V bị chập, cầu chì đứt ngay trước khi dây cháy hoặc chip nổ. Không bao giờ thay cầu chì 2A bằng dây đồng cắm xuyên — đó là cách tự đốt mạch.

### ✅ Việc 2 — Bọc co nhiệt kín 3 mối hàn L, N, FG
Sau khi hàn xong 3 dây vào ổ cắm và nguồn 24V (Bước 1 trong hướng dẫn lắp ráp), luồn ống co nhiệt phủ kín từng mối hàn. Không để bất kỳ sợi dây đồng nào lộ ra ngoài. Kiểm tra bằng cách kéo nhẹ từng dây: nếu dây bung thì hàn lại. Mối nối 220V hở 1 mm cũng đủ gây giật khi bạn vô tình sờ vào phía sau hộp.

### ✅ Việc 3 — Nối dây tiếp địa vào vỏ nhôm nguồn 24V
Dây FG (vàng/xanh lá) từ ổ cắm phải được bắt vít/hàn vào **vỏ nhôm** của nguồn 24V, không phải vào cọc nguồn. Vỏ nhôm nối đất giúp: (1) nếu dây L bị chạm vỏ, dòng rò chạy xuống đất và làm đứt cầu chì thay vì giật bạn; (2) giảm nhiễu cho mạch đo. Hầu hết nguồn 24V loại vỏ nhôm đã có chân FG sẵn — bạn chỉ cần nối đúng.

## 3. Một việc TUYỆT ĐỐI CẤM

### 🚫 Cấm chạm dây AC trần khi đang cắm điện
Đây là quy tắc duy nhất nhưng vi phạm một lần là đủ. Cụ thể:

- Không thử "đo thử xem có điện không" bằng tay, bút thử tự chế, hay dao kéo không có tay cách điện.
- Khi phải sửa phần 220V: **rút phích điện → chờ 30 giây → mới được chạm**. Tụ lọc trong nguồn 24V vẫn giữ điện sau khi rút phích, nên chờ thêm chút cho tụ xả.
- Không làm mạch khi tay ướt, khi đứng trần chân trên nền ướt, hoặc khi có thú cưng/đứa trẻ leo lẻo quanh bàn làm việc.

## 4. Dụng cụ an toàn nên có

- **Bút thử điện** (30.000đ) — cận cảnh xem ổ cắm có điện trước khi làm
- **VOM (đồng hồ vạn năng)** — bắt buộc, mọi bước kiểm tra trong hướng dẫn đều dùng VOM
- **Ống co nhiệt + máy sấy hoặc bật lửa** — bọc mối hàn
- **Thảm/đế cách điện** nếu bàn của bạn là bàn kim loại
- Kính bảo hộ khi cắt chân linh kiện (chân linh kiện bật văng rất đau mắt)

## 5. Checklist trước khi cắm điện lần đầu

In trang này ra và tick từng dòng. **Chỉ cắm điện khi 5/5 dòng đều tick:**

- [ ] Cầu chì 2A đã nằm trong ổ cắm, vặn kín
- [ ] 3 mối hàn L, N, FG đã bọc co nhiệt kín, kéo thử không bung
- [ ] Dây tiếp địa FG đã nối vào vỏ nhôm nguồn 24V
- [ ] VOM đo hai điểm L–N: **hở** (máy không kêu) khi công tắc tắt
- [ ] Đã rời tay khỏi toàn bộ dây AC, hộp đóng hoặc mặt sau hướng ra ngoài

## 6. Khi có sự cố

| Hiện tượng | Việc cần làm |
|---|---|
| Cắm điện, cầu chì nổ ngay | Rút phích. Đo lại L–N xem có chạm nhau không. Kiểm tra Bước 1–2 |
| Ngửi thấy mùi khét | Rút phích NGAY. Sờ lần lượt các module xem con nào nóng bất thường |
| Bị giật nhẹ khi chạm vỏ | Rút phích. Kiểm tra dây FG đã nối thật sự vào vỏ nhôm chưa |
| Không chắc mình làm đúng | ĐỪNG cắm điện thử. Chụp ảnh mạch, hỏi trên Issue của repo này |

> **Nguyên tắc vàng:** Nếu bạn phải tự hỏi "việc này có an toàn không?" — nghĩa là nó chưa an toàn. Rút phích điện rồi mới nghĩ tiếp.
