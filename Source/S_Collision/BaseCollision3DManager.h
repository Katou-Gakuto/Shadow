#pragma once

#include "AABB.h"
#include "CollisionCheckResult.h"
#include "BaseCollision.h"
#include <vector>

class BaseCollision3D;
class Point3D;
class Sphere3D;
class Capsule3D;
class Triangle3D;
class Quadrangle3D;
class Cube3D;
class Ray3D;

// 3Dの当たり判定の計算を行うクラス
// 当たり判定の計算は未来座標を使って行います
// なので当たり判定が埋もれてない状態を前提しています
class BaseCollision3DManager
{
private:
    BaseCollision3D *mpFirstBaseCollision3D;
    static AABB3D aabbA;
    static AABB3D aabbB;

private:
    // この二つの当たり判定が互いの処理対象なのかを確認する関数
    static bool CheckProcessing(BaseCollision3D *collision3DA, BaseCollision3D *collision3DB);

    // 移動ベクトルを使ったカリング処理を行う関数
    static bool CheckHitMoveVecToCollision3D(BaseCollision3D *collision3DA, BaseCollision3D *collision3DB);

    // 円を使ったカリング処理を行う関数
    static bool CheckHitBaseCollision3DToBaseCollision3D(BaseCollision3D *collision3DA, BaseCollision3D *collision3DB);

    // AABBを使ったカリング処理を行う関数
    static bool CheckHitAABBAABB(BaseCollision3D *collision3DA, BaseCollision3D *collision3DB);

    // カリング処理をまとめて行い、当たり判定の計算を飛ばすべきかを確認する関数
    static bool CheckCulling(BaseCollision3D *collision3DA, BaseCollision3D *collision3DB);

    // 二つの当たり判定のスライド移動を行う関数
    static int CollisionSlide(BaseCollision3D *collision3DA, BaseCollision3D *collision3DB, const CollisionCheckResult3D &hitResult);

    // 点と謎の当たり判定の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitPoint3DToCollision3D(Point3D *point3D, BaseCollision3D *collision);

    // 球と謎の当たり判定の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitSphere3DToCollision3D(Sphere3D *sphere3D, BaseCollision3D *collision);

    // カプセルと謎の当たり判定の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitCapsule3DToCollision3D(Capsule3D *capsule3D, BaseCollision3D *collision);

    // 三角形と謎の当たり判定の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitTriangle3DToCollision3D(Triangle3D *triangle3D, BaseCollision3D *collision);

    // 四角形と謎の当たり判定の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitQuadrangle3DToCollision3D(Quadrangle3D *quadrangle3D, BaseCollision3D *collision);

    // キューブと謎の当たり判定の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitCube3DToCollision3D(Cube3D *cube3D, BaseCollision3D *collision);

    // レイと謎の当たり判定の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitRay3DToCollision3D(Ray3D *ray3D, BaseCollision3D *collision);

    // 点と点の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitPoint3DToPoint3D(Point3D *point3Da, Point3D *point3Db);

    // 点と円の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitPoint3DToSphere3D(Point3D *point3D, Sphere3D *sphere3D);

    // 点とカプセルの衝突確認を行う関数
    static CollisionCheckResult3D CheckHitPoint3DToCapsule3D(Point3D *point3D, Capsule3D *capsule3D);

    // 点と三角形の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitPoint3DToTriangle3D(Point3D *point3D, Triangle3D *triangle3D);

    // 点と四角形の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitPoint3DToQuadrangle3D(Point3D *point3D, Quadrangle3D *quadrangle3D);

    // 点とキューブの衝突確認を行う関数
    static CollisionCheckResult3D CheckHitPoint3DToCube3D(Point3D *point3D, Cube3D *cube3D);

    // 点とレイの衝突確認を行う関数
    static CollisionCheckResult3D CheckHitPoint3DToRay3D(Point3D *point3D, Ray3D *ray3D);

    // 球と球の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitSphere3DToSphere3D(Sphere3D *sphere3Da, Sphere3D *sphere3Db);

    // 球とカプセルの衝突確認を行う関数
    static CollisionCheckResult3D CheckHitSphere3DToCapsule3D(Sphere3D *sphere3D, Capsule3D *capsule3D);

    // 球と三角形の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitSphere3DToTriangle3D(Sphere3D *sphere3D, Triangle3D *triangle3D);

