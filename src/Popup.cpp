#include "Popup.h"

#include <windows.h>
#include <gdiplus.h>   // GDI+ - thu vien do hoa co san tren Windows, ho tro PNG
#include <string>

// Yeu cau linker lien ket thu vien GDI+
#pragma comment(lib, "gdiplus.lib")

// Bien luu trang thai GDI+ (can de shutdown dung cach)
static ULONG_PTR s_gdiplusToken = 0;

// ============================================================
// Khoi tao va don dep GDI+
// ============================================================

bool InitGdiPlus()
{
    Gdiplus::GdiplusStartupInput startupInput; // Cau hinh mac dinh
    Gdiplus::Status status = Gdiplus::GdiplusStartup(&s_gdiplusToken, &startupInput, nullptr);
    return (status == Gdiplus::Ok);
}

void ShutdownGdiPlus()
{
    if (s_gdiplusToken != 0)
    {
        Gdiplus::GdiplusShutdown(s_gdiplusToken);
        s_gdiplusToken = 0;
    }
}

// ============================================================
// Du lieu truyen vao WndProc qua con tro
// ============================================================

// Cau truc nay luu thong tin can thiet de ve anh trong cua so
struct PopupData
{
    std::wstring imagePath; // Duong dan file PNG (dang wide string vi GDI+ yeu cau)
};

// ============================================================
// WndProc - Ham xu ly message cua cua so popup
// ============================================================

static LRESULT CALLBACK PopupWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Lay con tro den PopupData da luu khi tao cua so
    PopupData* data = reinterpret_cast<PopupData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (msg)
    {
    case WM_CREATE:
    {
        // Khi cua so duoc tao, lay du lieu truyen vao qua CREATESTRUCT
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
        return 0;
    }

    case WM_PAINT:
    {
        // Ve anh PNG len cua so bang GDI+
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        if (data != nullptr)
        {
            // Lay kich thuoc vung ve cua cua so
            RECT rect;
            GetClientRect(hwnd, &rect);
            int winW = rect.right - rect.left;
            int winH = rect.bottom - rect.top;

            // Ve nen mau trang
            HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(hdc, &rect, whiteBrush);
            DeleteObject(whiteBrush);

            // Tai anh PNG bang GDI+
            Gdiplus::Graphics graphics(hdc);
            Gdiplus::Image image(data->imagePath.c_str());

            if (image.GetLastStatus() == Gdiplus::Ok)
            {
                // Tinh toan vi tri de hien anh chinh giua cua so
                // Giu ti le goc cua anh, thu nho neu anh qua lon
                int imgW = static_cast<int>(image.GetWidth());
                int imgH = static_cast<int>(image.GetHeight());

                // Ti le thu nho de anh vua voi cua so (co margin 20px moi canh)
                int maxW = winW - 40;
                int maxH = winH - 40;

                float scaleX = (imgW > maxW) ? static_cast<float>(maxW) / imgW : 1.0f;
                float scaleY = (imgH > maxH) ? static_cast<float>(maxH) / imgH : 1.0f;
                float scale  = (scaleX < scaleY) ? scaleX : scaleY; // Lay scale nho hon

                int drawW = static_cast<int>(imgW * scale);
                int drawH = static_cast<int>(imgH * scale);

                // Vi tri de anh o chinh giua
                int x = (winW - drawW) / 2;
                int y = (winH - drawH) / 2;

                // Ve anh len cua so
                graphics.DrawImage(&image, x, y, drawW, drawH);
            }
            else
            {
                // Neu khong tai duoc anh, hien text thay the
                TextOutA(hdc, 10, 10, "Khong tim thay cat.png!", 24);
            }
        }

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_TIMER:
    {
        // Timer ket thuc -> dong cua so
        KillTimer(hwnd, 1);
        DestroyWindow(hwnd);
        return 0;
    }

    case WM_DESTROY:
    {
        // Dang bao message loop thoat khoi vong lap nay
        PostQuitMessage(0);
        return 0;
    }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// ============================================================
// ShowCatPopup - Tao va hien thi popup
// ============================================================

void ShowCatPopup(const std::string& imagePath, int autoCloseMs)
{
    // --- Chuyen doi duong dan sang wide string (GDI+ yeu cau wstring) ---
    int len = MultiByteToWideChar(CP_UTF8, 0, imagePath.c_str(), -1, nullptr, 0);
    std::wstring wImagePath(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, imagePath.c_str(), -1, &wImagePath[0], len);

    PopupData data;
    data.imagePath = wImagePath;

    // --- Dang ky class cua so ---
    const wchar_t* CLASS_NAME = L"CatPopupClass";
    HINSTANCE hInst = GetModuleHandle(nullptr);

    WNDCLASSW wc = {};
    wc.lpfnWndProc   = PopupWndProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    RegisterClassW(&wc); // Neu da dang ky roi thi bo qua (khong sao)

    // --- Lay kich thuoc man hinh de dat popup o chinh giua ---
    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);

    int popupW = 400; // Chieu rong popup
    int popupH = 400; // Chieu cao popup

    int posX = (screenW - popupW) / 2; // Canh giua ngang
    int posY = (screenH - popupH) / 2; // Canh giua doc

    // --- Tao cua so popup ---
    // WS_POPUP: cua so khong co thanh tieu de, khong co nut dong
    // WS_EX_TOPMOST: luon hien tren cac cua so khac
    // WS_EX_TOOLWINDOW: an khoi thanh taskbar
    HWND hwnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW, // Extended styles
        CLASS_NAME,                         // Ten class vua dang ky
        L"Cat!",                            // Tieu de (khong hien vi la WS_POPUP)
        WS_POPUP | WS_VISIBLE,              // Style: popup, hien ngay
        posX, posY,                         // Vi tri
        popupW, popupH,                     // Kich thuoc
        nullptr,                            // Khong co cua so cha
        nullptr,                            // Khong co menu
        hInst,                              // Handle chuong trinh
        &data                               // Truyen PopupData vao WndProc qua lpCreateParams
    );

    if (hwnd == nullptr)
    {
        return; // Tao cua so that bai, thoat
    }

    // --- Dat timer tu dong dong cua so sau autoCloseMs mili giay ---
    SetTimer(hwnd, 1, autoCloseMs, nullptr);

    // --- Message loop rieng cho popup nay ---
    // Vong lap nay se xu ly cac message cua cua so popup
    // va tu thoat khi cua so bi dong (WM_QUIT tu PostQuitMessage)
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Don dep: huy dang ky class sau khi dung xong
    UnregisterClassW(CLASS_NAME, hInst);
}
