#pragma once

#include "DotWeenEnum.h"

struct DOT_WEEN_DATA
{
public:
    float *ChangeData;  // 変更変数

    DOT_WEEN_TYPE DotWeenType;  // 種類

    float DotWeenElapsedTime;   // 現在経過時間
    float DotWeenTotalTime;     // 使用総時間

    float StartData;    // 開始時の値
    float EndData;      // 終了時の値

    int Id; // 個別番号

    bool GameTimeFlag = false;
};