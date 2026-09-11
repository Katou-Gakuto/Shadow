#pragma once

#include "BehaviorNode.h"
#include "MonoBehavior.h"

#include "../Y_Tool/CountDownSystem.h"

enum EnemyStateNode : unsigned long
{
    EnemyStateNode_Normal = 0,

};

enum EnemyStateTag : unsigned long
{
    EnemyStateTag_Idol = 0,

    EnemyStateTag_Search,
    EnemyStateTag_Target,
    EnemyStateTag_Random,
};

class EnemyState_Search : public MonoBehavior
{
private:
    CountDownSystem mnSearch2Random;

public:
    EnemyState_Search(GameObject *myObject);
    ~EnemyState_Search();

    int Initialize() override;
    int Finalize() override;
    BehaviorAccessData Update(const BehaviorAccessData &myAccessData) override;
    BehaviorAccessData HitOnCollision(const BehaviorAccessData &myAccessData, BaseCollision *myCollision, BaseCollision *hitCollision) override;
};

class EnemyState_Target : public MonoBehavior
{
private:
    bool mbCollisionHit;

public:
    EnemyState_Target(GameObject *myObject);
    ~EnemyState_Target();

    int Initialize() override;
    int Finalize() override;
    BehaviorAccessData Update(const BehaviorAccessData &myAccessData) override;
    BehaviorAccessData HitOnCollision(const BehaviorAccessData &myAccessData, BaseCollision *myCollision, BaseCollision *hitCollision) override;
    BehaviorAccessData LateUpdate(const BehaviorAccessData &myAccessData) override;
};

class EnemyState_Random : public MonoBehavior
{
private:

public:
    EnemyState_Random(GameObject *myObject);
    ~EnemyState_Random();

    int Initialize() override;
    int Finalize() override;
    BehaviorAccessData Update(const BehaviorAccessData &myAccessData) override;
    BehaviorAccessData HitOnCollision(const BehaviorAccessData &myAccessData, BaseCollision *myCollision, BaseCollision *hitCollision) override;
    BehaviorAccessData LateUpdate(const BehaviorAccessData &myAccessData) override;
};
