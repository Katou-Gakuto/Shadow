#include "BaseCollision3D.h"

#include "BaseCollision3DManager.h"
#include "../Z_Except/Master.h"

BaseCollision3D::BaseCollision3D(
    GameObject *object, CollisionType type, CollisionTag tag, CollisionNorm normNum,
    bool penetrate, bool hitMove, float moveLate) :
    mvBasePos(VECTOR3D::GetZero()),
    mfBaseRadiusNoSqrt(0.0f),
    mbBasePosUpdateFlag(true),
    mbBaseRadiusUpdateFlag(true),
    mvMoveVec(VECTOR3D::GetZero()),
    mpPrevWorld(nullptr),
    mpNextWorld(nullptr),
    BaseCollision(object, type, tag, normNum, penetrate, hitMove, moveLate)
{

}

BaseCollision3D::~BaseCollision3D()
{

}

int BaseCollision3D::Draw(const Material2D &color)
{
    return 0;
}

int BaseCollision3D::SetPosToMoveVec()
{
    return 0;
}

int BaseCollision3D::SetMoveVec(void *moveData)
{
    if (moveData != nullptr)
    {
        VECTOR3D *moveVec = (VECTOR3D *)(moveData);
        this->mvMoveVec = *moveVec;
    }
    return 0;
}

int BaseCollision3D::SetNextPos(void *posData)
{
    if (posData != nullptr)
    {
        VECTOR3D moveVec = *((VECTOR3D *)(posData)) - this->mvBasePos;
        this->mvMoveVec = moveVec;
    }
    return 0;
}

int BaseCollision3D::WorldConnectMySelf()
{
    return Master::mpBaseCollision3DManager->Add(this);
}

int BaseCollision3D::WorldIsolateMySelf()
{
    return Master::mpBaseCollision3DManager->IsolateTarget(this);
}

int BaseCollision3D::SetBaseParamMySelf()
{
    return 0;
}
