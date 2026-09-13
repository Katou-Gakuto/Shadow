#pragma once

#include "../B_BehaviorTree/BehaviorAccessData.h"

#include "../Y_Tool/CreateFlag.h"

// プレイヤーなのか、壁なのか、いったい何なのか
enum GameObjectTag : unsigned long
{
    GOT_TPSCamera = 0,
    GOT_FPSCamera,
    GOT_ScrollCamera,
    GOT_Fence,
    GOT_LineMap,
    GOT_SoundVisualizer,
    GOT_TestEmitter,

    GOT_Obstacle,
    GOT_Skyscraper,

    GOT_PowerSpot,

    GOT_Player,
    GOT_Enemy,
    GOT_OtherPlayer,

    GOT_Max,
};

class BaseModel;
class BaseModelList;
class BaseCollision;
class BaseCollisionList;
class MonoBehavior;
class BehaviorNode;
class BehaviorTree;
class BaseComponent;
class BaseComponentList;

// 2D、3Dを問わないオブジェクトの基本的な構造を持ったクラス
class GameObject
{
private:
    typedef enum GameObjectFlagBit : unsigned char
    {
        GOBF_ModelCreate = 1 << 0,
        GOBF_CollisionListCreate = 1 << 1,
        GOBF_BehaviorCreate = 1 << 2,
        GOBF_ComponentCreate = 1 << 3,
        GOBF_DeleteFlag = 1 << 4,
        GOBF_ActiveFlag = 1 << 5,
        GOBF_StopFlag = 1 << 6,
        GOBF_DrawFlag = 1 << 7,
    } GOBF;

    GameObjectTag mnObjectTag;
    CreateFlag mbCreateFlag;
    unsigned char mcBitFlag;

    BehaviorAccessData mdInitBehavior;

    BaseModelList *mpModelList;
    BaseCollisionList *mpBaseCollisionList;
    BehaviorTree *mpBehaviorTree;
    BaseComponentList *mpBaseComponentList;

    GameObject *mpPrev;
    GameObject *mpNext;

    void OnModelCreateFlag();
    void OnCollisionListCreateFlag();
    void OnBehaviorCreateFlag();
    void OnComponentCreateFlag();
    void SetDeleteBit(bool flag);
    void SetActiveBit(bool flag);
    void SetStopBit(bool flag);
    void SetDrawBit(bool flag);

public:
    GameObject(GameObjectTag tag);
    virtual ~GameObject();

    virtual int Create();
    int Initialize();
    int Finalize();

    int CollisionActivate();
    int CollisionDeactivate();

    virtual int ResetMoveVec() = 0;                                                         // 移動ベクトルを0ベクトルに設定する
    int EarlyUpdate();                                                                      // 通常更新の前にしなければいけない何か
    int Update();                                                                           // 通常更新
    int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision);            // 当たり判定計算でオブジェクトと当たった場合の処理
    virtual int SetPositionToMoveVec() = 0;                                                 // 当たり判定計算後のオブジェクトの座標の確定
    virtual int SetCollisionPosToCollisionMoveVec() = 0;                                    // 当たり判定計算後の当たり判定の座標の確定
    int LateUpdate();                                                                       // 当たり判定計算後の更新
    int DeleteToFlagMember();                                                               // 削除フラグの立っている当たり判定を削除する
    int UpdateModel();                                                                      // モデルの更新
    int ObjectDraw();                                                                       // 描画関連の関数を実行する関数
    int Draw();                                                                             // モデルの描画
    virtual int UniqueDraw();                                                               // この継承先オブジェクトでモデルを使用しない描画を行うときの関数

    void OnDeleteFlag();
    void SetActiveFlag(bool active);
    void SetStopFlag(bool stop);
    void SetDrawFlag(bool draw);
    int CreateObjectModelList();
    int CreateObjectCollisionList();
    int CreateObjectBehavior(const BehaviorAccessData &initAccessData);
    int CreateObjectComponentList();
    void SetInitBehavior(const BehaviorAccessData &accessData);

    GameObjectTag GetObjectTag() const;
    bool GetModelCreateFlag() const;
    bool GetCollisionListCreateFlag() const;
    bool GetBehaviorCreateFlag() const;
    bool GetComponentCreateFlag() const;
    bool GetDeleteFlag() const;
    bool GetActiveFlag() const;
    bool GetStopFlag() const;
    bool GetDrawFlag() const;
    BaseModelList *GetModelList()const;
    BaseCollisionList *GetBaseCollisionList()const;
    BehaviorTree *GetBehaviorTree()const;
    BaseComponentList *GetBaseComponentList()const;

    int AddModel(BaseModel *model, unsigned long number);
    int AddCollision(BaseCollision *collision, unsigned long number);
    int AddBehaviorNode(BehaviorNode *node, unsigned long number);
    int AddMonoBehavior(MonoBehavior *mono, const BehaviorAccessData &accessData);
    int AddComponent(BaseComponent *component);

    void SetPrev(GameObject *prev);
    void SetNext(GameObject *next);
    GameObject *GetPrev() const;
    GameObject *GetNext() const;
};