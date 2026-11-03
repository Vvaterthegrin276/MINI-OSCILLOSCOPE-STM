# 💻 Hướng dẫn nạp code cho STM32 — từ số 0 cho người mới

> Mục tiêu: nạp file `firmware/nguon_dc_da_nang/nguon_dc_da_nang.ino` lên board **STM32F411CEU6 (Black Pill)** bằng ST-Link V2. Có **2 cách làm**, chọn 1 trong 2:
>
> - **Cách A — VS Code + PlatformIO** *(khuyên dùng — cách tác giả dự án dùng hàng ngày)*: cài 1 lần, sau đó biên dịch + nạp đều bằng 1 nút bấm, thư viện tự tải về.
> - **Cách B — Arduino IDE**: quen thuộc với ai đã từng dùng Arduino, thao tác thủ công hơn chút.
>
> Lần đầu làm mất khoảng 30–45 phút (chủ yếu là chờ tải), các lần sau chỉ mất 2 phút.

---

## CÁCH A — VS Code + PlatformIO (khuyên dùng)

### A1. Cài VS Code + extension PlatformIO

1. Tải **Visual Studio Code** tại: <https://code.visualstudio.com/> — cài như phần mềm bình thường.
2. Mở VS Code, bấm biểu tượng **Extensions** (4 ô vuông, thanh trái) → gõ `PlatformIO IDE` → **Install** (tác giả PlatformIO.org).
3. Chờ cài xong (~5 phút, có lúc tải phụ thuộc Python — cứ để yên). Khởi động lại VS Code, thấy biểu tượng **kiểu con kiến (anteater)** xuất hiện trên thanh trái là OK.

### A2. Mở dự án và để PlatformIO tự sắp xếp phần còn lại

1. Tải repo này về máy (nút **Code → Download ZIP**, giải nén) — hoặc git clone nếu bạn đã quen.
2. Trong VS Code: **File → Open Folder...** → chọn thư mục **`firmware/`** (thư mục chứa file `platformio.ini`, KHÔNG phải thư mục gốc của repo).
3. PlatformIO đọc thấy `platformio.ini` → tự động cài: platform **ststm32**, core Arduino cho STM32, và thư viện màn hình **TFT_eSPI** (khai báo sẵn trong mục `lib_deps`). Tải khoảng 500MB–1GB lần đầu, đợi tới khi góc dưới phải hiện `Project has been successfully imported`.
4. Mở file `firmware/nguon_dc_da_nang/nguon_dc_da_nang.ino` — không cần chỉnh sửa gì thêm.

> 💡 File `platformio.ini` đã cấu hình sẵn mọi thứ: board `blackpill_f411ce`, framework `arduino`, nạp qua `stlink`, và toàn bộ cấu hình màn hình ST7789 (chân CS/DC/RST/SCL/SDA, tần số SPI) nằm trong `build_flags` — bạn không phải chọn board gì cả, cũng không phải sửa file nào của thư viện. Khác với Arduino IDE phải chỉnh tay từng mục trong menu Tools.

### A3. Nối ST-Link với Black Pill

Nối **4 dây** giữa ST-Link V2 và board STM32:

| ST-Link V2 | Black Pill |
|---|---|
| 3.3V | 3V3 |
| GND | GND |
| SWDIO | SWDIO (chân có chữ DIO, cạnh SWCLK) |
| SWCLK | SWCLK (chân cạnh SWDIO) |

> 💡 **Hai trường hợp cấp nguồn khi nạp:**
> - **Board riêng lẻ** (chưa ráp mạch): cứ để 4 dây như bảng — ST-Link tự nuôi 3.3V cho board.
> - **Board đã lắp vào mạch** (đã có nguồn 5V từ LM2596): **rút dây 3.3V của ST-Link ra** (chỉ giữ GND, SWDIO, SWCLK) và để mạch tự nuôi bằng nguồn 5V chính. Không cấp 2 nguồn song song cùng lúc.
>
> Trước khi nạp, kiểm tra **jumper BOOT0 đã ở vị trí 0 (GND)** — nếu BOOT0 = 1, ST-Link nạp được nhưng chip không chạy firmware trong Flash.

> 💡 Nối xong, cắm ST-Link vào máy tính. Nếu đèn xanh trên ST-Link sáng và Windows phát hiện thiết bị là đã sẵn sàng.

### A4. Nạp!

1. Nhìn **thanh trạng thái dưới cùng** của VS Code: bấm nút **➜ (Upload)** — hoặc biểu tượng con kiến → sidebar PlatformIO → **Project Tasks → blackpill_f411ce → General → Upload**.
2. Lần đầu biên dịch mất 1–2 phút, sau đó nạp qua ST-Link trong ~20 giây.
3. Thấy dòng `**SUCCESS** [upload]` trong cửa sổ Terminal → rút ST-Link, cắm nguồn 5V từ LM2596 vào chân 5V của STM32 → **màn hình TFT sáng lên hiển thị Dashboard**.
4. Muốn xem bản biên dịch có lỗi hay không mà chưa nạp: nút **✓ (Build)** trên cùng thanh đó.

### A5. Nếu nạp lỗi — các lỗi kinh điển với PlatformIO

