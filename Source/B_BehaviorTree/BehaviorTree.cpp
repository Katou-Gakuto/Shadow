#include "BehaviorTree.h"

#include "BehaviorNode.h"

int BehaviorTree::ChangeBehavior(BehaviorNode *nowNode)
{
    // 
    if (this->mdNow == this->mdNext)
    {
        return 0;
    }
    // 
    if (nowNode == nullptr)
    {
        return -1;
    }
    // 
    BehaviorNode *nextNode = this->SearchNode(this->mdNext);
    if (nextNode == nullptr)
    {
        return -1;
    }

    // 関数の返り値を取得する変数
    int result = 0;

    // 前回まで使っていたステートの後処理を行う
    result = nowNode->Finalize(this->mdNow);
    if (result < 0)
    {
        return result;
    }
    // 次回から使用するステートの初期化を行う
    result = nextNode->Initialize(this->mdNext);
    if (result < 0)
    {
        return result;
    }

    // 次回から新しいステートで処理を行う
    this->mdNow = this->mdNext;

    // 関数が正常終了したことを返す
    return 0;
}

int BehaviorTree::DeleteAll()
{
    if (this->mpFirstNode == nullptr)
    {
        return 0;
    }
    BehaviorNode *target = this->mpFirstNode;
    BehaviorNode *targetNext = target->GetNext();
    do
    {
        targetNext = target->GetNext();
        if (targetNext != nullptr)
        {
            targetNext->SetPrevNext(nullptr, targetNext->GetNext());
        }
        target->SetPrevNext(nullptr, nullptr);
        delete target;
        target = targetNext;
    } while (target != nullptr);
    this->mpFirstNode = nullptr;
    return 0;
}

BehaviorNode *BehaviorTree::SearchNode(unsigned long node)
{
    BehaviorNode *current = this->mpFirstNode;
    while (current != nullptr)
    {
        if (current->GetNodeNumber() == node)
        {
            break;
        }
        current = current->GetNext();
    }
    return current;
}

BehaviorNode *BehaviorTree::SearchNode(const BehaviorAccessData &accessData)
{
    BehaviorNode *current = this->mpFirstNode;
    while (current != nullptr)
    {
        if (current->GetNodeNumber() == accessData.mnNodeNumber)
        {
            break;
        }
        current = current->GetNext();
    }
    return current;
}

BehaviorTree::BehaviorTree() :
    mpFirstNode(),
    mdNow(GetBehaviorAccessDataZero()),
    mdNext(GetBehaviorAccessDataZero()),
    mbStateChange(true),
    mbInitializedFlag(false)
{

}

BehaviorTree::~BehaviorTree()
{
    this->DeleteAll();
}

int BehaviorTree::Initialize(const BehaviorAccessData &startBehavior)
{
    auto node = this->SearchNode(startBehavior);
    if (node == nullptr)
    {
        return -1;
    }
    int temp = node->Initialize(startBehavior);
    if (0 <= temp)
    {
        this->mdNow = startBehavior;
        this->mdNext = startBehavior;
    }
    return temp;
}

int BehaviorTree::Finalize()
{
    // 多分することないっすね
    return 0;
}

int BehaviorTree::EarlyUpdate()
{
    // ツリーにノードが一つもない場合は処理を行わない
    if (this->mpFirstNode == nullptr)
    {
        return 0;
    }

    // ノードを取得
    BehaviorNode *nowNode = this->SearchNode(this->mdNow);
    if (nowNode == nullptr)
    {
        // 明らかにエラーなので関数を強制終了させる
        return -1;
    }

    // FSMのアップデート
    this->mdNext = nowNode->EarlyUpdate(this->mdNow);

    // 最後にFSMの切り替えを行い、その返り値を返す
    return this->ChangeBehavior(nowNode);
}

int BehaviorTree::Update()
{
    // ツリーにノードが一つもない場合は処理を行わない
    if (this->mpFirstNode == nullptr)
    {
        return 0;
    }

    // ノードを取得
    BehaviorNode *nowNode = this->SearchNode(this->mdNow);
    if (nowNode == nullptr)
    {
        // 明らかにエラーなので関数を強制終了させる
        return -1;
    }

    // FSMのアップデート
    this->mdNext = nowNode->Update(this->mdNow);

    // 最後にFSMの切り替えを行い、その返り値を返す
    return this->ChangeBehavior(nowNode);
}

int BehaviorTree::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    // ツリーにノードが一つもない場合は処理を行わない
    if (this->mpFirstNode == nullptr)
    {
        return 0;
    }

    // ノードを取得
    BehaviorNode *nowNode = this->SearchNode(this->mdNow);
    if (nowNode == nullptr)
    {
        // 明らかにエラーなので関数を強制終了させる
        return -1;
    }

    // FSMのアップデート
    this->mdNext = nowNode->HitOnCollision(this->mdNow, myCollision, hitCollision);

    // 当たり判定の処理に関してはFSMの切り替えを行ず、正常終了を示す0を返す
    return 0;
}

int BehaviorTree::LateUpdate()
{
    // ツリーにノードが一つもない場合は処理を行わない
    if (this->mpFirstNode == nullptr)
    {
        return 0;
    }

    // ノードを取得
    BehaviorNode *nowNode = this->SearchNode(this->mdNow);
    if (nowNode == nullptr)
    {
        // 明らかにエラーなので関数を強制終了させる
        return -1;
    }

    // FSMのアップデート
    this->mdNext = nowNode->LateUpdate(this->mdNow);

    // 最後にFSMの切り替えを行い、その返り値を返す
    return this->ChangeBehavior(nowNode);
}

int BehaviorTree::UpdateModel(BaseModelList *modeList)
{
    if (this->mpFirstNode == nullptr)
    {
        return 0;
    }
    // ノードを取得
    BehaviorNode *nowNode = this->SearchNode(this->mdNow);
    if (nowNode == nullptr)
    {
        return -1;
    }
    return nowNode->UpdateModel(this->mdNow, modeList);
}

int BehaviorTree::AddMono(MonoBehavior *mono, const BehaviorAccessData &data)
{
    if (mono == nullptr ||
        this->mpFirstNode == nullptr)
    {
        return -1;
    }
    BehaviorNode *current = this->mpFirstNode;
    while (current->GetNext() != nullptr)
    {
        if (current->GetNodeNumber() == data.mnNodeNumber)
        {
            break;
        }
        current = current->GetNext();
    }
    if (current == nullptr)
    {
        return -1;
    }
    return current->Add(mono, data.mnMonoNumber);
}

int BehaviorTree::AddNode(BehaviorNode *node, unsigned long number)
{
    if (node == nullptr)
    {
        return -1;
    }
    if (this->mpFirstNode ==nullptr)
    {
        this->mpFirstNode = node;
        node->SetNodeNumber(number);
        return 0;
    }
    BehaviorNode *current = this->mpFirstNode;
    while (current->GetNext() != nullptr)
    {
        if (current->GetNodeNumber() == number)
        {
            current = nullptr;
            break;
        }
        current = current->GetNext();
    }
    if (current == nullptr)
    {
        return -1;
    }

    current->SetPrevNext(current->GetPrev(), node);
    node->SetPrevNext(current, nullptr);
    node->SetNodeNumber(number);
    return 0;
}