    // 球と四角形の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitSphere3DToQuadrangle3D(Sphere3D *sircle3Da, Quadrangle3D *quadrangle3D);

    // 球とキューブの衝突確認を行う関数
    static CollisionCheckResult3D CheckHitSphere3DToCube3D(Sphere3D *sphere3D, Cube3D *cube3D);

    // 球とレイの衝突確認を行う関数
    static CollisionCheckResult3D CheckHitSphere3DToRay3D(Sphere3D *sphere3D, Ray3D *ray3D);

    // カプセルとカプセルの衝突確認を行う関数
    static CollisionCheckResult3D CheckHitCapsule3DToCapsule3D(Capsule3D *capsule3Da, Capsule3D *capsule3Db);

    // カプセルと三角形の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitCapsule3DToTriangle3D(Capsule3D *capsule3D, Triangle3D *triangle3D);

    // カプセルと四角形の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitCapsule3DToQuadrangle3D(Capsule3D *capsule3D, Quadrangle3D *quadrangle3D);

    // カプセルとキューブの衝突確認を行う関数
    static CollisionCheckResult3D CheckHitCapsule3DToCube3D(Capsule3D *capsule3D, Cube3D *cube3D);

    // カプセルとレイの衝突確認を行う関数
    static CollisionCheckResult3D CheckHitCapsule3DToRay3D(Capsule3D *capsule3D, Ray3D *ray3D);

    // 三角形と三角形の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitTriangle3DToTriangle3D(Triangle3D *triangle3Da, Triangle3D *triangle3Db);

    // 三角形と四角形の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitTriangle3DToQuadrangle3D(Triangle3D *triangle3D, Quadrangle3D *quadrangle3D);

    // 三角形とキューブの衝突確認を行う関数
    static CollisionCheckResult3D CheckHitTriangle3DToCube3D(Triangle3D *triangle3D, Cube3D *cube3D);

    // 三角形とレイの衝突確認を行う関数
    static CollisionCheckResult3D CheckHitTriangle3DToRay3D(Triangle3D *triangle3D, Ray3D *ray3D);

    // 四角形と四角形の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitQuadrangle3DToQuadrangle3D(Quadrangle3D *quadrangle3Da, Quadrangle3D *quadrangle3Db);

    // 四角形とキューブの衝突確認を行う関数
    static CollisionCheckResult3D CheckHitQuadrangle3DToCube3D(Quadrangle3D *quadrangle3D, Cube3D *cube3D);

    // 四角形とレイの衝突確認を行う関数
    static CollisionCheckResult3D CheckHitQuadrangle3DToRay3D(Quadrangle3D *quadrangle3D, Ray3D *ray3D);

    // キューブとキューブの衝突確認を行う関数
    static CollisionCheckResult3D CheckHitCube3DToCube3D(Cube3D *cube3Da, Cube3D *cube3Db);

    // キューブとレイの衝突確認を行う関数
    static CollisionCheckResult3D CheckHitCube3DToRay3D(Cube3D *cube3D, Ray3D *ray3D);

    // レイとレイの衝突確認を行う関数
    static CollisionCheckResult3D CheckHitRay3DToRay3D(Ray3D *targetRay3D, Ray3D *ray3D);

    // 当たり判定の寿命に関する更新処理を行う関数
    int UpdateCollisionLife();
    
public:
    BaseCollision3DManager();
    ~BaseCollision3DManager();

    int Initialize();
    int Finalize();

    // ワールドに登録されている当たり判定とワールドに登録されている当たり判定を移動込みの総当たりで計算し、スライド移動も行う関数(カリング処理 : 移動ベクトル、基準円)
    int CheckHitAllMove();

    // 謎の当たり判定と謎の当たり判定の衝突確認を行う関数
    static CollisionCheckResult3D CheckHitCollision3DToCollision3D(BaseCollision3D *collision3DA, BaseCollision3D *collision3DB);

    // 指定のタグをつけられた当たり判定を取得する関数
    std::vector<BaseCollision3D *> SearchCollisionToTag(CollisionTag tag) const;

    int Add(BaseCollision3D *target);
    int ConnectTarget(BaseCollision3D *prev, BaseCollision3D *target, BaseCollision3D *next);
    int IsolateTarget(BaseCollision3D *target);
    int IsolateAll();
};