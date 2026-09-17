#pragma once

#include "../Y_Tool/VECTOR.h"
#include "DxLib.h"

// スプライトアニメーションの一コマをポーズと言っているだけです
class Pose2D
{
private:
	VECTOR2D mvLeftUpUV; // 左上のUV座標
	VECTOR2D mvRightBottomUV; // 右下のUV座標

	Pose2D *mpPrev; // 前のポーズ
	Pose2D *mpNext; // 次のポーズ

public:
    Pose2D();
    Pose2D(float left, float up, float right, float down);
    ~Pose2D();

    int Initialize();
    int Finalize();
    int Update();
    int Draw();

    VECTOR2D GetLeftUpUV() const { return this->mvLeftUpUV; }
    VECTOR2D GetRightBottomUV() const { return this->mvRightBottomUV; }

	void SetPrev(Pose2D *prev) { this->mpPrev = prev; } // 前のポーズを設定する
	void SetNext(Pose2D *next) { this->mpNext = next; } // 次のポーズを設定する
	Pose2D *GetPrev() { return this->mpPrev; } // 前のポーズを取得する
	Pose2D *GetNext() { return this->mpNext; } // 次のポーズを取得する
};