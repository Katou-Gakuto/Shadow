#pragma once

#include "../Y_Tool/VECTOR.h"

// スプライトアニメーションの一コマをポーズと言っているだけです
class Pose2D
{
private:
    VECTOR2D mvLeftUpUV;
    VECTOR2D mvRightBottomUV;

    Pose2D *mpPrev;
    Pose2D *mpNext;

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

    void SetPrev(Pose2D *prev) { this->mpPrev = prev; }
    void SetNext(Pose2D *next) { this->mpNext = next; }
    Pose2D *GetPrev() { return this->mpPrev; }
    Pose2D *GetNext() { return this->mpNext; }
};