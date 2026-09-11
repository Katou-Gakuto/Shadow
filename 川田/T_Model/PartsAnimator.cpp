#include "PartsAnimator.h"
#include "PartsAnimationClip.h"

PartsAnimator::PartsAnimator() :
    mlAnimationBox(),
    msAnimationNameNow(),
    mfFrameNow(0.0f),
    mpUp(nullptr)
{
    
}

PartsAnimator::PartsAnimator(ModelPartsBox *up) :
    mlAnimationBox(),
    msAnimationNameNow(),
    mfFrameNow(0.0f),
    mpUp(up)
{

}

PartsAnimator::~PartsAnimator()
{
    for (auto currentAnimation : this->mlAnimationBox)
    {
        delete currentAnimation.second;
    }

    this->mlAnimationBox.clear();
}

int PartsAnimator::Initialize()
{
    return 0;
}

int PartsAnimator::Finalize()
{
    return 0;
}

int PartsAnimator::Update()
{
    // 現在のアニメーションを再生する
    this->mlAnimationBox[this->msAnimationNameNow]->Update();

    // アニメーションのフレームを進める
    this->mfFrameNow += 0.5f;
    if (this->mlAnimationBox[this->msAnimationNameNow]->GetEndFrame() <= this->mfFrameNow)
    {
        if (this->mlAnimationBox[this->msAnimationNameNow]->GetLoopFrag() == true)
        {
            this->mfFrameNow -= this->mlAnimationBox[this->msAnimationNameNow]->GetEndFrame();
        }
        else
        {
            // アニメーションの最後のフレームのまま止まる。本当はアニメーション切り替えをしたいね。
            this->mfFrameNow = this->mlAnimationBox[this->msAnimationNameNow]->GetEndFrame();
        }
    }

    return 0;
}

int PartsAnimator::Draw()
{
    return 0;
}

std::string PartsAnimator::GetAnimationNameNow() const
{
    return this->msAnimationNameNow;
}

float PartsAnimator::GetFrameNow() const
{
    return this->mfFrameNow;
}

ModelPartsBox *PartsAnimator::GetUp() const
{
    return this->mpUp;
}

PartsAnimationClip *PartsAnimator::GetAnimation(std::string animationName)
{
    PartsAnimationClip *temp = nullptr;

    // 指定した要素が存在するか、イテレーターを使って確かめている
    auto it = mlAnimationBox.find(animationName);

    // 指定した要素が存在しているなら、tempの中身をその要素にする
    if (it != mlAnimationBox.end())
    {
        temp = this->mlAnimationBox[this->msAnimationNameNow];
    }
    return temp;
}
