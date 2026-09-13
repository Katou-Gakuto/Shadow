#include "BaseCollision2D.h"
#include "BaseCollision2DManager.h"

#include "Point2D.h"
#include "Circle2D.h"
#include "Capsule2D.h"
#include "Triangle2D.h"
#include "Quadrangle2D.h"
#include "Ray2D.h"

#include "../Y_Tool/MyFunctions.h"

// BaseCollision3DManagerの変数定義
AABB2D BaseCollision2DManager::aabbA;
AABB2D BaseCollision2DManager::aabbB;

// この二つの当たり判定が互いの処理対象なのかを確認する関数
bool BaseCollision2DManager::CheckProcessing(BaseCollision2D *collision2DA, BaseCollision2D *collision2DB)
{
    // 互いに処理をする当たり判定かを調べ、計算対象ならtrueを返す
    if (collision2DA->GetCollisionTag() == CollisionTag::CollisionTag_Checker ||
        collision2DB->GetCollisionTag() == CollisionTag::CollisionTag_Checker)
    {
        return true;
    }
    if (collision2DA->CheckProcessingFlag(collision2DB->GetCollisionTag()) &&
        collision2DB->CheckProcessingFlag(collision2DA->GetCollisionTag()))
    {
        return true;
    }
    return false;
}

// 当たり判定と指定の当たり判定の移動のカリング処理を行う関数
bool BaseCollision2DManager::CheckHitMoveVecToCollision2D(BaseCollision2D *collision2DA, BaseCollision2D *collision2DB)
{
    // どれか一つでも無効な値が入っていた場合はfalseを返す
    if (collision2DA == nullptr ||
        collision2DB == nullptr)
    {
        return false;
    }

    // 当たり判定の片方だけでも貫通を許すのなら、移動の進行ベクトルによるカリングをスキップする
    if (collision2DA->GetPenetrate() == true ||
        collision2DB->GetPenetrate() == true)
    {
        return true;
    }

    VECTOR2D moveVecA = collision2DA->GetMoveVec();
    VECTOR2D moveVecB = collision2DB->GetMoveVec();

    // 当たり判定Aの移動ベクトル(以下Aベクトルと呼ぶ)
    bool aMoveFlag = moveVecA != VECTOR2D::GetZero();

    // 当たり判定Bの移動ベクトル(以下Bベクトルと呼ぶ)
    bool bMoveFlag = moveVecB != VECTOR2D::GetZero();

    // どちらも移動している場合
    if (aMoveFlag && bMoveFlag)
    {
        // 移動の進行ベクトルとコリジョンベクトルの内積を取る
        float moveA_moveB_Dot = VECTOR2D::Dot(moveVecA, moveVecB);

        // コリジョンと移動ベクトルの内積の結果で返す値を変える
        if (0 < moveA_moveB_Dot)
        {
            // コリジョンの移動ベクトル同士が向き合っているので当たる可能性がある
            return true;
        }
    }
    // 片方のみ移動している場合
    else if (aMoveFlag != bMoveFlag)
    {
        // 処理を行う前に、基準円の更新を行う
        collision2DA->SetBaseParamMySelf();
        collision2DB->SetBaseParamMySelf();
        if (aMoveFlag)
        {
            // 当たり判定A→当たり判定Bの進行ベクトルを作る(以下a2bVと呼ぶ)
            VECTOR2D a2bVec = collision2DB->GetBasePos() - collision2DA->GetBasePos();

            // a2bVとAベクトルの内積を取る
            float a2b_moveA_Dot = VECTOR2D::Dot(a2bVec, moveVecA);

            // 内積の結果、二つのベクトルが直角でなく、さらに同じような方向を向いているならtrue
            if (0 < a2b_moveA_Dot)
            {
                return true;
            }
        }
        else
        {
            // 当たり判定B→当たり判定Aの進行ベクトルを作る(以下b2aVと呼ぶ)
            VECTOR2D b2aVec = collision2DA->GetBasePos() - collision2DB->GetBasePos();

            // b2aVとBベクトルの内積を取る
            float b2a_moveA_Dot = VECTOR2D::Dot(b2aVec, moveVecB);

            // 内積の結果、二つのベクトルが直角でなく、さらに同じような方向を向いているならtrue
            if (0 < b2a_moveA_Dot)
            {
                return true;
            }
        }
    }
    // ※どちらも移動していない場合は以降の衝突・内外判定の処理を行わない

    // 二つの当たり判定のことはないのでfalseを返す
    return false;
}

// 当たり判定と指定の当たり判定の円を使ったカリング処理を行う関数
bool BaseCollision2DManager::CheckHitBaseCollision2DToBaseCollision2D(BaseCollision2D *collision2DA, BaseCollision2D *collision2DB)
{
    // 二つの当たり判定の基準円を取得する
    collision2DA->SetBaseParamMySelf();
    collision2DB->SetBaseParamMySelf();

    // 1の中心点→2の中心点の進行ベクトルを取得
    VECTOR2D distanceVec = collision2DB->GetMovedPos(collision2DB->GetBasePos()) - collision2DA->GetMovedPos(collision2DA->GetBasePos());

    // 二つの円の半径を取得
    float baseRadius = collision2DA->GetBaseRadiusNoSqrt() + collision2DB->GetBaseRadiusNoSqrt();

    // 進行ベクトルと二つの円の半径を比較し、進行ベクトルの方が小さかったらfalseを返す
    if (distanceVec.MagnitudeNoSqrt() <= baseRadius)
    {
        return true;
    }

    // falseを返す
    return false;
}

// AABBを使ったカリング処理を行う関数
bool BaseCollision2DManager::CheckHitAABBAABB(BaseCollision2D *collision2DA, BaseCollision2D *collision2DB)
{
    if (collision2DA->GetAABB(aabbA) != 0 || collision2DB->GetAABB(aabbB) != 0)
    {
        return false;
    }
    VECTOR2D aabb1min = aabbA.mvMinPos + aabbA.mvMoveVec;
    VECTOR2D aabb1max = aabbA.mvMaxPos + aabbA.mvMoveVec;
    VECTOR2D aabb2min = aabbB.mvMinPos + aabbB.mvMoveVec;
    VECTOR2D aabb2max = aabbB.mvMaxPos + aabbB.mvMoveVec;
    if (aabb1max.GetX() < aabb2min.GetX() || aabb2max.GetX() < aabb1min.GetX())
    {
        return false;
    }
    if (aabb1max.GetY() < aabb2min.GetY() || aabb2max.GetY() < aabb1min.GetY())
    {
        return false;
    }
    return true;
}

// カリング処理をまとめて行い、当たり判定の計算を飛ばすべきかを確認する関数
bool BaseCollision2DManager::CheckCulling(BaseCollision2D *collision2DA, BaseCollision2D *collision2DB)
{
    // 二つの当たり判定が互いの処理対象かを確認する
    if (BaseCollision2DManager::CheckProcessing(collision2DA, collision2DB) == false)
    {
        return true;
    }
    // 移動ベクトルを使ったカリング処理を行う
    if (BaseCollision2DManager::CheckHitMoveVecToCollision2D(collision2DA, collision2DB) == false)
    {
        return true;
    }
    // 円を使ったカリング処理を行う
    if (BaseCollision2DManager::CheckHitBaseCollision2DToBaseCollision2D(collision2DA, collision2DB) == false)
    {
        return true;
    }
    // AABBを使ったカリング処理を行う
    if (BaseCollision2DManager::CheckHitAABBAABB(collision2DA, collision2DB) == false)
    {
        return true;
    }

    // 飛ばすべきでないことを返す
    return false;
}

// 二つの当たり判定のスライド移動を行う関数
int BaseCollision2DManager::CollisionSlide(BaseCollision2D *collision2DA, BaseCollision2D *collision2DB, const CollisionCheckResult2D &hitResult)
{
    // どちらも貫通を許さない場合はスライド移動の処理を行う
    if (collision2DA->GetPenetrate() == false && collision2DB->GetPenetrate() == false)
    {
        // collision2DAのスライド移動を行う
        if (collision2DA->GetHitMove())
        {
            // collision2DAの移動の係数を設定する
            float aMoveLate = 0.0f;
            if (collision2DB->GetHitMove() == false)
            {
                aMoveLate = 2.0f;
            }
            else
            {
                aMoveLate = MyFunctions::GetRatio(
                    collision2DA->GetMoveLate() + collision2DB->GetMoveLate(),
                    collision2DA->GetMoveLate(),
                    2.0f);
            }

            // あとは当たり判定側に任せる
            collision2DA->SlideMove(hitResult, aMoveLate);
        }

        // collision2DBのスライド移動を行う
        if (collision2DB->GetHitMove())
        {
            // collision2DBの移動の係数を設定する
            float bMoveLate = 0.0f;
            if (collision2DA->GetHitMove() == false)
            {
                bMoveLate = 2.0f;
            }
            else
            {
                bMoveLate = MyFunctions::GetRatio(
                    collision2DB->GetMoveLate() + collision2DA->GetMoveLate(),
                    collision2DB->GetMoveLate(),
                    2.0f);
            }

            // あとは当たり判定側に任せる
            collision2DB->SlideMove(hitResult, bMoveLate);
        }
    }

    // 関数が正常終了したことを返す
    return 0;
}

// 点と謎の当たり判定の内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitPoint2DToCollision2D(Point2D *point2D, BaseCollision2D *collision)
{
    // 返り値を保持する変数
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    // 点じゃない方の当たり判定のタイプに合わせた関数に飛ぶ
    switch (collision->GetCollisionType())
    {
    case CollisionType::CollisionType_Point2D:
        temp = BaseCollision2DManager::CheckHitPoint2DToCollision2D(point2D, dynamic_cast<Point2D *>(collision));
        break;
    case CollisionType::CollisionType_Circle2D:
        temp = BaseCollision2DManager::CheckHitPoint2DToCircle2D(point2D, dynamic_cast<Circle2D *>(collision));
        break;
    case CollisionType::CollisionType_Capsule2D:
        temp = BaseCollision2DManager::CheckHitPoint2DToCapsule2D(point2D, dynamic_cast<Capsule2D *>(collision));
        break;
    case CollisionType::CollisionType_Triangle2D:
        temp = BaseCollision2DManager::CheckHitPoint2DToTriangle2D(point2D, dynamic_cast<Triangle2D *>(collision));
        break;
    case CollisionType::CollisionType_Quadrangle2D:
        temp = BaseCollision2DManager::CheckHitPoint2DToQuadrangle2D(point2D, dynamic_cast<Quadrangle2D *>(collision));
        break;
    default:

        break;
    }
    return temp;
}

