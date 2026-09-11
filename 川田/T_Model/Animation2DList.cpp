#include "Animation2D.h"
#include "Animation2DList.h"

Animation2DList::Animation2DList(bool tagMode) :
    mpFirstAnimation(nullptr),
    mnAnimationOldTag(Animation2DTag::Animation2DTag_Max),
    mnAnimationNowTag(Animation2DTag::Animation2DTag_Default),
    mnReserveTag(Animation2DTag::Animation2DTag_Max),
    mnAnimationOldIndex(0),
    mnAnimationNowIndex(0),
    mnAnimationReserveIndex(0),
    mbTagMode(tagMode),
    mbReserveLoopFlag(true),
    mbFirstFrame(false)
{
    
}

Animation2DList::~Animation2DList()
{
    this->DeleteAll();
}

int Animation2DList::ChangeAnimationTag(Animation2DTag animTag, bool loopFlag)
{
    if (this->mbTagMode == true)
    {
        // 再生アニメーションを切り替える
        this->mnAnimationOldTag = this->mnAnimationNowTag;

        // これから再生するアニメーションを設定する
        this->mnAnimationNowTag = animTag;

        // これから再生するアニメーションのループ設定を記憶させておく
        this->GetNowAnimation2D()->SetLoopFlag(loopFlag);

        // このアニメーションを最初に再生するフレームだということを記憶しておく
        this->mbFirstFrame = true;
    }

    return 0;
}

int Animation2DList::ChangeAnimationIndex(unsigned short index, bool loopFlag)
{
    if (this->mbTagMode == false)
    {
        // 再生アニメーションを切り替える
        this->mnAnimationOldIndex = this->mnAnimationNowIndex;

        // これから再生するアニメーションを設定する
        this->mnAnimationNowIndex = index;

        // これから再生するアニメーションのループ設定を記憶させておく
        this->GetNowAnimation2D()->SetLoopFlag(loopFlag);

        // このアニメーションを最初に再生するフレームだということを記憶しておく
        this->mbFirstFrame = true;
    }

    return 0;
}

int Animation2DList::ChangeAnimationResarve()
{
    if (this->mbTagMode == true)
    {
        // 再生アニメーションを切り替える
        this->mnAnimationOldTag = this->mnAnimationNowTag;

        // これから再生するアニメーションを設定する
        this->mnAnimationNowTag = this->mnReserveTag;

        // これから再生するアニメーションのループ設定を記憶させておく
        this->GetNowAnimation2D()->SetLoopFlag(this->mbReserveLoopFlag);

        // このアニメーションを最初に再生するフレームだということを記憶しておく
        this->mbFirstFrame = true;
    }
    else
    {
        // 再生アニメーションを切り替える
        this->mnAnimationOldIndex = this->mnAnimationNowIndex;

        // これから再生するアニメーションを設定する
        this->mnAnimationNowIndex = this->mnAnimationReserveIndex;

        // これから再生するアニメーションのループ設定を記憶させておく
        this->GetNowAnimation2D()->SetLoopFlag(this->mbReserveLoopFlag);

        // このアニメーションを最初に再生するフレームだということを記憶しておく
        this->mbFirstFrame = true;
    }

    return 0;
}

int Animation2DList::Initialize()
{
    this->mnAnimationOldTag = Animation2DTag::Animation2DTag_Default;
    this->mnAnimationNowTag = Animation2DTag::Animation2DTag_Default;

    return 0;
}

int Animation2DList::Finalize()
{
    return 0;
}

