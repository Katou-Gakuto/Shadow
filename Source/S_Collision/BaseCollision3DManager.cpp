#include "BaseCollision3D.h"
#include "BaseCollision3DManager.h"

#include "Point3D.h"
#include "Sphere3D.h"
#include "Capsule3D.h"
#include "Triangle3D.h"
#include "Quadrangle3D.h"
#include "Cube3D.h"
#include "Ray3D.h"

#include "../Y_Tool/MyFunctions.h"

// 無限円柱とRayの衝突判定
CollisionCheckResult3D calcRayInfCilinder(
    const VECTOR3D &rayStartPos,
    const VECTOR3D &rayVec,
    const VECTOR3D &centerLinePos1,
    const VECTOR3D &centerLinePos2,
    float radiusNoSqrt)
{
    // 返り値
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();

    // 作業用変数
    VECTOR3D pos1 = centerLinePos1 - rayStartPos;
    VECTOR3D centerLinePos2a = centerLinePos2 - rayStartPos;
    VECTOR3D pos1Pos2 = centerLinePos2a - pos1;

    // 各種内積値
    float Dvv = VECTOR3D::Dot(rayVec, rayVec);
    float Dsv = VECTOR3D::Dot(pos1Pos2, rayVec);
    float Dpv = VECTOR3D::Dot(pos1, rayVec);
    float Dss = VECTOR3D::Dot(pos1Pos2, pos1Pos2);
    float Dps = VECTOR3D::Dot(pos1, pos1Pos2);
    float Dpp = VECTOR3D::Dot(pos1, pos1);

    if (Dss == 0.0f)
    {
        // 円柱が定義されない
        return temp;
    }

    float A = Dvv - Dsv * Dsv / Dss;
    float B = Dpv - Dps * Dsv / Dss;
    float C = Dpp - Dps * Dps / Dss - radiusNoSqrt;

    if (A == 0.0f)
    {
        return temp;
    }

    float s = B * B - A * C;
    if (s < 0.0f)
    {
        // レイが円柱と衝突していない
        return temp;
    }

    s = sqrtf(s);
    float a1 = (B - s) / A;
    float a2 = (B + s) / A;

    VECTOR3D hitPos1 = rayStartPos + (rayVec * a1);
    VECTOR3D hitPos2 = rayStartPos + (rayVec * a2);

    temp = GetCollisionCheckResult3D(
        nullptr,
        nullptr,
        0,
        VECTOR3D::GetZero(),
        VECTOR3D::GetZero(),
        hitPos1,
        hitPos2);

    return temp;
}

// ∠P1P2P3の内積を算出
// x1, y1, z1: 点P1
// x2, y2, z2: 点P2
// x3, y3, z3: 点P3
float checkDot(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
    return (x1 - x2) * (x3 - x2) + (y1 - y2) * (y3 - y2) + (z1 - z2) * (z3 - z2);
}

// BaseCollision3DManagerの変数定義
AABB3D BaseCollision3DManager::aabbA;
AABB3D BaseCollision3DManager::aabbB;

// この二つの当たり判定が互いの処理対象なのかを確認する関数
bool BaseCollision3DManager::CheckProcessing(BaseCollision3D *collision3DA, BaseCollision3D *collision3DB)
{
    // 互いに処理をする当たり判定かを調べ、計算対象ならtrueを返す
    if (collision3DA->GetCollisionTag() == CollisionTag::CollisionTag_Checker ||
        collision3DB->GetCollisionTag() == CollisionTag::CollisionTag_Checker)
    {
        return true;
    }
    if (collision3DA->CheckProcessingFlag(collision3DB->GetCollisionTag()) &&
        collision3DB->CheckProcessingFlag(collision3DA->GetCollisionTag()))
    {
        return true;
    }
    return false;
}

// 移動ベクトルを使ったカリング処理を行う関数
bool BaseCollision3DManager::CheckHitMoveVecToCollision3D(BaseCollision3D *collision3DA, BaseCollision3D *collision3DB)
{
    return true;

    // 当たり判定の片方だけでも貫通を許すのなら、移動の進行ベクトルによるカリングをスキップする
    if (collision3DA->GetPenetrate() == true ||
        collision3DB->GetPenetrate() == true)
    {
        return true;
    }
    // 当たり判定の法線の向きが一つでも内向きの場合はカリングができない(能力的な問題)ので好きに当たってください
    if (collision3DA->CheckCollisionNormIn() || collision3DB->CheckCollisionNormIn())
    {
        return true;
    }

    // 当たり判定Aの移動ベクトル(以下Aベクトルと呼ぶ)
    bool aMoveFlag = collision3DA->GetMoveVec() != VECTOR3D::GetZero();

    // 当たり判定Bの移動ベクトル(以下Bベクトルと呼ぶ)
    bool bMoveFlag = collision3DB->GetMoveVec() != VECTOR3D::GetZero();

    // どちらも移動している場合
    if (aMoveFlag && bMoveFlag)
    {
        // 移動の進行ベクトルとコリジョンベクトルの内積を取る
        float moveA_moveB_Dot = VECTOR3D::Dot(collision3DA->GetMoveVec(), collision3DB->GetMoveVec());

        // コリジョンと移動ベクトルの内積の結果で返す値を変える
        if (moveA_moveB_Dot < 0.0f)
        {
            // コリジョンの移動ベクトル同士が向き合っているので当たる可能性がある
            return true;
        }
    }
    // 片方のみ移動している場合
    else if (aMoveFlag != bMoveFlag)
    {
        // 処理を行う前に、基準円の更新を行う
        collision3DA->SetBaseParamMySelf();
        collision3DB->SetBaseParamMySelf();
        if (aMoveFlag)
        {
            // 当たり判定A→当たり判定Bの進行ベクトルを作る(以下a2bVと呼ぶ)
            VECTOR3D a2bVec = collision3DB->GetBasePos() - collision3DA->GetBasePos();

            // a2bVとAベクトルの内積を取る
            float a2b_moveA_Dot = VECTOR3D::Dot(a2bVec, collision3DA->GetMoveVec());

            // 内積の結果、二つのベクトルが直角でなく、さらに同じような方向を向いているならtrue
            if (0 < a2b_moveA_Dot)
            {
                return true;
            }
        }
        else
        {
            // 当たり判定B→当たり判定Aの進行ベクトルを作る(以下b2aVと呼ぶ)
            VECTOR3D b2aVec = collision3DA->GetBasePos() - collision3DB->GetBasePos();

            // b2aVとBベクトルの内積を取る
            float b2a_moveB_Dot = VECTOR3D::Dot(b2aVec, collision3DB->GetMoveVec());

            // 内積の結果、二つのベクトルが直角でなく、さらに同じような方向を向いているならtrue
            if (0 < b2a_moveB_Dot)
            {
                return true;
            }
        }
    }
    else
    {
        // ※どちらも移動していない場合は衝突・内外判定の処理を無条件で行います
        // 本当は良くないんですがね。
        return true;
    }

    // 二つの当たり判定が当たることはないのでfalseを返す
    return false;
}

// 円を使ったカリング処理を行う関数
bool BaseCollision3DManager::CheckHitBaseCollision3DToBaseCollision3D(BaseCollision3D *collision3DA, BaseCollision3D *collision3DB)
{
    // 二つの当たり判定の基準円を取得する
    collision3DA->SetBaseParamMySelf();
    collision3DB->SetBaseParamMySelf();

    // 1の中心点→2の中心点の進行ベクトルを取得
    VECTOR3D distanceVec = collision3DB->GetMovedPos(collision3DB->GetBasePos()) - collision3DA->GetMovedPos(collision3DA->GetBasePos());

    // 二つの円の半径を取得
    float baseRadiusNoSqrt = collision3DA->GetBaseRadiusNoSqrt() + collision3DB->GetBaseRadiusNoSqrt();

    // 進行ベクトルと二つの円の半径を比較し、進行ベクトルの方が小さかったらfalseを返す
    if (distanceVec.MagnitudeNoSqrt() <= baseRadiusNoSqrt)
    {
        return true;
    }

    // falseを返す
    return false;
}

// AABBを使ったカリング処理を行う関数
bool BaseCollision3DManager::CheckHitAABBAABB(BaseCollision3D *collision3DA, BaseCollision3D *collision3DB)
{
    if (collision3DA->GetAABB(aabbA) != 0 || collision3DB->GetAABB(aabbB) != 0)
    {
        return false;
    }
    float aabb1minX = aabbA.mvMinPos.GetX() + aabbA.mvMoveVec.GetX();
    float aabb1minY = aabbA.mvMinPos.GetY() + aabbA.mvMoveVec.GetY();
    float aabb1minZ = aabbA.mvMinPos.GetZ() + aabbA.mvMoveVec.GetZ();
    float aabb1maxX = aabbA.mvMaxPos.GetX() + aabbA.mvMoveVec.GetX();
    float aabb1maxY = aabbA.mvMaxPos.GetY() + aabbA.mvMoveVec.GetY();
    float aabb1maxZ = aabbA.mvMaxPos.GetZ() + aabbA.mvMoveVec.GetZ();
    float aabb2minX = aabbB.mvMinPos.GetX() + aabbB.mvMoveVec.GetX();
    float aabb2minY = aabbB.mvMinPos.GetY() + aabbB.mvMoveVec.GetY();
    float aabb2minZ = aabbB.mvMinPos.GetZ() + aabbB.mvMoveVec.GetZ();
    float aabb2maxX = aabbB.mvMaxPos.GetX() + aabbB.mvMoveVec.GetX();
    float aabb2maxY = aabbB.mvMaxPos.GetY() + aabbB.mvMoveVec.GetY();
    float aabb2maxZ = aabbB.mvMaxPos.GetZ() + aabbB.mvMoveVec.GetZ();
    if (aabb1maxX < aabb2minX || aabb2maxX < aabb1minX)
    {
        return false;
    }
    if (aabb1maxY < aabb2minY || aabb2maxY < aabb1minY)
    {
        return false;
    }
    if (aabb1maxZ < aabb2minZ || aabb2maxZ < aabb1minZ)
    {
        return false;
    }
    return true;
}

// カリング処理をまとめて行い、当たり判定の計算を飛ばすべきかを確認する関数
bool BaseCollision3DManager::CheckCulling(BaseCollision3D *collision3DA, BaseCollision3D *collision3DB)
{
    // 二つの当たり判定が互いの処理対象かを確認する
    if (BaseCollision3DManager::CheckProcessing(collision3DA, collision3DB) == false)
    {
        return true;
    }
    // @Debug
    if (collision3DA->GetCollisionTag() == CollisionTag_Attack &&
        collision3DA->GetCollisionType() == CollisionType_Sphere3D)
    {
        collision3DA = collision3DA;
    }
    if (collision3DB->GetCollisionTag() == CollisionTag_Attack &&
        collision3DB->GetCollisionType() == CollisionType_Sphere3D)
    {
        collision3DB = collision3DB;
    }

    // 移動ベクトルを使ったカリング処理を行う
    if (BaseCollision3DManager::CheckHitMoveVecToCollision3D(collision3DA, collision3DB) == false)
    {
        return true;
    }
    // 円を使ったカリング処理を行う
    if (BaseCollision3DManager::CheckHitBaseCollision3DToBaseCollision3D(collision3DA, collision3DB) == false)
    {
        return true;
    }
    // AABBを使ったカリング処理を行う
    if (BaseCollision3DManager::CheckHitAABBAABB(collision3DA, collision3DB) == false)
    {
        return true;
    }

    // 飛ばすべきでないことを返す
    return false;
}

// 二つの当たり判定のスライド移動を行う関数
int BaseCollision3DManager::CollisionSlide(BaseCollision3D *collision3DA, BaseCollision3D *collision3DB, const CollisionCheckResult3D &hitResult)
{
    // どちらも貫通を許さない場合はスライド移動の処理を行う
    if (collision3DA->GetPenetrate() == false && collision3DB->GetPenetrate() == false)
    {
        // collision3DAのスライド移動を行う
        if (collision3DA->GetHitMove())
        {
            // collision3DAの移動の係数を設定する
            float aMoveLate = 0.0f;
            if (collision3DB->GetHitMove() == false)
            {
                aMoveLate = 2.0f;
            }
            else
            {
                aMoveLate = MyFunctions::GetRatio(
                    collision3DA->GetMoveLate() + collision3DB->GetMoveLate(),
                    collision3DA->GetMoveLate(),
                    2.0f);
            }

            // あとは当たり判定側に任せる
            collision3DA->SlideMove(hitResult, aMoveLate);
        }

        // collision3DBのスライド移動を行う
        if (collision3DB->GetHitMove())
        {
            // collision3DBの移動の係数を設定する
            float bMoveLate = 0.0f;
            if (collision3DA->GetHitMove() == false)
            {
                bMoveLate = 2.0f;
            }
            else
            {
                bMoveLate = MyFunctions::GetRatio(
                    collision3DB->GetMoveLate() + collision3DA->GetMoveLate(),
                    collision3DB->GetMoveLate(),
                    2.0f);
            }

            // あとは当たり判定側に任せる
            collision3DB->SlideMove(hitResult, bMoveLate);
        }
    }

    // 関数が正常終了したことを返す
    return 0;
}

