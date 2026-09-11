#pragma once

#include "BaseComponent.h"

#include <cstdint>
#include <vector>

#include "../Y_Tool/VECTOR.h"
#include "../Z_Except/MoveData.h"

/*

GameManagerにAddする順番は
Button→MapObject
にしてくださいな。

*/

/*
鏡は基本的には普通のオブジェクトと同じようにMapObjectControllerを持たせる

工夫する部分は当たり判定がただの四角じゃなくて

GameObjectに持たせる当たり判定のn個のRayを
『鏡面判定のRay + 普通の壁のRay * 3』
にするところ

*/

// 
struct SimulationData
{
    // 
    uint32_t mnFrameCount;

    // 
    VECTOR2D mvPos;

    // 
    float mfRotate;
};

// 
class ButtonObjectController;

// 
class MapObjectController : public BaseComponent
{
public:
    // 
    MapObjectController();

    //
    ~MapObjectController();

    virtual int Create() override;
    virtual int Initialize() override;
    virtual int Finalize() override;
    virtual int EarlyUpdate() override;
    virtual int Update() override;
    virtual int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision) override;
    virtual int LateUpdate() override;
    virtual int Draw() override;

    // 
    bool SetMoveData(const MoveData &data);

    // 
    bool AddButton(ButtonObjectController *button);

    // 自身の次の動きを計算で導き出す関数
    // 出力先はメンバーのmdSumilationDataです
    // 
    bool Sumilation();

    // 全てのボタンの上にPlayerが乗っているかを取得する関数
    bool GetButtonOnPlayer() const;

    // 全てのボタンの状態がtrueなのかを取得する関数
    bool GetButtonFlag() const;

private:
    // 
    MoveData mdMoveData;

    // 
    SimulationData mdSumilationData;

    // 
    bool mbSumilationNow;

    // 
    bool mbMoving;

    // これらのやつが全てFlagとなるので
    std::vector<ButtonObjectController *> mlFlagButtonBox;
};

