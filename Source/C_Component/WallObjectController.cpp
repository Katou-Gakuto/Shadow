#include "WallObjectController.h"

#include "../A_GameObject/GameObject2D.h"

#include "../S_Collision/BaseCollisionList.h"
#include "../S_Collision/Quadrangle2D.h"


WallObjectController::WallObjectController(GameObject *myObject, const VECTOR2D &leftUp, const VECTOR2D &rightDown) :
    mvLeftUpInit(leftUp),
    mvRightDownInit(rightDown),
    mvLeftUp(leftUp),
    mvRightDown(rightDown),
    BaseComponent(myObject, ComponentTagAndOrder::CTAO_MirrorController)
{
}

WallObjectController::~WallObjectController()
{
}

int WallObjectController::Create()
{
    // 
    GameObject2D *obj = this->GetMyObject2D();
    if (obj == nullptr)
    {
        // 
        return -1;
    }

    // 
    BaseCollision *collision = nullptr;

    // 
    VECTOR2D tempPos[4];

    // 
    collision = new Quadrangle2D(
        tempPos[0],
        tempPos[1],
        tempPos[2],
        tempPos[3],
        obj,
        CollisionTag::CollisionTag_Wall,
        CollisionNorm::CollisionNorm_Out,
        false,
        false,
        0.0f);

    // 
    if (obj->AddCollision(collision, 0) != 0)
    {
        // 
        return -1;
    }

    // 
    collision->WorldConnectMySelf();

    // 
    return 0;
}

int WallObjectController::Initialize()
{
    // 
    this->mvLeftUp = this->mvLeftUpInit;
    this->mvRightDown = this->mvRightDownInit;

    // 
    GameObject2D *obj = this->GetMyObject2D();
    if (obj == nullptr)
    {
        // 
        return -1;
    }

    // 
    Quadrangle2D *collision = nullptr;

    // 
    VECTOR2D tempPos[4];
    tempPos[0] = this->mvLeftUp;
    tempPos[1].SetXY(this->mvRightDown.GetX(), this->mvLeftUp.GetY());
    tempPos[2].SetXY(this->mvLeftUp.GetX(), this->mvRightDown.GetY());
    tempPos[3] = this->mvRightDown;

    // 
    BaseCollisionList *list = obj->GetBaseCollisionList();

    // 
    collision = static_cast<Quadrangle2D *>(list->SearchCollisionNum(0));

    // 
    collision->SetShapeParameter(tempPos[0], tempPos[1], tempPos[2], tempPos[3]);

    // 
    return 0;
}

int WallObjectController::Finalize()
{
    // 
    return 0;
}

int WallObjectController::EarlyUpdate()
{
    // 
    return 0;
}

int WallObjectController::Update()
{
    // 
    return 0;
}

int WallObjectController::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    // 
    return 0;
}

int WallObjectController::LateUpdate()
{
    // 
    return 0;
}

int WallObjectController::Draw()
{
    // 
    return 0;
}

WallObjectController::WallObjectController(GameObject *myObject, ComponentTagAndOrder ctao, const VECTOR2D &leftUp, const VECTOR2D &rightDown) :
    mvLeftUpInit(leftUp),
    mvRightDownInit(rightDown),
    mvLeftUp(leftUp),
    mvRightDown(rightDown),
    BaseComponent(myObject, ctao)
{
}