// 点と謎の当たり判定の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitPoint3DToCollision3D(Point3D *point3D, BaseCollision3D *collision)
{
    // 返り値を保持する変数
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();
    if (point3D == nullptr)
    {
        return temp;
    }

    // 点じゃない方の当たり判定のタイプに合わせた関数に飛ぶ
    switch (collision->GetCollisionType())
    {
    case CollisionType::CollisionType_Point3D:
        temp = BaseCollision3DManager::CheckHitPoint3DToCollision3D(point3D, dynamic_cast<Point3D *>(collision));
        break;
    case CollisionType::CollisionType_Sphere3D:
        temp = BaseCollision3DManager::CheckHitPoint3DToSphere3D(point3D, dynamic_cast<Sphere3D *>(collision));
        break;
    case CollisionType::CollisionType_Capsule3D:
        temp = BaseCollision3DManager::CheckHitPoint3DToCapsule3D(point3D, dynamic_cast<Capsule3D *>(collision));
        break;
    case CollisionType::CollisionType_Triangle3D:
        temp = BaseCollision3DManager::CheckHitPoint3DToTriangle3D(point3D, dynamic_cast<Triangle3D *>(collision));
        break;
    case CollisionType::CollisionType_Quadrangle3D:
        temp = BaseCollision3DManager::CheckHitPoint3DToQuadrangle3D(point3D, dynamic_cast<Quadrangle3D *>(collision));
        break;
    case CollisionType::CollisionType_Cube3D:
        temp = BaseCollision3DManager::CheckHitPoint3DToCube3D(point3D, dynamic_cast<Cube3D *>(collision));
        break;
    case CollisionType::CollisionType_Ray3D:
        temp = BaseCollision3DManager::CheckHitPoint3DToRay3D(point3D, dynamic_cast<Ray3D *>(collision));
        break;
    default:
        break;
    }
    return temp;
}

// 円と謎の当たり判定の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitSphere3DToCollision3D(Sphere3D *sphere3D, BaseCollision3D *collision)
{
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();
    if (sphere3D == nullptr)
    {
        return temp;
    }

    // 円じゃない方の当たり判定のタイプに合わせた関数に飛ぶ
    switch (collision->GetCollisionType())
    {
    case CollisionType::CollisionType_Point3D:
        temp = BaseCollision3DManager::CheckHitPoint3DToSphere3D(dynamic_cast<Point3D *>(collision), sphere3D);
        break;
    case CollisionType::CollisionType_Sphere3D:
        temp = BaseCollision3DManager::CheckHitSphere3DToSphere3D(sphere3D, dynamic_cast<Sphere3D *>(collision));
        break;
    case CollisionType::CollisionType_Capsule3D:
        temp = BaseCollision3DManager::CheckHitSphere3DToCapsule3D(sphere3D, dynamic_cast<Capsule3D *>(collision));
        break;
    case CollisionType::CollisionType_Triangle3D:
        temp = BaseCollision3DManager::CheckHitSphere3DToTriangle3D(sphere3D, dynamic_cast<Triangle3D *>(collision));
        break;
    case CollisionType::CollisionType_Quadrangle3D:
        temp = BaseCollision3DManager::CheckHitSphere3DToQuadrangle3D(sphere3D, dynamic_cast<Quadrangle3D *>(collision));
        break;
    case CollisionType::CollisionType_Cube3D:
        temp = BaseCollision3DManager::CheckHitSphere3DToCube3D(sphere3D, dynamic_cast<Cube3D *>(collision));
        break;
    case CollisionType::CollisionType_Ray3D:
        temp = BaseCollision3DManager::CheckHitSphere3DToRay3D(sphere3D, dynamic_cast<Ray3D *>(collision));
        break;
    default:

        break;
    }
    return temp;
}

// カプセルと謎の当たり判定の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitCapsule3DToCollision3D(Capsule3D *capsule3D, BaseCollision3D *collision)
{
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();
    if (capsule3D == nullptr)
    {
        return temp;
    }

    // カプセルじゃない方の当たり判定のタイプに合わせた関数に飛ぶ
    switch (collision->GetCollisionType())
    {
    case CollisionType::CollisionType_Point3D:
        temp = BaseCollision3DManager::CheckHitPoint3DToCapsule3D(dynamic_cast<Point3D *>(collision), capsule3D);
        break;
    case CollisionType::CollisionType_Sphere3D:
        temp = BaseCollision3DManager::CheckHitSphere3DToCapsule3D(dynamic_cast<Sphere3D *>(collision), capsule3D);
        break;
    case CollisionType::CollisionType_Capsule3D:
        temp = BaseCollision3DManager::CheckHitCapsule3DToCapsule3D(capsule3D, dynamic_cast<Capsule3D *>(collision));
        break;
    case CollisionType::CollisionType_Triangle3D:
        temp = BaseCollision3DManager::CheckHitCapsule3DToTriangle3D(capsule3D, dynamic_cast<Triangle3D *>(collision));
        break;
    case CollisionType::CollisionType_Quadrangle3D:
        temp = BaseCollision3DManager::CheckHitCapsule3DToQuadrangle3D(capsule3D, dynamic_cast<Quadrangle3D *>(collision));
        break;
    case CollisionType::CollisionType_Cube3D:
        temp = BaseCollision3DManager::CheckHitCapsule3DToCube3D(capsule3D, dynamic_cast<Cube3D *>(collision));
        break;
    case CollisionType::CollisionType_Ray3D:
        temp = BaseCollision3DManager::CheckHitCapsule3DToRay3D(capsule3D, dynamic_cast<Ray3D *>(collision));
        break;
    default:

        break;
    }
    return temp;
}

// 三角形と謎の当たり判定の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitTriangle3DToCollision3D(Triangle3D *triangle3D, BaseCollision3D *collision)
{
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();
    if (triangle3D == nullptr)
    {
        return temp;
    }

    // 三角形じゃない方の当たり判定のタイプに合わせた関数に飛ぶ
    switch (collision->GetCollisionType())
    {
    case CollisionType::CollisionType_Point3D:
        temp = BaseCollision3DManager::CheckHitPoint3DToTriangle3D(dynamic_cast<Point3D *>(collision), triangle3D);
        break;
    case CollisionType::CollisionType_Sphere3D:
        temp = BaseCollision3DManager::CheckHitSphere3DToTriangle3D(dynamic_cast<Sphere3D *>(collision), triangle3D);
        break;
    case CollisionType::CollisionType_Capsule3D:
        temp = BaseCollision3DManager::CheckHitCapsule3DToTriangle3D(dynamic_cast<Capsule3D *>(collision), triangle3D);
        break;
    case CollisionType::CollisionType_Triangle3D:
        temp = BaseCollision3DManager::CheckHitTriangle3DToTriangle3D(triangle3D, dynamic_cast<Triangle3D *>(collision));
        break;
    case CollisionType::CollisionType_Quadrangle3D:
        temp = BaseCollision3DManager::CheckHitTriangle3DToQuadrangle3D(triangle3D, dynamic_cast<Quadrangle3D *>(collision));
        break;
    case CollisionType::CollisionType_Cube3D:
        temp = BaseCollision3DManager::CheckHitTriangle3DToCube3D(triangle3D, dynamic_cast<Cube3D *>(collision));
        break;
    case CollisionType::CollisionType_Ray3D:
        temp = BaseCollision3DManager::CheckHitTriangle3DToRay3D(triangle3D, dynamic_cast<Ray3D *>(collision));
        break;
    default:

        break;
    }
    return temp;
}

// 四角形と謎の当たり判定の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitQuadrangle3DToCollision3D(Quadrangle3D *quadrangle3D, BaseCollision3D *collision)
{
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();
    if (quadrangle3D == nullptr)
    {
        return temp;
    }

    // 四角形じゃない方の当たり判定のタイプに合わせた関数に飛ぶ
    switch (collision->GetCollisionType())
    {
    case CollisionType::CollisionType_Point3D:
        temp = BaseCollision3DManager::CheckHitPoint3DToQuadrangle3D(dynamic_cast<Point3D *>(collision), quadrangle3D);
        break;
    case CollisionType::CollisionType_Sphere3D:
        temp = BaseCollision3DManager::CheckHitSphere3DToQuadrangle3D(dynamic_cast<Sphere3D *>(collision), quadrangle3D);
        break;
    case CollisionType::CollisionType_Capsule3D:
        temp = BaseCollision3DManager::CheckHitCapsule3DToQuadrangle3D(dynamic_cast<Capsule3D *>(collision), quadrangle3D);
        break;
    case CollisionType::CollisionType_Triangle3D:
        temp = BaseCollision3DManager::CheckHitTriangle3DToQuadrangle3D(dynamic_cast<Triangle3D *>(collision), quadrangle3D);
        break;
    case CollisionType::CollisionType_Quadrangle3D:
        temp = BaseCollision3DManager::CheckHitQuadrangle3DToQuadrangle3D(quadrangle3D, dynamic_cast<Quadrangle3D *>(collision));
        break;
    case CollisionType::CollisionType_Cube3D:
        temp = BaseCollision3DManager::CheckHitQuadrangle3DToCube3D(quadrangle3D, dynamic_cast<Cube3D *>(collision));
        break;
    case CollisionType::CollisionType_Ray3D:
        temp = BaseCollision3DManager::CheckHitQuadrangle3DToRay3D(quadrangle3D, dynamic_cast<Ray3D *>(collision));
        break;
    default:
        break;
    }
    return temp;
}

// キューブと謎の当たり判定の衝突確認を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitCube3DToCollision3D(Cube3D *cube3D, BaseCollision3D *collision)
{
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();
    if (cube3D == nullptr)
    {
        return temp;
    }

    // 四角形じゃない方の当たり判定のタイプに合わせた関数に飛ぶ
    switch (collision->GetCollisionType())
    {
    case CollisionType::CollisionType_Point3D:
        temp = BaseCollision3DManager::CheckHitPoint3DToCube3D(dynamic_cast<Point3D *>(collision), cube3D);
        break;
    case CollisionType::CollisionType_Sphere3D:
        temp = BaseCollision3DManager::CheckHitSphere3DToCube3D(dynamic_cast<Sphere3D *>(collision), cube3D);
        break;
    case CollisionType::CollisionType_Capsule3D:
        temp = BaseCollision3DManager::CheckHitCapsule3DToCube3D(dynamic_cast<Capsule3D *>(collision), cube3D);
        break;
    case CollisionType::CollisionType_Triangle3D:
        temp = BaseCollision3DManager::CheckHitTriangle3DToCube3D(dynamic_cast<Triangle3D *>(collision), cube3D);
        break;
    case CollisionType::CollisionType_Quadrangle3D:
        temp = BaseCollision3DManager::CheckHitQuadrangle3DToCube3D(dynamic_cast<Quadrangle3D *>(collision), cube3D);
        break;
    case CollisionType::CollisionType_Cube3D:
        temp = BaseCollision3DManager::CheckHitCube3DToCube3D(cube3D, dynamic_cast<Cube3D *>(collision));
        break;
    case CollisionType::CollisionType_Ray3D:
        temp = BaseCollision3DManager::CheckHitCube3DToRay3D(cube3D, dynamic_cast<Ray3D *>(collision));
        break;
    default:

        break;
    }
    return temp;
}

// レイと謎の当たり判定の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitRay3DToCollision3D(Ray3D *ray3D, BaseCollision3D *collision)
{
    // 返り値
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();
    if (ray3D == nullptr)
    {
        return temp;
    }

    switch (collision->GetCollisionType())
    {
    case CollisionType::CollisionType_Point3D:
        temp = BaseCollision3DManager::CheckHitPoint3DToRay3D(dynamic_cast<Point3D *>(collision), ray3D);
        break;
    case CollisionType::CollisionType_Sphere3D:
        temp = BaseCollision3DManager::CheckHitSphere3DToRay3D(dynamic_cast<Sphere3D *>(collision), ray3D);
        break;
    case CollisionType::CollisionType_Capsule3D:
        temp = BaseCollision3DManager::CheckHitCapsule3DToRay3D(dynamic_cast<Capsule3D *>(collision), ray3D);
        break;
    case CollisionType::CollisionType_Triangle3D:
        temp = BaseCollision3DManager::CheckHitTriangle3DToRay3D(dynamic_cast<Triangle3D *>(collision), ray3D);
        break;
    case CollisionType::CollisionType_Quadrangle3D:
        temp = BaseCollision3DManager::CheckHitQuadrangle3DToRay3D(dynamic_cast<Quadrangle3D *>(collision), ray3D);
        break;
    case CollisionType::CollisionType_Cube3D:
        temp = BaseCollision3DManager::CheckHitCube3DToRay3D(dynamic_cast<Cube3D *>(collision), ray3D);
        break;
    case CollisionType::CollisionType_Ray3D:
        temp = BaseCollision3DManager::CheckHitRay3DToRay3D(dynamic_cast<Ray3D *>(collision), ray3D);
        break;
    default:
        break;
    }

    return temp;

    // 衝突地点の計算はこのサイトを参考にしました
    // http://marupeke296.com/COL_2D_No11_CircleVsRay.html
}

