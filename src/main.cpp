// main.cpp - Diem khoi dau cua chuong trinh CatPopup
//
// Chuong trinh nay chay nen (khong co cua so console), lang nghe phim tat
// Ctrl + Alt + 1. Khi nhan phim, hien popup co anh meo va phat tieng keu.
//
// Luong hoat dong:
//   main() -> dang ky hotkey -> message loop -> nhan WM_HOTKEY
//          -> phat am thanh -> hien popup -> tiep tuc lang nghe

#include <windows.h>
#include <string>

#include "Hotkey.h"
#include "Popup.h"
#include "Audio.h"

// ============================================================
// Ham lay thu muc chua file thuc thi (.exe)
// Vi du: "C:\Users\User\CatPopup\build\" 
// ============================================================
std::string GetExecutableDirectory()
{
    char path[MAX_PATH];
    GetModuleFileNameA(nullptr, path, MAX_PATH); // Lay duong dan day du den .exe

    std::string fullPath(path);

    // Cat phan ten file, chi giu lai thu muc
    size_t lastSlash = fullPath.find_last_of("\\/");
    if (lastSlash != std::string::npos)
    {
        return fullPath.substr(0, lastSlash + 1); // Giu lai dau /
    }

    return "./"; // Fallback
}

// ============================================================
// WndProc cho cua so an (invisible window)
// Cua so nay khong hien thi, chi de nhan WM_HOTKEY message
// ============================================================
static LRESULT CALLBACK HiddenWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_HOTKEY)
    {
        // Kiem tra dung ID hotkey cua chung ta khong
        if (wParam == HOTKEY_ID_CAT)
        {
            std::string exeDir = GetExecutableDirectory();

            // Duong dan den assets (thu muc assets nam cung cap voi exe)
            std::string catImage = exeDir + "assets\\cat.png";
            std::string meowSound = exeDir + "assets\\meow.mp3";

            // Phat am thanh truoc (non-blocking, khong can cho)
            PlayMeow(meowSound);

            // Hien popup (ham nay co vong lap rieng, se cho popup dong moi thoat)
            ShowCatPopup(catImage, 2500); // Tu dong dong sau 2.5 giay

            // Sau khi popup dong, dung am thanh
            StopMeow();
        }
        return 0;
    }

    if (msg == WM_DESTROY)
    {
        PostQuitMessage(0); // Bao message loop chinh thoat
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// ============================================================
// WinMain - Entry point cho chuong trinh Windows (khong co console)
// ============================================================
int WINAPI WinMain(
    HINSTANCE hInstance,    // Handle chuong trinh hien tai
    HINSTANCE hPrevInstance,// Khong dung (cu tu thoi Win16)
    LPSTR     lpCmdLine,    // Tham so dong lenh (chung ta khong dung)
    int       nCmdShow      // Che do hien cua so (chung ta khong dung)
)
{
    // --- Buoc 1: Khoi tao GDI+ (can thiet de hien anh PNG) ---
    if (!InitGdiPlus())
    {
        MessageBoxA(nullptr, "Khoi tao GDI+ that bai!", "Loi", MB_OK | MB_ICONERROR);
        return 1;
    }

    // --- Buoc 2: Tao cua so an de nhan WM_HOTKEY ---
    // Cua so nay hoan toan vo hinh voi nguoi dung
    const char* HIDDEN_CLASS = "CatPopupHidden";

    WNDCLASSA wc = {};
    wc.lpfnWndProc   = HiddenWndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = HIDDEN_CLASS;

    if (!RegisterClassA(&wc))
    {
        MessageBoxA(nullptr, "Khong the dang ky window class!", "Loi", MB_OK | MB_ICONERROR);
        ShutdownGdiPlus();
        return 1;
    }

    // Tao cua so voi kich thuoc 0x0, khong hien (khong co WS_VISIBLE)
    HWND hiddenHwnd = CreateWindowA(
        HIDDEN_CLASS,  // Class name
        "CatPopup",    // Tieu de (khong hien)
        0,             // Khong co style (an hoan toan)
        0, 0, 0, 0,    // Vi tri va kich thuoc = 0
        nullptr, nullptr, hInstance, nullptr
    );

    if (hiddenHwnd == nullptr)
    {
        MessageBoxA(nullptr, "Khong the tao cua so an!", "Loi", MB_OK | MB_ICONERROR);
        ShutdownGdiPlus();
        return 1;
    }

    // --- Buoc 3: Dang ky phim tat Ctrl + Alt + 1 ---
    if (!RegisterCatHotkey(hiddenHwnd))
    {
        // Thong bao loi da hien trong RegisterCatHotkey
        DestroyWindow(hiddenHwnd);
        ShutdownGdiPlus();
        return 1;
    }

    // --- Buoc 4: Message loop chinh ---
    // Vong lap nay chay mai mai cho den khi chuong trinh bi dong.
    // No lang nghe cac su kien tu he dieu hanh (bao gom WM_HOTKEY).
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg); // Gui message den HiddenWndProc
    }

    // --- Buoc 5: Don dep truoc khi thoat ---
    UnregisterCatHotkey(hiddenHwnd);
    ShutdownGdiPlus();

    return 0; // Chuong trinh ket thuc binh thuong
}
