#include "PartsAnimationClip.h"

#include "PartsAnimation.h"

PartsAnimationClip::PartsAnimationClip() :
    mlPartsAnimation(),
    mfEndFrame(0.0f),
    mbLoopFlag(false),
    mpUp(nullptr)
{
    this->mlPartsAnimation.clear();
}

PartsAnimationClip::PartsAnimationClip(float endFrame, bool loopFlag, PartsAnimator *up) :
    mlPartsAnimation(),
    mfEndFrame(endFrame),
    mbLoopFlag(loopFlag),
    mpUp(up)
{
    this->mlPartsAnimation.clear();
}

PartsAnimationClip::~PartsAnimationClip()
{
    for (auto currentPartsAnimation : this->mlPartsAnimation)
    {
        delete currentPartsAnimation;
    }

    this->mlPartsAnimation.clear();
}

int PartsAnimationClip::Initialize()
{
    return 0;
}

int PartsAnimationClip::Finalize()
{
    return 0;
}

int PartsAnimationClip::Update()
{
    for (auto currentPartsAnimation : this->mlPartsAnimation)
    {
        currentPartsAnimation->Update();
    }
    return 0;
}

int PartsAnimationClip::Draw()
{
    return 0;
}

int PartsAnimationClip::AddAnimation(PartsAnimation *addPartsAnimation)
{
    return 0;
}

float PartsAnimationClip::GetEndFrame() const
{
    return this->mfEndFrame;
}

bool PartsAnimationClip::GetLoopFrag() const
{
    return this->mbLoopFlag;
}

PartsAnimator *PartsAnimationClip::GetUp() const
{
    return this->mpUp;
}