// 円と謎の当たり判定の内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitCircle2DToCollision2D(Circle2D *circle2D, BaseCollision2D *collision)
{
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();
    
    // 円じゃない方の当たり判定のタイプに合わせた関数に飛ぶ
    switch (collision->GetCollisionType())
    {
    case CollisionType::CollisionType_Point2D:
        temp = BaseCollision2DManager::CheckHitPoint2DToCircle2D(dynamic_cast<Point2D *>(collision), circle2D);
        break;
    case CollisionType::CollisionType_Circle2D:
        temp = BaseCollision2DManager::CheckHitCircle2DToCircle2D(circle2D, dynamic_cast<Circle2D *>(collision));
        break;
    case CollisionType::CollisionType_Capsule2D:
        temp = BaseCollision2DManager::CheckHitCircle2DToCapsule2D(circle2D, dynamic_cast<Capsule2D *>(collision));
        break;
    case CollisionType::CollisionType_Triangle2D:
        temp = BaseCollision2DManager::CheckHitCircle2DToTriangle2D(circle2D, dynamic_cast<Triangle2D *>(collision));
        break;
    case CollisionType::CollisionType_Quadrangle2D:
        temp = BaseCollision2DManager::CheckHitCircle2DToQuadrangle2D(circle2D, dynamic_cast<Quadrangle2D *>(collision));
        break;
    default:

        break;
    }
    return temp;
}

// カプセルと謎の当たり判定の内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitCapsule2DToCollision2D(Capsule2D *capsule2D, BaseCollision2D *collision)
{
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();
    
    // カプセルじゃない方の当たり判定のタイプに合わせた関数に飛ぶ
    switch (collision->GetCollisionType())
    {
    case CollisionType::CollisionType_Point2D:
        temp = BaseCollision2DManager::CheckHitPoint2DToCapsule2D(dynamic_cast<Point2D *>(collision), capsule2D);
        break;
    case CollisionType::CollisionType_Circle2D:
        temp = BaseCollision2DManager::CheckHitCircle2DToCapsule2D(dynamic_cast<Circle2D *>(collision), capsule2D);
        break;
    case CollisionType::CollisionType_Capsule2D:
        temp = BaseCollision2DManager::CheckHitCapsule2DToCapsule2D(capsule2D, dynamic_cast<Capsule2D *>(collision));
        break;
    case CollisionType::CollisionType_Triangle2D:
        temp = BaseCollision2DManager::CheckHitCapsule2DToTriangle2D(capsule2D, dynamic_cast<Triangle2D *>(collision));
        break;
    case CollisionType::CollisionType_Quadrangle2D:
        temp = BaseCollision2DManager::CheckHitCapsule2DToQuadrangle2D(capsule2D, dynamic_cast<Quadrangle2D *>(collision));
        break;
    default:

        break;
    }
    return temp;
}

// 三角形と謎の当たり判定の内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitTriangle2DToCollision2D(Triangle2D *triangle2D, BaseCollision2D *collision)
{
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();
    
    // 三角形じゃない方の当たり判定のタイプに合わせた関数に飛ぶ
    switch (collision->GetCollisionType())
    {
    case CollisionType::CollisionType_Point2D:
        temp = BaseCollision2DManager::CheckHitPoint2DToTriangle2D(dynamic_cast<Point2D *>(collision), triangle2D);
        break;
    case CollisionType::CollisionType_Circle2D:
        temp = BaseCollision2DManager::CheckHitCircle2DToTriangle2D(dynamic_cast<Circle2D *>(collision), triangle2D);
        break;
    case CollisionType::CollisionType_Capsule2D:
        temp = BaseCollision2DManager::CheckHitCapsule2DToTriangle2D(dynamic_cast<Capsule2D *>(collision), triangle2D);
        break;
    case CollisionType::CollisionType_Triangle2D:
        temp = BaseCollision2DManager::CheckHitTriangle2DToTriangle2D(triangle2D, dynamic_cast<Triangle2D *>(collision));
        break;
    case CollisionType::CollisionType_Quadrangle2D:
        temp = BaseCollision2DManager::CheckHitTriangle2DToQuadrangle2D(triangle2D, dynamic_cast<Quadrangle2D *>(collision));
        break;
    default:

        break;
    }
    return temp;
}

// 四角形と謎の当たり判定の内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitQuadrangle2DToCollision2D(Quadrangle2D *quadrangle2D, BaseCollision2D *collision)
{
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();
    
    // 四角形じゃない方の当たり判定のタイプに合わせた関数に飛ぶ
    switch (collision->GetCollisionType())
    {
    case CollisionType::CollisionType_Point2D:
        temp = BaseCollision2DManager::CheckHitPoint2DToQuadrangle2D(dynamic_cast<Point2D *>(collision), quadrangle2D);
        break;
    case CollisionType::CollisionType_Circle2D:
        temp = BaseCollision2DManager::CheckHitCircle2DToQuadrangle2D(dynamic_cast<Circle2D *>(collision), quadrangle2D);
        break;
    case CollisionType::CollisionType_Capsule2D:
        temp = BaseCollision2DManager::CheckHitCapsule2DToQuadrangle2D(dynamic_cast<Capsule2D *>(collision), quadrangle2D);
        break;
    case CollisionType::CollisionType_Triangle2D:
        temp = BaseCollision2DManager::CheckHitTriangle2DToQuadrangle2D(dynamic_cast<Triangle2D *>(collision), quadrangle2D);
        break;
    case CollisionType::CollisionType_Quadrangle2D:
        temp = BaseCollision2DManager::CheckHitQuadrangle2DToQuadrangle2D(quadrangle2D, dynamic_cast<Quadrangle2D *>(collision));
        break;
    default:

        break;
    }
    return temp;
}

// レイと謎の当たり判定の衝突確認を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitRay2DToCollision2D(Ray2D *ray2D, BaseCollision2D *collision)
{
    // 返り値
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    switch (collision->GetCollisionType())
    {
    case CollisionType::CollisionType_Point2D:
        temp = BaseCollision2DManager::CheckHitPoint2DToRay2D(dynamic_cast<Point2D *>(collision), ray2D);
        break;
    case CollisionType::CollisionType_Circle2D:
        temp = BaseCollision2DManager::CheckHitCircle2DToRay2D(dynamic_cast<Circle2D *>(collision), ray2D);
        break;
    case CollisionType::CollisionType_Capsule2D:
        temp = BaseCollision2DManager::CheckHitCapsule2DToRay2D(dynamic_cast<Capsule2D *>(collision), ray2D);
        break;
    case CollisionType::CollisionType_Triangle2D:
        temp = BaseCollision2DManager::CheckHitTriangle2DToRay2D(dynamic_cast<Triangle2D *>(collision), ray2D);
        break;
    case CollisionType::CollisionType_Quadrangle2D:
        temp = BaseCollision2DManager::CheckHitQuadrangle2DToRay2D(dynamic_cast<Quadrangle2D *>(collision), ray2D);
        break;
    case CollisionType::CollisionType_Ray2D:
        temp = BaseCollision2DManager::CheckHitRay2DToRay2D(ray2D, dynamic_cast<Ray2D *>(collision));
        break;

    default:
        break;
    }

    return temp;

    // 衝突地点の計算はこのサイトを参考にしました
    // http://marupeke296.com/COL_2D_No11_CircleVsRay.html
}

// 点と点の内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitPoint2DToPoint2D(Point2D *point2Da, Point2D *point2Db)
{
    // 既に当たってることを確認しているのですぐ値を設定して、計算結果を返す
    return GetCollisionCheckResult2D(
        point2Da, 
        point2Db, 
        0, 
        point2Da->GetBasePos() - point2Db->GetBasePos(), 
        point2Db->GetBasePos() - point2Da->GetBasePos(),
        VECTOR2D::GetZero(),
        VECTOR2D::GetZero());
}

// 点と円の内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitPoint2DToCircle2D(Point2D *point2D, Circle2D *circle2D)
{
    // 既に当たってることを確認しているのですぐ値を設定して、計算結果を返す
    return GetCollisionCheckResult2D(
        point2D,
        circle2D,
        0,
        point2D->GetBasePos() - circle2D->GetBasePos(),
        circle2D->GetBasePos() - point2D->GetBasePos(),
        VECTOR2D::GetZero(),
        VECTOR2D::GetZero());
}

