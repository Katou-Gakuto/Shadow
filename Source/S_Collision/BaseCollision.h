#pragma once

#include "../Y_Tool/CountDownSystem.h"
#include "../V_Display/Material2D.h"
#include "../Y_Tool/BitFlag.h"

#include "CollisionCheckResult.h"

enum CollisionTag : unsigned short
{
    CollisionTag_Checker = 0,                   // 計算用の当たり判定。無条件で全ての当たり判定と計算をする
    CollisionTag_Wall,                          // 壁の当たり判定
    CollisionTag_Ground,                        // 地面の当たり判定
    CollisionTag_Mirror,                        // 鏡面の当たり判定
    CollisionTag_CharaBody,                     // キャラクターの当たり判定
    CollisionTag_Attack,                        // 攻撃の当たり判定
    CollisionTag_Max,                           // この列挙子の数の合計


    // 以下、現在は使用していないもの
    CollisionTag_SoundLine,                     
};

enum CollisionType : unsigned char
{
    CollisionType_Point2D = 0,
    CollisionType_Circle2D,
    CollisionType_Capsule2D,
    CollisionType_Triangle2D,
    CollisionType_Quadrangle2D,
    CollisionType_Ray2D,

    CollisionType_Point3D,
    CollisionType_Sphere3D,
    CollisionType_Capsule3D,
    CollisionType_Triangle3D,
    CollisionType_Quadrangle3D,
    CollisionType_Cube3D,
    CollisionType_Ray3D,

    CollisionType_Max,
};

enum CollisionNorm : unsigned char
{
    CollisionNorm_In = 0,
    CollisionNorm_Out = 1,
};

enum CollisionDimension : unsigned char
{
    CollisionDimension_2D = 0,
    CollisionDimension_3D,
    CollisionDimension_Max,
};

class GameObject;
class GameObject2D;
class GameObject3D;

// 2D、3Dを問わない当たり判定の基本的な構造
class BaseCollision
{
private:
    GameObject *mpMyObject;                     // この当たり判定はどのゲームオブジェクトが持っているのか
    CollisionTag mnCollisionTag;                // この当たり判定の用途はどういうものか
    CollisionType mnCollisionType;              // この当たり判定はどんな図形なのか
    CollisionNorm mnNormNum;                    // この当たり判定の法線ベクトルが内側か外側か
    unsigned long mnCollisionListNum;           // この当たり判定がリスト内のどの当たり判定なのかの識別番号
    bool mbDeleteFlag;                          // この当たり判定を消去していいのか
    bool mbActiveFlag;                          // この当たり判定は現在有効なのか
    bool mbPenetrate;                           // この当たり判定は貫通するのか
    bool mbHitMove;                             // この当たり判定は他の当たり判定と衝突した際に反発移動するのか
    float mfMoveLate;                           // この当たり判定は他の当たり判定と当たった際の反発移動の量。数が大きいほど動く(0.0f～1.0f)いわばスライド時の法線ベクトルの取得量みたいな？
    CountDownSystem mnDeleteCountDown;          // この当たり判定が消えるまでのカウントダウン(他の当たり判定との計算を行うごとに1ずつ減っていく)
    BitFlag mbProcessingFlag;                   // 当たり判定の計算をする対象を選別するための仕組み
    BaseCollision *mpPrevList;                  // 前の当たり判定へのポインタ(オブジェクト線形リスト)
    BaseCollision *mpNextList;                  // 次の当たり判定へのポインタ(オブジェクト線形リスト)

public:
    BaseCollision(
        GameObject *object, CollisionType type, CollisionTag tag, CollisionNorm normNum,
        bool penetrate, bool hitMove, float moveLate);
    virtual ~BaseCollision();

    int HitOnCollision(BaseCollision *hitCollision);
    int AddProcessingTag(CollisionTag tag);
    bool CheckProcessingFlag(CollisionTag tag);
    CollisionDimension GetCollisionDimension()const;
    int SetLife(unsigned long life);
    int UpdateLife();

    virtual int Draw(const Material2D &color);
    virtual int SetPosToMoveVec();

    virtual int SetMoveVec(void *moveData) = 0;
    virtual int SetNextPos(void *posData) = 0;
    virtual int WorldConnectMySelf() = 0;
    virtual int WorldIsolateMySelf() = 0;

    void SetListNum(unsigned long number)                   { this->mnCollisionListNum = number; }
    void SetActiveFlag(bool active)                         { this->mbActiveFlag = active; }
    void OnDeleteFlag()                                     { this->mbDeleteFlag = true; }
    void SetPenetrate(bool flag)                            { this->mbPenetrate = flag; }

    GameObject *GetMyObject()                       const   { return this->mpMyObject; }
    GameObject2D *GetMyObject2D() const;
    GameObject3D *GetMyObject3D() const;

    CollisionType GetCollisionType()                const   { return this->mnCollisionType; }
    CollisionTag GetCollisionTag()                  const   { return this->mnCollisionTag; }
    CollisionNorm GetCollisionNorm()                const   { return this->mnNormNum; }
    bool CheckCollisionNormIn()                     const   { return this->mnNormNum == CollisionNorm::CollisionNorm_In; }
    bool CheckCollisionNormOut()                    const   { return this->mnNormNum == CollisionNorm::CollisionNorm_Out; }

    unsigned long GetListNum()                      const   { return this->mnCollisionListNum; }
    bool GetActiveFlag()                            const   { return this->mbActiveFlag; }
    bool GetDeleteFlag()                            const   { return this->mbDeleteFlag; }
    bool GetPenetrate()                             const   { return this->mbPenetrate; }
    bool GetHitMove()                               const   { return this->mbHitMove; }
    float GetMoveLate()                             const   { return this->mfMoveLate; }

    void SetPrevList(BaseCollision *prev)                   { this->mpPrevList = prev; }
    void SetNextList(BaseCollision *next)                   { this->mpNextList = next; }
    BaseCollision *GetPrevList()                    const   { return this->mpPrevList; }
    BaseCollision *GetNextList()                    const   { return this->mpNextList; }
};