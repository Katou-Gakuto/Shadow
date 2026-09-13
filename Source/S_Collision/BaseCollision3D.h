#pragma once

#include "BaseCollision.h"

#include <math.h>

#include "AABB.h"

class GameObject3D;

class BaseCollision3D : public BaseCollision
{
private:
    VECTOR3D mvBasePos;                         // カリング処理で作る円の中心点
    float mfBaseRadiusNoSqrt;                   // カリング処理で作る円の大きさ
    bool mbBasePosUpdateFlag;                   // カリング処理で使用する円の中心座標を更新する必要があるかどうか    
    bool mbBaseRadiusUpdateFlag;                // カリング処理で使用する円の半径を更新する必要があるかどうか    
    VECTOR3D mvMoveVec;                         // 移動の進行ベクトル

    BaseCollision3D *mpPrevWorld;             // 前の当たり判定へのポインタ(ワールド線形リスト)
    BaseCollision3D *mpNextWorld;             // 次の当たり判定へのポインタ(ワールド線形リスト)

public:
    BaseCollision3D(
        GameObject *object, CollisionType type, CollisionTag tag, CollisionNorm normNum,
        bool penetrate, bool hitMove, float moveLate);
    virtual ~BaseCollision3D() override;

    virtual int Draw(const Material2D &color) override;
    virtual int SetPosToMoveVec() override;

    int SetMoveVec(void *moveData) override;
    int SetNextPos(void *posData) override;
    virtual int SlideMove(const CollisionCheckResult3D &result, float moveLate) = 0;

    int WorldConnectMySelf() override;
    int WorldIsolateMySelf() override;

    virtual int SetBaseParamMySelf();
    virtual int GetAABB(AABB3D &out) = 0;

    void SetBasePos(const VECTOR3D &pos)                { this->mvBasePos = pos; }
    void SetBaseRadiusSqrt(float radius)                { this->mfBaseRadiusNoSqrt = radius * radius; }
    void SetBaseRadiusNoSqrt(float radiusNoSprt)        { this->mfBaseRadiusNoSqrt = radiusNoSprt; }
    void SetBasePosUpdateFlag(bool flag)                { this->mbBasePosUpdateFlag = flag; }
    void SetBaseRadiusUpdateFlag(bool flag)             { this->mbBaseRadiusUpdateFlag = flag; }
    void SetMoveVec(const VECTOR3D &vec)                { this->mvMoveVec = vec; }
    VECTOR3D GetBasePos()                       const   { return this->mvBasePos; }
    const VECTOR3D *GetBasePosPtr()                     { return &(this->mvBasePos); }
    float GetBaseRadius()                       const   { return sqrtf(this->mfBaseRadiusNoSqrt); }
    float GetBaseRadiusNoSqrt()                 const   { return this->mfBaseRadiusNoSqrt; }
    bool GetBasePosUpdateFlag()                 const   { return this->mbBasePosUpdateFlag; }
    bool GetBaseRadiusUpdateFlag()              const   { return this->mbBaseRadiusUpdateFlag; }
    VECTOR3D GetMoveVec()                       const   { return this->mvMoveVec; }
    VECTOR3D GetMovedPos(const VECTOR3D &pos)   const   { return pos + this->mvMoveVec; }

    void SetPrevWorld(BaseCollision3D *prev)            { this->mpPrevWorld = prev; }
    void SetNextWorld(BaseCollision3D *next)            { this->mpNextWorld = next; }
    BaseCollision3D *GetPrevWorld()             const   { return this->mpPrevWorld; }
    BaseCollision3D *GetNextWorld()             const   { return this->mpNextWorld; }
};