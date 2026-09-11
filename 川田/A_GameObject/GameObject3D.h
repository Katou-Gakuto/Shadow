#pragma once

#include "GameObject.h"
#include "../Y_Tool/VECTOR.h"
#include "../Y_Tool/MyMatrix.h"

class GameObject3D : public GameObject
{
private:
    VECTOR3D mvPosition3D;                      // 自身の現在の座標
    VECTOR3D mvMoveVec3D;                       // 移動の進行ベクトル

    VECTOR3D mvRotation;                        // 各軸の回転量(x == pitch, y == yaw, z == roll)
    VECTOR3D mvScaling;                         // 各軸の拡縮量

public:
    GameObject3D(const VECTOR3D &pos, GameObjectTag tag);
    ~GameObject3D() override;

    virtual int Create() override;

    int ResetMoveVec() override;                                                            // 移動ベクトルを0ベクトルに設定する
    virtual int SetPositionToMoveVec() override;                                            // 当たり判定計算後のオブジェクトの座標の確定
    virtual int SetCollisionPosToCollisionMoveVec() override;                               // 当たり判定計算後の当たり判定の座標の確定

    MyMatrix GetWorldMatrix() const;
    MyMatrix GetRotationMatrix() const;
    void SetForword(const VECTOR3D &forwordVec, float zRotate);
    VECTOR3D GetForwordVec() const;

    void SetPosition(const VECTOR3D &pos) { this->mvPosition3D = pos; }
    void SetNextPos(const VECTOR3D &pos) { this->mvMoveVec3D = pos - this->mvPosition3D; }
    void SetMoveVec3D(const VECTOR3D &vec) { this->mvMoveVec3D = vec; }
    void SetRotationX(float radian) { this->mvRotation.SetX(radian); }
    void SetRotationY(float radian) { this->mvRotation.SetY(radian); }
    void SetRotationZ(float radian) { this->mvRotation.SetZ(radian); }
    void SetRotation(const VECTOR3D &rot) { this->mvRotation = rot; }
    void SetScaling(const VECTOR3D &scl) { this->mvScaling = scl; }
    VECTOR3D GetPosition() const { return this->mvPosition3D; }
    const VECTOR3D *GetPositionPtr()const { return &(this->mvPosition3D); }
    VECTOR3D GetMoveVec3D()                     const { return this->mvMoveVec3D; }
    const VECTOR3D *GetMoveVec3DPtr() const { return &(this->mvMoveVec3D); }
    VECTOR3D GetRotation()                     const { return this->mvRotation; }
    VECTOR3D GetScaling()                     const { return this->mvScaling; }
};