// 点と点の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitPoint3DToPoint3D(Point3D *point3Da, Point3D *point3Db)
{
    // 既に当たってることを確認しているのですぐ値を設定して、計算結果を返す
    return GetCollisionCheckResult3D(
        point3Da,
        point3Db,
        0,
        point3Da->GetBasePos() - point3Db->GetBasePos(),
        point3Db->GetBasePos() - point3Da->GetBasePos(),
        VECTOR3D::GetZero(),
        VECTOR3D::GetZero());
}

// 点と円の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitPoint3DToSphere3D(Point3D *point3D, Sphere3D *sphere3D)
{
    // 既に当たってることを確認しているのですぐ値を設定して、計算結果を返す
    return GetCollisionCheckResult3D(
        point3D,
        sphere3D,
        0,
        point3D->GetBasePos() - sphere3D->GetBasePos(),
        sphere3D->GetBasePos() - point3D->GetBasePos(),
        VECTOR3D::GetZero(),
        VECTOR3D::GetZero());
}

// 点とカプセルの内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitPoint3DToCapsule3D(Point3D *point3D, Capsule3D *capsule3D)
{
    // 返り値
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();

    // 球体とカプセルの線上最近点を求める(世界座標 無限延長線編)
    VECTOR3D newrPos = VECTOR3D::GetNewrPosOnLine(
        point3D->GetMovedPos(point3D->GetBasePos()),
        capsule3D->GetMovedPos(capsule3D->GetStartPos()),
        capsule3D->GetMovedPos(capsule3D->GetEndPos()));

    // ベクトル二つを改めて作る
    VECTOR3D startToNewr = newrPos - capsule3D->GetMovedPos(capsule3D->GetStartPos());
    VECTOR3D startToEnd = capsule3D->GetMovedPos(capsule3D->GetEndPos()) - capsule3D->GetMovedPos(capsule3D->GetStartPos());

    // 線上最近点を中心としたカプセルのスフィアを作る
    Sphere3D capsuleCircle2D;

    // 線上最近点が、どっちのセンターベクトルを軸とした+方向にあるか、-方向にあるかが確定していないため、それを判断するswitch文
    switch (VECTOR3D::CheckAxisVecPlusOrMinus(startToNewr, startToEnd))
    {
    case 1:
        // startNewrがプラス方向にあるため、線上最近点が確定していない
        if (startToNewr.MagnitudeNoSqrt() <= startToEnd.MagnitudeNoSqrt())
        {
            // カプセルのセンターベクトル内に線上最近点があったということで、作業用のスフィアの値を設定する
            capsuleCircle2D.SetShapeParameter(newrPos, capsule3D->GetBaseRadiusNoSqrt());
            temp = BaseCollision3DManager::CheckHitPoint3DToSphere3D(point3D, &capsuleCircle2D);
            if (0 <= temp.mnResultParam)
            {
                temp = GetCollisionCheckResult3D(
                    point3D,
                    capsule3D,
                    0,
                    point3D->GetBasePos() - capsule3D->GetBasePos(),
                    capsule3D->GetBasePos() - point3D->GetBasePos(),
                    point3D->GetBasePos(),
                    newrPos);
            }
        }
        else
        {
            // カプセルのセンターベクトル内に線上最近点はなかったということで、+方向の端点とスフィアで当たり判定を計算する
            capsuleCircle2D.SetShapeParameter(capsule3D->GetMovedPos(capsule3D->GetEndPos()), capsule3D->GetBaseRadiusNoSqrt());
            temp = BaseCollision3DManager::CheckHitPoint3DToSphere3D(point3D, &capsuleCircle2D);
            if (0 <= temp.mnResultParam)
            {
                temp = GetCollisionCheckResult3D(
                    point3D,
                    capsule3D,
                    0,
                    point3D->GetBasePos() - capsule3D->GetBasePos(),
                    capsule3D->GetBasePos() - point3D->GetBasePos(),
                    point3D->GetBasePos(),
                    newrPos);
            }
        }

        break;
    case -1:
        // 線上最近点がマイナス方向にあるため、-方向の端点とスフィアで当たり判定を計算する
        capsuleCircle2D.SetShapeParameter(capsule3D->GetMovedPos(capsule3D->GetBasePos()), capsule3D->GetBaseRadius());
        temp = BaseCollision3DManager::CheckHitPoint3DToSphere3D(point3D, &capsuleCircle2D);
        if (0 <= temp.mnResultParam)
        {
            temp = GetCollisionCheckResult3D(
                point3D,
                capsule3D,
                0,
                point3D->GetBasePos() - capsule3D->GetBasePos(),
                capsule3D->GetBasePos() - point3D->GetBasePos(),
                point3D->GetBasePos(),
                newrPos);
        }
        break;
    case 0:
        // anewrがプラス方向にもマイナス方向にもないため、何もできない。

        break;
    default:
        // 謎エラー。

        break;
    }

    // 計算結果を返す
    return temp;
}

// 点と三角形の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitPoint3DToTriangle3D(Point3D *point3D, Triangle3D *triangle3D)
{
    // 返り値
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();
    VECTOR3D v0 = triangle3D->GetMovedPos(triangle3D->GetVertexPos(1)) - triangle3D->GetMovedPos(triangle3D->GetVertexPos(0));
    VECTOR3D v1 = triangle3D->GetMovedPos(triangle3D->GetVertexPos(2)) - triangle3D->GetMovedPos(triangle3D->GetVertexPos(0));
    VECTOR3D v2 = point3D->GetMovedPos(point3D->GetBasePos()) - triangle3D->GetMovedPos(triangle3D->GetVertexPos(0));

    float dot00 = VECTOR3D::Dot(v0, v0);
    float dot01 = VECTOR3D::Dot(v0, v1);
    float dot02 = VECTOR3D::Dot(v0, v2);
    float dot11 = VECTOR3D::Dot(v1, v1);
    float dot12 = VECTOR3D::Dot(v1, v2);

    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // u,v が範囲内なら三角形内にある
    if (u >= 0 && v >= 0 && u + v <= 1)
    {
        // 点が三角形の中に入っているということなのでtrueを返す
        temp = GetCollisionCheckResult3D(
            point3D,
            triangle3D,
            0,
            triangle3D->GetBasePos() - point3D->GetBasePos(),
            point3D->GetBasePos() - triangle3D->GetBasePos(),
            point3D->GetBasePos(),
            VECTOR3D::GetZero());
    }

    // 計算結果を返す
    return temp;
}

// 点と四角形の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitPoint3DToQuadrangle3D(Point3D *point3D, Quadrangle3D *quadrangle3D)
{
    // 返り値
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();

    Triangle3D tempTriangle3D;
    tempTriangle3D.SetShapeParameter(
        quadrangle3D->GetMovedPos(quadrangle3D->GetVertexPos(0)),
        quadrangle3D->GetMovedPos(quadrangle3D->GetVertexPos(1)),
        quadrangle3D->GetMovedPos(quadrangle3D->GetVertexPos(2)));


    temp = BaseCollision3DManager::CheckHitPoint3DToTriangle3D(point3D, &tempTriangle3D);

    if (0 <= temp.mnResultParam)
    {
        temp = GetCollisionCheckResult3D(
            point3D,
            quadrangle3D,
            1,
            point3D->GetBasePos() - quadrangle3D->GetBasePos(),
            quadrangle3D->GetBasePos() - point3D->GetBasePos(),
            point3D->GetBasePos(),
            VECTOR3D::GetZero());
    }
    else
    {
        tempTriangle3D.SetShapeParameter(
            quadrangle3D->GetMovedPos(quadrangle3D->GetVertexPos(3)),
            quadrangle3D->GetMovedPos(quadrangle3D->GetVertexPos(2)),
            quadrangle3D->GetMovedPos(quadrangle3D->GetVertexPos(1)));

        temp = BaseCollision3DManager::CheckHitPoint3DToTriangle3D(point3D, &tempTriangle3D);
        if (0 <= temp.mnResultParam)
        {
            temp = GetCollisionCheckResult3D(
                point3D,
                quadrangle3D,
                2,
                point3D->GetBasePos() - quadrangle3D->GetBasePos(),
                quadrangle3D->GetBasePos() - point3D->GetBasePos(),
                point3D->GetBasePos(),
                VECTOR3D::GetZero());
        }
    }

    return temp;
}

// 点とキューブの衝突確認を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitPoint3DToCube3D(Point3D *point3D, Cube3D *cube3D)
{
    // 返り値
    CollisionCheckResult3D returnValue = GetCollisionCheckResult3DZero();

    // 6面の法線ベクトルが全て内向きだった場合と、6面の中にある場合はtrue

    // 作業用変数
    Quadrangle3D cubeQuadrangle[6];
    VECTOR3D normVecBox[6];
    CubeFaceNumber currentFace = CubeFace_Max;
    bool pointIn = true;
    int result = 0;

    // 法線ベクトルによる内外判定を行う
    for (int i = 0; i < (int)(CubeFaceNumber::CubeFace_Max); i++)
    {
        currentFace = Cube3D::Int2CFN(i);
        result = cube3D->GetFaceQuad(&cubeQuadrangle[i], currentFace, cube3D->GetCollisionNorm());
        if (result != 0)
        {
            continue;
        }
        normVecBox[i] = cubeQuadrangle[i].GetNormVec();
        if (normVecBox[i] == VECTOR3D::GetTargetDirectionNewrVec(
            point3D->GetBasePos() - cubeQuadrangle[i].GetVertexPos(0),
            normVecBox[i],
            -normVecBox[i]))
        {
            pointIn = false;
            break;
        }
    }

    if (pointIn == true)
    {
        returnValue = GetCollisionCheckResult3D(
            point3D,
            cube3D,
            1,
            point3D->GetBasePos() - cube3D->GetBasePos(),
            cube3D->GetBasePos() - point3D->GetBasePos(),
            point3D->GetBasePos(),
            VECTOR3D::GetZero());

        return returnValue;
    }

    // 返り値を保持しておく変数
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();
    for (int i = 0; i < 6; i++)
    {
        temp = BaseCollision3DManager::CheckHitCollision3DToCollision3D(&cubeQuadrangle[i], point3D);
        if (0 <= temp.mnResultParam)
        {
            returnValue = GetCollisionCheckResult3D(
                point3D,
                cube3D,
                2 + i,
                normVecBox[i],
                -normVecBox[i],
                point3D->GetBasePos(),
                VECTOR3D::GetZero());
            break;
        }
    }

    return returnValue;
}

// 点とレイの内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitPoint3DToRay3D(Point3D *point3D, Ray3D *ray3D)
{
    // 返り値
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();
    do
    {
        VECTOR3D start2point = point3D->GetBasePos() - ray3D->GetStartPos();
        VECTOR3D start2end = ray3D->GetLineVec();
        float dot = VECTOR3D::Dot(start2point, start2end);
        if (MyFunctions::EqualInRangeF((float)fabs((double)(dot)), 1.0f, 0.0001f) == false)
        {
            // 別ベクトルの場所にいました
            break;
        }
        if (start2end.MagnitudeNoSqrt() < start2point.MagnitudeNoSqrt())
        {
            // Rayの延長線上にはいるけど、Rayの上にはいないので
            break;
        }

        temp = GetCollisionCheckResult3D(
            point3D,
            ray3D,
            0,
            point3D->GetBasePos() - ray3D->GetBasePos(),
            ray3D->GetBasePos() - point3D->GetBasePos(),
            point3D->GetBasePos(),
            VECTOR3D::GetZero());
    } while (false);
    return temp;
}

// 球と球の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitSphere3DToSphere3D(Sphere3D *sphere3Da, Sphere3D *sphere3Db)
{
    // 返り値
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();

    // 二つの円の半径を取得
    float baseRadiusNoSqrt = sphere3Da->GetBaseRadiusNoSqrt() + sphere3Db->GetBaseRadiusNoSqrt();

    VECTOR3D movedSpherePosA = sphere3Da->GetMovedPos(sphere3Da->GetBasePos());
    VECTOR3D movedSpherePosB = sphere3Db->GetMovedPos(sphere3Db->GetBasePos());

    // 1の中心点→2の中心点の進行ベクトルを取得
    VECTOR3D distanceVec = movedSpherePosB - movedSpherePosA;

    // 二つの円の半径と進行ベクトルを比較し、二つの円の半径のほうが小さかったらfalseを返す
    if (distanceVec.MagnitudeNoSqrt() <= baseRadiusNoSqrt)
    {
        temp = GetCollisionCheckResult3D(
            sphere3Da,
            sphere3Db,
            0,
            movedSpherePosA - movedSpherePosB,
            movedSpherePosB - movedSpherePosA,
            VECTOR3D::GetLerpPos(movedSpherePosA, movedSpherePosB, 0.5f),
            VECTOR3D::GetZero());
    }

    // trueを返す
    return temp;
}

