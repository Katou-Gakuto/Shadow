#pragma once

#include <map>
#include <string>

class ModelPartsBox;
class PartsAnimationClip;
class PartsAnimator
{
private:
    std::map<std::string, PartsAnimationClip *> mlAnimationBox;
    std::string msAnimationNameNow;
    float mfFrameNow;
    ModelPartsBox *mpUp;

public:
    PartsAnimator();
    PartsAnimator(ModelPartsBox *up);
    ~PartsAnimator();

    int Initialize();
    int Finalize();
    int Update();
    int Draw();

    PartsAnimationClip *GetAnimation(std::string animationName);
    std::string GetAnimationNameNow() const;
    float GetFrameNow() const;
    ModelPartsBox *GetUp() const;
};