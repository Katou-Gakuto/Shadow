#include "MonoBehavior.h"
#include "BehaviorTree.h"

#include "../A_GameObject/GameObject.h"
#include "../A_GameObject/GameObject2D.h"
#include "../A_GameObject/GameObject3D.h"

MonoBehavior::MonoBehavior(GameObject *myObject) :
    mnMonoNumber(0),
    mpMyObject(myObject),
    mpPrev(nullptr),
    mpNext(nullptr)
{

}

MonoBehavior::~MonoBehavior()
{
    this->mnMonoNumber = 0;
    this->mpMyObject = nullptr;
    this->mpPrev = nullptr;
    this->mpNext = nullptr;
}

BehaviorAccessData MonoBehavior::EarlyUpdate(const BehaviorAccessData &myAccessData)
{
    return myAccessData;
}

BehaviorAccessData MonoBehavior::HitOnCollision(const BehaviorAccessData &myAccessData, BaseCollision *myCollision, BaseCollision *hitCollision)
{
    return myAccessData;
}

BehaviorAccessData MonoBehavior::LateUpdate(const BehaviorAccessData &myAccessData)
{
    return myAccessData;
}

int MonoBehavior::UpdateModel(BaseModelList *modelList)
{
    return 0;
}

void MonoBehavior::SetMonoNumber(unsigned long number)
{
    this->mnMonoNumber = number;
}

unsigned long MonoBehavior::GetMonoNumber() const
{
    return this->mnMonoNumber;
}

GameObject *MonoBehavior::GetMyObject() const
{
    return this->mpMyObject;
}

GameObject2D *MonoBehavior::GetMyObject2D() const
{
    return dynamic_cast<GameObject2D *>(this->mpMyObject);
}

GameObject3D *MonoBehavior::GetMyObject3D() const
{
    return dynamic_cast<GameObject3D *>(this->mpMyObject);
}

void MonoBehavior::SetPrevNext(MonoBehavior *prev, MonoBehavior *next)
{
    this->mpPrev = prev;
    this->mpNext = next;
}

MonoBehavior *MonoBehavior::GetPrev() const
{
    return this->mpPrev;
}

MonoBehavior *MonoBehavior::GetNext() const
{
    return this->mpNext;
}
