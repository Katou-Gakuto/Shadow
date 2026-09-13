#include "CollisionCheckResult.h"

// 2D当たり判定×2D当たり判定の衝突・内外判定の結果をまとめた構造体を取得する関数
CollisionCheckResult2D GetCollisionCheckResult2D(
    BaseCollision2D *collisionA,
    BaseCollision2D *collisionB,
    signed long  resultParam,
    const VECTOR2D &repulsionVecA,
    const VECTOR2D &repulsionVecB,
    const VECTOR2D &hitPos,
    const VECTOR2D &floatBox)
{
    CollisionCheckResult2D temp;
    temp.mpCollisionA = collisionA;
    temp.mpCollisionB = collisionB;
    temp.mnResultParam = resultParam;
    temp.mvRepulsionVecA = repulsionVecA;
    temp.mvRepulsionVecB = repulsionVecB;
    temp.mvHitPos = hitPos;
    temp.mvFloatBox = floatBox;
    return temp;
}

// 3D当たり判定×3D当たり判定の衝突・内外判定の結果をまとめた構造体を取得する関数
CollisionCheckResult3D GetCollisionCheckResult3D(
    BaseCollision3D *collisionA,
    BaseCollision3D *collisionB,
    signed long  resultParam,
    const VECTOR3D &repulsionVecA,
    const VECTOR3D &repulsionVecB,
    const VECTOR3D &hitPos,
    const VECTOR3D &floatBox)
{
    CollisionCheckResult3D temp;
    temp.mpCollisionA = collisionA;
    temp.mpCollisionB = collisionB;
    temp.mnResultParam = resultParam;
    temp.mvRepulsionVecA = repulsionVecA;
    temp.mvRepulsionVecB = repulsionVecB;
    temp.mvHitPos = hitPos;
    temp.mvFloatBox = floatBox;
    return temp;
}

CollisionCheckResult2D GetCollisionCheckResult2DZero()
{
    CollisionCheckResult2D temp;
    temp.mpCollisionA = nullptr;
    temp.mpCollisionB = nullptr;
    temp.mnResultParam = -1;
    temp.mvRepulsionVecA = VECTOR2D::GetZero();
    temp.mvRepulsionVecB = VECTOR2D::GetZero();
    temp.mvHitPos = VECTOR2D::GetZero();
    temp.mvFloatBox = VECTOR2D::GetZero();
    return temp;
}

CollisionCheckResult3D GetCollisionCheckResult3DZero()
{
    CollisionCheckResult3D temp;
    temp.mpCollisionA = nullptr;
    temp.mpCollisionB = nullptr;
    temp.mnResultParam = -1;
    temp.mvRepulsionVecA = VECTOR3D::GetZero();
    temp.mvRepulsionVecB = VECTOR3D::GetZero();
    temp.mvHitPos = VECTOR3D::GetZero();
    temp.mvFloatBox = VECTOR3D::GetZero();
    return temp;
}
