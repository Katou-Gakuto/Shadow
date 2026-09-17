#pragma once

#include "../Y_Tool/VECTOR.h"

#include "BaseCollision.h"
#include "CollisionHandle.h"

// GameObjectが持っている当たり判定の線形リスト管理クラス
class BaseCollisionList
{
private:
    GameObject *mpGameObject;               // 処理を楽にするための変数
    BaseCollision *mpFirstBaseCollision;    // 線形リストの先頭にある当たり判定へのポインタ

    // 
    unsigned long long mnNextNumber;

public:
    BaseCollisionList();
    BaseCollisionList(GameObject *object);
    ~BaseCollisionList();

    int SetCollisionMoveVec(CollisionDimension dimension, const void *moveData);
    int SetCollisionNextPos(CollisionDimension dimension, const void *posData);
    int SetCollisionPosCollisionMoveVec();

    int Initialize();
    int Finalize();

    int ListCollisionActivate();
    int ListCollisionDeactivate();

    int Add(BaseCollision *target, CollisionHandle &out);
    int DeleteTarget(BaseCollision *target);
    int DeleteToFlag();
    int DeleteAll();
    int ConnectTarget(BaseCollision *prev, BaseCollision *target, BaseCollision *next);
    int IsolateTarget(BaseCollision *target);
    BaseCollision *SearchCollision(const CollisionHandle &handle);

    // 
    unsigned long long *Handle2NumberPtr(CollisionHandle &handle);

    // 
    const unsigned long long *Handle2NumberPtr(const CollisionHandle &handle);
};