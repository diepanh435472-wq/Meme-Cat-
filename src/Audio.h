#pragma once
// Audio.h - Khai bao cac ham phat am thanh
// Su dung winmm.lib (thu vien co san tren Windows)

#include <string>

// Phat file MP3 va tu dong dung sau khoang thoi gian nhat dinh.
// Ham nay khong chan chuong trinh (non-blocking).
void PlayMeow(const std::string& filePath);

// Dung phat am thanh (goi khi popup dong)
void StopMeow();