// 球とカプセルの内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitSphere3DToCapsule3D(Sphere3D *sphere3D, Capsule3D *capsule3D)
{
    // 返り値
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();

    // 球体とカプセルの線上最近点を求める(世界座標 無限延長線編)
    VECTOR3D newrPos = VECTOR3D::GetNewrPosOnLine(
        sphere3D->GetMovedPos(sphere3D->GetBasePos()),
        capsule3D->GetMovedPos(capsule3D->GetStartPos()),
        capsule3D->GetMovedPos(capsule3D->GetEndPos()));

    // ベクトル二つを改めて作る
    VECTOR3D startToNewr = newrPos - capsule3D->GetMovedPos(capsule3D->GetStartPos());
    VECTOR3D startToEnd = capsule3D->GetMovedPos(capsule3D->GetEndPos()) - capsule3D->GetMovedPos(capsule3D->GetStartPos());

    // 線上最近点を中心としたカプセルのスフィアを作る
    Sphere3D capsuleCircle2D;

    // そもそも近くで当たりすぎて、startToNewrが0ベクトルの場合もありうるのでその可能性を調べる
    if (VECTOR3D::GetZero() == startToNewr)
    {
        temp = GetCollisionCheckResult3D(
            sphere3D,
            capsule3D,
            0,
            sphere3D->GetBasePos() - newrPos,
            newrPos - sphere3D->GetBasePos(),
            VECTOR3D::GetZero(),
            newrPos);
    }
    else
    {
        // startToNewrが0ベクトルではない→startから離れたところにnewrPosがある
        // ↓
        // 線上最近点が、センターベクトルを軸とした+方向にあるか、-方向にあるかが確定していないため、それを判断するswitch文
        switch (VECTOR3D::CheckAxisVecPlusOrMinus(startToNewr, startToEnd))
        {
        case 1:
            // startNewrがプラス方向にあるため、線上最近点が確定していない
            if (startToNewr.MagnitudeNoSqrt() <= startToEnd.MagnitudeNoSqrt())
            {
                // カプセルのセンターベクトル内に線上最近点があったということで、作業用のスフィアの値を設定する
                capsuleCircle2D.SetShapeParameter(newrPos, capsule3D->GetBaseRadiusNoSqrt());
                temp = BaseCollision3DManager::CheckHitSphere3DToSphere3D(sphere3D, &capsuleCircle2D);
                if (0 <= temp.mnResultParam)
                {
                    // カプセルの法線ベクトルを取得する
                    VECTOR3D capsuleCrossVec3D = VECTOR3D::Cross(capsule3D->GetLineVec(), VECTOR3D(0.0f, 1.0f, 0.0f));
                    VECTOR3D capsule2circle = sphere3D->GetBasePos() - newrPos;
                    capsuleCrossVec3D = VECTOR3D::GetTargetDirectionNewrVec(capsule2circle, capsuleCrossVec3D, -capsuleCrossVec3D);

                    temp = GetCollisionCheckResult3D(
                        sphere3D,
                        capsule3D,
                        1,
                        capsuleCrossVec3D,
                        capsule3D->GetBasePos() - sphere3D->GetBasePos(),
                        VECTOR3D::GetZero(),
                        newrPos);
                }
            }
            else
            {
                // カプセルのセンターベクトル内に線上最近点はなかったということで、+方向の端点とスフィアで当たり判定を計算する
                capsuleCircle2D.SetShapeParameter(capsule3D->GetMovedPos(capsule3D->GetEndPos()), capsule3D->GetBaseRadiusNoSqrt());
                temp = BaseCollision3DManager::CheckHitSphere3DToSphere3D(sphere3D, &capsuleCircle2D);
                if (0 <= temp.mnResultParam)
                {
                    temp = GetCollisionCheckResult3D(
                        sphere3D,
                        capsule3D,
                        2,
                        sphere3D->GetBasePos() - newrPos,
                        newrPos - sphere3D->GetBasePos(),
                        VECTOR3D::GetZero(),
                        newrPos);
                }
            }

            break;
        case -1:
            // 線上最近点がマイナス方向にあるため、-方向の端点とスフィアで当たり判定を計算する
            capsuleCircle2D.SetShapeParameter(capsule3D->GetMovedPos(capsule3D->GetStartPos()), capsule3D->GetBaseRadiusNoSqrt());
            temp = BaseCollision3DManager::CheckHitSphere3DToSphere3D(sphere3D, &capsuleCircle2D);
            if (0 <= temp.mnResultParam)
            {
                temp = GetCollisionCheckResult3D(
                    sphere3D,
                    capsule3D,
                    3,
                    sphere3D->GetBasePos() - newrPos,
                    newrPos - sphere3D->GetBasePos(),
                    VECTOR3D::GetZero(),
                    newrPos);
            }
            break;
        case 0:
            // 別ベクトルにあるので。
            break;
        default:
            // 謎エラー。
            break;
        }
    }

    // 値を返す
    return temp;
}

// 球と三角形の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitSphere3DToTriangle3D(Sphere3D *sphere3D, Triangle3D *triangle3D)
{
    // 返り値
    CollisionCheckResult3D returnValue = GetCollisionCheckResult3DZero();

    // 円の中心点を点に変換する
    Point3D spherePoint;
    spherePoint.SetBasePos(sphere3D->GetMovedPos(sphere3D->GetBasePos()));

    // 三角形の中に円の中心点が入っていたらtrueを返す
    returnValue = BaseCollision3DManager::CheckHitPoint3DToTriangle3D(&spherePoint, triangle3D);
    if (0 <= returnValue.mnResultParam)
    {
        returnValue = GetCollisionCheckResult3D(
            sphere3D,
            triangle3D,
            1,
            sphere3D->GetBasePos() - triangle3D->GetBasePos(),
            triangle3D->GetBasePos() - sphere3D->GetBasePos(),
            sphere3D->GetBasePos(),
            VECTOR3D::GetZero());
    }
    else
    {
        // 球の中心点の座標を取得する
        VECTOR3D centerPos = sphere3D->GetMovedPos(sphere3D->GetBasePos());

        // 三角形の法線ベクトルと三角形の頂点の座標を取得する
        VECTOR3D triangleVertex = triangle3D->GetMovedPos(triangle3D->GetVertexPos(0));
        VECTOR3D triangleNorm = triangle3D->GetNormVec();

        // 球の中心点と無限平面の距離を求める
        float newrMagnitude = VECTOR3D::GetPointToPlaneDistance(centerPos, triangleVertex, triangleNorm);

        // 返り値を設定する
        if (newrMagnitude <= sphere3D->GetBaseRadius())
        {
            VECTOR3D hitPos = sphere3D->GetBasePos() - triangleNorm.SetMagnitude(newrMagnitude);
            returnValue = GetCollisionCheckResult3D(
                sphere3D,
                triangle3D,
                2,
                triangleNorm,
                -triangleNorm,
                hitPos,
                VECTOR3D::GetZero());
        }
    }

    // 計算結果を返す
    return returnValue;
}

// 球と四角形の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitSphere3DToQuadrangle3D(Sphere3D *sphere3D, Quadrangle3D *quadrangle3D)
{
    // 返り値
    CollisionCheckResult3D returnValue = GetCollisionCheckResult3DZero();

    // 円の中心点を点に変換する
    Point3D spherePoint;
    spherePoint.SetBasePos(sphere3D->GetMovedPos(sphere3D->GetBasePos()));

    // 四角形の中に円の中心点が入っていたらtrueを返す
    returnValue = BaseCollision3DManager::CheckHitCollision3DToCollision3D(&spherePoint, quadrangle3D);
    if (0 < returnValue.mnResultParam)
    {
        returnValue = GetCollisionCheckResult3D(
            sphere3D,
            quadrangle3D,
            1,
            sphere3D->GetBasePos() - quadrangle3D->GetBasePos(),
            quadrangle3D->GetBasePos() - sphere3D->GetBasePos(),
            sphere3D->GetBasePos(),
            VECTOR3D::GetZero());
    }
    else
    {
        // 関数の返り値を保持するための変数
        CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();

        // 作業用変数
        Triangle3D quadTriangle;
        quadTriangle.SetShapeParameter(quadrangle3D->GetVertexPos(0), quadrangle3D->GetVertexPos(1), quadrangle3D->GetVertexPos(2));
        quadTriangle.SetMoveVec(quadrangle3D->GetMoveVec());

        // 三角形と球の当たり判定を行う
        temp = BaseCollision3DManager::CheckHitCollision3DToCollision3D(&quadTriangle, sphere3D);
        if (temp.mnResultParam == -1)
        {
            // 三角形と球の当たり判定を行う
            quadTriangle.SetShapeParameter(quadrangle3D->GetVertexPos(3), quadrangle3D->GetVertexPos(2), quadrangle3D->GetVertexPos(1));
            temp = BaseCollision3DManager::CheckHitCollision3DToCollision3D(&quadTriangle, sphere3D);
        }

        // 返り値の設定をする
        if (0 <= temp.mnResultParam)
        {
            returnValue = GetCollisionCheckResult3D(
                sphere3D,
                quadrangle3D,
                2,
                temp.mvRepulsionVecA,
                temp.mvRepulsionVecB,
                temp.mvHitPos,
                temp.mvFloatBox);
        }
    }

    // 計算結果を返す
    return returnValue;
}

// 球とキューブの衝突確認を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitSphere3DToCube3D(Sphere3D *sphere3D, Cube3D *cube3D)
{
    // 返り値
    CollisionCheckResult3D returnValue = GetCollisionCheckResult3DZero();

    // 6面の法線ベクトルが全て内向きだった場合と、6面の中にある場合はtrue

    // 作業用変数
    Quadrangle3D cubeQuadrangle[6];
    bool processingFlag[6] = { true,true,true,true,true,true, };
    VECTOR3D normVecBox[6];
    CubeFaceNumber currentFace = CubeFace_Max;
    bool pointIn = true;
    int result = 0;

    // 法線ベクトルによる内外判定を行う
    for (int i = 0; i < (int)(CubeFaceNumber::CubeFace_Max); i++)
    {
        currentFace = Cube3D::Int2CFN(i);
        result = cube3D->GetFaceQuad(&cubeQuadrangle[i], currentFace, cube3D->GetCollisionNorm());
        if (result != 0)
        {
            processingFlag[i] = false;
            continue;
        }
        normVecBox[i] = cubeQuadrangle[i].GetNormVec();
        if (normVecBox[i] == VECTOR3D::GetTargetDirectionNewrVec(
            sphere3D->GetBasePos() - cubeQuadrangle[i].GetVertexPos(0),
            normVecBox[i],
            -normVecBox[i]))
        {
            pointIn = false;
            break;
        }
    }

    if (pointIn == true)
    {
        returnValue = GetCollisionCheckResult3D(
            sphere3D,
            cube3D,
            1,
            sphere3D->GetBasePos() - cube3D->GetBasePos(),
            cube3D->GetBasePos() - sphere3D->GetBasePos(),
            sphere3D->GetBasePos(),
            VECTOR3D::GetZero());

        return returnValue;
    }

    // 返り値を保持しておく変数
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();
    for (int i = 0; i < 6; i++)
    {
        if (processingFlag[i])
        {
            temp = BaseCollision3DManager::CheckHitCollision3DToCollision3D(&cubeQuadrangle[i], sphere3D);
            if (0 <= temp.mnResultParam)
            {
                returnValue = GetCollisionCheckResult3D(
                    sphere3D,
                    cube3D,
                    2 + i,
                    normVecBox[i],
                    -normVecBox[i],
                    temp.mvHitPos,
                    VECTOR3D::GetZero());
                break;
            }
        }
    }

    return returnValue;
}

// 球とレイの衝突確認を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitSphere3DToRay3D(Sphere3D *sphere3D, Ray3D *ray3D)
{
    // 返り値
    CollisionCheckResult3D returnValue = GetCollisionCheckResult3DZero();

    // やっぱりマルペケさんでした(http://marupeke296.com/COL_3D_No24_RayToSphere.html)

    VECTOR3D startPos = ray3D->GetMovedPos(ray3D->GetStartPos());
    VECTOR3D lineVec = ray3D->GetLineVec();
    VECTOR3D centerPos = sphere3D->GetMovedPos(ray3D->GetBasePos()) - startPos;

    float a = VECTOR3D::Dot(lineVec, lineVec);
    float b = VECTOR3D::Dot(lineVec, centerPos);
    float c = VECTOR3D::Dot(centerPos, centerPos) - sphere3D->GetBaseRadiusNoSqrt();
    do
    {
        if (a == 0.0f)
        {
            // レイの長さが0
            break;
        }

        float s = b * b - a * c;
        if (s < 0.0f)
        {
            // 衝突していない
            break;;
        }

        s = sqrtf(s);
        float a1 = (b - s) / a;
        float a2 = (b + s) / a;

        if (a1 < 0.0f || a2 < 0.0f)
        {
            // レイの反対で衝突
            break;
        }

        VECTOR3D hitPos1 = startPos + (lineVec * a1);
        VECTOR3D hitPos2 = startPos + (lineVec * a2);
        returnValue = GetCollisionCheckResult3D(
            sphere3D,
            ray3D,
            1,
            VECTOR3D::GetZero(),
            VECTOR3D::GetZero(),
            hitPos1,
            hitPos2);
    } while (false);
    return returnValue;
}

