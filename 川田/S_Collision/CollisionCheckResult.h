#pragma once

#include "../Y_Tool/VECTOR.h"

class BaseCollision2D;
class BaseCollision3D;

// 2D当たり判定×2D当たり判定の衝突・内外判定の結果をまとめた構造体
struct CollisionCheckResult2D
{
    BaseCollision2D *mpCollisionA;      // 当たり判定A
    BaseCollision2D *mpCollisionB;      // 当たり判定B
    signed long  mnResultParam;         // 衝突・内外判定の結果(-1は当たっていない、0はとりあえず当たっただけ、1以降は詳細な計算結果)
    VECTOR2D mvRepulsionVecA;           // 当たり判定Aがスライド移動・反発移動に使用するベクトル(法線ベクトルなど)
    VECTOR2D mvRepulsionVecB;           // 当たり判定Bがスライド移動・反発移動に使用するベクトル(法線ベクトルなど)
    VECTOR2D mvHitPos;                  // 二つの当たり判定の衝突地点
    VECTOR2D mvFloatBox;                // その他、処理に使用するパラメータを持っておくfloat2つの構造体
};

// 3D当たり判定×3D当たり判定の衝突・内外判定の結果をまとめた構造体
struct CollisionCheckResult3D
{
    BaseCollision3D *mpCollisionA;      // 当たり判定A
    BaseCollision3D *mpCollisionB;      // 当たり判定B
    signed long  mnResultParam;         // 衝突・内外判定の結果(-1は当たっていない、0はとりあえず当たっただけ、1以降は詳細な計算結果)
    VECTOR3D mvRepulsionVecA;           // 当たり判定Aがスライド移動・反発移動に使用するベクトル(法線ベクトルなど)
    VECTOR3D mvRepulsionVecB;           // 当たり判定Bがスライド移動・反発移動に使用するベクトル(法線ベクトルなど)
    VECTOR3D mvHitPos;                  // 二つの当たり判定の衝突地点
    VECTOR3D mvFloatBox;                // その他、処理に使用するパラメータを持っておくfloat3つの構造体
};

// 2D当たり判定×2D当たり判定の衝突・内外判定の結果をまとめた構造体を取得する関数
CollisionCheckResult2D GetCollisionCheckResult2D(
    BaseCollision2D *collisionA,
    BaseCollision2D *collisionB,
    signed long  resultParam,
    const VECTOR2D &repulsionVecA,
    const VECTOR2D &repulsionVecB,
    const VECTOR2D &hitPos,
    const VECTOR2D &floatBox);

// 3D当たり判定×3D当たり判定の衝突・内外判定の結果をまとめた構造体を取得する関数
CollisionCheckResult3D GetCollisionCheckResult3D(
    BaseCollision3D *collisionA,
    BaseCollision3D *collisionB,
    signed long  resultParam,
    const VECTOR3D &repulsionVecA,
    const VECTOR3D &repulsionVecB,
    const VECTOR3D &hitPos,
    const VECTOR3D &floatBox);

CollisionCheckResult2D GetCollisionCheckResult2DZero();
CollisionCheckResult3D GetCollisionCheckResult3DZero();
