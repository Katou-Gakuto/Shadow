#include "LightLineNode.h"

#include "../A_GameObject/GameObject2D.h"

#include "../C_Component/LightObjectController.h"

#include "../S_Collision/BaseCollision2DManager.h"

#include "../Y_Tool/MyFunctions.h"

#include "Master.h"

// コンストラクタ
LightLineNode::LightLineNode(void *parent) :
    mpParentLight(parent),
    mvEndPos(),
    mbNext(false),
    mpPrev(nullptr),
    mpNext(nullptr)
{
}

// デストラクタ
LightLineNode::~LightLineNode()
{
    // 自身の一つ後のノードが有効な場合は処理を行う
    if (this->mpNext != nullptr)
    {
        // 削除する
        delete this->mpNext;

        // 明示的にnullptrにする
        this->mpNext = nullptr;
    }

    // 
    this->mpPrev->mpNext = nullptr;
}

// 自身の光の終了地点を計算し、設定する関数
// 次のノードにつながる場合はtrueを返す
bool LightLineNode::CalculateEndPos(
    const VECTOR2D &startPos,
    const VECTOR2D &toEndNorm,
    const std::vector<BaseCollision2D *> &objectCollBox,
    const std::vector<BaseCollision2D *> &mirrorCollBox,
    float &stockMagunitude,
    Ray2D &tempRay,
    CollisionCheckResult2D &tempResultCurrent,
    CollisionCheckResult2D &tempResultNewr)
{
    // 
    VECTOR2D start2current;
    VECTOR2D start2newr;

    // 
    tempResultCurrent.mnResultParam = -1;
    tempResultNewr.mnResultParam = -1;

    // 
    tempRay.SetShapeParameter(startPos, startPos + (toEndNorm * stockMagunitude));

    // このレイが最も近くで当たったオブジェクトを走査する関数
    for (uint32_t i = 0; i < objectCollBox.size(); i++)
    {
        // 当たり判定の計算を行う
        tempResultCurrent = BaseCollision2DManager::CheckHitCollision2DToCollision2D(&tempRay, objectCollBox[i]);

        // 当たっていなかった場合は処理を行う
        if (tempResultCurrent.mnResultParam <= 0)
        {
            // ループの頭まで処理を飛ばす
            continue;
        }

        // 今回のレイの開始地点から、当たった場所までの距離を取得する
        start2current = tempResultCurrent.mvHitPos - tempRay.GetStartPos();

        // 今回初めてオブジェクトに当たった場合、もしくはこれまでのどれよりも当たった場所が近かった場合は処理を行う
        if (tempResultNewr.mnResultParam < 0 ||
            start2current.Magnitude() < start2newr.Magnitude())
        {
            // 今回の結果をそのまま記憶しておく
            tempResultNewr = tempResultCurrent;

            // スタート地点から今回当たった地点までのベクトルを記憶しておく
            start2newr = start2current;
        }
    }

    // このレイが最も近くで当たった鏡面を走査する関数
    for (uint32_t i = 0; i < mirrorCollBox.size(); i++)
    {
        // 当たり判定の計算を行う
        tempResultCurrent = BaseCollision2DManager::CheckHitCollision2DToCollision2D(&tempRay, mirrorCollBox[i]);

        // 当たっていなかった場合は処理を行う
        if (tempResultCurrent.mnResultParam <= 0)
        {
            // ループの頭まで処理を飛ばす
            continue;
        }

        // 今回のレイの開始地点から、当たった場所までの距離を取得する
        start2current = tempResultCurrent.mvHitPos - tempRay.GetStartPos();

        // 今回初めてオブジェクトに当たった場合、もしくはこれまでのどれよりも当たった場所が近かった場合は処理を行う
        if (tempResultNewr.mnResultParam < 0 ||
            start2current.Magnitude() < start2newr.Magnitude())
        {
            // 今回の結果をそのまま記憶しておく
            tempResultNewr = tempResultCurrent;

            // スタート地点から今回当たった地点までのベクトルを記憶しておく
            start2newr = start2current;

            // 鏡に当たったことを記憶しておく
            this->mbNext = true;
        }
    }

    // 今回のレイが何にあたったかで処理を変更する
    if (tempResultNewr.mnResultParam < 0)
    {
        // レイの終点を光の線の終点に記憶しておく
        this->mvEndPos = tempRay.GetEndPos();
    }
    else
    {
        // レイが当たった中で最もスタートに近い地点を光の線の終点に設定する
        this->mvEndPos = tempResultNewr.mvHitPos;

        // 
        if (this->mbNext)
        {
            // 鏡に光線とライトを設定する？
            stockMagunitude -= start2newr.Magnitude();
        }
    }

    // 
    return this->mbNext;
}

// 自身のノードのひとつ前の終了地点、つまり自身のスタート地点を返す関数
// mpPrevがnullptrの場合はライトの根元を返す
const VECTOR2D &LightLineNode::GetMyStartPos() const
{
    // 
    if (this->mpPrev == nullptr)
    {
        // 
        return this->GetListStartPos();
    }

    // 
    return this->mpPrev->mvEndPos;
}

// このライトの根元地点を返す関数
const VECTOR2D &LightLineNode::GetListStartPos() const
{
    // 
    GameObject2D *light = reinterpret_cast<GameObject2D *>(this->mpParentLight);

    // 
    return *light->GetPositionPtr();
}

// 自身のノードの終了地点を返す関数
const VECTOR2D &LightLineNode::GetMyEndPos() const
{
    // 
    return this->mvEndPos;
}