// 点とカプセルの内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitPoint2DToCapsule2D(Point2D *point2D, Capsule2D *capsule2D)
{
    // 返り値
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    // 球体とカプセルの線上最近点を求める(世界座標 無限延長線編)
    VECTOR2D newrPos = VECTOR2D::GetNewrPosOnLine(
        point2D->GetMovedPos(point2D->GetBasePos()),
        capsule2D->GetMovedPos(capsule2D->GetStartPos()),
        capsule2D->GetMovedPos(capsule2D->GetEndPos()));

    // ベクトル二つを改めて作る
    VECTOR2D startToNewr = newrPos - capsule2D->GetMovedPos(capsule2D->GetStartPos());
    VECTOR2D startToEnd = capsule2D->GetMovedPos(capsule2D->GetEndPos()) - capsule2D->GetMovedPos(capsule2D->GetStartPos());

    // 線上最近点を中心としたカプセルのスフィアを作る
    Circle2D capsuleCircle2D;

    // 線上最近点が、どっちのセンターベクトルを軸とした+方向にあるか、-方向にあるかが確定していないため、それを判断するswitch文
    switch (VECTOR2D::CheckAxisVecPlusOrMinus(startToNewr, startToEnd))
    {
    case 1:
        // startNewrがプラス方向にあるため、線上最近点が確定していない
        if (startToNewr.MagnitudeNoSqrt() <= startToEnd.MagnitudeNoSqrt())
        {
            // カプセルのセンターベクトル内に線上最近点があったということで、作業用のスフィアの値を設定する
            capsuleCircle2D.SetShapeParameter(newrPos, capsule2D->GetBaseRadiusNoSqrt());
            temp = BaseCollision2DManager::CheckHitPoint2DToCircle2D(point2D, &capsuleCircle2D);
            if (0 <= temp.mnResultParam)
            {
                temp = GetCollisionCheckResult2D(
                    point2D,
                    capsule2D,
                    0,
                    point2D->GetBasePos() - capsule2D->GetBasePos(),
                    capsule2D->GetBasePos() - point2D->GetBasePos(),
                    point2D->GetBasePos(),
                    newrPos);
            }
        }
        else
        {
            // カプセルのセンターベクトル内に線上最近点はなかったということで、+方向の端点とスフィアで当たり判定を計算する
            capsuleCircle2D.SetShapeParameter(capsule2D->GetMovedPos(capsule2D->GetEndPos()), capsule2D->GetBaseRadiusNoSqrt());
            temp = BaseCollision2DManager::CheckHitPoint2DToCircle2D(point2D, &capsuleCircle2D);
            if (0 <= temp.mnResultParam)
            {
                temp = GetCollisionCheckResult2D(
                    point2D,
                    capsule2D,
                    0,
                    point2D->GetBasePos() - capsule2D->GetBasePos(),
                    capsule2D->GetBasePos() - point2D->GetBasePos(),
                    point2D->GetBasePos(),
                    newrPos);
            }
        }

        break;
    case -1:
        // 線上最近点がマイナス方向にあるため、-方向の端点とスフィアで当たり判定を計算する
        capsuleCircle2D.SetShapeParameter(capsule2D->GetMovedPos(capsule2D->GetBasePos()), capsule2D->GetBaseRadiusNoSqrt());
        temp = BaseCollision2DManager::CheckHitPoint2DToCircle2D(point2D, &capsuleCircle2D);
        if (0 <= temp.mnResultParam)
        {
            temp = GetCollisionCheckResult2D(
                point2D,
                capsule2D,
                0,
                point2D->GetBasePos() -  capsule2D->GetBasePos(),
                capsule2D->GetBasePos() - point2D->GetBasePos(),
                point2D->GetBasePos(),
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
CollisionCheckResult2D BaseCollision2DManager::CheckHitPoint2DToTriangle2D(Point2D *point2D, Triangle2D *triangle2D)
{
    // 返り値
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    // 計算に使うベクトルを取得する
    VECTOR2D pos0_pos1Vec = (triangle2D->GetMovedPos(triangle2D->GetVertexPos(1)) - triangle2D->GetMovedPos(triangle2D->GetVertexPos(0)));
    VECTOR2D posP_pos1Vec = (point2D->GetMovedPos(point2D->GetBasePos()) - triangle2D->GetMovedPos(triangle2D->GetVertexPos(1)));
    VECTOR2D pos1_pos2Vec = (triangle2D->GetMovedPos(triangle2D->GetVertexPos(2)) - triangle2D->GetMovedPos(triangle2D->GetVertexPos(1)));
    VECTOR2D posP_pos2Vec = (point2D->GetMovedPos(point2D->GetBasePos()) - triangle2D->GetMovedPos(triangle2D->GetVertexPos(2)));
    VECTOR2D pos2_pos0Vec = (triangle2D->GetMovedPos(triangle2D->GetVertexPos(0)) - triangle2D->GetMovedPos(triangle2D->GetVertexPos(2)));
    VECTOR2D posP_pos0Vec = (point2D->GetMovedPos(point2D->GetBasePos()) - triangle2D->GetMovedPos(triangle2D->GetVertexPos(0)));

    // 三角形の頂点からマウスの座標へのベクトルと、三角形の辺ベクトルの外積を求め、架空のz成分を取得する
    float cross1 = VECTOR2D::Cross(pos0_pos1Vec, posP_pos1Vec);
    float cross2 = VECTOR2D::Cross(pos1_pos2Vec, posP_pos2Vec);
    float cross0 = VECTOR2D::Cross(pos2_pos0Vec, posP_pos0Vec);

    // 外積で求めた架空のz成分の向きが揃っているかを調べる
    if ((cross1 < 0.0f && cross2 < 0.0f && cross0 < 0.0f) ||
        (0.0f < cross1 && 0.0f < cross2 && 0.0f < cross0))
    {
        // 点が三角形の中に入っているということなのでtrueを返す
        temp = GetCollisionCheckResult2D(
            point2D,
            triangle2D,
            0,
            triangle2D->GetBasePos() - point2D->GetBasePos(),
            point2D->GetBasePos() - triangle2D->GetBasePos(), 
            point2D->GetBasePos(),
            VECTOR2D::GetZero());
    }

    // 計算結果を返す
    return temp;
}

// 点と四角形の内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitPoint2DToQuadrangle2D(Point2D *point2D, Quadrangle2D *quadrangle2D)
{
    // 返り値
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    Triangle2D tempTriangle2D;
    tempTriangle2D.SetShapeParameter(
        quadrangle2D->GetMovedPos(quadrangle2D->GetVertexPos(0)),
        quadrangle2D->GetMovedPos(quadrangle2D->GetVertexPos(1)),
        quadrangle2D->GetMovedPos(quadrangle2D->GetVertexPos(2)));


    temp = BaseCollision2DManager::CheckHitPoint2DToTriangle2D(point2D, &tempTriangle2D);

    if (0 <= temp.mnResultParam)
    {
        temp = GetCollisionCheckResult2D(
            point2D, 
            quadrangle2D, 
            1, 
            point2D->GetBasePos() - quadrangle2D->GetBasePos(),
            quadrangle2D->GetBasePos() - point2D->GetBasePos(),
            point2D->GetBasePos(),
            VECTOR2D::GetZero());
    }
    else
    {
        tempTriangle2D.SetShapeParameter(
            quadrangle2D->GetMovedPos(quadrangle2D->GetVertexPos(3)),
            quadrangle2D->GetMovedPos(quadrangle2D->GetVertexPos(2)),
            quadrangle2D->GetMovedPos(quadrangle2D->GetVertexPos(1)));

        temp = BaseCollision2DManager::CheckHitPoint2DToTriangle2D(point2D, &tempTriangle2D);
        if (0 <= temp.mnResultParam)
        {
            temp = GetCollisionCheckResult2D(
                point2D,
                quadrangle2D,
                2,
                point2D->GetBasePos() - quadrangle2D->GetBasePos(),
                quadrangle2D->GetBasePos() - point2D->GetBasePos(),
                point2D->GetBasePos(),
                VECTOR2D::GetZero());
        }
    }

    return temp;
}

// 点とレイの内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitPoint2DToRay2D(Point2D *point2D, Ray2D *ray2D)
{
    // 返り値
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    do
    {
        VECTOR2D start2point = point2D->GetBasePos() - ray2D->GetStartPos();
        VECTOR2D start2end = ray2D->GetLineVec();
        float dot = VECTOR2D::Dot(start2point, start2end);
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

        temp = GetCollisionCheckResult2D(
            point2D,
            ray2D,
            0,
            point2D->GetBasePos() - ray2D->GetBasePos(),
            ray2D->GetBasePos() - point2D->GetBasePos(),
            point2D->GetBasePos(),
            VECTOR2D::GetZero());
    } while (false);
    return temp;
}

// 円と円の内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitCircle2DToCircle2D(Circle2D *circle2Da, Circle2D *circle2Db)
{
    // 返り値
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    // 二つの円の半径を取得
    float baseRadius = circle2Da->GetBaseRadius() + circle2Db->GetBaseRadius();

    // 1の中心点→2の中心点の進行ベクトルを取得
    VECTOR2D distanceVec = circle2Db->GetMovedPos(circle2Db->GetBasePos()) - circle2Da->GetMovedPos(circle2Da->GetBasePos());

    // 二つの円の半径と進行ベクトルを比較し、二つの円の半径のほうが小さかったらfalseを返す
    if (baseRadius * baseRadius < distanceVec.MagnitudeNoSqrt())
    {
        temp = GetCollisionCheckResult2D(
            circle2Da,
            circle2Db,
            0,
            circle2Da->GetBasePos() - circle2Db->GetBasePos(),
            circle2Db->GetBasePos() - circle2Da->GetBasePos(),
            VECTOR2D::GetZero(),           // HitPosの計算がめんどくさい。
            VECTOR2D::GetZero());
    }

    // trueを返す
    return temp;
}

// 円とカプセルの内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitCircle2DToCapsule2D(Circle2D *circle2D, Capsule2D *capsule2D)
{
    // 返り値
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    // 球体とカプセルの線上最近点を求める(世界座標 無限延長線編)
    VECTOR2D newrPos = VECTOR2D::GetNewrPosOnLine(circle2D->GetMovedPos(
        circle2D->GetBasePos()), 
        capsule2D->GetMovedPos(capsule2D->GetStartPos()), 
        capsule2D->GetMovedPos(capsule2D->GetEndPos()));

    // ベクトル二つを改めて作る
    VECTOR2D startToNewr = newrPos - capsule2D->GetMovedPos(capsule2D->GetStartPos());
    VECTOR2D startToEnd = capsule2D->GetMovedPos(capsule2D->GetEndPos()) - capsule2D->GetMovedPos(capsule2D->GetStartPos());

    // 線上最近点を中心としたカプセルのスフィアを作る
    Circle2D capsuleCircle2D;

    // 線上最近点が、どっちのセンターベクトルを軸とした+方向にあるか、-方向にあるかが確定していないため、それを判断するswitch文
    switch (VECTOR2D::CheckAxisVecPlusOrMinus(startToNewr, startToEnd))
    {
    case 1:
        // startNewrがプラス方向にあるため、線上最近点が確定していない
        if (startToNewr.MagnitudeNoSqrt() <= startToEnd.MagnitudeNoSqrt())
        {
            // カプセルのセンターベクトル内に線上最近点があったということで、作業用のスフィアの値を設定する
            capsuleCircle2D.SetShapeParameter(newrPos, capsule2D->GetBaseRadiusNoSqrt());
            temp = BaseCollision2DManager::CheckHitCircle2DToCircle2D(circle2D, &capsuleCircle2D);
            if (0 <= temp.mnResultParam)
            {
                // カプセルの法線ベクトルを取得する
                VECTOR3D capsuleCrossVec3D = VECTOR3D::Cross(capsule2D->GetLineVec().To3D(0.0f), VECTOR3D(0.0f, 0.0f, 1.0f));
                VECTOR2D capsuleCrossVec2D = capsuleCrossVec3D.To2D('x', 'y');
                VECTOR2D capsule2circle = circle2D->GetBasePos() - newrPos;
                capsuleCrossVec2D = VECTOR2D::GetTargetDirectionNewrVec(capsule2circle, capsuleCrossVec2D, -capsuleCrossVec2D);

                temp = GetCollisionCheckResult2D(
                    circle2D,
                    capsule2D,
                    1,
                    capsuleCrossVec2D,
                    capsule2D->GetBasePos() - circle2D->GetBasePos(),
                    VECTOR2D::GetZero(),
                    newrPos);
            }
        }
        else
        {
            // カプセルのセンターベクトル内に線上最近点はなかったということで、+方向の端点とスフィアで当たり判定を計算する
            capsuleCircle2D.SetShapeParameter(capsule2D->GetMovedPos(capsule2D->GetEndPos()), capsule2D->GetBaseRadiusNoSqrt());
            temp = BaseCollision2DManager::CheckHitCircle2DToCircle2D(circle2D, &capsuleCircle2D);
            if (0 <= temp.mnResultParam)
            {
                temp = GetCollisionCheckResult2D(
                    circle2D,
                    capsule2D,
                    2,
                    circle2D->GetBasePos() - newrPos,
                    newrPos - circle2D->GetBasePos(),
                    VECTOR2D::GetZero(),
                    newrPos);
            }
        }

        break;
    case -1:
        // 線上最近点がマイナス方向にあるため、-方向の端点とスフィアで当たり判定を計算する
        capsuleCircle2D.SetShapeParameter(capsule2D->GetMovedPos(capsule2D->GetStartPos()), capsule2D->GetBaseRadiusNoSqrt());
        temp = BaseCollision2DManager::CheckHitCircle2DToCircle2D(circle2D, &capsuleCircle2D);
        if (0 <= temp.mnResultParam)
        {
            temp = GetCollisionCheckResult2D(
                circle2D,
                capsule2D,
                3,
                circle2D->GetBasePos() - newrPos,
                newrPos - circle2D->GetBasePos(),
                VECTOR2D::GetZero(),
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

    // 値を返す
    return temp;
}

// 円と三角形の内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitCircle2DToTriangle2D(Circle2D *circle2D, Triangle2D *triangle2D)
{
    // 返り値
    CollisionCheckResult2D returnValue = GetCollisionCheckResult2DZero();

    // 円の中心点を点に変換する
    Point2D circlePoint;
    circlePoint.SetBasePos(circle2D->GetMovedPos(circle2D->GetBasePos()));

    Ray2D triangleRay;
    triangleRay.SetMoveVec(triangle2D->GetMoveVec());

    bool hitFlag = false;
    int minNum;

    // 三角形の中に円の中心点が入っていたらtrueを返す
    returnValue = BaseCollision2DManager::CheckHitPoint2DToTriangle2D(&circlePoint, triangle2D);
    if (0 <= returnValue.mnResultParam)
    {
        returnValue = GetCollisionCheckResult2D(
            circle2D,
            triangle2D,
            1,
            circle2D->GetBasePos() - triangle2D->GetBasePos(),
            triangle2D->GetBasePos() - circle2D->GetBasePos(),
            circle2D->GetBasePos(),
            VECTOR2D::GetZero());
    }
    else
    {
        // 関数の返り値を保持するための変数
        CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();
        for (int i = 0; i < 3; i++)
        {
            int j = i + 1;
            if (3 <= j)
            {
                j -= 3;
            }

            // 三角形の辺の一つをRayに変換する
            triangleRay.SetShapeParameter(triangle2D->GetVertexPos(i), triangle2D->GetVertexPos(j));
            temp = BaseCollision2DManager::CheckHitCircle2DToRay2D(circle2D, &triangleRay);

            // 辺の一つでも円に当たっていたら処理を行う
            if (0 <= temp.mnResultParam)
            {
                if (returnValue.mnResultParam == -1)
                {
                    returnValue = GetCollisionCheckResult2D(
                        circle2D,
                        triangle2D,
                        (signed long)(i + 1),
                        temp.mvRepulsionVecA, 
                        temp.mvRepulsionVecB,
                        temp.mvHitPos,
                        temp.mvFloatBox);
                }
                else
                {
                    VECTOR2D currentPos = VECTOR2D::GetNewrPosOnLine(circle2D->GetBasePos(), triangleRay.GetStartPos(), triangleRay.GetEndPos());
                    if ((currentPos - circle2D->GetBasePos()).MagnitudeNoSqrt() < (returnValue.mvHitPos - circle2D->GetBasePos()).MagnitudeNoSqrt())
                    {
                        returnValue = GetCollisionCheckResult2D(
                            circle2D,
                            triangle2D,
                            (signed long)(i + 1),
                            temp.mvRepulsionVecA,
                            temp.mvRepulsionVecB,
                            temp.mvHitPos,
                            temp.mvFloatBox);
                    }
                }
            }
        }
    }

    // どのへんにも当たっていなかったのでfalseを返す
    return returnValue;
}

// 円と四角形の内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitCircle2DToQuadrangle2D(Circle2D *circle2D, Quadrangle2D *quadrangle2D)
{
    // 返り値
    CollisionCheckResult2D returnValue = GetCollisionCheckResult2DZero();

    // 円の中心点を点に変換する
    Point2D circlePoint;
    circlePoint.SetBasePos(circle2D->GetMovedPos(circle2D->GetBasePos()));

    // 四角形の中に円の中心点が入っていたらtrueを返す
    returnValue = BaseCollision2DManager::CheckHitCollision2DToCollision2D(&circlePoint, quadrangle2D);
    if (0 < returnValue.mnResultParam)
    {
        returnValue = GetCollisionCheckResult2D(
            circle2D,
            quadrangle2D,
            1,
            circle2D->GetBasePos() - quadrangle2D->GetBasePos(),
            quadrangle2D->GetBasePos() - circle2D->GetBasePos(),
            circle2D->GetBasePos(),
            VECTOR2D::GetZero());
    }
    else
    {
        // 関数の返り値を保持するための変数
        CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

        // 四角形の辺をRayに変える
        Ray2D quadrangleRay;
        quadrangleRay.SetMoveVec(quadrangle2D->GetMoveVec());

        // 四角形の辺の一つをRayに変換する
        quadrangleRay.SetShapeParameter(quadrangle2D->GetVertexPos(0), quadrangle2D->GetVertexPos(1));

        // 辺の一つでも円に当たっていたらtrueを返す
        temp = BaseCollision2DManager::CheckHitCircle2DToRay2D(circle2D, &quadrangleRay);
        if (0 <= temp.mnResultParam)
        {
            returnValue = GetCollisionCheckResult2D(
                circle2D,
                quadrangle2D,
                2,
                temp.mvRepulsionVecA,
                temp.mvRepulsionVecB,
                temp.mvHitPos,
                temp.mvFloatBox);
        }

        // 四角形の辺の一つをRayに変換する
        quadrangleRay.SetShapeParameter(quadrangle2D->GetVertexPos(1), quadrangle2D->GetVertexPos(3));

        // 辺の一つでも円に当たっていたらtrueを返す
        temp = BaseCollision2DManager::CheckHitCircle2DToRay2D(circle2D, &quadrangleRay);
        if (0 <= temp.mnResultParam)
        {
            if (returnValue.mnResultParam == -1)
            {
                returnValue = GetCollisionCheckResult2D(
                    circle2D,
                    quadrangle2D,
                    3,
                    temp.mvRepulsionVecA,
                    temp.mvRepulsionVecB,
                    temp.mvHitPos,
                    temp.mvFloatBox);
            }
            else
            {
                VECTOR2D currentPos = VECTOR2D::GetNewrPosOnLine(circle2D->GetBasePos(), quadrangleRay.GetStartPos(), quadrangleRay.GetEndPos());
                if ((currentPos - circle2D->GetBasePos()).MagnitudeNoSqrt() < (returnValue.mvHitPos, circle2D->GetBasePos()).MagnitudeNoSqrt())
                {
                    returnValue = GetCollisionCheckResult2D(
                        circle2D,
                        quadrangle2D,
                        3,
                        temp.mvRepulsionVecA,
                        temp.mvRepulsionVecB,
                        temp.mvHitPos,
                        temp.mvFloatBox);
                }
            }
        }

        // 四角形の辺の一つをRayに変換する
        quadrangleRay.SetShapeParameter(quadrangle2D->GetVertexPos(3), quadrangle2D->GetVertexPos(2));

        // 辺の一つでも円に当たっていたらtrueを返す
        temp = BaseCollision2DManager::CheckHitCircle2DToRay2D(circle2D, &quadrangleRay);
        if (0 <= temp.mnResultParam)
        {
            if (returnValue.mnResultParam == -1)
            {
                returnValue = GetCollisionCheckResult2D(
                    circle2D,
                    quadrangle2D,
                    4,
                    temp.mvRepulsionVecA,
                    temp.mvRepulsionVecB,
                    temp.mvHitPos,
                    temp.mvFloatBox);
            }
            else
            {
                VECTOR2D currentPos = VECTOR2D::GetNewrPosOnLine(circle2D->GetBasePos(), quadrangleRay.GetStartPos(), quadrangleRay.GetEndPos());
                if ((currentPos, circle2D->GetBasePos()).MagnitudeNoSqrt() < (returnValue.mvHitPos - circle2D->GetBasePos()).MagnitudeNoSqrt())
                {
                    returnValue = GetCollisionCheckResult2D(
                        circle2D,
                        quadrangle2D,
                        4,
                        temp.mvRepulsionVecA,
                        temp.mvRepulsionVecB,
                        temp.mvHitPos,
                        temp.mvFloatBox);
                }
            }
        }


        // 四角形の辺の一つをRayに変換する
        quadrangleRay.SetShapeParameter(quadrangle2D->GetVertexPos(2), quadrangle2D->GetVertexPos(0));

        // 辺の一つでも円に当たっていたらtrueを返す
        temp = BaseCollision2DManager::CheckHitCircle2DToRay2D(circle2D, &quadrangleRay);
        if (0 <= temp.mnResultParam)
        {
            if (returnValue.mnResultParam == -1)
            {
                returnValue = GetCollisionCheckResult2D(
                    circle2D,
                    quadrangle2D,
                    5,
                    temp.mvRepulsionVecA,
                    temp.mvRepulsionVecB,
                    temp.mvHitPos,
                    temp.mvFloatBox);
            }
            else
            {
                VECTOR2D currentPos = VECTOR2D::GetNewrPosOnLine(circle2D->GetBasePos(), quadrangleRay.GetStartPos(), quadrangleRay.GetEndPos());
                if ((currentPos - circle2D->GetBasePos()).MagnitudeNoSqrt() < (returnValue.mvHitPos, circle2D->GetBasePos()).MagnitudeNoSqrt())
                {
                    returnValue = GetCollisionCheckResult2D(
                        circle2D,
                        quadrangle2D,
                        5,
                        temp.mvRepulsionVecA,
                        temp.mvRepulsionVecB,
                        temp.mvHitPos,
                        temp.mvFloatBox);
                }
            }
        }
    }

    // どの辺にも当たっていなかったのでfalseを返す
    return returnValue;
}

// 円とレイの内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitCircle2DToRay2D(Circle2D *circle2D, Ray2D *ray2D)
{
    // 返り値
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    // 下のdo文はbreakで一番下に行きたいだけのdo文なのでループはしません
    do
    {
        // 円の半径
        float radius = circle2D->GetBaseRadius();

        // 半径がマイナスはエラー（半径ゼロは許容）
        if (radius < 0.0f)
        {
            break;
        }
        if (ray2D->GetLineVec() == VECTOR2D(0.0f, 0.0f))
        {
            break;
        }

        // 円の中心点が原点になるように始点をオフセット
        VECTOR2D circleCenterPos = circle2D->GetBasePos();
        VECTOR2D center2start = ray2D->GetStartPos() - circleCenterPos;

        // 正規化したレイの方向ベクトルを取得
        VECTOR2D lineVecNorm = ray2D->GetLineVec().Normalize();

        // LVN : lineVecNormの略
        float c2s_LVNDot = VECTOR2D::Dot(center2start, lineVecNorm);
        float c2s_c2sDot = VECTOR2D::Dot(center2start, center2start);

        // 係数tを算出
        float s = c2s_LVNDot * c2s_LVNDot - c2s_c2sDot + radius * radius;
        if ((float)fabs((double)(s)) < 0.000001f)
        {
            s = 0.0f; // 誤差修正
        }
        if (s < 0.0f)
        {
            // そもそも当たっていない
            break;
        }
        float sq = sqrtf(s);
        float t1 = -c2s_LVNDot - sq;
        float t2 = -c2s_LVNDot + sq;

        // もしt1及びt2がマイナスだったら始点が
        // 円内にめり込んでいるのでエラーとする
        if (t1 < 0.0f || t2 < 0.0f)
        {
            break;
        }

        // 衝突座標を出力
        VECTOR2D hitPoint1(0.0f, 0.0f);        // outC1x, outC1y: 衝突点C1（近い方）の座標。必要無い場合はNULL
        VECTOR2D hitPoint2(0.0f, 0.0f);        // outC2x, outC2y: 衝突点C2（遠い方）の座標。必要無い場合はNULL
        hitPoint1 = (center2start + (lineVecNorm * t1)) + circleCenterPos;
        hitPoint2 = (center2start + (lineVecNorm * t2)) + circleCenterPos;

        // Rayの法線ベクトルを取得する
        VECTOR3D crossVec3D = VECTOR3D::Cross((ray2D->GetStartPos() - ray2D->GetEndPos()).To3D(0.0f), VECTOR3D(0.0f, 0.0f, 1.0f));
        VECTOR2D crossVec2D = crossVec3D.To2D('x', 'y');

        // 返り値を設定する
        temp = GetCollisionCheckResult2D(
            circle2D,
            ray2D,
            0,
            crossVec2D,
            ray2D->GetBasePos() - circle2D->GetBasePos(),
            hitPoint1,
            VECTOR2D::GetZero());
    } while (false);

    return temp;
}

// カプセルとカプセルの内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitCapsule2DToCapsule2D(Capsule2D *capsule2Da, Capsule2D *capsule2Db)
{
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    if (capsule2Da == nullptr ||
        capsule2Db == nullptr)
    {
        return temp;
    }

    /* 最終的にスフィアとスフィアの計算にする予定。 */
    // 計算用のスフィアの座標を記憶しておく変数
    VECTOR2D circle2DPos = VECTOR2D::GetZero();

    // スフィアとスフィアの当たり判定の計算で使う、スフィアのもとになるカプセル
    Capsule2D targetCapsule;

    // 計算用のスフィアからみた計算用のカプセルの線上最近点の世界座標を保持する変数
    VECTOR2D sphereToCapsuleNewrPointPos = VECTOR2D::GetZero();

    // 計算用のスフィアの半径
    float radius = 0.0f;

    // スフィアになってくれるカプセルの端を探している
    for (int i = 0; i < 4; i++)
    {
        VECTOR2D thisToTargetNewrPos = VECTOR2D::GetZero();
        VECTOR2D thisCapsulePos = VECTOR2D::GetZero();
        VECTOR2D targetCapsuleStartPos = VECTOR2D::GetZero();
        VECTOR2D targetCapsuleEndPos = VECTOR2D::GetZero();

        switch (i)
        {
        case 0:
            thisCapsulePos = capsule2Da->GetMovedPos(capsule2Da->GetStartPos());
            targetCapsuleStartPos = capsule2Db->GetMovedPos(capsule2Db->GetStartPos());
            targetCapsuleEndPos = capsule2Db->GetMovedPos(capsule2Db->GetEndPos());
            thisToTargetNewrPos = VECTOR2D::GetNewrPosOnLine(thisCapsulePos, targetCapsuleStartPos, targetCapsuleEndPos);
            break;
        case 1:
            thisCapsulePos = capsule2Da->GetMovedPos(capsule2Da->GetEndPos());
            targetCapsuleStartPos = capsule2Db->GetMovedPos(capsule2Db->GetStartPos());
            targetCapsuleEndPos = capsule2Db->GetMovedPos(capsule2Db->GetEndPos());
            thisToTargetNewrPos = VECTOR2D::GetNewrPosOnLine(thisCapsulePos, targetCapsuleStartPos, targetCapsuleEndPos);
            break;
        case 2:
            thisCapsulePos = capsule2Db->GetMovedPos(capsule2Db->GetStartPos());
            targetCapsuleStartPos = capsule2Da->GetMovedPos(capsule2Da->GetStartPos());
            targetCapsuleEndPos = capsule2Da->GetMovedPos(capsule2Da->GetEndPos());
            thisToTargetNewrPos = VECTOR2D::GetNewrPosOnLine(thisCapsulePos, targetCapsuleStartPos, targetCapsuleEndPos);
            break;
        case 3:
            thisCapsulePos = capsule2Db->GetMovedPos(capsule2Db->GetEndPos());
            targetCapsuleStartPos = capsule2Da->GetMovedPos(capsule2Da->GetStartPos());
            targetCapsuleEndPos = capsule2Da->GetMovedPos(capsule2Da->GetEndPos());
            thisToTargetNewrPos = VECTOR2D::GetNewrPosOnLine(thisCapsulePos, targetCapsuleStartPos, targetCapsuleEndPos);
            break;
        default:
            break;
        }

        // 直前で計算した点Pからカプセルの直線ABの線上最近点までのベクトルを持つ変数
        VECTOR2D nowThisToTargetNewrVec = VECTOR2D::GetZero();

        // これまでの計算で一番、点Pからカプセルの直線ABの線上最近点までのベクトルが小さかった時の結果を持つ変数
        VECTOR2D minThisToTargetNewrVec = sphereToCapsuleNewrPointPos - circle2DPos;

        // これまでの計算で求めた線上最近点が直線ABの中に入っているならtrue
        if (VECTOR2D::CheckPointInVector(thisToTargetNewrPos, targetCapsuleStartPos, targetCapsuleEndPos) == 1)
        {
            // 直前の計算からでた点Pから線上最近点へのベクトルをそのまま代入する。
            nowThisToTargetNewrVec = thisToTargetNewrPos - thisCapsulePos;
        }
        else
        {
            // 線上最近点をカプセルの両端のどちらかにしなければいけならず、線上最近点が-方向にある可能性も捨てきれない場合の処理
            switch (VECTOR2D::CheckAxisVecPlusOrMinus(
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
            circle2DPos = thisCapsulePos;

            // カプセルとスフィアの当たり判定の計算で使うカプセルを再設定する
            if (i <= 1)
            {
                targetCapsule = *capsule2Db;

                // 計算用のスフィアの半径を再設定する
                radius = capsule2Da->GetBaseRadius();

            }
            else /* 2 <= i */
            {
                targetCapsule = *capsule2Da;

                // 計算用のスフィアの半径を再設定する
                radius = capsule2Db->GetBaseRadius();
            }

            // 計算用のスフィアから見た、カプセルの線上最近点の座標を記憶しておく
            sphereToCapsuleNewrPointPos = thisToTargetNewrPos;
        }
    }

    // 計算用のスフィアを作る * 2
    Circle2D circle2Da;
    circle2Da.SetBasePos(circle2DPos);
    circle2Da.SetBaseRadiusSqrt(radius);
    Circle2D circle2Db;
    circle2Db.SetBasePos(sphereToCapsuleNewrPointPos);
    circle2Db.SetBaseRadiusSqrt(targetCapsule.GetBaseRadius());

    // スフィアとスフィアの計算をし、結果をtempに代入する
    temp = BaseCollision2DManager::CheckHitCircle2DToCircle2D(&circle2Da, &circle2Db);
    if (0 <= temp.mnResultParam)
    {
        temp = GetCollisionCheckResult2D(
            capsule2Da,
            capsule2Db,
            0,
            capsule2Da->GetBasePos() - capsule2Db->GetBasePos(),
            capsule2Db->GetBasePos() - capsule2Da->GetBasePos(),
            circle2DPos,
            sphereToCapsuleNewrPointPos);
    }

    // 計算結果を返す
    return temp;
}

// カプセルと三角形の内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitCapsule2DToTriangle2D(Capsule2D *capsule2D, Triangle2D *triangle2D)
{
    return GetCollisionCheckResult2DZero();
}

// カプセルと四角形の内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitCapsule2DToQuadrangle2D(Capsule2D *capsule2D, Quadrangle2D *quadrangle2D)
{
    return GetCollisionCheckResult2DZero();
}

// カプセルとレイの内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitCapsule2DToRay2D(Capsule2D *capsule2D, Ray2D *ray2D)
{
    // 返り値
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    return temp;
}

// 三角形と三角形の内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitTriangle2DToTriangle2D(Triangle2D *triangle2Da, Triangle2D *triangle2Db)
{
    CollisionCheckResult2D returnValue = GetCollisionCheckResult2DZero();
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    Ray2D triangleRay;
    triangleRay.SetMoveVec(triangle2Db->GetMoveVec());
    for (int i = 0; i < 3; i++)
    {
        int j = i + 1;
        if (3 <= j)
        {
            j -= 3;
        }

        // 三角形の辺の一つをRayに変換する
        triangleRay.SetShapeParameter(triangle2Db->GetVertexPos(i), triangle2Db->GetVertexPos(j));

        // 辺の一つでももう片方の三角形に当たっていたらtrueを返す
        temp = BaseCollision2DManager::CheckHitTriangle2DToRay2D(triangle2Da, &triangleRay);
        if (0 <= temp.mnResultParam)
        {
            if (returnValue.mnResultParam == -1)
            {
                returnValue = GetCollisionCheckResult2D(
                    triangle2Da,
                    triangle2Db,
                    (temp.mnResultParam * 3) + i + 1,
                    temp.mvRepulsionVecA,
                    temp.mvRepulsionVecB,
                    temp.mvHitPos,
                    temp.mvFloatBox);
            }
            else
            {
                // これまでで一番近かった衝突地点と今回の衝突地点と、どちらがより近かったかを比較する
                VECTOR2D minVec = returnValue.mvHitPos - triangleRay.GetStartPos();
                VECTOR2D tempVec = temp.mvHitPos - triangleRay.GetStartPos();
                if (tempVec.MagnitudeNoSqrt() < minVec.MagnitudeNoSqrt())
                {
                    returnValue = GetCollisionCheckResult2D(
                        triangle2Da,
                        triangle2Db,
                        (temp.mnResultParam * 3) + i + 1,
                        temp.mvRepulsionVecA,
                        temp.mvRepulsionVecB,
                        temp.mvHitPos,
                        temp.mvFloatBox);
                }
            }
        }
    }

    // どのへんにも当たっていなかったのでfalseを返す
    return returnValue;
}

// 三角形と四角形の内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitTriangle2DToQuadrangle2D(Triangle2D *triangle2D, Quadrangle2D *quadrangle2D)
{
    CollisionCheckResult2D returnValue = GetCollisionCheckResult2DZero();
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    Ray2D triangleRay;
    triangleRay.SetMoveVec(triangle2D->GetMoveVec());

    for (int i = 0; i < 3; i++)
    {
        int j = i + 1;
        if (3 <= j)
        {
            j -= 3;
        }

        // 三角形の辺の一つをRayに変換する
        triangleRay.SetShapeParameter(triangle2D->GetVertexPos(i), triangle2D->GetVertexPos(j));

        // 辺の一つでももう片方の三角形に当たっていたらtrueを返す
        temp = BaseCollision2DManager::CheckHitQuadrangle2DToRay2D(quadrangle2D, &triangleRay);
        if (0 <= temp.mnResultParam)
        {
            if (returnValue.mnResultParam == -1)
            {
                returnValue = GetCollisionCheckResult2D(
                    triangle2D,
                    quadrangle2D,
                    (temp.mnResultParam * 3) + i + 1,
                    temp.mvRepulsionVecB,
                    temp.mvRepulsionVecA,
                    temp.mvHitPos,
                    temp.mvFloatBox);
            }
            else
            {
                // これまでで一番近かった衝突地点と今回の衝突地点と、どちらがより近かったかを比較する
                VECTOR2D minVec = returnValue.mvHitPos - triangleRay.GetStartPos();
                VECTOR2D tempVec = temp.mvHitPos - triangleRay.GetStartPos();
                if (tempVec.MagnitudeNoSqrt() < minVec.MagnitudeNoSqrt())
                {
                    returnValue = GetCollisionCheckResult2D(
                        triangle2D,
                        quadrangle2D,
                        (temp.mnResultParam * 3) + i + 1,
                        temp.mvRepulsionVecB,
                        temp.mvRepulsionVecA,
                        temp.mvHitPos,
                        temp.mvFloatBox);
                }
            }
        }
    }

    // どのへんにも当たっていなかったのでfalseを返す
    return returnValue;
}

CollisionCheckResult2D BaseCollision2DManager::CheckHitTriangle2DToRay2D(Triangle2D *triangle2D, Ray2D *ray2D)
{
    // 返り値
    CollisionCheckResult2D returnValue = GetCollisionCheckResult2DZero();

    // 関数の返り値を受け取る変数
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    // 作業用変数
    Ray2D triangleRay;

    // 三角形の辺の一つ一つをRayに変換し、RayとRayの当たり判定の計算を行うループ
    for (int i = 0; i < 3; i++)
    {
        int j = 1 + 1;
        if (3 <= j)
        {
            j -= 3;
        }

        // 三角形の辺をRayに変換する
        triangleRay.SetShapeParameter(triangle2D->GetVertexPos(i), triangle2D->GetVertexPos(j));

        // RayとRayの当たり判定の計算を行う
        temp = BaseCollision2DManager::CheckHitRay2DToRay2D(&triangleRay, ray2D);

        // 三角形の辺と自分のRayが当たっている場合は特別な処理を行う
        if (0 <= temp.mnResultParam)
        {
            signed long resultParam = i + 1;
            if (returnValue.mnResultParam == -1)
            {
                returnValue = GetCollisionCheckResult2D(
                    triangle2D,
                    ray2D,
                    resultParam,
                    temp.mvRepulsionVecA,
                    temp.mvRepulsionVecB,
                    temp.mvHitPos,
                    temp.mvFloatBox);
            }
            else
            {
                // これまでで一番近かった衝突地点と今回の衝突地点と、どちらがより近かったかを比較する
                VECTOR2D minVec = returnValue.mvHitPos - ray2D->GetStartPos();
                VECTOR2D tempVec = temp.mvHitPos - ray2D->GetStartPos();
                if (tempVec.MagnitudeNoSqrt() < minVec.MagnitudeNoSqrt())
                {
                    // 今回の衝突地点を一番近かった衝突地点として保存しておく
                    returnValue = GetCollisionCheckResult2D(
                        triangle2D,
                        ray2D,
                        resultParam,
                        temp.mvRepulsionVecA,
                        temp.mvRepulsionVecB,
                        temp.mvHitPos,
                        temp.mvFloatBox);
                }
            }
        }
    }

    // 計算結果を返す
    return returnValue;
}

// 四角形と四角形の内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitQuadrangle2DToQuadrangle2D(Quadrangle2D *quadrangle2Da, Quadrangle2D *quadrangle2Db)
{
    CollisionCheckResult2D returnValue = GetCollisionCheckResult2DZero();
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    Ray2D quadrangleRay;
    quadrangleRay.SetMoveVec(quadrangle2Db->GetMoveVec());

    // 四角形の辺の一つをRayに変換する
    quadrangleRay.SetShapeParameter(quadrangle2Db->GetVertexPos(0), quadrangle2Db->GetVertexPos(1));

    // 辺の一つでももう片方の三角形に当たっていたらtrueを返す
    temp = BaseCollision2DManager::CheckHitQuadrangle2DToRay2D(quadrangle2Da, &quadrangleRay);
    if (0 <= temp.mnResultParam)
    {
        returnValue = GetCollisionCheckResult2D(
            quadrangle2Da,
            quadrangle2Db,
            (temp.mnResultParam * 4) + (0 * 4) + 1,
            temp.mvRepulsionVecA,
            temp.mvRepulsionVecB,
            temp.mvHitPos,
            temp.mvFloatBox);
    }

    // 四角形の辺の一つをRayに変換する
    quadrangleRay.SetShapeParameter(quadrangle2Db->GetVertexPos(1), quadrangle2Db->GetVertexPos(3));

    // 辺の一つでももう片方の三角形に当たっていたらtrueを返す
    temp = BaseCollision2DManager::CheckHitQuadrangle2DToRay2D(quadrangle2Da, &quadrangleRay);
    if (0 <= temp.mnResultParam)
    {
        if (returnValue.mnResultParam == -1)
        {
            returnValue = GetCollisionCheckResult2D(
                quadrangle2Da,
                quadrangle2Db,
                (temp.mnResultParam * 4) + (1 * 4) + 1,
                temp.mvRepulsionVecA,
                temp.mvRepulsionVecB,
                temp.mvHitPos,
                temp.mvFloatBox);
        }
        else
        {
            // これまでで一番近かった衝突地点と今回の衝突地点と、どちらがより近かったかを比較する
            VECTOR2D minVec = returnValue.mvHitPos - quadrangle2Da->GetBasePos();
            VECTOR2D tempVec = temp.mvHitPos - quadrangle2Da->GetBasePos();
            if (tempVec.MagnitudeNoSqrt() < minVec.MagnitudeNoSqrt())
            {
                returnValue = GetCollisionCheckResult2D(
                    quadrangle2Da,
                    quadrangle2Db,
                    (temp.mnResultParam * 4) + (1 * 4) + 1,
                    temp.mvRepulsionVecA,
                    temp.mvRepulsionVecB,
                    temp.mvHitPos,
                    temp.mvFloatBox);
            }
        }
    }

    // 四角形の辺の一つをRayに変換する
    quadrangleRay.SetShapeParameter(quadrangle2Da->GetVertexPos(3), quadrangle2Da->GetVertexPos(2));

    // 辺の一つでももう片方の三角形に当たっていたらtrueを返す
    temp = BaseCollision2DManager::CheckHitQuadrangle2DToRay2D(quadrangle2Da, &quadrangleRay);
    if (0 <= temp.mnResultParam)
    {
        if (returnValue.mnResultParam == -1)
        {
            returnValue = GetCollisionCheckResult2D(
                quadrangle2Da,
                quadrangle2Db,
                (temp.mnResultParam * 4) + (2 * 4) + 1,
                temp.mvRepulsionVecA,
                temp.mvRepulsionVecB,
                temp.mvHitPos,
                temp.mvFloatBox);
        }
        else
        {
            // これまでで一番近かった衝突地点と今回の衝突地点と、どちらがより近かったかを比較する
            VECTOR2D minVec = returnValue.mvHitPos - quadrangle2Da->GetBasePos();
            VECTOR2D tempVec = temp.mvHitPos - quadrangle2Da->GetBasePos();
            if (tempVec.MagnitudeNoSqrt() < minVec.MagnitudeNoSqrt())
            {
                returnValue = GetCollisionCheckResult2D(
                    quadrangle2Da,
                    quadrangle2Db,
                    (temp.mnResultParam * 4) + (2 * 4) + 1,
                    temp.mvRepulsionVecA,
                    temp.mvRepulsionVecB,
                    temp.mvHitPos,
                    temp.mvFloatBox);
            }
        }
    }

    // 四角形の辺の一つをRayに変換する
    quadrangleRay.SetShapeParameter(quadrangle2Db->GetVertexPos(2), quadrangle2Db->GetVertexPos(0));

    // 辺の一つでももう片方の三角形に当たっていたらtrueを返す
    temp = BaseCollision2DManager::CheckHitQuadrangle2DToRay2D(quadrangle2Da, &quadrangleRay);
    if (0 <= temp.mnResultParam)
    {
        if (returnValue.mnResultParam == -1)
        {
            returnValue = GetCollisionCheckResult2D(
                quadrangle2Da,
                quadrangle2Db,
                (temp.mnResultParam * 4) + (3 * 4) + 1,
                temp.mvRepulsionVecA,
                temp.mvRepulsionVecB,
                temp.mvHitPos,
                temp.mvFloatBox);
        }
        else
        {
            // これまでで一番近かった衝突地点と今回の衝突地点と、どちらがより近かったかを比較する
            VECTOR2D minVec = returnValue.mvHitPos - quadrangle2Da->GetBasePos();
            VECTOR2D tempVec = temp.mvHitPos - quadrangle2Da->GetBasePos();
            if (tempVec.MagnitudeNoSqrt() < minVec.MagnitudeNoSqrt())
            {
                returnValue = GetCollisionCheckResult2D(
                    quadrangle2Da,
                    quadrangle2Db,
                    (temp.mnResultParam * 4) + (3 * 4) + 1,
                    temp.mvRepulsionVecA,
                    temp.mvRepulsionVecB,
                    temp.mvHitPos,
                    temp.mvFloatBox);
            }
        }
    }

    return returnValue;
}

// 四角形とレイの内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitQuadrangle2DToRay2D(Quadrangle2D *quadrangle2D, Ray2D *ray2D)
{
    // 返り値
    CollisionCheckResult2D returnValue = GetCollisionCheckResult2DZero();

    // 関数の返り値を受け取る変数
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    // 作業用変数
    Ray2D quadrangleRay;

    // 四角形の辺をRayに変換する
    quadrangleRay.SetShapeParameter(quadrangle2D->GetVertexPos(0), quadrangle2D->GetVertexPos(1));

    // 四角形の辺と自分のRayが当たっている場合は特別な処理を行う
    temp = BaseCollision2DManager::CheckHitRay2DToRay2D(&quadrangleRay, ray2D);
    if (0 <= temp.mnResultParam)
    {
        returnValue = GetCollisionCheckResult2D(
            quadrangle2D,
            ray2D,
            1,
            temp.mvRepulsionVecA,
            temp.mvRepulsionVecB,
            temp.mvHitPos,
            temp.mvFloatBox);
    }

    // 四角形の辺をRayに変換する
    quadrangleRay.SetShapeParameter(quadrangle2D->GetVertexPos(1), quadrangle2D->GetVertexPos(3));

    // 四角形の辺と自分のRayが当たっている場合は特別な処理を行う
    temp = BaseCollision2DManager::CheckHitRay2DToRay2D(&quadrangleRay, ray2D);
    if (0 <= temp.mnResultParam)
    {
        if (returnValue.mnResultParam == -1)
        {
            returnValue = GetCollisionCheckResult2D(
                quadrangle2D,
                ray2D,
                2,
                temp.mvRepulsionVecA,
                temp.mvRepulsionVecB,
                temp.mvHitPos,
                temp.mvFloatBox);
        }
        else
        {
            // これまでで一番近かった衝突地点と今回の衝突地点と、どちらがより近かったかを比較する
            VECTOR2D minVec = returnValue.mvHitPos - ray2D->GetStartPos();
            VECTOR2D tempVec = temp.mvHitPos - ray2D->GetStartPos();
            if (tempVec.MagnitudeNoSqrt() < minVec.MagnitudeNoSqrt())
            {
                // 今回の衝突地点を一番近かった衝突地点として保存しておく
                returnValue = GetCollisionCheckResult2D(
                    quadrangle2D,
                    ray2D,
                    2,
                    temp.mvRepulsionVecA,
                    temp.mvRepulsionVecB,
                    temp.mvHitPos,
                    temp.mvFloatBox);
            }
        }
    }

    // 四角形の辺をRayに変換する
    quadrangleRay.SetShapeParameter(quadrangle2D->GetVertexPos(3), quadrangle2D->GetVertexPos(2));

    // 四角形の辺と自分のRayが当たっている場合は特別な処理を行う
    temp = BaseCollision2DManager::CheckHitRay2DToRay2D(&quadrangleRay, ray2D);
    if (0 <= temp.mnResultParam)
    {
        if (returnValue.mnResultParam == -1)
        {
            returnValue = GetCollisionCheckResult2D(
                quadrangle2D,
                ray2D,
                3,
                temp.mvRepulsionVecA,
                temp.mvRepulsionVecB,
                temp.mvHitPos,
                temp.mvFloatBox);
        }
        else
        {
            // これまでで一番近かった衝突地点と今回の衝突地点と、どちらがより近かったかを比較する
            VECTOR2D minVec = returnValue.mvHitPos - ray2D->GetStartPos();
            VECTOR2D tempVec = temp.mvHitPos - ray2D->GetStartPos();
            if (tempVec.MagnitudeNoSqrt() < minVec.MagnitudeNoSqrt())
            {
                // 今回の衝突地点を一番近かった衝突地点として保存しておく
                returnValue = GetCollisionCheckResult2D(
                    quadrangle2D,
                    ray2D,
                    3,
                    temp.mvRepulsionVecA,
                    temp.mvRepulsionVecB,
                    temp.mvHitPos,
                    temp.mvFloatBox);
            }
        }
    }

    // 四角形の辺をRayに変換する
    quadrangleRay.SetShapeParameter(quadrangle2D->GetVertexPos(2), quadrangle2D->GetVertexPos(0));

    // 四角形の辺と自分のRayが当たっている場合は特別な処理を行う
    temp = BaseCollision2DManager::CheckHitRay2DToRay2D(&quadrangleRay, ray2D);
    if (0 <= temp.mnResultParam)
    {
        if (returnValue.mnResultParam == -1)
        {
            returnValue = GetCollisionCheckResult2D(
                quadrangle2D,
                ray2D,
                4,
                temp.mvRepulsionVecA,
                temp.mvRepulsionVecB,
                temp.mvHitPos,
                temp.mvFloatBox);
        }
        else
        {
            // これまでで一番近かった衝突地点と今回の衝突地点と、どちらがより近かったかを比較する
            VECTOR2D minVec = returnValue.mvHitPos - ray2D->GetStartPos();
            VECTOR2D tempVec = temp.mvHitPos - ray2D->GetStartPos();
            if (tempVec.MagnitudeNoSqrt() < minVec.MagnitudeNoSqrt())
            {
                // 今回の衝突地点を一番近かった衝突地点として保存しておく
                returnValue = GetCollisionCheckResult2D(
                    quadrangle2D,
                    ray2D,
                    4,
                    temp.mvRepulsionVecA,
                    temp.mvRepulsionVecB,
                    temp.mvHitPos,
                    temp.mvFloatBox);
            }
        }
    }

    // 計算結果を返す
    return returnValue;
}

CollisionCheckResult2D BaseCollision2DManager::CheckHitRay2DToRay2D(Ray2D *ray2Da, Ray2D *ray2Db)
{
    // 返り値
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    // 下のdo文はbreakで一番下に行きたいだけのdo文なのでループはしません
    do
    {
        float Crs_v1_v2 = VECTOR2D::Cross(ray2Da->GetLineVec(), ray2Db->GetLineVec());
        if (Crs_v1_v2 == 0.0f)
        {
            // 平行状態
            break;
        }

        VECTOR2D aStart2bStart = ray2Db->GetStartPos() - ray2Da->GetStartPos();
        float Crs_v_v1 = VECTOR2D::Cross(aStart2bStart, ray2Da->GetLineVec());
        float Crs_v_v2 = VECTOR2D::Cross(aStart2bStart, ray2Db->GetLineVec());

        float t1 = Crs_v_v2 / Crs_v1_v2;
        float t2 = Crs_v_v1 / Crs_v1_v2;

        // 余裕を少しだけ持たせて、交差しているかを確認する
        const float eps = 0.00001f;
        if (t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1)
        {
            // 交差していない
            break;
        }

        // 線分の内分比なるもの(よくわかってない)
        VECTOR2D floatBox = VECTOR2D(t1, t2);

        // Rayの法線ベクトルを取得する
        VECTOR3D aCrossVec3D = VECTOR3D::Cross((ray2Da->GetStartPos() - ray2Da->GetEndPos()).To3D(0.0f), VECTOR3D(0.0f, 0.0f, 1.0f));
        VECTOR2D aCrossVec2D = aCrossVec3D.To2D('x', 'y');
        VECTOR2D a2b = ray2Db->GetBasePos() - ray2Da->GetBasePos();
        aCrossVec2D = VECTOR2D::GetTargetDirectionNewrVec(a2b, aCrossVec2D, -aCrossVec2D);

        // Rayの法線ベクトルを取得する
        VECTOR3D bCrossVec3D = VECTOR3D::Cross((ray2Db->GetStartPos() - ray2Db->GetEndPos()).To3D(0.0f), VECTOR3D(0.0f, 0.0f, 1.0f));
        VECTOR2D bCrossVec2D = bCrossVec3D.To2D('x', 'y');
        VECTOR2D b2a = ray2Db->GetBasePos() - ray2Da->GetBasePos();
        bCrossVec2D = VECTOR2D::GetTargetDirectionNewrVec(b2a, bCrossVec2D, -bCrossVec2D);

        // 当たったことを記憶しておく
        temp = GetCollisionCheckResult2D(
            ray2Da,
            ray2Db,
            0,
            bCrossVec2D,
            aCrossVec2D,
            VECTOR2D::GetZero(),
            floatBox);

        // ↓ここをSetMagnitude()にしていると、当たった地点がthisのRayの始点に極端に近くなる。これが角のめり込みの原因でした。
        // temp.mvHitPos = VAdd2D(this->mvStartPos, VSetMagnitude2D(this->GetLineVec(), t1));
        temp.mvHitPos = ray2Da->GetStartPos() + (ray2Da->GetLineVec() * t1);
    } while (false);

    // 結果を返す
    return temp;
}

// 当たり判定の寿命に関する更新処理を行う関数
int BaseCollision2DManager::UpdateCollisionLife()
{
    // 今回のループで確認する当たり判定
    BaseCollision2D *current = this->mpFirstBaseCollision2D;

    // 次のループで確認する当たり判定
    BaseCollision2D *next = nullptr;

    // 作業用変数
    BaseCollision *listPrev = nullptr;
    BaseCollision *listNext = nullptr;
    BaseCollision2D *worldPrev = nullptr;
    BaseCollision2D *worldNext = nullptr;

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

BaseCollision2DManager::BaseCollision2DManager() :
    mpFirstBaseCollision2D(nullptr)
{

}

BaseCollision2DManager::~BaseCollision2DManager()
{
    this->IsolateAll();
}

int BaseCollision2DManager::Initialize()
{
    BaseCollision2D *current = mpFirstBaseCollision2D;
    while (current != nullptr)
    {
        current = current->GetNextWorld();
    }

    return 0;
}

int BaseCollision2DManager::Finalize()
{
    BaseCollision2D *current = mpFirstBaseCollision2D;
    while (current != nullptr)
    {
        current = current->GetNextWorld();
    }

    return 0;
}

// ワールドに登録されている当たり判定とワールドに登録されている当たり判定を移動込みの総当たりで計算し、スライド移動も行う関数(カリング処理 : 移動ベクトル、基準円)
int BaseCollision2DManager::CheckHitAllMove()
{
    if (this->mpFirstBaseCollision2D == nullptr ||
        this->mpFirstBaseCollision2D->GetNextWorld() == nullptr)
    {
        return 0;
    }

    // 関数の返り値を保持するための変数
    CollisionCheckResult2D temp;

    // 移動する当たり判定を取得する
    for (BaseCollision2D *current = this->mpFirstBaseCollision2D; current != nullptr; current = current->GetNextWorld())
    {
        // 後者のオブジェクトを取得する
        for (BaseCollision2D *target = current->GetNextWorld(); target != nullptr; target = target->GetNextWorld())
        {
            // 二つの当たり判定の削除フラグと有効フラグを確認し、問題があった場合は処理を飛ばす
            if (current->GetDeleteFlag() ||
                target->GetDeleteFlag() ||
                current->GetActiveFlag() == false ||
                target->GetActiveFlag() == false)
            {
                continue;
            }

            // 円のカリングを含めた当たり細かい判定の計算を行う
            temp = BaseCollision2DManager::CheckHitCollision2DToCollision2D(current, target);

            // 当たっている場合は処理を行う
            if (0 <= temp.mnResultParam)
            {
                // 二つの当たり判定のスライド移動を行う
                BaseCollision2DManager::CollisionSlide(current, target, temp);

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

// 
int BaseCollision2DManager::SearchTag(CollisionTag tag, std::vector<BaseCollision2D *> &out)
{
    // 
    uint32_t count = 0;

    // 
    BaseCollision2D *current = this->mpFirstBaseCollision2D;
    while (current != nullptr)
    {
        // 
        if (current->CheckProcessingFlag(tag))
        {
            // 
            count++;
        }
    }
    
    // 
    out.resize(count, nullptr);

    // 
    count = 0;

    // 
    while (current != nullptr)
    {
        // 
        if (out.size() <= count)
        {
            // 
            break;
        }

        // 
        if (current->CheckProcessingFlag(tag))
        {
            // 
            out[count] = current;

            // 
            count++;
        }
    }

    // 
    out;

    // 
    return 0;
}

// 謎の当たり判定と謎の当たり判定の内外判定と衝突判定を行う関数
CollisionCheckResult2D BaseCollision2DManager::CheckHitCollision2DToCollision2D(BaseCollision2D *collision2DA, BaseCollision2D *collision2DB)
{
    // 返り値を保持する変数
    CollisionCheckResult2D temp = GetCollisionCheckResult2DZero();

    // 片方でもnullptrだった場合は計算ができないので
    if (collision2DA == nullptr || collision2DB == nullptr)
    {
        return temp;
    }

    // カリング処理をまとめて行い、飛ばすべきでない場合のみ詳しい計算を行う
    if (BaseCollision2DManager::CheckCulling(collision2DA, collision2DB) == false)
    {
        // まず片方のコリジョンタイプにあった関数に飛ぶ
        switch (collision2DA->GetCollisionType())
        {
        case CollisionType::CollisionType_Point2D:
            temp = BaseCollision2DManager::CheckHitPoint2DToCollision2D(dynamic_cast<Point2D *>(collision2DA), collision2DB);
            break;
        case CollisionType::CollisionType_Circle2D:
            temp = BaseCollision2DManager::CheckHitCircle2DToCollision2D(dynamic_cast<Circle2D *>(collision2DA), collision2DB);
            break;
        case CollisionType::CollisionType_Capsule2D:
            temp = BaseCollision2DManager::CheckHitCapsule2DToCollision2D(dynamic_cast<Capsule2D *>(collision2DA), collision2DB);
            break;
        case CollisionType::CollisionType_Triangle2D:
            temp = BaseCollision2DManager::CheckHitTriangle2DToCollision2D(dynamic_cast<Triangle2D *>(collision2DA), collision2DB);
            break;
        case CollisionType::CollisionType_Quadrangle2D:
            temp = BaseCollision2DManager::CheckHitQuadrangle2DToCollision2D(dynamic_cast<Quadrangle2D *>(collision2DA), collision2DB);
            break;
        default:
            break;
        }
    }

    // 計算結果を返す
    return temp;
}

/* 線形リスト関連の関数 */
int BaseCollision2DManager::Add(BaseCollision2D *target)
{
    // そもそもtargetがnullの場合
    if (target == nullptr)
    {
        return -1;
    }
    // mpCollisionすらない場合
    if (mpFirstBaseCollision2D == nullptr)
    {
        mpFirstBaseCollision2D = target;
        return 0;
    }

    BaseCollision2D *current = mpFirstBaseCollision2D;
    // 最後尾を見つける
    while (current->GetNextWorld() != nullptr)
    {
        current = current->GetNextWorld();
    }

    return ConnectTarget(current, target, nullptr);
}

int BaseCollision2DManager::ConnectTarget(BaseCollision2D *prev, BaseCollision2D *target, BaseCollision2D *next)
{
    if (prev != nullptr)
    {
        prev->SetNextWorld(target);
    }
    if (next != nullptr)
    {
        next->SetPrevWorld(target);
    }
    if (next == mpFirstBaseCollision2D)
    {
        mpFirstBaseCollision2D = target;
    }

    target->SetPrevWorld(prev);
    target->SetNextWorld(next);

    return 0;
}

int BaseCollision2DManager::IsolateTarget(BaseCollision2D *target)
{
    if (mpFirstBaseCollision2D == nullptr)
    {
        return 0;
    }

    // 作業用変数を用意
    BaseCollision2D *prev = target->GetPrevWorld();
    BaseCollision2D *next = target->GetNextWorld();

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

    if (target == mpFirstBaseCollision2D)
    {
        // 自分が最初のコリジョンだった場合は一つ後のコリジョンを最初のコリジョンとする
        mpFirstBaseCollision2D = next;
    }

    target->SetPrevWorld(nullptr);
    target->SetNextWorld(nullptr);

    return 0;
}

int BaseCollision2DManager::IsolateAll()
{
    if (this->mpFirstBaseCollision2D == nullptr)
    {
        return 0;
    }

    BaseCollision2D *target = mpFirstBaseCollision2D;
    BaseCollision2D *next = target->GetNextWorld();

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

    return 0;
}