int Animation2DList::Update()
{
    // 前回まで再生していたアニメーションを取得
    Animation2D *currentAnimation = this->GetNowAnimation2D();

    // 前回まで再生していたアニメーションがもう終わる瞬間なら、アニメーションを切り替える
    if (currentAnimation->CheckEndTimingNow())
    {
        if (this->mbTagMode == true)
        {
            // 予約されていたアニメーションを再生する
            this->ChangeAnimationTag(this->mnReserveTag, this->mbReserveLoopFlag);
        }
        else
        {
            // 予約されていたアニメーションを再生する
            this->ChangeAnimationIndex(this->mnReserveTag, this->mbReserveLoopFlag);
        }
    }

    // 現在再生中のアニメーションを取得
    Animation2D *oldAnimation = this->GetOldAnimation2D();

    // 現在再生中のアニメーションを取得
    Animation2D *nowAnimation = this->GetNowAnimation2D();

    // 再生を始めた瞬間かで、処理を返す
    if (this->mbFirstFrame)
    {
        // 再生を始めたばかりのアニメーションを初期化する
        nowAnimation->Initialize();
    }
    else
    {
        // アニメーションの更新処理を行う
        currentAnimation->Update();
    }

    if (this->mbFirstFrame)
    {
        // 再生が終わったばかりのアニメーションの後処理を行う
        oldAnimation->Finalize();

        // 最初のフレームが終わったことを記憶しておく
        this->mbFirstFrame = false;
    }

    return 0;
}

int Animation2DList::Draw()
{
    return 0;
}

int Animation2DList::Add(Animation2D *target)
{
    if (target == nullptr)
    {
        return -1;
    }

    if (this->mpFirstAnimation == nullptr)
    {
        this->mpFirstAnimation = target;
    }

    Animation2D *current = this->mpFirstAnimation;
    while (current->GetNext() != nullptr)
    {
        current = current->GetNext();
    }

    return this->ConnectTarget(current, target, nullptr);
}

int Animation2DList::DeleteTarget(Animation2D *target)
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

int Animation2DList::DeleteAll()
{
    if (this->mpFirstAnimation == nullptr)
    {
        return 0;
    }

    Animation2D *current = this->mpFirstAnimation;
    Animation2D *next = current->GetNext();
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

int Animation2DList::ConnectTarget(Animation2D *prev, Animation2D *target, Animation2D *next)
{
    if (prev != nullptr)
    {
        prev->SetNext(target);
    }
    if (next != nullptr)
    {
        next->SetPrev(target);
    }
    if (next == this->mpFirstAnimation)
    {
        this->mpFirstAnimation = target;
    }
    target->SetPrev(prev);
    target->SetNext(next);

    return 0;
}

int Animation2DList::IsolateTarget(Animation2D *target)
{
    Animation2D *prev = target->GetPrev();
    Animation2D *next = target->GetNext();

    if (prev != nullptr)
    {
        prev->SetNext(next);
    }
    if (next != nullptr)
    {
        next->SetPrev(prev);
    }
    if (target == this->mpFirstAnimation)
    {
        this->mpFirstAnimation = next;
    }
    target->SetPrev(nullptr);
    target->SetNext(nullptr);

    return 0;
}

Animation2D *Animation2DList::SearchAnimation2DTag(Animation2DTag tag) const
{
    Animation2D *current = this->mpFirstAnimation;
    while (current != nullptr)
    {
        if (current->GetTag() == tag)
        {
            break;
        }
        current = current->GetNext();
    }
    return current;
}

Animation2D *Animation2DList::SearchAnimation2DIndex(unsigned short index) const
{
    Animation2D *current = this->mpFirstAnimation;
    unsigned short counter = 0;
    while (current != nullptr)
    {
        if (counter == index)
        {
            break;
        }
        current = current->GetNext();
        counter++;
    }
    return current;
}

Animation2D *Animation2DList::GetNowAnimation2D() const
{
    Animation2D *temp = nullptr;
    if (this->mbTagMode)
    {
        temp = SearchAnimation2DTag(this->mnAnimationNowTag);
    }
    else
    {
        temp = SearchAnimation2DIndex(this->mnAnimationNowIndex);
    }
    return temp;
}

Animation2D *Animation2DList::GetOldAnimation2D() const
{
    Animation2D *temp = nullptr;
    if (this->mbTagMode)
    {
        temp = SearchAnimation2DTag(this->mnAnimationOldTag);
    }
    else
    {
        temp = SearchAnimation2DIndex(this->mnAnimationOldIndex);
    }
    return temp;
}

bool Animation2DList::GetNowLoopFlag() const
{
    bool temp = false;
    if (this->mbTagMode)
    {
        temp = SearchAnimation2DTag(this->mnAnimationNowTag)->GetLoopFlag();
    }
    else
    {
        temp = SearchAnimation2DIndex(this->mnAnimationNowIndex)->GetLoopFlag();
    }
    return temp;
}