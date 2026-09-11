#pragma once

#include "../Y_Tool/VECTOR.h"

enum CollisionDimension : unsigned char;
class GameObject;
class BaseCollision;

// GameObjectが持っている当たり判定の線形リスト管理クラス
class BaseCollisionList
{
private:
    GameObject *mpGameObject;               // 処理を楽にするための変数
    BaseCollision *mpFirstBaseCollision;    // 線形リストの先頭にある当たり判定へのポインタ    

public:
    BaseCollisionList();
    BaseCollisionList(GameObject *object);
    ~BaseCollisionList();

    int SetCollisionMoveVec(CollisionDimension dimension, void *moveData);
    int SetCollisionNextPos(CollisionDimension dimension, void *posData);
    int SetCollisionPosCollisionMoveVec();

    int Initialize();
    int Finalize();

    int ListCollisionActivate();
    int ListCollisionDeactivate();

    int Add(BaseCollision *target, unsigned long listNum);
    int DeleteTarget(BaseCollision *target);
    int DeleteToFlag();
    int DeleteAll();
    int ConnectTarget(BaseCollision *prev, BaseCollision *target, BaseCollision *next);
    int IsolateTarget(BaseCollision *target);
    BaseCollision *SearchCollisionNum(unsigned long collisionNum);
};