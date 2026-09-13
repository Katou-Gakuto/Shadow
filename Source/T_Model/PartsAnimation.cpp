#include "PartsAnimation.h"

#include "PartsAnimationClip.h"
#include "PartsAnimator.h"
#include "ModelPartsBox.h"
#include "PartsBox.h"

PartsAnimation::PartsAnimation() :
    mlKeyFrameBox(),
    msPartsName(),
    mpUp(nullptr)
{

}

PartsAnimation::PartsAnimation(std::string partsName, PartsAnimationClip *up) :
    mlKeyFrameBox(),
    msPartsName(partsName),
    mpUp(up)
{

}

PartsAnimation::~PartsAnimation()
{
    for (auto currentKeyFrame : this->mlKeyFrameBox)
    {
        delete currentKeyFrame;
    }
    this->mlKeyFrameBox.clear();
}

int PartsAnimation::Initialize()
{
    return 0;
}

int PartsAnimation::Finalize()
{
    return 0;
}

int PartsAnimation::Update()
{
    PartsAnimator *animator = this->mpUp->GetUp();

    float frame = 0.0f;

    int animCount = (int)(this->mlKeyFrameBox.size());
    int keyFase = 0;
    VECTOR3D temp_rot = VECTOR3D(0.0f, 0.0f, 0.0f);

    // 現在のアニメーションの進捗状況を取得する
    for (int i = 0; i < animCount; i++)
    {
        if (frame < this->mlKeyFrameBox[i]->mfFrame)
        {
            keyFase = i;
            break;
        }
    }
    
    // 自身と対応するパーツを取得
    PartsBox *myParts = animator->GetUp()->GetPartsBox(this->msPartsName);

    // アニメーションの進捗状況に応じて、アニメーション更新処理を行う
    if (keyFase == 0)
    {
        // 
        temp_rot = this->mlKeyFrameBox[0]->mvRotation;
    }
    else
    {
        // t = 現在アニメーション経過フレーム÷次回アニメーション開始フレーム
        float t = ((frame - mlKeyFrameBox[keyFase - 1]->mfFrame) / (mlKeyFrameBox[keyFase]->mfFrame - mlKeyFrameBox[keyFase - 1]->mfFrame));

        // 本当は回転の補間にはSLerpを使いたい。
        temp_rot = VECTOR3D::GetLerpPos(mlKeyFrameBox[keyFase - 1]->mvRotation, mlKeyFrameBox[keyFase]->mvRotation, t);
        myParts->SetRotation(temp_rot.GetX(), temp_rot.GetY(), temp_rot.GetZ());
    }

    return 0;
}

int PartsAnimation::Draw()
{
    return 0;
}

int PartsAnimation::AddKeyFrame(KeyFrame *keyFrame)
{
    this->mlKeyFrameBox.push_back(keyFrame);
    return 0;
}

KeyFrame *PartsAnimation::GetKeyFrame(int index) const
{
    KeyFrame *temp = nullptr;
    if (index < this->mlKeyFrameBox.size())
    {
        temp = this->mlKeyFrameBox[index];
    }
    return temp;
}

PartsAnimationClip *PartsAnimation::GetUp() const
{
    return this->mpUp;
}
