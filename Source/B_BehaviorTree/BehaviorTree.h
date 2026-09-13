#pragma once

#include "BehaviorAccessData.h"

class MonoBehavior;
class BehaviorNode;
class BaseCollision;
class BaseModelList;

// BehaviorTreeとは処理の入口、出口をわかりやすくするものです
// コンポーネントのような最小単位を集めるようなコードとは違います
// よって今回はStateの切り替えが実行されるのは次のフレームからとなります
class BehaviorTree
{
private:
    BehaviorNode *mpFirstNode;
    BehaviorAccessData mdNow;
    BehaviorAccessData mdNext;
    bool mbStateChange;
    bool mbInitializedFlag;

    int ChangeBehavior(BehaviorNode *nowNode);
    int DeleteAll();
    BehaviorNode *SearchNode(unsigned long node);
    BehaviorNode *SearchNode(const BehaviorAccessData &accessData);

public:
    BehaviorTree();
    ~BehaviorTree();

    int Initialize(const BehaviorAccessData &startBehavior);
    int Finalize();
    int EarlyUpdate();
    int Update();
    int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision);
    int LateUpdate();
    int UpdateModel(BaseModelList *modeList);

    int AddMono(MonoBehavior *mono, const BehaviorAccessData &data);
    int AddNode(BehaviorNode *node, unsigned long number);
};