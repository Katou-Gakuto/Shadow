#pragma once

#include <vector>
#include <string>
#include "../Y_Tool/VECTOR.h"

struct KeyFrame
{
    float mfFrame;
    VECTOR3D mvRotation;
};

class PartsAnimationClip;
class PartsAnimation
{
private:
    std::vector<KeyFrame *> mlKeyFrameBox;
    std::string msPartsName;
    PartsAnimationClip *mpUp;

public:
    PartsAnimation();
    PartsAnimation(std::string partsName, PartsAnimationClip *up);
    ~PartsAnimation();

    int Initialize();
    int Finalize();
    int Update();
    int Draw();

    int AddKeyFrame(KeyFrame *keyFrame);
    KeyFrame *GetKeyFrame(int index) const;
    PartsAnimationClip *GetUp() const;
};