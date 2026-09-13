#include "BaseCollision.h"

#include "BaseCollision2D.h"
#include "BaseCollision2DManager.h"
#include "BaseCollision3D.h"
#include "BaseCollision3DManager.h"
#include "BaseCollisionList.h"

#include "../A_GameObject/GameObject.h"
#include "../A_GameObject/GameObject2D.h"
#include "../A_GameObject/GameObject3D.h"

#include "../Z_Except/Master.h"

BaseCollision::BaseCollision(
    GameObject *object, CollisionType type, CollisionTag tag, CollisionNorm normNum,
    bool penetrate, bool hitMove, float moveLate) :
    mpMyObject(object),
    mnCollisionTag(tag),
    mnCollisionType(type),
    mnNormNum(normNum),
    mnCollisionListNum(0),
    mbDeleteFlag(false),
    mbActiveFlag(true),
    mbPenetrate(penetrate),
    mbHitMove(hitMove),
    mfMoveLate(moveLate),
    mbProcessingFlag(CollisionTag::CollisionTag_Max),
    mnDeleteCountDown(),
    mpPrevList(nullptr),
    mpNextList(nullptr)
{

}

BaseCollision::~BaseCollision()
{

}

int BaseCollision::HitOnCollision(BaseCollision *hitCollision)
{
    if (this->mpMyObject != nullptr)
    {
        return this->mpMyObject->HitOnCollision(this, hitCollision);
    }
    return 0;
}

int BaseCollision::AddProcessingTag(CollisionTag tag)
{
    return this->mbProcessingFlag.OnFlag((unsigned long)(tag));
}

bool BaseCollision::CheckProcessingFlag(CollisionTag tag)
{
    return (this->mbProcessingFlag.CheckActive() && this->mbProcessingFlag.CheckFlag((unsigned long)(tag)));
}

CollisionDimension BaseCollision::GetCollisionDimension() const
{
    CollisionDimension dimension = CollisionDimension_Max;
    switch (this->mnCollisionType)
    {
    case CollisionType_Point2D:
    case CollisionType_Circle2D:
    case CollisionType_Capsule2D:
    case CollisionType_Triangle2D:
    case CollisionType_Quadrangle2D:
    case CollisionType_Ray2D:
        dimension = CollisionDimension::CollisionDimension_2D;
        break;
    case CollisionType_Point3D:
    case CollisionType_Sphere3D:
    case CollisionType_Capsule3D:
    case CollisionType_Triangle3D:
    case CollisionType_Quadrangle3D:
    case CollisionType_Cube3D:
    case CollisionType_Ray3D:
        dimension = CollisionDimension::CollisionDimension_3D;
        break;
    case CollisionType_Max:
    default:
        break;
    }
    return dimension;
}

int BaseCollision::SetLife(unsigned long life)
{
    if (life == 0)
    {
        return -1;
    }
    return this->mnDeleteCountDown.Initialize(life);
}

int BaseCollision::UpdateLife()
{
    // 既に削除が予定されている場合、寿命が設定されていない場合は処理を飛ばす
    if (this->mbDeleteFlag || this->mnDeleteCountDown.CheckActive() == false)
    {
        return 0;
    }

    // 余命の更新を行い、余命が来たら処理を行う
    if (this->mnDeleteCountDown.Update(false))
    {
        // 自身を削除するようにお願いする
        this->OnDeleteFlag();
    }

    // 関数が正常終了したことを返す
    return 0;
}

int BaseCollision::Draw(const Material2D &color)
{
    return 0;
}

int BaseCollision::SetPosToMoveVec()
{
    return 0;
}

GameObject2D *BaseCollision::GetMyObject2D() const
{
    return dynamic_cast<GameObject2D *>(this->mpMyObject);
}

GameObject3D *BaseCollision::GetMyObject3D() const
{
    return dynamic_cast<GameObject3D *>(this->mpMyObject);
}
