#include "GameObject2D.h"

#include "../E_Scene/BaseScene.h"
#include "../E_Scene/BaseSceneManager.h"
#include "../T_Model/Model2D.h"
#include "../S_Collision/BaseCollisionList.h"
#include "../Z_Except/Master.h"

GameObject2D::GameObject2D(GameObjectTag tag) :
    mvPosition2D(VECTOR2D::GetZero()),
    mvMoveVec2D(VECTOR2D::GetZero()),
    GameObject(tag)
{

}

GameObject2D::~GameObject2D()
{

}

int GameObject2D::Create()
{
    return 0;
}

int GameObject2D::SetPositionToMoveVec()
{
    this->mvPosition2D += this->mvMoveVec2D;
    return 0;
}

// “–‚½‚è”»’èŒvŽZŒã‚Ì“–‚½‚è”»’è‚ÌÀ•W‚ÌŠm’è
int GameObject2D::SetCollisionPosToCollisionMoveVec()
{
    int temp = 0;
    if (this->GetBaseCollisionList() != nullptr)
    {
        temp = this->GetBaseCollisionList()->SetCollisionPosCollisionMoveVec();
    }
    return temp;
}

int GameObject2D::ResetMoveVec()
{
    this->mvMoveVec2D = VECTOR2D::GetZero();
    return 0;
}
