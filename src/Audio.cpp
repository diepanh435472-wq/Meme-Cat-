#include "Audio.h"

// Windows header can thiet
#include <windows.h>

// winmm.lib cho phep phat am thanh qua MCI (Media Control Interface)
// MCI la API cu nhung don gian, ho tro MP3 tren Windows
#pragma comment(lib, "winmm.lib")

// Ten "alias" de MCI nhan ra file am thanh chung ta dang xu ly
static const char* MCI_ALIAS = "CatMeow";

void PlayMeow(const std::string& filePath)
{
    // Buoc 1: Dong am thanh cu neu dang phat (tranh chong cheo)
    StopMeow();

    // Buoc 2: Mo file MP3 bang lenh MCI
    // mciSendString nhan mot chuoi lenh van ban, rat de hieu
    std::string openCmd = "open \"" + filePath + "\" type mpegvideo alias " + MCI_ALIAS;
    MCIERROR err = mciSendStringA(openCmd.c_str(), nullptr, 0, nullptr);

    if (err != 0)
    {
        // Neu mo file that bai, in thong bao loi (chi hien trong debug)
        char errorMsg[256];
        mciGetErrorStringA(err, errorMsg, sizeof(errorMsg));
        OutputDebugStringA(("PlayMeow - Mo file that bai: " + std::string(errorMsg) + "\n").c_str());
        return;
    }

    // Buoc 3: Phat am thanh (play) - khong chan chuong trinh (wait = khong co)
    std::string playCmd = "play " + std::string(MCI_ALIAS);
    mciSendStringA(playCmd.c_str(), nullptr, 0, nullptr);
}

void StopMeow()
{
    // Dung phat va dong file de giai phong bo nho
    mciSendStringA(("stop " + std::string(MCI_ALIAS)).c_str(), nullptr, 0, nullptr);
    mciSendStringA(("close " + std::string(MCI_ALIAS)).c_str(), nullptr, 0, nullptr);
}
