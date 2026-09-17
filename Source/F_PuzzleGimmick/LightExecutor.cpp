#include "LightExecutor.h"

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
LightExecutor::LightExecutor(PuzzleGimmickActiveParam param) :
    mfLength(0.0f),
    mfBaseAngle(0.0f),
    mfLightAngle(MyFunctions::GetAllRad()),
    mvStartPos(),
    mnAreaIndex(0),
    mbLightOn(true),
    BaseGimmickExecutor(param)
{
}

// デストラクタ
LightExecutor::~LightExecutor()
{
}

// ギミックの内容を実行する関数
// ※GameObject::EarlyUpdate()のタイミングで呼ばれます
int LightExecutor::EarlyUpdate(bool triggerSignal)
{
    // 
    if (this->GetActiveParam().GetSignalNot())
    {
        // 
        triggerSignal = !triggerSignal;
    }

    // 
    if (!triggerSignal)
    {
        // 
        return 0;
    }

    // 
    return 0;
}

// ギミックの内容を実行する関数
// ※GameObject::Update()のタイミングで呼ばれます
int LightExecutor::Update(bool triggerSignal)
{
    // 
    if (this->GetActiveParam().GetSignalNot())
    {
        // 
        triggerSignal = !triggerSignal;
    }

    // 
    this->mbLightOn = triggerSignal;

    // 
    return 0;
}

// ギミックの内容を実行する関数
// ※GameObject::LateUpdate()のタイミングで呼ばれます
int LightExecutor::LateUpdate(bool triggerSignal)
{
    // 
    if (this->mbLightOn)
    {
        // 
        this->GetMyLightArea()->OnLight();

        // 
        this->CalculateLineEndPos(this->mvStartPos);
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
int LightExecutor::Draw(bool triggerSignal)
{
    // 光域のシミュレーターは描画はしません
    return 0;
}

// このライトのレイの角度を設定する関数
void LightExecutor::SetLineAngleBox(uint32_t lineCount)
{
    // 
    this->SetLineAngleBox_In(lineCount, this->mfLightAngle);
}

// このライトの光域を計算する関数
bool LightExecutor::CalculateLineEndPos(const VECTOR2D &startPos)
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
VECTOR2D *LightExecutor::GetLineEndPosBox()
{
    // 
    return this->GetMyLightArea()->GetLineEndPosBox();
}

// このライトのレイの本数を取得する関数
uint32_t LightExecutor::GetLightLineCount() const
{
    // 
    return this->GetMyLightArea()->GetLightLineCount();
}

// このライトの長さを取得する関数
float LightExecutor::GetLength() const
{
    // 
    return this->mfLength;
}

// このライトのレイの本数を設定する関数
void LightExecutor::SetLightLineCount(uint32_t count)
{
    // 
    this->GetMyLightArea()->SetLightLineCount(count);
}

// このライトの長さを設定する関数
void LightExecutor::SetLength(float length)
{
    this->mfLength = length;
}

// このライトの視点を設定する関数
void LightExecutor::SetStartPos(const VECTOR2D &pos)
{
    this->mvStartPos = pos;
}

// このライトの全体の角度を設定する関数
void LightExecutor::SetLightAngle(float radian)
{
    this->mfLightAngle = radian;
}

// このライトのレイの角度を設定する関数
void LightExecutor::SetLineAngleBox_In(uint32_t lineCount, float maxAngle)
{
    // 
    this->GetMyLightArea()->SetLightLineAngleBox(lineCount, maxAngle);
}

// 
LightArea *LightExecutor::GetMyLightArea() const
{
    // 
    return Master::mpLightManager->SearchArea(this->mnAreaIndex);
}