// カプセルとカプセルの内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitCapsule3DToCapsule3D(Capsule3D *capsule3Da, Capsule3D *capsule3Db)
{
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();

    if (capsule3Da == nullptr ||
        capsule3Db == nullptr)
    {
        return temp;
    }

    /* 最終的にスフィアとスフィアの計算にする予定。 */
    // 計算用のスフィアの座標を記憶しておく変数
    VECTOR3D sphere3DPos = VECTOR3D::GetZero();

    // スフィアとスフィアの当たり判定の計算で使う、スフィアのもとになるカプセル
    Capsule3D targetCapsule;

    // 計算用のスフィアからみた計算用のカプセルの線上最近点の世界座標を保持する変数
    VECTOR3D sphereToCapsuleNewrPointPos = VECTOR3D::GetZero();

    // 計算用のスフィアの半径
    float radius = 0.0f;

    // スフィアになってくれるカプセルの端を探している
    for (int i = 0; i < 4; i++)
    {
        VECTOR3D thisToTargetNewrPos = VECTOR3D::GetZero();
        VECTOR3D thisCapsulePos = VECTOR3D::GetZero();
        VECTOR3D targetCapsuleStartPos = VECTOR3D::GetZero();
        VECTOR3D targetCapsuleEndPos = VECTOR3D::GetZero();

        switch (i)
        {
        case 0:
            thisCapsulePos = capsule3Da->GetMovedPos(capsule3Da->GetStartPos());
            targetCapsuleStartPos = capsule3Db->GetMovedPos(capsule3Db->GetStartPos());
            targetCapsuleEndPos = capsule3Db->GetMovedPos(capsule3Db->GetEndPos());
            thisToTargetNewrPos = VECTOR3D::GetNewrPosOnLine(thisCapsulePos, targetCapsuleStartPos, targetCapsuleEndPos);
            break;
        case 1:
            thisCapsulePos = capsule3Da->GetMovedPos(capsule3Da->GetEndPos());
            targetCapsuleStartPos = capsule3Db->GetMovedPos(capsule3Db->GetStartPos());
            targetCapsuleEndPos = capsule3Db->GetMovedPos(capsule3Db->GetEndPos());
            thisToTargetNewrPos = VECTOR3D::GetNewrPosOnLine(thisCapsulePos, targetCapsuleStartPos, targetCapsuleEndPos);
            break;
        case 2:
            thisCapsulePos = capsule3Db->GetMovedPos(capsule3Db->GetStartPos());
            targetCapsuleStartPos = capsule3Da->GetMovedPos(capsule3Da->GetStartPos());
            targetCapsuleEndPos = capsule3Da->GetMovedPos(capsule3Da->GetEndPos());
            thisToTargetNewrPos = VECTOR3D::GetNewrPosOnLine(thisCapsulePos, targetCapsuleStartPos, targetCapsuleEndPos);
            break;
        case 3:
            thisCapsulePos = capsule3Db->GetMovedPos(capsule3Db->GetEndPos());
            targetCapsuleStartPos = capsule3Da->GetMovedPos(capsule3Da->GetStartPos());
            targetCapsuleEndPos = capsule3Da->GetMovedPos(capsule3Da->GetEndPos());
            thisToTargetNewrPos = VECTOR3D::GetNewrPosOnLine(thisCapsulePos, targetCapsuleStartPos, targetCapsuleEndPos);
            break;
        default:
            break;
        }

        // 直前で計算した点Pからカプセルの直線ABの線上最近点までのベクトルを持つ変数
        VECTOR3D nowThisToTargetNewrVec = VECTOR3D::GetZero();

        // これまでの計算で一番、点Pからカプセルの直線ABの線上最近点までのベクトルが小さかった時の結果を持つ変数
        VECTOR3D minThisToTargetNewrVec = sphereToCapsuleNewrPointPos - sphere3DPos;

        // これまでの計算で求めた線上最近点が直線ABの中に入っているならtrue
        if (VECTOR3D::CheckPointInVector(thisToTargetNewrPos, targetCapsuleStartPos, targetCapsuleEndPos) == 1)
        {
            // 直前の計算からでた点Pから線上最近点へのベクトルをそのまま代入する。
            nowThisToTargetNewrVec = thisToTargetNewrPos - thisCapsulePos;
        }
        else
        {
            // 線上最近点をカプセルの両端のどちらかにしなければいけならず、線上最近点が-方向にある可能性も捨てきれない場合の処理
            switch (VECTOR3D::CheckAxisVecPlusOrMinus(
                thisToTargetNewrPos - targetCapsuleStartPos,
                targetCapsuleEndPos - targetCapsuleStartPos))
            {
            case 0: /* わけわからんベクトルを向いているとき */
                // 線上最近点を……

                break;
            case 1: /* +方向に向かって伸びているとき */
                // 線上最近点をendPosに設定する
                thisToTargetNewrPos = targetCapsuleEndPos;
                break;
            case -1: /* -方向に向かって伸びているとき */
                // 線上最近点をstartPosに設定する
                thisToTargetNewrPos = targetCapsuleStartPos;
                break;
            default:
                /* 謎えらー */
                break;
            }

            // 点Pから線上最近点までのベクトルを設定する
            nowThisToTargetNewrVec = thisToTargetNewrPos - thisCapsulePos;
        }

        // 二つの点pから線上最近点までのベクトル同士を比べ、現在のベクトルの方が小さかった場合、もしくはiが0の場合はtrue
        if (nowThisToTargetNewrVec.MagnitudeNoSqrt() < minThisToTargetNewrVec.MagnitudeNoSqrt() ||
            i == 0)
        {
            // 計算用のスフィアの位置を再設定する
            sphere3DPos = thisCapsulePos;

            // カプセルとスフィアの当たり判定の計算で使うカプセルを再設定する
            if (i <= 1)
            {
                targetCapsule = *capsule3Db;

                // 計算用のスフィアの半径を再設定する
                radius = capsule3Da->GetBaseRadius();

            }
            else /* 2 <= i */
            {
                targetCapsule = *capsule3Da;

                // 計算用のスフィアの半径を再設定する
                radius = capsule3Db->GetBaseRadius();
            }

            // 計算用のスフィアから見た、カプセルの線上最近点の座標を記憶しておく
            sphereToCapsuleNewrPointPos = thisToTargetNewrPos;
        }
    }

    // 計算用のスフィアを作る * 2
    Sphere3D sphere3Da;
    sphere3Da.SetBasePos(sphere3DPos);
    sphere3Da.SetBaseRadiusSqrt(radius);
    Sphere3D sphere3Db;
    sphere3Db.SetBasePos(sphereToCapsuleNewrPointPos);
    sphere3Db.SetBaseRadiusSqrt(targetCapsule.GetBaseRadius());

    // スフィアとスフィアの計算をし、結果をtempに代入する
    temp = BaseCollision3DManager::CheckHitSphere3DToSphere3D(&sphere3Da, &sphere3Db);
    if (0 <= temp.mnResultParam)
    {
        temp = GetCollisionCheckResult3D(
            capsule3Da,
            capsule3Db,
            0,
            capsule3Da->GetBasePos() - capsule3Db->GetBasePos(),
            capsule3Db->GetBasePos() - capsule3Da->GetBasePos(),
            sphere3DPos,
            sphereToCapsuleNewrPointPos);
    }

    // 計算結果を返す
    return temp;
}

// カプセルと三角形の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitCapsule3DToTriangle3D(Capsule3D *capsule3D, Triangle3D *triangle3D)
{
    // 返り値
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();

    // 関数の返り値を保持する変数
    VECTOR3D closestSeg, closestTri;

    // 作業用変数
    VECTOR3D startPos = capsule3D->GetMovedPos(capsule3D->GetStartPos());
    VECTOR3D endPos = capsule3D->GetMovedPos(capsule3D->GetEndPos());
    VECTOR3D vertex0 = triangle3D->GetMovedPos(triangle3D->GetVertexPos(0));
    VECTOR3D vertex1 = triangle3D->GetMovedPos(triangle3D->GetVertexPos(1));
    VECTOR3D vertex2 = triangle3D->GetMovedPos(triangle3D->GetVertexPos(2));
    float radius = capsule3D->GetCenterRadiusSqrt();

    // カプセルの軸と三角形の最短距離を取得する
    float dist = VECTOR3D::GetSegmentToTriangleNewrPos(startPos, endPos, vertex0, vertex1, vertex2, closestSeg, closestTri);

    // 距離がカプセルの半径以下だったら当たってる
    if (dist <= radius)
    {
        // カプセルから三角形への方向
        VECTOR3D cap2tri = (closestSeg - closestTri).Normalize();

        // 返り値を設定する
        temp = GetCollisionCheckResult3D(
            capsule3D,
            triangle3D,
            0,
            triangle3D->GetNormVec(),
            cap2tri,
            closestTri, // 三角形上の衝突点
            VECTOR3D(radius - dist, 0.0f, 0.0f));// x == kanntuuryou
    }

    // 計算結果を返す
    return temp;
}

// カプセルと四角形の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitCapsule3DToQuadrangle3D(Capsule3D *capsule3D, Quadrangle3D *quadrangle3D)
{
    // 返り値
    CollisionCheckResult3D returnValue = GetCollisionCheckResult3DZero();

    // 関数の返り値を保持する変数
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();

    // 作業用の三角形
    Triangle3D quadTriangle;
    
    // 三角形の値を設定する
    quadrangle3D->GetTriangle3D(&quadTriangle, 0);

    // 三角形その1とカプセルの当たり判定の計算を行う
    temp = BaseCollision3DManager::CheckHitCapsule3DToTriangle3D(capsule3D, &quadTriangle);
    if (0 <= temp.mnResultParam)
    {
        // 返り値を設定する
        returnValue = GetCollisionCheckResult3D(
            capsule3D,
            quadrangle3D,
            0,
            temp.mvRepulsionVecA,
            temp.mvRepulsionVecB,
            temp.mvHitPos,
            temp.mvFloatBox);

        // 結果を返す
        return returnValue;
    }

    // 三角形の値を再度設定する
    quadrangle3D->GetTriangle3D(&quadTriangle, 1);

    // 三角形その2とカプセルの当たり判定の計算を行う
    temp = BaseCollision3DManager::CheckHitCapsule3DToTriangle3D(capsule3D, &quadTriangle);
    if (0 <= temp.mnResultParam)
    {
        // 返り値を設定する
        returnValue = GetCollisionCheckResult3D(
            capsule3D,
            quadrangle3D,
            1,
            temp.mvRepulsionVecA,
            temp.mvRepulsionVecB,
            temp.mvHitPos,
            temp.mvFloatBox);
    }

    // 結果を返す
    return returnValue;
}

