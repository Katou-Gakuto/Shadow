#include "ButtonObjectController.h"

#include "../A_GameObject/GameObject2D.h"
#include "../A_GameObject/GameObjectManager.h"

#include "../E_Scene/BaseSceneManager.h"

#include "../S_Collision/BaseCollision2DManager.h"
#include "../S_Collision/BaseCollisionList.h"
#include "../S_Collision/Quadrangle2D.h"
#include "../S_Collision/Point2D.h"

#include "../Z_Except/Master.h"

// 
ButtonObjectController::ButtonObjectController(GameObject *myObject, const VECTOR2D &localLeftUp, const VECTOR2D &localRightDown) :
    mbNowOn(),
    mbHitButton(),
    mbPushLastFrame(),
    mbLightSide(),
    mnOnFrameCount(),
    mnNowFrameCount(),
    mvLeftUp(localLeftUp),
    mvRightDown(localRightDown),
    BaseComponent(myObject, ComponentTagAndOrder::CTAO_ButtonController)
{
}

// 
ButtonObjectController::~ButtonObjectController()
{
}

int ButtonObjectController::Create()
{
    // 
    GameObject2D *obj = this->GetMyObject2D();

    //
    if (obj == nullptr)
    {
        // 
        return -1;
    }

    // 
    VECTOR2D tempPos;

    // 
    Quadrangle2D *collision = new Quadrangle2D(
        tempPos,
        tempPos,
        tempPos,
        tempPos,
        obj,
        CollisionTag::CollisionTag_Button,
        CollisionNorm_Out,
        true,
        false,
        0.0f);

    // 
    obj->AddCollision(collision, this->mbBody);

    // 
    return 0;
}

int ButtonObjectController::Initialize()
{
    // 
    GameObject2D *obj = this->GetMyObject2D();
    if (obj == nullptr)
    {
        // 
        return -1;
    }

    // 
    BaseCollisionList *list = obj->GetBaseCollisionList();
    if (list == nullptr)
    {
        // 
        return -1;
    }

    // 
    VECTOR2D tempPosBox[4];

    tempPosBox[0] = this->mvLeftUp;
    tempPosBox[1].SetXY(this->mvRightDown.GetX(), this->mvLeftUp.GetY());
    tempPosBox[2].SetXY(this->mvLeftUp.GetX(), this->mvRightDown.GetY());
    tempPosBox[3] = this->mvRightDown;

    // 
    Quadrangle2D *collision = static_cast<Quadrangle2D *>(list->SearchCollision(this->mbBody));

    // 
    collision->SetShapeParameter(tempPosBox[0], tempPosBox[1], tempPosBox[2], tempPosBox[3]);

    // 
    return 0;
}

int ButtonObjectController::Finalize()
{
    // 
    return 0;
}

int ButtonObjectController::EarlyUpdate()
{
    // 
    this->CheckLightSide();

    // 
    return 0;
}

int ButtonObjectController::Update()
{
    // 
    GameObject2D *obj = this->GetMyObject2D();
    if (obj == nullptr)
    {
        // 
        return -1;
    }

    // 
    BaseCollisionList *list = obj->GetBaseCollisionList();
    if (list == nullptr)
    {
        // 
        return -1;
    }

    // 
    Quadrangle2D *collision = static_cast<Quadrangle2D *>(list->SearchCollision(this->mbBody));
    if (collision == nullptr)
    {
        // 
        return -1;
    }

    // 
    Point2D tempPoint;
    tempPoint.SetShapeParameter(obj->GetPosition());

    // 
    auto result = BaseCollision2DManager::CheckHitCollision2DToCollision2D(collision, &tempPoint);
    if (0 < result.mnResultParam)
    {
        // 
        return false;
    }

    // 
    return 0;
}

int ButtonObjectController::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    // 
    return 0;
}

int ButtonObjectController::LateUpdate()
{
    // 
    return 0;
}

int ButtonObjectController::Draw()
{
    // 
    return 0;
}

// 現在このボタンはONの状態なのか
bool ButtonObjectController::CheckButtonSignal() const
{
    // 
    return this->mbNowOn;
}

// 現在このボタンの上にプレイヤーがいるのか
bool ButtonObjectController::GetLightSide() const
{
    // 
    return this->mbLightSide;
}

// 現在このボタンの上にプレイヤーがいるのか
bool ButtonObjectController::CheckOnPlayer() const
{
    // 
    GameObject2D *myObj = this->GetMyObject2D();
    if (myObj == nullptr)
    {
        // 
        return -1;
    }

    // 
    BaseCollisionList *list = myObj->GetBaseCollisionList();
    if (list == nullptr)
    {
        // 
        return -1;
    }

    // 
    Quadrangle2D *collision = static_cast<Quadrangle2D *>(list->SearchCollision(this->mbBody));
    if (collision == nullptr)
    {
        // 
        return -1;
    }

    // このボタンがある領域の方のプレイヤー
    GameObject2D *mySidePlayer = nullptr;

    // このボタンが現在光域にある場合と影域にある場合で処理を変える
    if (this->mbLightSide)
    {
        // @Debug
        // GameObjectManagerからプレイヤーを取得する関数が完成され次第ここを変更する
        mySidePlayer = nullptr;
    }
    else
    {
        // @Debug
        // GameObjectManagerからプレイヤーを取得する関数が完成され次第ここを変更する
        mySidePlayer = nullptr;
    }

    // 
    Point2D tempPoint;
    tempPoint.SetShapeParameter(mySidePlayer->GetPosition());

    // 
    auto result = BaseCollision2DManager::CheckHitCollision2DToCollision2D(collision, &tempPoint);
    if (0 < result.mnResultParam)
    {
        // 
        return false;
    }

    // 
    return true;
}

// 
void ButtonObjectController::CheckLightSide()
{
    // 
    GameObject2D *myObj = this->GetMyObject2D();
    if (myObj == nullptr)
    {
        // 
        return;
    }

    // 
    VECTOR2D myObjPos = myObj->GetPosition();

    // 
    

    // 
    this->mbLightSide = false;
}
