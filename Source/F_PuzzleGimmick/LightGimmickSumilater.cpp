#include "LightGimmickSumilater.h"

#include "../A_GameObject/GameObject2D.h"

#include "../E_Scene/BaseScene.h"
#include "../E_Scene/BaseSceneManager.h"

#include "../G_LightArea/LightArea.h"
#include "../G_LightArea/LightAreaManager.h"

#include "../S_Collision/BaseCollision2D.h"
#include "../S_Collision/Ray2D.h"
#include "../S_Collision/BaseCollision2DManager.h"

#include "../Y_Tool/MyFunctions.h"

#include "../Z_Except/Master.h"

// コンストラクタ
LightGimmickSumilater::LightGimmickSumilater() :
    mfLength(0.0f),
    mfBaseAngle(0.0f),
    mfLightAngle(MyFunctions::GetAllRad()),
    mvStartPos(),
    mnAreaIndex(0),
    mbLightOn(true),
    BaseGimmickSumilator()
{
}

// デストラクタ
LightGimmickSumilater::~LightGimmickSumilater()
{
}

// シミュレーションを行うかをCheckerの結果も踏まえたうえで判断する関数
bool LightGimmickSumilater::CheckSumilate(bool checkerSignal) const
{
    // 
    if (this->mdActiveParam.GetSignalNot())
    {
        // 
        checkerSignal = !checkerSignal;
    }

    // 
    return checkerSignal;
}

// シミュレーションを行う関数
int LightGimmickSumilater::Sumilate()
{
    // 
    this->mbLightOn = true;
    
    // 
    this->CalculateLineEndPos(this->mvStartPos);

    // 
    return 0;
}

// 実行に移すかをCheckerの結果も踏まえたうえで判断する関数
int LightGimmickSumilater::CheckExecute(bool checkerSignal) const
{
    // 
    if (this->mdActiveParam.GetSignalNot())
    {
        // 
        checkerSignal = !checkerSignal;
    }

    // 
    return checkerSignal;
}

// シミュレートの結果を実行を行う関数
int LightGimmickSumilater::Execute()
{
    // 
    if (this->mbLightOn)
    {
        // 
        this->GetMyLightArea()->OnLight();
    }
    else
    {
        // 
        this->GetMyLightArea()->OffLight();
    }

    // 
    return 0;
}

// シミュレーション内容を描画する関数
// ※既に実行段階である場合は引数がtrueになります。実行段階では描画しない、あるいはその逆の場合はこの引数を使ってください。
int LightGimmickSumilater::Draw(bool sumilating, bool executing)
{
    // 光域のシミュレーターは描画はしません
}

// このライトのレイの角度を設定する関数
void LightGimmickSumilater::SetLineAngleBox(uint32_t lineCount)
{
    // 
    this->SetLineAngleBox_In(lineCount, this->mfLightAngle);
}

// このライトの光域を計算する関数
bool LightGimmickSumilater::CalculateLineEndPos(const VECTOR2D &startPos)
{
    // 
    std::vector<BaseCollision2D *> objectCollisionBox;

    // 
    BaseScene *nowScene = Master::mpBaseSceneManager->SearchSceneNow();
    if (nowScene->GetBaseCollision2DManager()->SearchTag(CollisionTag::CollisionTag_Wall, objectCollisionBox) != 0)
    {
        // 
        return false;
    }

    // 
    std::vector<BaseCollision2D *> mirrorCollisionBox;
    if (nowScene->GetBaseCollision2DManager()->SearchTag(CollisionTag::CollisionTag_Mirror, mirrorCollisionBox) != 0)
    {
        // 
        return false;
    }

    // 
    Ray2D tempRay;

    // 
    CollisionCheckResult2D resultCurrent = GetCollisionCheckResult2DZero();

    // 
    CollisionCheckResult2D resultNewr = GetCollisionCheckResult2DZero();

    // 
    return this->GetMyLightArea()->CalculateNode(
        this->mfBaseAngle,
        this->mfLength,
        startPos,
        objectCollisionBox,
        mirrorCollisionBox,
        tempRay,
        resultCurrent,
        resultNewr);
}

// このライトのレイの終点座標の配列を取得する関数
VECTOR2D *LightGimmickSumilater::GetLineEndPosBox()
{
    // 
    return this->GetMyLightArea()->GetLineEndPosBox();
}

// このライトのレイの本数を取得する関数
uint32_t LightGimmickSumilater::GetLightLineCount() const
{
    // 
    return this->GetMyLightArea()->GetLightLineCount();
}

// このライトの長さを取得する関数
float LightGimmickSumilater::GetLength() const
{
    // 
    return this->mfLength;
}

// このライトのレイの本数を設定する関数
void LightGimmickSumilater::SetLightLineCount(uint32_t count)
{
    // 
    this->GetMyLightArea()->SetLightLineCount(count);
}

// このライトの長さを設定する関数
void LightGimmickSumilater::SetLength(float length)
{
    this->mfLength = length;
}

// このライトの視点を設定する関数
void LightGimmickSumilater::SetStartPos(const VECTOR2D &pos)
{
    this->mvStartPos = pos;
}

// このライトの全体の角度を設定する関数
void LightGimmickSumilater::SetLightAngle(float radian)
{
    this->mfLightAngle = radian;
}

// このライトのレイの角度を設定する関数
void LightGimmickSumilater::SetLineAngleBox_In(uint32_t lineCount, float maxAngle)
{
    // 
    this->GetMyLightArea()->SetLightLineAngleBox(lineCount, maxAngle);
}

// 
LightArea *LightGimmickSumilater::GetMyLightArea() const
{
    // 
    return Master::mpLightManager->SearchArea(this->mnAreaIndex);
}