// カプセルとキューブの衝突確認を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitCapsule3DToCube3D(Capsule3D *capsule3D, Cube3D *cube3D)
{
    // 返り値
    CollisionCheckResult3D returnValue = GetCollisionCheckResult3DZero();

    if (capsule3D->GetMoveVec() != VECTOR3D::GetZero())
    {
        capsule3D = capsule3D;
    }

    // 関数の返り値を保持する変数
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();
    int result = -1;

    // 作業用の四角形を取得する
    Quadrangle3D cubeFaceQuad;
    VECTOR3D capsuleBase = capsule3D->GetMovedPos(capsule3D->GetBasePos());
    CubeFaceNumber currentFace;
    bool capsuleIn = false;

    // カプセルの軸の始点とキューブの内外判定を行う
    VECTOR3D resultVec;

    // キューブの向きで処理を変える
    if (cube3D->CheckCollisionNormIn())
    {
        // 移動しない状態で内側に入っているかを確認する
        if (cube3D->CheckInPoint(capsule3D->GetStartPos(), VECTOR3D::GetZero(), resultVec))
        {
            // カプセルの始点がキューブの中に入っていたことを記憶しておく
            capsuleIn = true;
        }
    }
    else
    {
        // 移動する状態で内側に入っているかを確認する
        if (cube3D->CheckInPoint(capsule3D->GetStartPos(), capsule3D->GetMoveVec(), resultVec))
        {
            // 返り値を設定する
            returnValue = GetCollisionCheckResult3D(
                capsule3D,
                cube3D,
                0,
                resultVec,
                temp.mvRepulsionVecB,
                temp.mvHitPos,
                temp.mvFloatBox);

            // 結果を返す
            return returnValue;
        }
    }

    // 外向きキューブか、内向きだが始点がキューブ内にあることが確認できなかった場合は処理を行う
    if (cube3D->CheckCollisionNormIn() == false || capsuleIn == false)
    {
        // キューブの向きで処理を変える
        if (cube3D->CheckCollisionNormIn())
        {
            // カプセルの軸の終点とキューブの内外判定を行う
            if (cube3D->CheckInPoint(capsule3D->GetEndPos(), VECTOR3D::GetZero(), resultVec))
            {
                // カプセルの終点がキューブの中に入っていたことを記憶しておく
                capsuleIn = true;
            }
        }
        else
        {
            // カプセルの軸の終点とキューブの内外判定を行う
            if (cube3D->CheckInPoint(capsule3D->GetEndPos(), capsule3D->GetMoveVec(), resultVec))
            {
                // 返り値を設定する
                returnValue = GetCollisionCheckResult3D(
                    capsule3D,
                    cube3D,
                    1,
                    resultVec,
                    temp.mvRepulsionVecB,
                    temp.mvHitPos,
                    temp.mvFloatBox);

                // 結果を返す
                return returnValue;
            }
        }
    }

    // 内向きキューブだが、カプセルが中に入っていることが確認できなかったため計算を終える
    if (cube3D->CheckCollisionNormIn() == true &&
        cube3D->CheckCollisionNormOut() == false &&
        capsuleIn == false)
    {
        return returnValue;
    }

    // 作業用変数
    VECTOR3D base2hitMin;
    VECTOR3D base2hitNow;
    VECTOR3D cubeBasePos = cube3D->GetBasePos();
    VECTOR3D nowNorm;
    float minHitDistance = MyFunctions::GetMaxFloat();

    // 各面とカプセルの当たり判定の計算を行う
    for (int i = 0; i < (unsigned char)(CubeFaceNumber::CubeFace_Max); i++)
    {
        // 着目するキューブの面の番号を取得する
        currentFace = Cube3D::Int2CFN(i);

        // キューブの面を取得し、処理を行わない面であれば処理を飛ばす
        result = cube3D->GetFaceQuad(&cubeFaceQuad, currentFace, cube3D->GetCollisionNorm());
        if (result != 0)
        {
            continue;
        }

        // 今回の四角形の法線ベクトルを取得する
        nowNorm = cubeFaceQuad.GetNormVec();

        // 特例としてカプセルが動いている場合、追加でカリング処理を行う
        if (capsule3D->GetMoveVec() != VECTOR3D::GetZero())
        {
            float dot = VECTOR3D::Dot(nowNorm, capsule3D->GetMoveVec());
            if (0.0f <= dot)
            {
                continue;
            }
            else
            {
                dot = 0.0f;
            }
        }

        // 当たり判定の計算を行う
        temp = BaseCollision3DManager::CheckHitCapsule3DToQuadrangle3D(capsule3D, &cubeFaceQuad);

        // 当たっていたら処理を行う
        if (0 <= temp.mnResultParam)
        {
            // まだ一回も当たっていないかそうでないかで処理を変える
            if (0 <= returnValue.mnResultParam)
            {
                // 特例として移動するカプセルが動かない内向きキューブに二回以上当たっている場合は処理を変える
                if (capsule3D->GetMoveVec() != VECTOR3D::GetZero() &&
                    cube3D->GetHitMove() == false &&
                    cube3D->CheckCollisionNormIn())
                {
                    // カプセルが使用するVECTORを0にして、返り値を設定する
                    returnValue = GetCollisionCheckResult3D(
                        capsule3D,
                        cube3D,
                        (unsigned char)(CubeFaceNumber::CubeFace_Max)+ 2,
                        VECTOR3D::GetZero(),
                        temp.mvRepulsionVecB,
                        temp.mvHitPos,
                        temp.mvFloatBox);

                    // ループ処理を終える
                    break;
                }

                // 当たった座標などなど
                base2hitNow = temp.mvHitPos - cubeBasePos;
                if (base2hitMin.MagnitudeNoSqrt() <= base2hitNow.MagnitudeNoSqrt())
                {
                    continue;
                }
            }

            // 初めて当たったので無条件で返り値を設定する
            returnValue = GetCollisionCheckResult3D(
                capsule3D,
                cube3D,
                i + 2,
                nowNorm,
                temp.mvRepulsionVecB,
                temp.mvHitPos,
                temp.mvFloatBox);

            // 値の設定をする
            base2hitMin = temp.mvHitPos - cubeBasePos;
        }
    }

    // 計算結果を返す
    return returnValue;
}

// カプセルとレイの衝突確認を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitCapsule3DToRay3D(Capsule3D *capsule3D, Ray3D *ray3D)
{
    // 返り値
    CollisionCheckResult3D returnValue = GetCollisionCheckResult3DZero();
    CollisionCheckResult3D temp1 = GetCollisionCheckResult3DZero();
    CollisionCheckResult3D temp2 = GetCollisionCheckResult3DZero();

    VECTOR3D rayStart = ray3D->GetMovedPos(ray3D->GetStartPos());
    VECTOR3D rayVec = ray3D->GetLineVec();
    VECTOR3D capsuleStart = capsule3D->GetMovedPos(capsule3D->GetStartPos());
    VECTOR3D capsuleEnd = capsule3D->GetMovedPos(capsule3D->GetEndPos());
    bool Q1inP1 = false;
    bool Q1inP2 = false;
    bool Q1inCld = false;

    // 作業用変数
    Sphere3D capsuleStartSphere;
    capsuleStartSphere.SetShapeParameter(capsule3D->GetStartPos(), capsule3D->GetBaseRadiusNoSqrt());
    capsuleStartSphere.SetMoveVec(capsule3D->GetMoveVec());
    Sphere3D capsuleEndSphere;
    capsuleEndSphere.SetShapeParameter(capsule3D->GetEndPos(), capsule3D->GetCenterRadiusSqrt());
    capsuleEndSphere.SetMoveVec(capsule3D->GetMoveVec());

    // Q1の検査
    do
    {
        // スタートスフィア
        temp1 = BaseCollision3DManager::CheckHitSphere3DToRay3D(&capsuleStartSphere, ray3D);
        if (0 <= temp1.mnResultParam &&
            VECTOR3D::Dot(capsule3D->GetLineVec(), (temp1.mvHitPos - capsule3D->GetMovedPos(capsule3D->GetStartPos()))) <= 0.0f)
        {
            Q1inP1 = true; // Q1は球面P1上にある
            break;
        }

        // エンドスフィア
        temp1 = BaseCollision3DManager::CheckHitSphere3DToRay3D(&capsuleEndSphere, ray3D);
        if (0 <= temp1.mnResultParam &&
            VECTOR3D::Dot(capsule3D->GetLineVec(), (temp1.mvHitPos - capsule3D->GetMovedPos(capsule3D->GetStartPos()))) <= 0.0f)
        {
            Q1inP2 = true; // Q1は球面P2上にある
            break;
        }

        // 無限円柱
        temp1 = calcRayInfCilinder(rayStart, rayVec, capsuleStart, capsuleEnd, capsule3D->GetBaseRadius());
        if (0 <= temp1.mnResultParam &&
            0.0f < checkDot(capsuleStart.GetX(), capsuleStart.GetY(), capsuleStart.GetZ(), capsuleEnd.GetX(), capsuleEnd.GetY(), capsuleEnd.GetZ(), temp1.mvHitPos.GetX(), temp1.mvHitPos.GetY(), temp1.mvHitPos.GetZ()) &&
            0.0f < checkDot(capsuleEnd.GetX(), capsuleEnd.GetY(), capsuleEnd.GetZ(), capsuleStart.GetX(), capsuleStart.GetY(), capsuleStart.GetZ(), temp1.mvHitPos.GetX(), temp1.mvHitPos.GetY(), temp1.mvHitPos.GetZ()))
        {
            Q1inCld = true; // Q1は円柱面にある
            break;
        }

        // レイは衝突していない
        return returnValue;
    } while (false);

    // Q2の検査
    do
    {
        if (Q1inP1 && checkDot(
            capsuleEnd.GetX(), capsuleEnd.GetY(), capsuleEnd.GetZ(),
            capsuleStart.GetX(), capsuleStart.GetY(), capsuleStart.GetZ(),
            temp1.mvFloatBox.GetX(), temp1.mvFloatBox.GetY(), temp1.mvFloatBox.GetZ()) <= 0.0f)
        {
            // Q1、Q2共球P1上にある
            returnValue = GetCollisionCheckResult3D(
                capsule3D,
                ray3D,
                1,
                capsuleStart - temp1.mvHitPos,
                temp1.mvHitPos - capsuleStart,
                temp1.mvHitPos,
                temp1.mvFloatBox);
            break;
        }
        if (Q1inP2 && checkDot(
            capsuleStart.GetX(), capsuleStart.GetY(), capsuleStart.GetZ(),
            capsuleEnd.GetX(), capsuleEnd.GetY(), capsuleEnd.GetZ(),
            temp1.mvFloatBox.GetX(), temp1.mvFloatBox.GetY(), temp1.mvFloatBox.GetZ()) <= 0.0f)
        {
            // Q1、Q2共球P2上にある
            returnValue = GetCollisionCheckResult3D(
                capsule3D,
                ray3D,
                2,
                capsuleStart - temp1.mvHitPos,
                temp1.mvHitPos - capsuleStart,
                temp1.mvHitPos,
                temp1.mvFloatBox);
            break;
        }
        if (Q1inCld &&
            0.0f < checkDot(
                capsuleStart.GetX(), capsuleStart.GetY(), capsuleStart.GetZ(),
                capsuleEnd.GetX(), capsuleEnd.GetY(), capsuleEnd.GetZ(),
                temp1.mvFloatBox.GetX(), temp1.mvFloatBox.GetY(), temp1.mvFloatBox.GetZ()) &&
            0.0f < checkDot(
                capsuleEnd.GetX(), capsuleEnd.GetY(), capsuleEnd.GetZ(),
                capsuleStart.GetX(), capsuleStart.GetY(), capsuleStart.GetZ(),
                temp1.mvFloatBox.GetX(), temp1.mvFloatBox.GetY(), temp1.mvFloatBox.GetZ()))
        {
            // Q1、Q2共球円柱面にある
            returnValue = GetCollisionCheckResult3D(
                capsule3D,
                ray3D,
                3,
                capsuleStart - temp1.mvHitPos,
                temp1.mvHitPos - capsuleStart,
                temp1.mvHitPos,
                temp1.mvFloatBox);
            break;
        }

        // Q2を探すために再度スタートスフィア
        temp2 = BaseCollision3DManager::CheckHitSphere3DToRay3D(&capsuleStartSphere, ray3D);
        if (0 < temp2.mnResultParam &&
            checkDot(capsuleEnd.GetX(), capsuleEnd.GetY(), capsuleEnd.GetZ(),
                capsuleStart.GetX(), capsuleStart.GetY(), capsuleStart.GetZ(),
                temp2.mvFloatBox.GetX(), temp2.mvFloatBox.GetY(), temp2.mvFloatBox.GetZ()) <= 0.0f)
        {
            // Q2は球P1上にある
            returnValue = GetCollisionCheckResult3D(
                capsule3D,
                ray3D,
                4,
                capsuleStart - temp1.mvHitPos,
                temp1.mvHitPos - capsuleStart,
                temp1.mvHitPos,
                temp2.mvFloatBox);
            break;
        }

        // Q2を探すために再度エンドスフィア
        temp2 = BaseCollision3DManager::CheckHitSphere3DToRay3D(&capsuleEndSphere, ray3D);
        if (0 < temp2.mnResultParam == true &&
            checkDot(capsuleStart.GetX(), capsuleStart.GetY(), capsuleStart.GetZ(),
                capsuleEnd.GetX(), capsuleEnd.GetY(), capsuleEnd.GetZ(),
                temp2.mvFloatBox.GetX(), temp2.mvFloatBox.GetY(), temp2.mvFloatBox.GetZ()) <= 0.0f)
        {
            // Q2は球P2上にある
            returnValue = GetCollisionCheckResult3D(
                capsule3D,
                ray3D,
                5,
                capsuleStart - temp1.mvHitPos,
                temp1.mvHitPos - capsuleStart,
                temp1.mvHitPos,
                temp2.mvFloatBox);
            break;
        }

        // Q2が円柱上にある事が確定
        temp2 = calcRayInfCilinder(rayStart, rayVec, capsuleStart, capsuleEnd, capsule3D->GetBaseRadiusNoSqrt());
        returnValue = GetCollisionCheckResult3D(
            capsule3D,
            ray3D,
            6,
            capsuleStart - temp1.mvHitPos,
            temp1.mvHitPos - capsuleStart,
            temp1.mvHitPos,
            temp2.mvFloatBox);
    } while (false);

    // 計算結果を返す
    return returnValue;
}

