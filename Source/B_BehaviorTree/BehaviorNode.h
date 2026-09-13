#pragma once

#include "BehaviorAccessData.h"

class MonoBehavior;
class BaseModelList;
class GameObject;
class BaseCollision;
class BehaviorNode
{
private:
    unsigned long mnNodeNumber;
    MonoBehavior *mpFirstMonoBehavior;
    GameObject *mpMyObject;
    BehaviorNode *mpPrev;
    BehaviorNode *mpNext;

    int DeleteAll();

public:
    BehaviorNode(GameObject *myObject);
    ~BehaviorNode();

    int Initialize(const BehaviorAccessData &accessData);
    int Finalize(const BehaviorAccessData &accessData);
    BehaviorAccessData EarlyUpdate(const BehaviorAccessData &accessData);
    BehaviorAccessData Update(const BehaviorAccessData &accessData);
    BehaviorAccessData HitOnCollision(const BehaviorAccessData &accessData, BaseCollision *myCollision, BaseCollision *hitCollision);
    BehaviorAccessData LateUpdate(const BehaviorAccessData &accessData);
    int UpdateModel(const BehaviorAccessData &accessData, BaseModelList *modeList);

    void SetNodeNumber(unsigned long number);
    unsigned long GetNodeNumber()const;

    int Add(MonoBehavior *mono, unsigned long number);
    int Delete(unsigned long number);

    void SetPrevNext(BehaviorNode *prev, BehaviorNode *next);
    BehaviorNode *GetPrev() const;
    BehaviorNode *GetNext() const;
};

// BehaviorNode‚Ì•Ê–¼‚ªFSM
typedef BehaviorNode FSM;
