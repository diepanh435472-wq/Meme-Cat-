# CatPopup 🐱

Chương trình chạy ngầm trên Windows, lắng nghe phím tắt và hiện popup ảnh mèo kèm tiếng kêu.

---

## Phím tắt

| Tổ hợp phím | Hành động |
|-------------|-----------|
| **Ctrl + Alt + 1** | Hiện popup ảnh mèo + phát meow.mp3, tự đóng sau 2.5 giây |

> **Tại sao dùng Ctrl + Alt + 1?**  
> WinAPI yêu cầu hotkey toàn cục phải có ít nhất một modifier key (Ctrl, Alt, Shift, Win).  
> Tổ hợp `Ctrl + Alt + 1` đơn giản, ít bị xung đột với phần mềm khác, và dễ đăng ký.

---

## Cấu trúc project

```
CatPopup/
├── src/
│   ├── main.cpp      ← Entry point, message loop chính
│   ├── Hotkey.cpp/h  ← Đăng ký/hủy phím tắt toàn cục
│   ├── Popup.cpp/h   ← Tạo cửa sổ popup, hiển thị ảnh PNG
│   └── Audio.cpp/h   ← Phát file MP3 qua MCI (winmm)
├── assets/
│   ├── cat.png       ← Ảnh mèo (bạn tự chuẩn bị)
│   └── meow.mp3      ← Tiếng kêu mèo (bạn tự chuẩn bị)
├── CMakeLists.txt
└── README.md
```

---

## Yêu cầu

- **Windows 10/11**
- **CMake 3.16+**
- **Visual Studio 2019/2022** (với Desktop C++ workload) hoặc **MinGW-w64**
- Không cần cài thêm thư viện nào — GDI+ và winmm đều có sẵn trên Windows

---

## Cách build

### Dùng Visual Studio (khuyên dùng)

```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

### Dùng MinGW

```cmd
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

Sau khi build, file `CatPopup.exe` sẽ nằm trong thư mục `build\Release\` (hoặc `build\` với MinGW),  
cùng với thư mục `assets\` được tự động copy vào.

---

## Cách chạy

1. Đặt file `cat.png` và `meow.mp3` vào thư mục `assets/` (xem bên dưới)
2. Build project
3. Chạy `CatPopup.exe` — chương trình sẽ biến mất (chạy ngầm, không có cửa sổ)
4. Nhấn **Ctrl + Alt + 1** để xem mèo 🐱

### Thoát chương trình

Vào **Task Manager** → tìm `CatPopup.exe` → End Task.  
*(Hoặc bạn có thể thêm tray icon để thoát tiện hơn — phần nâng cao)*

---

## Chuẩn bị assets

Chương trình cần 2 file trong thư mục `assets/`:

| File | Yêu cầu |
|------|---------|
| `cat.png` | Ảnh PNG bất kỳ. Chương trình tự co giãn để vừa cửa sổ 400×400. |
| `meow.mp3` | File MP3 tiếng kêu mèo. Tìm trên [freesound.org](https://freesound.org) (miễn phí). |

Nếu thiếu file, chương trình vẫn chạy — popup sẽ hiện chữ thay thế khi thiếu ảnh, và im lặng khi thiếu âm thanh.

---

## Kiến trúc (dành cho người muốn hiểu sâu hơn)

```
WinMain()
  │
  ├─ InitGdiPlus()          ← Bật thư viện đồ họa để đọc PNG
  ├─ Tạo hidden window      ← Cửa sổ ẩn, chỉ để nhận WM_HOTKEY
  ├─ RegisterCatHotkey()    ← Đăng ký Ctrl+Alt+1 với Windows
  │
  └─ Message Loop (vô hạn)
       │
       └─ WM_HOTKEY nhận được
            ├─ PlayMeow()       ← Phát MP3 (non-blocking)
            └─ ShowCatPopup()   ← Tạo popup + message loop riêng
                 └─ WM_TIMER    ← Sau 2.5s → DestroyWindow → thoát loop
```

---

## Thư viện sử dụng

| Thư viện | Mục đích | Cần cài? |
|----------|----------|----------|
| `WinAPI` | Tạo cửa sổ, hotkey, message loop | Không (có sẵn) |
| `GDI+` (gdiplus) | Hiển thị ảnh PNG | Không (có sẵn) |
| `winmm` | Phát MP3 qua MCI | Không (có sẵn) |
