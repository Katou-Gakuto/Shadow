#pragma once

#include "AABB.h"
#include "CollisionCheckResult.h"
#include "BaseCollision.h"

class BaseCollision2D;
class Point2D;
class Circle2D;
class Capsule2D;
class Triangle2D;
class Quadrangle2D;
class Ray2D;

// 2Dの当たり判定の計算を行うクラス
class BaseCollision2DManager
{
private:
    BaseCollision2D *mpFirstBaseCollision2D;
    static AABB2D aabbA;
    static AABB2D aabbB;

    // この二つの当たり判定が互いの処理対象なのかを確認する関数
    static bool CheckProcessing(BaseCollision2D *collision2DA, BaseCollision2D *collision2DB);

    // 移動ベクトルを使ったカリング処理を行う関数
    static bool CheckHitMoveVecToCollision2D(BaseCollision2D *collision2DA, BaseCollision2D *collision2DB);

    // 円を使ったカリング処理を行う関数
    static bool CheckHitBaseCollision2DToBaseCollision2D(BaseCollision2D *collision2DA, BaseCollision2D *collision2DB);

    // AABBを使ったカリング処理を行う関数
    static bool CheckHitAABBAABB(BaseCollision2D *collision1, BaseCollision2D *collision2);

    // カリング処理をまとめて行い、当たり判定の計算を飛ばすべきかを確認する関数
    static bool CheckCulling(BaseCollision2D *collision2DA, BaseCollision2D *collision2DB);

    // 二つの当たり判定のスライド移動を行う関数
    static int CollisionSlide(BaseCollision2D *collision2DA, BaseCollision2D *collision2DB, const CollisionCheckResult2D &hitResult);

    // 点と謎の当たり判定の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitPoint2DToCollision2D(Point2D *point2D, BaseCollision2D *collision);

    // 円と謎の当たり判定の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitCircle2DToCollision2D(Circle2D *circle2D, BaseCollision2D *collision);

    // カプセルと謎の当たり判定の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitCapsule2DToCollision2D(Capsule2D *capsule2D, BaseCollision2D *collision);

    // 三角形と謎の当たり判定の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitTriangle2DToCollision2D(Triangle2D *triangle2D, BaseCollision2D *collision);

    // 四角形と謎の当たり判定の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitQuadrangle2DToCollision2D(Quadrangle2D *quadrangle2D, BaseCollision2D *collision);

    // レイと謎の当たり判定の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitRay2DToCollision2D(Ray2D *ray2D, BaseCollision2D *collision);

    // 点と点の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitPoint2DToPoint2D(Point2D *point2Da, Point2D *point2Db);

    // 点と円の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitPoint2DToCircle2D(Point2D *point2D, Circle2D *circle2D);

    // 点とカプセルの衝突確認を行う関数
    static CollisionCheckResult2D CheckHitPoint2DToCapsule2D(Point2D *point2D, Capsule2D *capsule2D);

    // 点と三角形の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitPoint2DToTriangle2D(Point2D *point2D, Triangle2D *triangle2D);

    // 点と四角形の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitPoint2DToQuadrangle2D(Point2D *point2D, Quadrangle2D *quadrangle2D);

    // 点とレイの衝突確認を行う関数
    static CollisionCheckResult2D CheckHitPoint2DToRay2D(Point2D *point2D, Ray2D *ray2D);

    // 円と円の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitCircle2DToCircle2D(Circle2D *circle2Da, Circle2D *circle2Db);

    // 円とカプセルの衝突確認を行う関数
    static CollisionCheckResult2D CheckHitCircle2DToCapsule2D(Circle2D *circle2D, Capsule2D *capsule2D);

    // 円と三角形の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitCircle2DToTriangle2D(Circle2D *circle2Da, Triangle2D *triangle2D);

    // 円と四角形の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitCircle2DToQuadrangle2D(Circle2D *circle2Da, Quadrangle2D *quadrangle2D);

    // 円とレイの衝突確認を行う関数
    static CollisionCheckResult2D CheckHitCircle2DToRay2D(Circle2D *circle2D, Ray2D *ray2D);

    // カプセルとカプセルの衝突確認を行う関数
    static CollisionCheckResult2D CheckHitCapsule2DToCapsule2D(Capsule2D *capsule2Da, Capsule2D *capsule2Db);

    // カプセルと三角形の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitCapsule2DToTriangle2D(Capsule2D *capsule2D, Triangle2D *triangle2D);

    // カプセルと四角形の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitCapsule2DToQuadrangle2D(Capsule2D *capsule2D, Quadrangle2D *quadrangle2D);

    // カプセルとレイの衝突確認を行う関数
    static CollisionCheckResult2D CheckHitCapsule2DToRay2D(Capsule2D *capsule2D, Ray2D *ray2D);

    // 三角形と三角形の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitTriangle2DToTriangle2D(Triangle2D *triangle2Da, Triangle2D *triangle2Db);

    // 三角形と四角形の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitTriangle2DToQuadrangle2D(Triangle2D *triangle2D, Quadrangle2D *quadrangle2D);

    // 三角形とレイの衝突確認を行う関数
    static CollisionCheckResult2D CheckHitTriangle2DToRay2D(Triangle2D *triangle2D, Ray2D *ray2D);

    // 四角形と四角形の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitQuadrangle2DToQuadrangle2D(Quadrangle2D *quadrangle2Da, Quadrangle2D *quadrangle2Db);

    // 四角形とレイの衝突確認を行う関数
    static CollisionCheckResult2D CheckHitQuadrangle2DToRay2D(Quadrangle2D *quadrangle2D, Ray2D *ray2D);

    // レイとレイの衝突確認を行う関数
    static CollisionCheckResult2D CheckHitRay2DToRay2D(Ray2D *ray2Da, Ray2D *ray2Db);

    // 当たり判定の寿命に関する更新処理を行う関数
    int UpdateCollisionLife();

public:
    BaseCollision2DManager();
    ~BaseCollision2DManager();

    int Initialize();
    int Finalize();

    // ワールドに登録されている当たり判定とワールドに登録されている当たり判定を移動込みの総当たりで計算し、スライド移動も行う関数(カリング処理 : 移動ベクトル、基準円)
    int CheckHitAllMove();

    // 
    int SearchTag(CollisionTag tag, std::vector<BaseCollision2D *> &out);

    // 謎の当たり判定と謎の当たり判定の衝突確認を行う関数
    static CollisionCheckResult2D CheckHitCollision2DToCollision2D(BaseCollision2D *collision2DA, BaseCollision2D *collision2DB);

    int Add(BaseCollision2D *target);
    int ConnectTarget(BaseCollision2D *prev, BaseCollision2D *target, BaseCollision2D *next);
    int IsolateTarget(BaseCollision2D *target);
    int IsolateAll();
};