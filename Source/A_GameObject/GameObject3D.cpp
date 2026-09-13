#include "GameObject3D.h"
#include "../T_Model/BaseModel.h"
#include "../S_Collision/BaseCollisionList.h"

#include "../Y_Tool/MyFunctions.h"

GameObject3D::GameObject3D(const VECTOR3D &pos, GameObjectTag tag) :
    mvPosition3D(pos),
    mvMoveVec3D(0.0f, 0.0f, 0.0f),
    mvRotation(0.0f, 0.0f, 0.0f),
    mvScaling(1.0f, 1.0f, 1.0f),
    GameObject(tag)
{
    this->SetForword(VECTOR3D::GetFront(), 0.0f);
}

GameObject3D::~GameObject3D()
{

}

int GameObject3D::Create()
{
    return 0;
}

int GameObject3D::ResetMoveVec()
{
    this->mvMoveVec3D = VECTOR3D::GetZero();
    return 0;
}

int GameObject3D::SetPositionToMoveVec()
{
    this->mvPosition3D += this->mvMoveVec3D;
    return 0;
}

int GameObject3D::SetCollisionPosToCollisionMoveVec()
{
    if (this->GetBaseCollisionList() == nullptr)
    {
        return 0;
    }
    return this->GetBaseCollisionList()->SetCollisionPosCollisionMoveVec();    
}

MyMatrix GameObject3D::GetWorldMatrix() const
{
    MyMatrix world = MyMatrix::GetIdentity(4);
    world *= MyMatrix::GetScaling3D(this->mvScaling);
    world *= MyMatrix::GetRotation3D(this->mvRotation);
    world *= MyMatrix::GetTransportion3D(this->mvPosition3D);
    return world;
}

MyMatrix GameObject3D::GetRotationMatrix() const
{
    return MyMatrix::GetRotation3D(this->mvRotation);
}

void GameObject3D::SetForword(const VECTOR3D &forwordVec, float zRotate)
{
    VECTOR3D norm = forwordVec.Normalize();
    float xParam = asinf(-norm.GetY());
    // float yParam = MyFunctions::GetFastAtan2InigoQuilez(norm.GetX(), norm.GetZ());          // ‹ß—®‚Á‚Ä‚â‚Â‚Å‚‘¬atan2‚ğ‚µ‚Ä‚Ü‚·B
    float yParam = atan2(norm.GetX(), norm.GetZ());                                         // ¸“x‚ª—Ç‚­‚Äˆ—‚ªd‚¢atan2‚Ìˆ—

    /*
    float yParam = acos(forwordVec.GetZ());
    if (forwordVec.GetX() < 0.0f)
    {
        yParam = -yParam;
    }
    */
    float zParam = zRotate;
    this->mvRotation.SetXYZ(xParam, yParam, zParam);
}

VECTOR3D GameObject3D::GetForwordVec() const
{
    return (VECTOR3D::GetFront() * MyMatrix::GetRotation3D(this->mvRotation)).To3VD();
}
