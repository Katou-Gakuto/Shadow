#include "LightObjectController.h"

#include "../A_GameObject/GameObject2D.h"

#include "../S_Collision/BaseCollision.h"
#include "../S_Collision/Ray2D.h"
#include "../S_Collision/BaseCollision2DManager.h"

#include "../Y_Tool/MyFunctions.h"

#include "../Z_Except/Master.h"

// 
LightObjectController::LightObjectController() :
    mfLength(10000.0f),
    mfBaseAngle(0.0f),
    mpNowLightArea(nullptr),
    MapObjectController()
{
}

// 
LightObjectController::~LightObjectController()
{

}

int LightObjectController::Create()
{
    // 
    return 0;
}

int LightObjectController::Initialize()
{
    // 
    return 0;
}

int LightObjectController::Finalize()
{
    // 
    return 0;
}

int LightObjectController::EarlyUpdate()
{
    // 
    return 0;
}

int LightObjectController::Update()
{
    // 
    return 0;
}

int LightObjectController::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    // 
    return 0;
}

int LightObjectController::LateUpdate()
{
    // 
    return 0;
}

int LightObjectController::Draw()
{
    // 
    return 0;
}

// 
void LightObjectController::SetLineAngleBox(uint32_t lineCount)
{
    // 
    this->SetLineAngleBox_In(lineCount, MyFunctions::GetAllRad());
}

// 
bool LightObjectController::CalculateLineEndPos()
{
    // 
    std::vector<BaseCollision2D *> objectCollisionBox;
    if (Master::mpBaseCollision2DManager->SearchTag(CollisionTag::CollisionTag_Wall, objectCollisionBox) != 0)
    {
        // 
        return false;
    }

    // 
    std::vector<BaseCollision2D *> mirrorCollisionBox;
    if (Master::mpBaseCollision2DManager->SearchTag(CollisionTag::CollisionTag_Mirror, mirrorCollisionBox) != 0)
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
    return this->mpNowLightArea->CalculateNode(
        this->mfBaseAngle,
        this->mfLength,
        objectCollisionBox,
        mirrorCollisionBox,
        tempRay,
        resultCurrent,
        resultNewr);
}

// このライトのレイの終点座標の配列を取得する関数
VECTOR2D *LightObjectController::GetLineEndPosBox()
{
    // 
    return this->mpNowLightArea->GetLineEndPosBox();
}

// このライトのレイの本数を取得する関数
uint32_t LightObjectController::GetLightLineCount() const
{
    // 
    return this->mpNowLightArea->GetLightLineCount();
}

// 
float LightObjectController::GetLength() const
{
    // 
    return this->mfLength;
}

// このライトのレイの本数を設定する関数
void LightObjectController::SetLightLineCount(uint32_t count)
{
    // 
    this->mpNowLightArea->SetLightLineCount(count);
}

// 
void LightObjectController::SetLength(float length)
{
    this->mfLength = length;
}

// このライトのレイの角度を設定する関数
void LightObjectController::SetLineAngleBox_In(uint32_t lineCount, float maxAngle)
{
    // 
    this->mpNowLightArea->SetLightLineAngleBox(lineCount, maxAngle);
}

// 
DirectionLightController::DirectionLightController() :
    mfAngle(),
    LightObjectController()
{
}

// 
DirectionLightController::~DirectionLightController()
{
}

int DirectionLightController::Create()
{
    // 
    return 0;
}

int DirectionLightController::Initialize()
{
    // 
    return 0;
}

int DirectionLightController::Finalize()
{
    // 
    return 0;
}

int DirectionLightController::EarlyUpdate()
{
    // 
    return 0;
}

int DirectionLightController::Update()
{
    // 
    return 0;
}

int DirectionLightController::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    // 
    return 0;
}

int DirectionLightController::LateUpdate()
{
    // 
    return 0;
}

int DirectionLightController::Draw()
{
    // 
    return 0;
}

// このライトのレイの角度を設定する関数
void DirectionLightController::SetLineAngleBox(uint32_t lineCount)
{
    this->SetLineAngleBox_In(lineCount, this->mfAngle);
}

// 
void DirectionLightController::SetAngle(float radian)
{
    // 
    this->mfAngle = radian;
}
