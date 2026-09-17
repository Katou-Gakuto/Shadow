#include "RouteMoveExecutor.h"

#include "../A_GameObject/GameObject2D.h"

#include "../S_Collision/BaseCollisionList.h"

// コンストラクタ
RouteData::RouteData(RouteOffMode offMode, bool loop) :
    mlKeyFramesBox(),
    mbLoop(loop),
    mnOffMode(offMode)
{
}

// コピーコンストラクタ
RouteData::RouteData(const RouteData &src) :
    mlKeyFramesBox(src.mlKeyFramesBox),
    mbLoop(src.mbLoop),
    mnOffMode(src.mnOffMode)
{
}

// デストラクタ
RouteData::~RouteData()
{
}

// キーフレームを登録する関数
int RouteData::Add(const RouteKeyFrame keyFrame)
{
    // 
    this->mlKeyFramesBox.push_back(keyFrame);

    // 
    return 0;
}

// フレームに合わせた座標を取得する関数
VECTOR2D RouteData::GetMovedPos(uint32_t frameCount) const
{
    // 
    uint32_t current = 0;

    // 
    for (uint32_t i = 0; i < this->mlKeyFramesBox.size() - 1; i++)
    {
        // 
        if (this->mlKeyFramesBox[i].mnFrameCount <= frameCount &&
            frameCount < this->mlKeyFramesBox[i + 1].mnFrameCount)
        {
            // 
            current = i;

            // 
            break;
        }
    }

    // 
    uint32_t currentAnimLength =
        this->mlKeyFramesBox[current + 1].mnFrameCount - this->mlKeyFramesBox[current].mnFrameCount;

    // 
    uint32_t frameInCurrentAnim = currentAnimLength - frameCount;

    // 今回の目的の場所みたいな
    float targetTimming = 0.0f;

    // 0除算の回避
    if (frameInCurrentAnim != 0)
    {
        // 
        targetTimming = static_cast<float>(frameInCurrentAnim) / static_cast<float>(currentAnimLength);
    }

    // 
    return VECTOR2D::GetLerpPos(
        this->mlKeyFramesBox[current].mvPos,
        this->mlKeyFramesBox[current + 1].mvPos,
        targetTimming);
}

// 
bool RouteData::GetLoopFlag()
{
    // 
    return this->mbLoop;
}

// OFF時の挙動を取得する関数
RouteOffMode RouteData::GetOffMode() const
{
    // 
    return this->mnOffMode;
}

// 
bool RouteData::GetReturnFlag()
{
    // 
    return this->mnOffMode == RouteOffMode::RouteOffMode_Return;
}

// 
uint32_t RouteData::GetFinalFrame() const
{
    // 
    return this->mlKeyFramesBox[this->mlKeyFramesBox.size() - 1].mnFrameCount;
}

// コンストラクタ
RouteMoveExecutor::RouteMoveExecutor(PuzzleGimmickActiveParam param, GameObject2D *myObject, const RouteData &routeData) :
    mdMoveData(routeData),
    mnFrameCount(0),
    mpMyObject(myObject),
    BaseGimmickExecutor(param)
{
}

// デストラクタ
RouteMoveExecutor::~RouteMoveExecutor()
{
}

// ギミックの内容を実行する関数
// ※GameObject::EarlyUpdate()のタイミングで呼ばれます
int RouteMoveExecutor::EarlyUpdate(bool triggerSignal)
{
    // 
    return 0;
}

// ギミックの内容を実行する関数
// ※GameObject::Update()のタイミングで呼ばれます
int RouteMoveExecutor::Update(bool triggerSignal)
{
    // 
    if (this->mpMyObject == nullptr)
    {
        // 
        return -1;
    }

    // 
    if (this->GetActiveParam().GetSignalNot())
    {
        triggerSignal = !triggerSignal;
    }

    // 
    bool move = true;

    // 
    bool framePlus = true;

    // 
    if (triggerSignal)
    {
        // ループしない場合は処理を行う
        if (!this->mdMoveData.GetLoopFlag())
        {
            // 最後のフレーム以降になっていたら処理を行う
            if (this->mdMoveData.GetFinalFrame() <= this->mnFrameCount)
            {
                // 
                move = false;
            }
        }
    }
    else
    {
        // 動かない前提でいきます
        move = false;

        // 
        if (this->GetActiveParam().GetOnePushActive())
        {
            // 既に動き始めている場合は処理を行う
            if (0 < this->mnFrameCount)
            {
                // 
                move = true;
            }
        }
        else
        {
            // 戻る場合かで処理を分ける
            if (this->mdMoveData.GetReturnFlag())
            {
                // 戻り切っていない場合場合は処理を行う
                if (0 < this->mnFrameCount)
                {
                    // 
                    move = true;

                    // 
                    framePlus = false;
                }
            }
        }
    }

    // 
    if (move)
    {
        // 
        if (framePlus)
        {
            // 
            if (this->mdMoveData.GetLoopFlag())
            {
                // 
                this->mnFrameCount++;

                // 
                this->mnFrameCount %= this->mdMoveData.GetFinalFrame();
            }
            else
            {
                // 
                if (this->mnFrameCount < this->mdMoveData.GetFinalFrame())
                {
                    // 
                    this->mnFrameCount++;
                }
            }
        }
        else
        {
            // Returnの場合しか-方向には進まないので
            this->mnFrameCount--;
        }

        // 
        VECTOR2D nextPos = this->mdMoveData.GetMovedPos(this->mnFrameCount);

        // 
        if (this->mpMyObject->GetPosition() != nextPos)
        {
            // 
            move = true;
        }

        // 
        this->mpMyObject->SetNextPos(nextPos);

        // 
        BaseCollisionList *list = this->mpMyObject->GetBaseCollisionList();
        if (list == nullptr)
        {
            // 
            return 0;
        }

        // 
        list->SetCollisionNextPos(CollisionDimension::CollisionDimension_2D, &nextPos);
    }

    // 
    return 0;
}

// ギミックの内容を実行する関数
// ※GameObject::LateUpdate()のタイミングで呼ばれます
int RouteMoveExecutor::LateUpdate(bool triggerSignal)
{
    // 
    return 0;
}

// ギミック内容を描画する関数
// ※既に実行段階である場合は引数がtrueになります。実行段階では描画しない、あるいはその逆の場合はこの引数を使ってください。
int RouteMoveExecutor::Draw(bool triggerSignal)
{
    // 
    return 0;
}

// 
void RouteMoveExecutor::SetFrameCount(uint32_t frameCount)
{
    // 
    this->mnFrameCount = frameCount;
}
