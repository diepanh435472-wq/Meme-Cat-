cmake_minimum_required(VERSION 3.16)

# Ten project va ngon ngu su dung
project(CatPopup LANGUAGES CXX)

# Yeu cau C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Danh sach cac file nguon
set(SOURCES
    src/main.cpp
    src/Hotkey.cpp
    src/Hotkey.h
    src/Popup.cpp
    src/Popup.h
    src/Audio.cpp
    src/Audio.h
)

# Tao file thuc thi
# WIN32 = entry point la WinMain (khong hien cua so console)
add_executable(CatPopup WIN32 ${SOURCES})

# Lien ket cac thu vien Windows can thiet:
# - gdiplus: Hien thi anh PNG
# - winmm:   Phat am thanh MP3 qua MCI
target_link_libraries(CatPopup PRIVATE gdiplus winmm)

# Neu dung MinGW (GCC tren Windows): link static runtime
# -> file .exe tu chay duoc, khong can copy DLL di kem
if (MINGW)
    target_link_options(CatPopup PRIVATE -static-libgcc -static-libstdc++ -static)
endif()

# ============================================================
# Sau khi build, sao chep thu muc assets vao ben canh file .exe
# De khi chay, chuong trinh tim duoc cat.png va meow.mp3
# ============================================================
add_custom_command(TARGET CatPopup POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:CatPopup>/assets"
    COMMENT "Sao chep thu muc assets vao thu muc build..."
)
if (MINGW)
    target_link_options(CatPopup PRIVATE -static-libgcc -static-libstdc++ -static)
endif()
