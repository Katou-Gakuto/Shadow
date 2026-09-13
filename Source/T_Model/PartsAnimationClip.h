#pragma once

#include <vector>

class PartsAnimation;
class PartsAnimator;
class PartsAnimationClip
{
private:
    std::vector<PartsAnimation *> mlPartsAnimation;
    float mfEndFrame;
    bool mbLoopFlag;

    PartsAnimator *mpUp;

public:
    PartsAnimationClip();
    PartsAnimationClip(float endFrame, bool loopFlag, PartsAnimator *up);
    ~PartsAnimationClip();

    int Initialize();
    int Finalize();
    int Update();
    int Draw();

    int AddAnimation(PartsAnimation *addPartsAnimation);

    float GetEndFrame() const;
    bool GetLoopFrag() const;
    PartsAnimator *GetUp() const;
};