// 三角形と三角形の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitTriangle3DToTriangle3D(Triangle3D *triangle3Da, Triangle3D *triangle3Db)
{
    CollisionCheckResult3D returnValue = GetCollisionCheckResult3DZero();
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();

    Ray3D aTriangleRay;
    aTriangleRay.SetMoveVec(triangle3Da->GetMoveVec());
    for (int i = 0; i < 3; i++)
    {
        int j = i + 1;
        if (3 <= j)
        {
            j -= 3;
        }

        // 三角形の辺の一つをRayに変換する
        aTriangleRay.SetShapeParameter(triangle3Da->GetVertexPos(i), triangle3Da->GetVertexPos(j));

        // 辺の一つでももう片方の三角形に当たっていたらtrueを返す
        temp = BaseCollision3DManager::CheckHitTriangle3DToRay3D(triangle3Db, &aTriangleRay);
        if (0 <= temp.mnResultParam)
        {
            if (returnValue.mnResultParam == -1)
            {
                returnValue = GetCollisionCheckResult3D(
                    triangle3Da,
                    triangle3Db,
                    (temp.mnResultParam * 3) + i + 1,
                    temp.mvRepulsionVecB,
                    temp.mvRepulsionVecA,
                    temp.mvHitPos,
                    temp.mvFloatBox);
            }
            else
            {
                // これまでで一番近かった衝突地点と今回の衝突地点と、どちらがより近かったかを比較する
                VECTOR3D minVec = returnValue.mvHitPos - aTriangleRay.GetStartPos();
                VECTOR3D tempVec = temp.mvHitPos - aTriangleRay.GetStartPos();
                if (tempVec.MagnitudeNoSqrt() < minVec.MagnitudeNoSqrt())
                {
                    returnValue = GetCollisionCheckResult3D(
                        triangle3Da,
                        triangle3Db,
                        (temp.mnResultParam * 3) + i + 1,
                        temp.mvRepulsionVecB,
                        temp.mvRepulsionVecA,
                        temp.mvHitPos,
                        temp.mvFloatBox);
                }
            }
        }
    }

    // aの三角形がダメだったらbの三角形の計算も行う
    if (returnValue.mnResultParam == -1)
    {
        Ray3D bTriangleRay;
        bTriangleRay.SetMoveVec(triangle3Db->GetMoveVec());
        for (int i = 0; i < 3; i++)
        {
            int j = i + 1;
            if (3 <= j)
            {
                j -= 3;
            }

            // 三角形の辺の一つをRayに変換する
            bTriangleRay.SetShapeParameter(triangle3Db->GetVertexPos(i), triangle3Db->GetVertexPos(j));

            // 辺の一つでももう片方の三角形に当たっていたらtrueを返す
            temp = BaseCollision3DManager::CheckHitTriangle3DToRay3D(triangle3Da, &bTriangleRay);
            if (0 <= temp.mnResultParam)
            {
                if (returnValue.mnResultParam == -1)
                {
                    returnValue = GetCollisionCheckResult3D(
                        triangle3Da,
                        triangle3Db,
                        (temp.mnResultParam * 3) + i + 1,
                        temp.mvRepulsionVecA,
                        temp.mvRepulsionVecB,
                        temp.mvHitPos,
                        temp.mvFloatBox);
                }
                else
                {
                    // これまでで一番近かった衝突地点と今回の衝突地点と、どちらがより近かったかを比較する
                    VECTOR3D minVec = returnValue.mvHitPos - bTriangleRay.GetStartPos();
                    VECTOR3D tempVec = temp.mvHitPos - bTriangleRay.GetStartPos();
                    if (tempVec.MagnitudeNoSqrt() < minVec.MagnitudeNoSqrt())
                    {
                        returnValue = GetCollisionCheckResult3D(
                            triangle3Da,
                            triangle3Db,
                            (temp.mnResultParam * 3) + i + 1,
                            temp.mvRepulsionVecA,
                            temp.mvRepulsionVecB,
                            temp.mvHitPos,
                            temp.mvFloatBox);
                    }
                }
            }
        }
    }

    // 計算結果を返す
    return returnValue;
}

// 三角形と四角形の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitTriangle3DToQuadrangle3D(Triangle3D *triangle3D, Quadrangle3D *quadrangle3D)
{
    CollisionCheckResult3D returnValue = GetCollisionCheckResult3DZero();
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();

    // 作業用変数の定義と設定
    Triangle3D quadTriangle;
    quadTriangle.SetShapeParameter(quadrangle3D->GetVertexPos(0), quadrangle3D->GetVertexPos(1), quadrangle3D->GetVertexPos(2));
    quadTriangle.SetMoveVec(quadrangle3D->GetMoveVec());

    // 計算結果を取得
    temp = BaseCollision3DManager::CheckHitCollision3DToCollision3D(&quadTriangle, triangle3D);
    if (temp.mnResultParam == -1)
    {
        // 作業用変数を再設定
        quadTriangle.SetShapeParameter(quadrangle3D->GetVertexPos(3), quadrangle3D->GetVertexPos(2), quadrangle3D->GetVertexPos(1));

        // 計算結果を取得
        temp = BaseCollision3DManager::CheckHitCollision3DToCollision3D(&quadTriangle, triangle3D);
    }

    // 返り値を設定する
    if (0 <= temp.mnResultParam)
    {
        returnValue = GetCollisionCheckResult3D(
            triangle3D,
            quadrangle3D,
            0,
            temp.mvRepulsionVecA,
            temp.mvRepulsionVecB,
            temp.mvHitPos,
            temp.mvFloatBox);
    }

    // 計算結果を返す
    return returnValue;
}

// 三角形とキューブの衝突確認を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitTriangle3DToCube3D(Triangle3D *triangle3D, Cube3D *cube3D)
{
    return GetCollisionCheckResult3DZero();
}

// 三角形とレイの衝突確認を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitTriangle3DToRay3D(Triangle3D *triangle3D, Ray3D *ray3D)
{
    // 返り値
    CollisionCheckResult3D returnValue = GetCollisionCheckResult3DZero();

    // 作業用変数
    VECTOR3D startPos = ray3D->GetMovedPos(ray3D->GetStartPos());
    VECTOR3D vertex0 = triangle3D->GetMovedPos(triangle3D->GetVertexPos(0));
    VECTOR3D dir = ray3D->GetLineVec();
    VECTOR3D ver02ver1 = triangle3D->GetMovedPos(triangle3D->GetVertexPos(1)) - vertex0;
    VECTOR3D ver02ver2 = triangle3D->GetMovedPos(triangle3D->GetVertexPos(2)) - vertex0;
    const float EPSILON = 1e-6f;

    // 線分の方向と三角形の法線の関係を計算し、平行の場合は衝突しないので処理を切り上げる
    VECTOR3D h = VECTOR3D::Cross(dir, ver02ver2);
    float a = VECTOR3D::Dot(ver02ver1, h);
    if (fabs(a) < EPSILON)
    {
        // 平行なので当たらない
        return returnValue;
    }

    // バリセントリック座標(u, v)を取得しながら、点の内外判定を行っている
    float f = 1.0f / a;
    VECTOR3D start2vertex0 = startPos - vertex0;
    float u = f * VECTOR3D::Dot(start2vertex0, h);
    if (u < 0.0f || 1.0f < u)
    {
        // u座標が三角形の外なので当たらない
        return returnValue;
    }

    // 補助ベクトルqを取得する
    VECTOR3D q = VECTOR3D::Cross(start2vertex0, ver02ver1);

    // v座標を取得し、改めて交点が三角形の内部にあるかを確認する
    float v = f * VECTOR3D::Dot(dir, q);
    if (v < 0.0f || 1.0f < u + v)
    {
        // 交点が三角形の外なので当たらない
        return returnValue;
    }

    // ここまでで計算したのは半直線(start地点から伸びる無限直線)と三角形の交点
    // よってこれからその交点が線分の範囲内かを確認する
    float t = f * VECTOR3D::Dot(ver02ver2, q);
    if (t < 0.0f || 1.0f < t)
    {
        // 交点が線分の範囲外なので当たっていない
        return returnValue;
    }

    // 衝突地点を調べ、返り値を設定する
    VECTOR3D hitPos = startPos + dir * t;
    returnValue = GetCollisionCheckResult3D(
        triangle3D, 
        ray3D, 
        1,
        dir,
        triangle3D->GetNormVec(),
        hitPos,
        VECTOR3D(u, v, t));

    // 計算結果を返す
    return returnValue;

    /*
    Dotを使った方法
    float dotStart = VECTOR3D::Dot(startPos - vertex0, triangleNorm);
    float dotEnd = VECTOR3D::Dot(endPos - vertex0, triangleNorm);
    if (dotStart * dotEnd > 0.0f)
    {
        return returnValue;
    }
    if (fabsf(dotStart - dotEnd) < 1e-6f)
    {
        return returnValue;
    }

    // 内分比を取得する
    float t = dotStart / (dotStart - dotEnd);

    // 交点を取得する
    VECTOR3D hitPos = startPos + (ray3D->GetLineVec() * t);

    // 衝突地点と三角形の当たり判定を行う
    Point3D hitPoint;
    hitPoint.SetBasePos(hitPos);
    CollisionCheckResult3D temp = BaseCollision3DManager::CheckHitPoint3DToTriangle3D(&hitPoint, triangle3D);

    // 当たっていた場合は返り値を変更する
    if (0 <= temp.mnResultParam)
    {
        returnValue = GetCollisionCheckResult3D(
            triangle3D,
            ray3D,
            0,
            ray3D->GetLineVec(),
            triangleNorm,
            hitPos + ray3D->GetMoveVec(),
            VECTOR3D(t, 1.0f - t, 0.0f));
    }
    */
}

// 四角形と四角形の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitQuadrangle3DToQuadrangle3D(Quadrangle3D *quadrangle3Da, Quadrangle3D *quadrangle3Db)
{
    // 返り値
    CollisionCheckResult3D returnValue = GetCollisionCheckResult3DZero();

    // 途中の返り値を取得する
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();

    // 作業用変数その1
    Triangle3D aQuadTriangle;
    aQuadTriangle.SetShapeParameter(quadrangle3Da->GetVertexPos(0), quadrangle3Da->GetVertexPos(1), quadrangle3Da->GetVertexPos(2));
    aQuadTriangle.SetMoveVec(quadrangle3Da->GetMoveVec());

    // 片方の三角形と四角形の当たり判定の計算を行う
    temp = BaseCollision3DManager::CheckHitCollision3DToCollision3D(&aQuadTriangle, quadrangle3Db);
    if (temp.mnResultParam == -1)
    {
        // もう片方の三角形と四角形の当たり判定の計算を行う
        aQuadTriangle.SetShapeParameter(quadrangle3Da->GetVertexPos(3), quadrangle3Da->GetVertexPos(2), quadrangle3Da->GetVertexPos(1));
        temp = BaseCollision3DManager::CheckHitCollision3DToCollision3D(&aQuadTriangle, quadrangle3Db);
        if (temp.mnResultParam == -1)
        {
            // 作業用変数その2
            Triangle3D bQuadTriangle;
            bQuadTriangle.SetShapeParameter(quadrangle3Db->GetVertexPos(0), quadrangle3Db->GetVertexPos(1), quadrangle3Db->GetVertexPos(2));
            bQuadTriangle.SetMoveVec(quadrangle3Db->GetMoveVec());
            temp = BaseCollision3DManager::CheckHitCollision3DToCollision3D(&bQuadTriangle, quadrangle3Da);
            if (temp.mnResultParam == -1)
            {
                bQuadTriangle.SetShapeParameter(quadrangle3Db->GetVertexPos(3), quadrangle3Db->GetVertexPos(2), quadrangle3Db->GetVertexPos(1));
                temp = BaseCollision3DManager::CheckHitCollision3DToCollision3D(&bQuadTriangle, quadrangle3Da);
            }
        }
    }

    // 返り値の設定を行う
    if (0 <= temp.mnResultParam)
    {
        returnValue = GetCollisionCheckResult3D(
            quadrangle3Da,
            quadrangle3Db,
            0,
            temp.mvRepulsionVecA,
            temp.mvRepulsionVecB,
            temp.mvHitPos,
            temp.mvFloatBox);
    }

    return returnValue;
}

// 四角形とキューブの衝突確認を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitQuadrangle3DToCube3D(Quadrangle3D *quadrangle3D, Cube3D *cube3D)
{
    return GetCollisionCheckResult3DZero();
}

// 四角形とレイの衝突確認を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitQuadrangle3DToRay3D(Quadrangle3D *quadrangle3D, Ray3D *ray3D)
{
    // 返り値
    CollisionCheckResult3D returnValue = GetCollisionCheckResult3DZero();

    // 関数の返り値を受け取る変数
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();

    // 作業用変数
    Triangle3D quadrangleTriangle;

    // 四角形を二つの三角形に分解し、まず片方の三角形に衝突していないかを確認する
    quadrangleTriangle.SetShapeParameter(
        quadrangle3D->GetVertexPos(0),
        quadrangle3D->GetVertexPos(1),
        quadrangle3D->GetVertexPos(2));
    quadrangleTriangle.SetMoveVec(quadrangle3D->GetMoveVec());
    temp = BaseCollision3DManager::CheckHitTriangle3DToRay3D(&quadrangleTriangle, ray3D);

    // 判定結果により処理を変更する
    if (0 <= temp.mnResultParam)
    {
        // 衝突していたので返り値を変更する
        returnValue = GetCollisionCheckResult3D(
            quadrangle3D,
            ray3D,
            0,
            ray3D->GetLineVec(),
            quadrangle3D->GetNormVec(),
            temp.mvHitPos + ray3D->GetMoveVec(),
            temp.mvFloatBox);
    }
    else
    {
        // もう片方の三角形に衝突していないか判定する
        quadrangleTriangle.SetShapeParameter(
            quadrangle3D->GetVertexPos(1),
            quadrangle3D->GetVertexPos(3),
            quadrangle3D->GetVertexPos(2));
        quadrangleTriangle.SetMoveVec(quadrangle3D->GetMoveVec());
        temp = BaseCollision3DManager::CheckHitTriangle3DToRay3D(&quadrangleTriangle, ray3D);

        // 衝突していた場合は返り値を変更する
        if (0 <= temp.mnResultParam)
        {
            returnValue = GetCollisionCheckResult3D(
                quadrangle3D,
                ray3D,
                0,
                ray3D->GetLineVec(),
                quadrangle3D->GetNormVec(),
                temp.mvHitPos + ray3D->GetMoveVec(),
                temp.mvFloatBox);
        }
    }

    // 計算結果を返す
    return returnValue;
}

