#pragma once

#include "GameObject.h"
#include "../Y_Tool/VECTOR.h"

class GameObject2D : public GameObject
{
private:
    VECTOR2D mvPosition2D;                      // 自身の現在の座標
    VECTOR2D mvMoveVec2D;                       // 移動の進行ベクトル

public:
    GameObject2D(GameObjectTag tag);
    virtual ~GameObject2D() override;

    virtual int Create() override;

    int ResetMoveVec() override;                                                            // 移動ベクトルを0ベクトルに設定する
    virtual int SetPositionToMoveVec() override;                                            // 当たり判定計算後のオブジェクトの座標の確定
    virtual int SetCollisionPosToCollisionMoveVec() override;                               // 当たり判定計算後の当たり判定の座標の確定

    void SetPosition(VECTOR2D pos) { this->mvPosition2D = pos; }
    void SetNextPos(VECTOR2D pos) { this->mvMoveVec2D = pos - this->mvPosition2D; }
    void SetMoveVec(VECTOR2D vec) { this->mvMoveVec2D = vec; }
    void ResetMoveVec2D() { this->mvMoveVec2D = VECTOR2D::GetZero(); }
    VECTOR2D GetPosition() const { return this->mvPosition2D; }
    const VECTOR2D *GetPositionPtr()const { return &(this->mvPosition2D); }
    VECTOR2D GetMoveVec2D()                     const { return this->mvMoveVec2D; }
    const VECTOR2D *GetMoveVec2DPtr() const { return &(this->mvMoveVec2D); }
};