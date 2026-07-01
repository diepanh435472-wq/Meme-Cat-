#pragma once
// Hotkey.h - Khai bao cac ham dang ky phim tat toan cuc
// WinAPI cho phep dang ky hotkey hoat dong khi chuong trinh chay nen

#include <windows.h>

// ID duy nhat de nhan biet hotkey cua chung ta khi xu ly message
// (co the dat bat ky so nguyen duong nao, mien la khong trung voi hotkey khac)
constexpr int HOTKEY_ID_CAT = 1;

// Dang ky phim tat: Ctrl + Alt + 1
// Tra ve true neu thanh cong, false neu that bai
bool RegisterCatHotkey(HWND hwnd);

// Huy dang ky phim tat (goi khi thoat chuong trinh)
void UnregisterCatHotkey(HWND hwnd);