// キューブとキューブの衝突確認を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitCube3DToCube3D(Cube3D *Cube3Da, Cube3D *cube3Db)
{
    return GetCollisionCheckResult3DZero();
}

// キューブとレイの衝突確認を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitCube3DToRay3D(Cube3D *cube3D, Ray3D *ray3D)
{
    // 作業用変数
    CollisionCheckResult3D returnValue = GetCollisionCheckResult3DZero();
    CollisionCheckResult3D min = GetCollisionCheckResult3DZero();
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();
    Quadrangle3D cubeQuad;
    VECTOR3D rayStartPos;
    VECTOR3D start2tempHit;
    VECTOR3D start2minHit;

    // 四角形の取得に成功したかを確認するための変数
    int result = 0;

    // 当たった回数。これが2以上になったら強制的に値をリセット
    unsigned char hitCount = 0;

    // Rayのスタート地点を取得しておく
    rayStartPos = ray3D->GetStartPos();

    // キューブの6面全てに当たり判定の計算を行うループ文章
    for (int i = 0; i < 6; i++)
    {
        // 今回判定を行う面の番号を取得する
        CubeFaceNumber currentFace = Cube3D::Int2CFN(i);

        // 四角形を取得し、取得に失敗していたら処理を飛ばす
        result = cube3D->GetFaceQuad(&cubeQuad, currentFace, cube3D->GetCollisionNorm());
        if (result != 0)
        {
            continue;
        }

        // 当たり判定の計算を行う
        temp = BaseCollision3DManager::CheckHitQuadrangle3DToRay3D(&cubeQuad, ray3D);
        if (0 <= temp.mnResultParam)
        {
            // 当たった回数を一つ増やす
            hitCount++;

            // 一回も当たっていない場合は処理を変える
            if (min.mnResultParam < 0)
            {
                // 値を設定する
                min = GetCollisionCheckResult3D(
                    cube3D,
                    ray3D,
                    i + 1,
                    min.mvRepulsionVecA,
                    min.mvRepulsionVecB,
                    min.mvHitPos,
                    min.mvFloatBox);

                // Rayのスタート地点から当たった座標までを取っておく
                start2minHit = min.mvHitPos - rayStartPos;
            }
            else
            {
                // Rayのスタート地点から今回当たった座標までを取っておく
                start2tempHit = temp.mvHitPos - rayStartPos;
                if (start2tempHit.MagnitudeNoSqrt() < start2minHit.MagnitudeNoSqrt())
                {
                    // 値を設定する
                    min = GetCollisionCheckResult3D(
                        cube3D,
                        ray3D,
                        i + 1,
                        min.mvRepulsionVecA,
                        min.mvRepulsionVecB,
                        min.mvHitPos,
                        min.mvFloatBox);

                    // Rayのスタート地点から当たった座標までを取っておく
                    start2minHit = start2tempHit;
                }
            }
        }

        // Rayがキューブの面に三回以上当たることはないので(多分)
        if (2 <= hitCount)
        {
            break;
        }
    }

    // 計算結果を返す
    return min;
}

// レイとレイの衝突確認を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitRay3DToRay3D(Ray3D *targetRay3D, Ray3D *ray3D)
{
    // 3D空間なのにRayとRayを計算しようとするな
    return GetCollisionCheckResult3DZero();
}

// 当たり判定の寿命に関する更新処理を行う関数
int BaseCollision3DManager::UpdateCollisionLife()
{
    // 今回のループで確認する当たり判定
    BaseCollision3D *current = this->mpFirstBaseCollision3D;

    // 次のループで確認する当たり判定
    BaseCollision3D *next = nullptr;

    // 作業用変数
    BaseCollision *listPrev = nullptr;
    BaseCollision *listNext = nullptr;
    BaseCollision3D *worldPrev = nullptr;
    BaseCollision3D *worldNext = nullptr;

    // ワールドに登録されている全ての当たり判定を確認するif文
    while (current != nullptr)
    {
        // 次のループで使用する当たり判定を取得しておく
        next = current->GetNextWorld();

        // 今回のループで使用する当たり判定の寿命の処理を進める
        current->UpdateLife();

        // 寿命が来たら処理をする
        if (current->GetDeleteFlag())
        {
            // リストの孤立処理をしておく
            listPrev = current->GetPrevList();
            listNext = current->GetNextList();
            worldPrev = current->GetPrevWorld();
            worldNext = current->GetNextWorld();
            if (listPrev != nullptr)
            {
                listPrev->SetNextList(listNext);
            }
            if (listNext != nullptr)
            {
                listNext->SetPrevList(listPrev);
            }
            if (worldPrev != nullptr)
            {
                worldPrev->SetNextWorld(worldNext);
            }
            if (worldNext != nullptr)
            {
                worldNext->SetPrevWorld(worldPrev);
            }
            current->SetPrevList(nullptr);
            current->SetNextList(nullptr);
            current->SetPrevWorld(nullptr);
            current->SetNextWorld(nullptr);

            // 当たり判定を削除する
            delete current;
        }

        // 次の当たり判定に移る
        current = next;
    }

    // 関数が正常終了したことを返す
    return 0;
}

BaseCollision3DManager::BaseCollision3DManager() :
    mpFirstBaseCollision3D(nullptr)
{

}

BaseCollision3DManager::~BaseCollision3DManager()
{
    this->IsolateAll();
}

int BaseCollision3DManager::Initialize()
{
    BaseCollision3D *current = mpFirstBaseCollision3D;
    while (current != nullptr)
    {
        current = current->GetNextWorld();
    }

    return 0;
}

int BaseCollision3DManager::Finalize()
{
    BaseCollision3D *current = mpFirstBaseCollision3D;
    while (current != nullptr)
    {
        current = current->GetNextWorld();
    }

    return 0;
}

// ワールドに登録されている当たり判定とワールドに登録されている当たり判定を移動込みの総当たりで計算し、スライド移動も行う関数(カリング処理 : 移動ベクトル、基準円)
int BaseCollision3DManager::CheckHitAllMove()
{
    // nullチェック
    if (this->mpFirstBaseCollision3D == nullptr ||
        this->mpFirstBaseCollision3D->GetNextWorld() == nullptr)
    {
        return 0;
    }

    // 関数の返り値を保持するための変数
    CollisionCheckResult3D temp;

    // 移動する当たり判定を取得する
    for (BaseCollision3D *current = this->mpFirstBaseCollision3D; current != nullptr; current = current->GetNextWorld())
    {
        // 後者のオブジェクトを取得する
        for (BaseCollision3D *target = current->GetNextWorld(); target != nullptr; target = target->GetNextWorld())
        {
            // 二つの当たり判定の削除フラグと有効フラグを確認し、問題があった場合は処理を飛ばす
            if (current->GetDeleteFlag() ||
                target->GetDeleteFlag() ||
                current->GetActiveFlag() == false || 
                target->GetActiveFlag() == false)
            {
                continue;
            }

            // 当たり判定の計算を行う
            temp = BaseCollision3DManager::CheckHitCollision3DToCollision3D(current, target);

            // 当たっている場合は処理を行う
            if (0 <= temp.mnResultParam)
            {
                // スライド移動の処理を行う
                BaseCollision3DManager::CollisionSlide(current, target, temp);

                // 二つの当たり判定に、衝突したことを知らせる
                current->HitOnCollision(target);
                target->HitOnCollision(current);
            }
        }
    }

    // 当たり判定の寿命に関する処理を行う
    this->UpdateCollisionLife();

    // 関数が正常終了したことを知らせる
    return 0;
}

// 謎の当たり判定と謎の当たり判定の内外判定と衝突判定を行う関数
CollisionCheckResult3D BaseCollision3DManager::CheckHitCollision3DToCollision3D(BaseCollision3D *collision3DA, BaseCollision3D *collision3DB)
{
    // 返り値を保持する変数
    CollisionCheckResult3D temp = GetCollisionCheckResult3DZero();

    // 片方でもnullptrだった場合は計算ができないので
    if (collision3DA == nullptr || collision3DB == nullptr)
    {
        return temp;
    }

    // カリング処理をまとめて行い、飛ばすべきでない場合のみ詳しい計算を行う
    if (BaseCollision3DManager::CheckCulling(collision3DA, collision3DB) == false)
    {
        // まず片方のコリジョンタイプにあった関数に飛ぶ
        switch (collision3DA->GetCollisionType())
        {
        case CollisionType::CollisionType_Point3D:
            temp = BaseCollision3DManager::CheckHitPoint3DToCollision3D(dynamic_cast<Point3D *>(collision3DA), collision3DB);
            break;
        case CollisionType::CollisionType_Sphere3D:
            temp = BaseCollision3DManager::CheckHitSphere3DToCollision3D(dynamic_cast<Sphere3D *>(collision3DA), collision3DB);
            break;
        case CollisionType::CollisionType_Capsule3D:
            temp = BaseCollision3DManager::CheckHitCapsule3DToCollision3D(dynamic_cast<Capsule3D *>(collision3DA), collision3DB);
            break;
        case CollisionType::CollisionType_Triangle3D:
            temp = BaseCollision3DManager::CheckHitTriangle3DToCollision3D(dynamic_cast<Triangle3D *>(collision3DA), collision3DB);
            break;
        case CollisionType::CollisionType_Quadrangle3D:
            temp = BaseCollision3DManager::CheckHitQuadrangle3DToCollision3D(dynamic_cast<Quadrangle3D *>(collision3DA), collision3DB);
            break;
        case CollisionType::CollisionType_Cube3D:
            temp = BaseCollision3DManager::CheckHitCube3DToCollision3D(dynamic_cast<Cube3D *>(collision3DA), collision3DB);
            break;
        case CollisionType::CollisionType_Ray3D:
            temp = BaseCollision3DManager::CheckHitRay3DToCollision3D(dynamic_cast<Ray3D *>(collision3DA), collision3DB);
            break;
        default:
            break;
        }
    }

    // 計算結果を返す
    return temp;
}

// 指定のタグをつけられた当たり判定を取得する関数
std::vector<BaseCollision3D *> BaseCollision3DManager::SearchCollisionToTag(CollisionTag tag) const
{
    std::vector<BaseCollision3D *> temp(0);
    unsigned long counter = 0;
    BaseCollision3D *current = this->mpFirstBaseCollision3D;
    while (current != nullptr)
    {
        if (current->GetCollisionTag() == tag)
        {
            counter++;
        }
        current = current->GetNextWorld();
    }
    temp.resize(counter);
    counter = 0;
    current = this->mpFirstBaseCollision3D;
    while (current != nullptr)
    {
        if (current->GetCollisionTag() == tag)
        {
            temp[counter] = current;
            counter++;
        }
        current = current->GetNextWorld();
    }
    return temp;
}

/* 線形リスト関連の関数 */
int BaseCollision3DManager::Add(BaseCollision3D *target)
{
    // そもそもtargetがnullの場合
    if (target == nullptr)
    {
        return -1;
    }
    // mpCollisionすらない場合
    if (mpFirstBaseCollision3D == nullptr)
    {
        mpFirstBaseCollision3D = target;
        return 0;
    }

    BaseCollision3D *current = mpFirstBaseCollision3D;
    // 最後尾を見つける
    while (current->GetNextWorld() != nullptr)
    {
        current = current->GetNextWorld();
    }

    return ConnectTarget(current, target, nullptr);
}

int BaseCollision3DManager::ConnectTarget(BaseCollision3D *prev, BaseCollision3D *target, BaseCollision3D *next)
{
    if (prev != nullptr)
    {
        prev->SetNextWorld(target);
    }
    if (next != nullptr)
    {
        next->SetPrevWorld(target);
    }
    if (next == mpFirstBaseCollision3D)
    {
        this->mpFirstBaseCollision3D = target;
    }

    target->SetPrevWorld(prev);
    target->SetNextWorld(next);

    return 0;
}

int BaseCollision3DManager::IsolateTarget(BaseCollision3D *target)
{
    if (mpFirstBaseCollision3D == nullptr)
    {
        return 0;
    }

    // 作業用変数を用意
    BaseCollision3D *prev = target->GetPrevWorld();
    BaseCollision3D *next = target->GetNextWorld();

    if (prev != nullptr)
    {
        // prevがnullでなかったら、nextがnullだろうとセットする。(targetが最後尾のコリジョンの場合など)
        prev->SetNextWorld(next);
    }
    if (next != nullptr)
    {
        // nextがnullでなかったら、prevがnullだろうとセットする。(target == mpCollisionの場合など)
        next->SetPrevWorld(prev);
    }

    if (target == mpFirstBaseCollision3D)
    {
        // 自分が最初のコリジョンだった場合は一つ後のコリジョンを最初のコリジョンとする
        mpFirstBaseCollision3D = next;
    }

    target->SetPrevWorld(nullptr);
    target->SetNextWorld(nullptr);
}

int BaseCollision3DManager::IsolateAll()
{
    if (this->mpFirstBaseCollision3D == nullptr)
    {
        return 0;
    }

    BaseCollision3D *target = mpFirstBaseCollision3D;
    BaseCollision3D *next = target->GetNextWorld();

    while (true)
    {
        IsolateTarget(target);
        target = next;
        if (target == nullptr)
        {
            break;
        }
        else
        {
            next = target->GetNextWorld();
        }
    }
}