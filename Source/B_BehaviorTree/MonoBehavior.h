#pragma once

#include "BehaviorAccessData.h"

class GameObject;
class GameObject2D;
class GameObject3D;
class BaseCollision;
class BaseModelList;

// ç≈è¨íPà 
class MonoBehavior
{
private:
    unsigned long mnMonoNumber;
    GameObject *mpMyObject;
    MonoBehavior *mpPrev;
    MonoBehavior *mpNext;

public:
    MonoBehavior(GameObject *myObject);
    ~MonoBehavior();

    virtual int Initialize() = 0;
    virtual int Finalize() = 0;
    virtual BehaviorAccessData EarlyUpdate(const BehaviorAccessData &myAccessData);
    virtual BehaviorAccessData Update(const BehaviorAccessData &myAccessData) = 0;
    virtual BehaviorAccessData HitOnCollision(const BehaviorAccessData &myAccessData, BaseCollision *myCollision, BaseCollision *hitCollision);
    virtual BehaviorAccessData LateUpdate(const BehaviorAccessData &myAccessData);
    virtual int UpdateModel(BaseModelList *modelList);

    void SetMonoNumber(unsigned long number);

    unsigned long  GetMonoNumber() const;
    GameObject *GetMyObject() const;
    GameObject2D *GetMyObject2D() const;
    GameObject3D *GetMyObject3D() const;

    void SetPrevNext(MonoBehavior *prev, MonoBehavior *next);
    MonoBehavior *GetPrev() const;
    MonoBehavior *GetNext() const;
};

// MonoBihaviorÇÃï ñºÇ™IState(Interface State)
typedef MonoBehavior BaseState;

// MonoBihaviorÇÃï ñºÇ™MonoBihavior
typedef MonoBehavior IState;