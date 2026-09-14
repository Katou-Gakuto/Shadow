#include "MirrorObjectController.h"

#include "../A_GameObject/GameObject2D.h"

#include "../S_Collision/BaseCollisionList.h"
#include "../S_Collision/Ray2D.h"

MirrorObjectController::MirrorObjectController(GameObject *myObject, const VECTOR2D &leftUp, const VECTOR2D &rightDown, unsigned char mirrorFace) :
    mnMirrorFace(mirrorFace),
    WallObjectController(myObject, ComponentTagAndOrder::CTAO_MirrorController, leftUp, rightDown)
{
}

MirrorObjectController::~MirrorObjectController()
{
}

int MirrorObjectController::Create()
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
    CollisionTag setTag;

    // ê›íËÇ∑ÇÈ
    for (unsigned char i = 0; i < 4; i++)
    {
        // 
        if (this->mnMirrorFace == i)
        {
            // 
            setTag = CollisionTag::CollisionTag_Mirror;
        }
        else
        {
            // 
            setTag = CollisionTag::CollisionTag_Wall;
        }

        // 
        collision = new Ray2D(VECTOR2D(), VECTOR2D(), obj, setTag, false, false, 0.0f);

        // 
        if (obj->AddCollision(collision, 0) != 0)
        {
            // 
            return -1;
        }
    }

    // 
    return 0;
}

int MirrorObjectController::Initialize()
{
    // 
    this->mvLeftUp = this->mvLeftUpInit;

    // 
    this->mvRightDown = this->mvRightDownInit;

    // 
    VECTOR2D posMap[4];
    posMap[0] = this->mvLeftUp;
    posMap[1].SetXY(this->mvRightDown.GetX(), this->mvLeftUp.GetY());
    posMap[2].SetXY(this->mvLeftUp.GetX(), this->mvRightDown.GetY());
    posMap[3] = this->mvRightDown;

    // 
    GameObject2D *obj = this->GetMyObject2D();
    if (obj == nullptr)
    {
        // 
        return -1;
    }

    // 
    BaseCollisionList *list = obj->GetBaseCollisionList();

    // 
    for (unsigned char i = 0; i < 4; i++)
    {
        // 
        Ray2D *currentRay = static_cast<Ray2D *>(list->SearchCollisionNum(i));

        // 
        if (currentRay == nullptr)
        {
            // 
            return -1;
        }

        // 
        switch (i)
        {
        case 0:
            currentRay->SetShapeParameter(posMap[0], posMap[1]);
            break;
        case 1:
            currentRay->SetShapeParameter(posMap[1], posMap[3]);
            break;
        case 2:
            currentRay->SetShapeParameter(posMap[3], posMap[2]);
            break;
        case 3:
            currentRay->SetShapeParameter(posMap[2], posMap[0]);
            break;
        default:
            break;
        }
    }

    // 
    return 0;
}

int MirrorObjectController::Finalize()
{
    // 
    return 0;
}

int MirrorObjectController::EarlyUpdate()
{
    // 
    return 0;
}

int MirrorObjectController::Update()
{
    // 
    return 0;
}

int MirrorObjectController::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    // 
    return 0;
}

int MirrorObjectController::LateUpdate()
{
    // 
    return 0;
}

int MirrorObjectController::Draw()
{
    // 
    return 0;
}
