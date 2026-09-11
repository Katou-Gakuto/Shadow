#include "BaseCollision2D.h"

#include "BaseCollision2DManager.h"
#include "../Z_Except/Master.h"

BaseCollision2D::BaseCollision2D(
    GameObject *object, CollisionType type, CollisionTag tag, CollisionNorm normNum,
    bool penetrate, bool hitMove, float moveLate) :
    mvBasePos(VECTOR2D::GetZero()),
    mfBaseRadiusNoSqrt(0.0f),
    mbBasePosUpdateFlag(true),
    mbBaseRadiusUpdateFlag(true),
    mvMoveVec(VECTOR2D::GetZero()),
    mpPrevWorld(nullptr),
    mpNextWorld(nullptr),
    BaseCollision(object, type, tag, normNum, penetrate, hitMove, moveLate)
{

}

BaseCollision2D::~BaseCollision2D()
{

}

int BaseCollision2D::Draw(const Material2D &color)
{
    return 0;
}

int BaseCollision2D::SetPosToMoveVec()
{
    return 0;
}

int BaseCollision2D::SetMoveVec(void *moveData)
{
    if (moveData != nullptr)
    {
        VECTOR2D *moveVec = (VECTOR2D *)(moveData);
        this->mvMoveVec = *moveVec;
    }
    return 0;
}

int BaseCollision2D::SetNextPos(void *posData)
{
    if (posData != nullptr)
    {
        VECTOR2D moveVec = *((VECTOR2D *)(posData)) - this->mvBasePos;
        this->mvMoveVec = moveVec;
    }
    return 0;
}

int BaseCollision2D::WorldConnectMySelf()
{
    return Master::mpBaseCollision2DManager->Add(this);
}

int BaseCollision2D::WorldIsolateMySelf()
{
    return Master::mpBaseCollision2DManager->IsolateTarget(this);
}

int BaseCollision2D::SetBaseParamMySelf()
{
    return 0;
}