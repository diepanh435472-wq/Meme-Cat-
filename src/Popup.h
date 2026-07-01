#pragma once
// Popup.h - Khai bao cac ham tao va hien thi cua so popup meo
// Su dung WinAPI + GDI+ de hien anh PNG

#include <windows.h>
#include <string>

// Khoi tao GDI+ (phai goi truoc khi dung bat ky ham nao cua GDI+)
// Tra ve true neu thanh cong
bool InitGdiPlus();

// Don dep GDI+ (goi khi thoat chuong trinh)
void ShutdownGdiPlus();

// Hien thi cua so popup co anh meo va tu dong dong sau `autoCloseMs` mili giay.
// imagePath: duong dan den file cat.png
// autoCloseMs: thoi gian hien roi tu dong dong (2000 = 2 giay)
void ShowCatPopup(const std::string& imagePath, int autoCloseMs = 2500);
