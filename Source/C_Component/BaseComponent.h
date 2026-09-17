#pragma once

class GameObject;
class GameObject2D;
class GameObject3D;
class BaseCollision;

// コンポーネントを識別する番号とコンポーネントの処理順を合わせた定数
// どちらかというと処理順からコンポーネントの識別をするイメージです
// 設定する値はそのコンポーネントの処理順を設定してください
// 処理順は0から始まります
typedef enum ComponentTagAndOrder : unsigned long
{
    // 最初にオブジェクトそのもの
    CTAO_PlayerController = 0,
    CTAO_WallController,
    CTAO_MirrorController,

    // ここからギミックトリガー関連
    CTAO_ButtonController,
    CTAO_HoldController,

    // ここがギミック本体
    CTAO_GimmickController,

} CTAO;

// オブジェクトの機能拡張を行うクラス
class BaseComponent
{
private:
    ComponentTagAndOrder mnTagOrder;
    bool mbDeleteFlag;
    bool mbActiveFlag;
    bool mbCreateFlag;
    bool mbInitializedFlag;
    GameObject *mpUp;

    BaseComponent *mpPrev;
    BaseComponent *mpNext;

public:
    BaseComponent(GameObject *myObject, ComponentTagAndOrder tagOrder);
    virtual ~BaseComponent();

    virtual int Create();
    virtual int Initialize();
    virtual int Finalize();
    virtual int EarlyUpdate();
    virtual int Update();
    virtual int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision);
    virtual int LateUpdate();
    virtual int Draw();

    int BaseInitialize();

    void OnDeleteFlag();

    ComponentTagAndOrder GetTagAndOrder() const;
    bool GetDeleteFlag() const;
    bool GetActiveFlag() const;
    bool GetInitializedFlag() const;
    GameObject *GetMyObject() const;
    GameObject2D *GetMyObject2D() const;
    GameObject3D *GetMyObject3D() const;

    void SetPrevNext(BaseComponent *prev, BaseComponent *next);
    BaseComponent *GetPrev() const;
    BaseComponent *GetNext() const;
};