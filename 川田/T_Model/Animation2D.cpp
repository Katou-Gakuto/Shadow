#include "Animation2D.h"

Animation2D::Animation2D() : 
    mpFirstPose(nullptr),
    mnAnimation2DTag(Animation2DTag::Animation2DTag_Max),
    mcCurrentPose(0),
    mcPoseMax(0),
    mcChangeInterval(1),
    mcChangeCount(1),
    mbLoopFlag(true),
    mbEndTimingNow(false),
    mpPrev(nullptr),
    mpNext(nullptr)
{

}

Animation2D::Animation2D(Animation2DTag tag, unsigned char poseChangeInterval, bool loopFlag) :
    mpFirstPose(nullptr),
    mnAnimation2DTag(tag),
    mcCurrentPose(0),
    mcPoseMax(0),
    mcChangeInterval(poseChangeInterval),
    mcChangeCount(1),
    mbLoopFlag(loopFlag),
    mbEndTimingNow(false),
    mpPrev(nullptr),
    mpNext(nullptr)
{

}

Animation2D::~Animation2D()
{
    this->DeleteAll();
}

int Animation2D::UpdatePoseChangeCount()
{
    // 次のポーズに移るまでのカウントダウンを進める
    this->mcChangeCount--;

    // カウントダウンが0以下になれば処理を行う
    if (this->mcChangeCount <= 0)
    {
        // ループするアニメーションかどうかで処理を変える
        if (this->mbLoopFlag)
        {
            // ループするアニメーションなので、無条件で次のポーズに移る
            this->mcCurrentPose++;

            // 次のポーズが最後のポーズより後のポーズだったら処理を行う
            if (this->mcPoseMax <= this->mcCurrentPose)
            {
                // 進みすぎたポーズを戻す
                this->mcCurrentPose -= this->mcPoseMax;
            }
        }
        else
        {
            // ループしないアニメーションなので、次のポーズに進んでも大丈夫かを確認する
            if (this->mcCurrentPose + 1 < this->mcPoseMax)
            {
                // 進んでも大丈夫なので、次のアニメーションに移る
                this->mcCurrentPose++;
            }
        }

        // カウントダウンの値を設定し直す
        this->mcChangeCount = this->mcChangeInterval;
    }

    // 非ループのアニメーションが最後のフレームに入ったかを確認する
    if (this->mbEndTimingNow == true &&
        this->mcChangeCount == 1 &&
        this->mcCurrentPose == this->mcPoseMax - 1)
    {
        // 最後のフレームに入ったことを保存しておく
        this->mbEndTimingNow = true;
    }

    return 0;
}

int Animation2D::Initialize()
{
    this->mcCurrentPose = 0;
    this->mcChangeCount = this->mcChangeInterval;
    this->mbEndTimingNow = false;

    return 0;
}

int Animation2D::Finalize()
{
    this->mcCurrentPose = 0;
    this->mcChangeCount = 0;
    this->mbEndTimingNow = false;

    return 0;
}

int Animation2D::Update()
{
    // ポーズを進めるカウントダウンの更新処理を行う
    this->UpdatePoseChangeCount();

    return 0;
}

int Animation2D::Draw()
{
    return 0;
}

int Animation2D::Add(Pose2D *target)
{
    if (target == nullptr)
    {
        return -1;
    }

    if (this->mpFirstPose == nullptr)
    {
        this->mpFirstPose = target;
        this->mcPoseMax++;
        return 0;
    }

    Pose2D *current = this->mpFirstPose;
    while (current->GetNext() != nullptr)
    {
        current = current->GetNext();
    }

    return this->ConnectTarget(current, target, nullptr);
}

int Animation2D::DeleteTarget(Pose2D *target)
{
    if (target == nullptr)
    {
        return 0;
    }

    this->IsolateTarget(target);

    target->Finalize();

    delete target;

    return 0;
}

int Animation2D::DeleteAll()
{
    if (this->mpFirstPose == nullptr)
    {
        return 0;
    }

    Pose2D *current = this->mpFirstPose;
    Pose2D *next = current->GetNext();
    while (true)
    {
        DeleteTarget(current);
        current = next;
        if (current != nullptr)
        {
            next = current->GetNext();
        }
        else
        {
            break;
        }
    }

    return 0;
}

int Animation2D::ConnectTarget(Pose2D *prev, Pose2D *target, Pose2D *next)
{
    if (prev != nullptr)
    {
        prev->SetNext(target);
    }
    if (next != nullptr)
    {
        next->SetPrev(target);
    }
    if (next == this->mpFirstPose)
    {
        this->mpFirstPose = target;
    }
    target->SetPrev(prev);
    target->SetNext(next);

    this->mcPoseMax++;

    return 0;
}

int Animation2D::IsolateTarget(Pose2D *target)
{
    if (this->mpFirstPose == nullptr)
    {
        return 0;
    }

    Pose2D *prev = target->GetPrev();
    Pose2D *next = target->GetNext();

    if (prev != nullptr)
    {
        prev->SetNext(next);
    }
    if (next != nullptr)
    {
        next->SetPrev(prev);
    }
    if (target == this->mpFirstPose)
    {
        this->mpFirstPose = next;
    }
    target->SetPrev(nullptr);
    target->SetNext(nullptr);

    this->mcPoseMax--;

    return 0;
}

Pose2D *Animation2D::SearchPose2D(unsigned char index)
{
    if (this->mpFirstPose == nullptr)
    {
        return nullptr;
    }

    Pose2D *current = nullptr;
    unsigned char counter = 0;
    bool doBreakFlag = false;
    do
    {
        current = this->mpFirstPose;
        while (current != nullptr)
        {
            if (counter == index)
            {
                doBreakFlag = true;
                break;
            }
            counter++;
            current = current->GetNext();
        }
        if (this->mbLoopFlag == false)
        {
            doBreakFlag = true;
        }

    } while (doBreakFlag == false);

    return current;
}