| Lỗi | Ý nghĩa | Cách xử lý |
|---|---|---|
| `Error: failed to connect to target` / `No target connected` | ST-Link không "thấy" board | Kiểm tra 4 dây nối (nhất là SWDIO/SWCLK không bị đảo); cắm lại USB |
| `up_time is limited` / `Chip is protected` | Chip bị khóa read protection | Dùng **STM32CubeProgrammer** → tab OB (Option Bytes) → tắt RDP → nạp lại. Tải miễn phí: <https://www.st.com/en/development-tools/stm32cubeprog.html> |
| Nạp xong, board có điện nhưng màn hình không chạy | Jumper **BOOT0** đang ở vị trí 1 | Đặt jumper BOOT0 về **0 (GND)**, nhấn nút NRST hoặc rút/cắm lại nguồn |
| Không thấy nút ➜ trên thanh dưới | Extension chưa load xong hoặc mở sai thư mục | Đảm bảo đã mở đúng thư mục `firmware/` (có `platformio.ini`); khởi động lại VS Code |
| `Library ... not found` khi Build | Chưa nối mạng lúc cài | Nối mạng, xóa thư mục `firmware/.pio/`, bấm Build lại — PlatformIO tải lại thư viện |
| `'ltoa' was not declared in this scope` (trong TFT_eSPI.cpp) | Core STM32duino 4.x không còn khai báo hàm `ltoa()` | Đã xử lý sẵn trong `platformio.ini` của repo (dòng `-include api/itoa.h`) — giữ nguyên dòng đó; nếu tự viết `platformio.ini` thì thêm dòng này vào `build_flags` |

---

## CÁCH B — Arduino IDE (cách truyền thống)

### B1. Cài Arduino IDE + hỗ trợ STM32

1. Tải **Arduino IDE 2.x** tại: <https://www.arduino.cc/en/software>, cài đặt Next liên tục đến hết.
2. Mở **File → Preferences**, ô **Additional boards manager URLs** dán:
   ```
   https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json
   ```
3. Bấm **OK**, mở **Tools → Board → Boards Manager...**, gõ `STM32`, cài **STM32 MCU based boards** (tác giả STMicroelectronics).

### B2. Cài thư viện màn hình

**Sketch → Include Library → Manage Libraries...**, tìm và cài:

| Thư viện | Tác giả |
|---|---|
| **TFT_eSPI** | Bodmer |

Sau đó copy file **`firmware/User_Setup.h`** trong repo **đè lên** file cùng tên trong thư viện:

```
Documents/Arduino/libraries/TFT_eSPI/User_Setup.h   ← thay toàn bộ nội dung bằng file firmware/User_Setup.h
```

> File này khai báo đúng chân CS/DC/RST/SCL/SDA + tần số SPI cho mạch của dự án. Bỏ qua bước đè file này, màn hình sẽ không sáng (thư viện mặc định cấu hình cho ESP32).
>
> ⚠️ **Với core STM32 4.x mới (nếu biên dịch báo lỗi `'ltoa' was not declared in this scope`):** core mới thiếu khai báo hàm `ltoa()` mà TFT_eSPI dùng để in số. Xử lý: mở file `Documents/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp`, thêm dòng `#include <api/itoa.h>` ngay dưới dòng `#include "TFT_eSPI.h"` đầu tiên, rồi biên dịch lại. (Bản PlatformIO — Cách A — đã fix sẵn trong `platformio.ini`, không cần bước này.)

### B3. Cấu hình board + mở firmware

Vào menu **Tools**, chọn: Board = **Generic STM32F4 series**, Board part number = **BlackPill F411CE**, Upload method = **STM32CubeProgrammer (SWD)**, USB support = **None (if needed)**, Optimize = Smallest (-Os).

> ⚠️ Nếu IDE báo thiếu STM32CubeProgrammer, tải và cài miễn phí tại <https://www.st.com/en/development-tools/stm32cubeprog.html> (cần đăng ký email miễn phí).

Mở file `firmware/nguon_dc_da_nang/nguon_dc_da_nang.ino` bằng Arduino IDE (double-click). Arduino yêu cầu file `.ino` nằm trong **thư mục cùng tên** — repo đã để đúng cấu trúc nên mở là chạy luôn.

### B4. Nối ST-Link + nạp

Nối 4 dây **giống hệt bảng ở mục A3** ở trên (nhớ: board đã lắp mạch có nguồn 5V riêng thì rút dây 3.3V của ST-Link; kiểm tra jumper BOOT0 = 0). Sau đó:

1. Bấm **Verify** (✓) — chờ "Done compiling".
2. Bấm **Upload** (→). Thông báo `Upload complete` → rút ST-Link, cắm nguồn 5V từ LM2596 → màn hình sáng lên.
3. Lỗi thường gặp xử lý giống bảng mục A5 (với Arduino IDE: nếu mục `Upload method` không có SWD thì chưa cài STM32CubeProgrammer — cài xong khởi động lại IDE).

---

## Test nhanh sau khi nạp (áp dụng cho cả 2 cách)

- Bấm **MODE** → chuyển qua lại giữa Dashboard ↔ Oscilloscope ↔ Thông tin.
- Vặn **chiết áp Volt** → số điện áp trên Dashboard đổi theo.
- Bấm **CHANNEL** ở màn Oscilloscope → đổi kênh sóng (Gợn AC / Volt / Dòng).
- Bấm **HOLD** → màn hình đóng băng, bấm lại để chạy tiếp.

Nếu mọi thứ chạy như trên → sang [docs/06-hieu-chinh.md](06-hieu-chinh.md) để hiệu chỉnh độ chính xác. Chúc thành công! 🎉
