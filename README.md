# MEME CAT

Chương trình chạy ngầm trên Windows, lắng nghe phím tắt và hiện popup ảnh mèo kèm tiếng kêu.

---

## Phím tắt

| Tổ hợp phím | Hành động |
|-------------|-----------|
| **Ctrl + Alt + 1** | Hiện popup ảnh mèo + phát meow.mp3, tự đóng sau 2.5 giây |

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
1. Chạy `CatPopup.exe` — chương trình sẽ biến mất (chạy ngầm, không có cửa sổ)
4. Nhấn **Ctrl + Alt + 1** để xem con mèo cười vào mặt bạn :))

### Thoát chương trình

Vào **Task Manager** → tìm `CatPopup.exe` → End Task.  

---

## Thư viện sử dụng

| Thư viện | Mục đích | Cần cài? |
|----------|----------|----------|
| `WinAPI` | Tạo cửa sổ, hotkey, message loop | Không (có sẵn) |
| `GDI+` (gdiplus) | Hiển thị ảnh PNG | Không (có sẵn) |
| `winmm` | Phát MP3 qua MCI | Không (có sẵn) |