// 前のノードに移る関数
bool LightLineNode::AccessPrev(LightLineNode **out)
{
    // 
    if (out == nullptr)
    {
        // 
        return false;
    }

    // 
    *out = this->mpPrev;

    // 
    return true;
}

// 次のノードに移る関数
// mpNextがtrueの場合はnewを行い、自身のmpNextにヒープのインスタンスを記憶してから、そのポインタを返します
bool LightLineNode::AccessNext(LightLineNode **out)
{
    // 
    if (out == nullptr)
    {
        // 
        return false;
    }

    // 
    if (this->mbNext == false)
    {
        // 
        *out = nullptr;

        // 
        return false;
    }

    // 
    if (this->mpNext == nullptr)
    {
        // 
        LightLineNode *newNode = new LightLineNode(this->mpParentLight);

        // 
        newNode->mpPrev = this;

        // 
        this->mpNext = newNode;
    }

    // 
    *out = this->mpNext;

    // 
    return true;
}

// 次のノードが有効かを確認する関数
bool LightLineNode::CheckNext() const
{
    // 
    return this->mbNext;
}

// 
LightArea::LightArea(void *parentLight) :
    mlLightFirstNode(),
    mpParentLight(parentLight),
    mnLightLineCount(0)
{
}

// 
LightArea::~LightArea()
{
}

// 
void LightArea::SetLightLineAngleBox(uint32_t lineCount, float maxAngle)
{
    // 
    float oneAngle = MyFunctions::GetAllRad() / static_cast<float>(lineCount);

    // 
    this->SetLightLineCount(lineCount);

    // 
    float startAngle = maxAngle * 0.5f;

    // 
    for (uint32_t i = 0; i < lineCount; i++)
    {
        // 
        this->mlLineAngleBox[i] = startAngle + (oneAngle * static_cast<float>(i));
    }
}

// 自身の光の終了地点を計算し、設定する関数
// 次のノードにつながる場合はtrueを返す
bool LightArea::CalculateNode(
    float baseAngle,
    float stockMagunitude,
    const VECTOR2D &startPos,
    const std::vector<BaseCollision2D *> &objectCollBox,
    const std::vector<BaseCollision2D *> &mirrorCollBox,
    Ray2D &tempRay,
    CollisionCheckResult2D &tempResultCurrent,
    CollisionCheckResult2D &tempResultNewr)
{
    // 
    LightLineNode *currentNode = nullptr;
    
    // 
    float tempMagnitudeNoSqrt = 0.0f;

    // 
    VECTOR2D tempStartPos = startPos;

    // 
    VECTOR2D toEndNorm;

    // 
    for (uint32_t i = 0; i < this->mnLightLineCount; i++)
    {
        // 
        toEndNorm = VECTOR2D::GetOnUnitCirclePos('x', 'y', baseAngle + this->mlLineAngleBox[i]);

        // 
        currentNode = this->mlLightFirstNode[i];

        // 
        tempMagnitudeNoSqrt = stockMagunitude;

        // 計算を行い、次のノードに移る場合は処理を行う
        while (currentNode->CalculateEndPos(
            tempStartPos,
            toEndNorm,
            objectCollBox,
            mirrorCollBox,
            tempMagnitudeNoSqrt,
            tempRay,
            tempResultCurrent,
            tempResultNewr))
        {
            // 
            tempStartPos = currentNode->GetMyEndPos();

            // 
            toEndNorm = VECTOR2D::Reflect(toEndNorm, tempResultNewr.mvRepulsionVecA);

            // 
            this->mlLightFirstNode[i]->AccessNext(&currentNode);
        }

        // 
        this->mlLineEndPosBox[i] = currentNode->GetMyEndPos();
    }

    // 
    return true;
}

// 
uint32_t LightArea::GetAllNodeCount() const
{
    // 
    uint32_t counter = 0;

    // 
    LightLineNode *current = nullptr;

    // 
    LightLineNode *next = nullptr;

    // 
    for (size_t i = 0; i < this->mlLightFirstNode.size(); i++)
    {
        // 
        current = this->mlLightFirstNode[i];

        // 
        while (current != nullptr)
        {
            // 
            if (current->AccessNext(&next))
            {
                // 
                counter++;

                // 
                current = next;
            }
            else
            {
                // 
                break;
            }
        }
    }

    // 
    return counter;
}

// 
LightLineNode **LightArea::GetFirstNodeBox()
{
    // 
    return this->mlLightFirstNode.data();
}

// 
LightLineNode * const *LightArea::GetFirstNodeBox() const
{
    // 
    return this->mlLightFirstNode.data();
}

// 
VECTOR2D *LightArea::GetLineEndPosBox()
{
    // 
    return this->mlLineEndPosBox.data();
}

// 
const VECTOR2D *LightArea::GetLineEndPosBox() const
{
    // 
    return this->mlLineEndPosBox.data();
}

// 
uint32_t LightArea::GetLightLineCount() const
{
    // 
    return this->mnLightLineCount;
}

// 
void LightArea::SetLightLineCount(uint32_t count)
{
    // 
    if (this->mnLightLineCount < count)
    {
        // 
        this->mlLightFirstNode.resize(count);

        // 
        this->mlLineEndPosBox.resize(count);

        // 
        this->mlLineAngleBox.resize(count);
    }

    // 
    this->mnLightLineCount = count;
}

// 
LightObjectController *LightArea::GetLightController() const
{
    // 
    return reinterpret_cast<LightObjectController *>(this->mpParentLight);
}

