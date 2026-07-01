#include "Hotkey.h"
#include <string>

bool RegisterCatHotkey(HWND hwnd)
{
    // RegisterHotKey(cua_so_nhan_message, id_hotkey, modifier_keys, virtual_key)
    //
    // MOD_CONTROL = phim Ctrl
    // MOD_ALT     = phim Alt
    // '1'         = phim so 1 (ma ASCII = 0x31)
    //
    // Ket qua: nguoi dung nhan Ctrl + Alt + 1 de mo popup meo
    BOOL success = RegisterHotKey(
        hwnd,           // Cua so se nhan WM_HOTKEY message
        HOTKEY_ID_CAT,  // ID de phan biet hotkey nay voi hotkey khac
        MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, // Giu Ctrl+Alt, MOD_NOREPEAT tranh giu phim lien tuc
        0x31            // Ma phim '1' (Virtual Key Code)
    );

    if (!success)
    {
        // Thuong gap khi mot chuong trinh khac da dang ky cung to hop phim nay
        MessageBoxA(nullptr,
            "Khong the dang ky phim tat Ctrl+Alt+1.\n"
            "Co the to hop phim nay da duoc dung boi chuong trinh khac.",
            "Loi Hotkey",
            MB_OK | MB_ICONWARNING);
        return false;
    }

    return true;
}

void UnregisterCatHotkey(HWND hwnd)
{
    // Giai phong hotkey khi chuong trinh dong
    UnregisterHotKey(hwnd, HOTKEY_ID_CAT);
}
