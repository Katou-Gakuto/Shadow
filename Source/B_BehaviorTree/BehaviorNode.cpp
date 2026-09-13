#include "BehaviorNode.h"

#include "MonoBehavior.h"
#include "BehaviorTree.h"
#include "../A_GameObject/GameObject.h"

int BehaviorNode::DeleteAll()
{
    if (this->mpFirstMonoBehavior == nullptr)
    {
        return 0;
    }
    MonoBehavior *current = this->mpFirstMonoBehavior;
    MonoBehavior *next = nullptr;
    do
    {
        next = current->GetNext();
        if (next != nullptr)
        {
            next->SetPrevNext(nullptr, next->GetNext());
        }
        current->SetPrevNext(nullptr, nullptr);
        delete current;
        current = next;
    } while (current != nullptr);
    this->mpFirstMonoBehavior = nullptr;
    return 0;
}

BehaviorNode::BehaviorNode(GameObject *myObject) :
    mnNodeNumber(0),
    mpFirstMonoBehavior(nullptr),
    mpMyObject(myObject),
    mpPrev(nullptr),
    mpNext(nullptr)
{

}

BehaviorNode::~BehaviorNode()
{
    this->DeleteAll();
}

int BehaviorNode::Initialize(const BehaviorAccessData &accessData)
{
    int temp = -1;
    MonoBehavior *current = this->mpFirstMonoBehavior;
    while (current != nullptr)
    {
        if (current->GetMonoNumber() == accessData.mnMonoNumber)
        {
            temp = current->Initialize();
            break;
        }
        current = current->GetNext();
    }
    return temp;
}

int BehaviorNode::Finalize(const BehaviorAccessData &accessData)
{
    int temp = -1;
    MonoBehavior *current = this->mpFirstMonoBehavior;
    while (current != nullptr)
    {
        if (current->GetMonoNumber() == accessData.mnMonoNumber)
        {
            temp = current->Finalize();
            break;
        }
        current = current->GetNext();
    }
    return temp;
}

BehaviorAccessData BehaviorNode::EarlyUpdate(const BehaviorAccessData &accessData)
{
    BehaviorAccessData temp = accessData;
    MonoBehavior *current = this->mpFirstMonoBehavior;
    while (current != nullptr)
    {
        if (current->GetMonoNumber() == accessData.mnMonoNumber)
        {
            temp = current->EarlyUpdate(accessData);
            break;
        }
        current = current->GetNext();
    }
    return temp;
}

BehaviorAccessData BehaviorNode::Update(const BehaviorAccessData &accessData)
{
    BehaviorAccessData temp = accessData;
    MonoBehavior *current = this->mpFirstMonoBehavior;
    while (current != nullptr)
    {
        if (current->GetMonoNumber() == accessData.mnMonoNumber)
        {
            temp = current->Update(accessData);
            break;
        }
        current = current->GetNext();
    }
    return temp;
}

BehaviorAccessData BehaviorNode::HitOnCollision(const BehaviorAccessData &accessData, BaseCollision *myCollision, BaseCollision *hitCollision)
{
    BehaviorAccessData temp = accessData;
    MonoBehavior *current = this->mpFirstMonoBehavior;
    while (current != nullptr)
    {
        if (current->GetMonoNumber() == accessData.mnMonoNumber)
        {
            temp = current->HitOnCollision(accessData, myCollision, hitCollision);
            break;
        }
        current = current->GetNext();
    }
    return temp;
}

BehaviorAccessData BehaviorNode::LateUpdate(const BehaviorAccessData &accessData)
{
    BehaviorAccessData temp = accessData;
    MonoBehavior *current = this->mpFirstMonoBehavior;
    while (current != nullptr)
    {
        if (current->GetMonoNumber() == accessData.mnMonoNumber)
        {
            temp = current->LateUpdate(accessData);
            break;
        }
        current = current->GetNext();
    }
    return temp;
}

int BehaviorNode::UpdateModel(const BehaviorAccessData &accessData, BaseModelList *modeList)
{
    int temp = -1;
    MonoBehavior *current = this->mpFirstMonoBehavior;
    while (current != nullptr)
    {
        if (current->GetMonoNumber() == accessData.mnMonoNumber)
        {
            temp = current->UpdateModel(modeList);
            break;
        }
        current = current->GetNext();
    }
    return temp;
}

void BehaviorNode::SetNodeNumber(unsigned long number)
{
    this->mnNodeNumber = number;
}

unsigned long BehaviorNode::GetNodeNumber() const
{
    return this->mnNodeNumber;
}

int BehaviorNode::Add(MonoBehavior *mono, unsigned long number)
{
    if (mono == nullptr)
    {
        return -1;
    }
    if (this->mpFirstMonoBehavior == nullptr)
    {
        this->mpFirstMonoBehavior = mono;
        mono->SetMonoNumber(number);
        return 0;
    }
    int temp = 0;
    MonoBehavior *current = this->mpFirstMonoBehavior;
    while (current->GetNext() != nullptr)
    {
        if (current->GetMonoNumber() == number)
        {
            temp = -1;
            break;
        }
        current = current->GetNext();
    }
    if (temp == 0)
    {
        current->SetPrevNext(current->GetPrev(), mono);
        mono->SetPrevNext(current, nullptr);
        mono->SetMonoNumber(number);
    }
    return temp;
}

int BehaviorNode::Delete(unsigned long number)
{
    if (this->mpFirstMonoBehavior == nullptr)
    {
        return 0;
    }
    MonoBehavior *current = this->mpFirstMonoBehavior;
    while (current->GetNext() != nullptr)
    {
        if (current->GetMonoNumber() == number)
        {
            break;
        }
        current = current->GetNext();
    }
    if (current != nullptr)
    {
        MonoBehavior *prev = current->GetPrev();
        MonoBehavior *next = current->GetNext();
        if (prev != nullptr)
        {
            prev->SetPrevNext(prev->GetPrev(), next);
        }
        if (next != nullptr)
        {
            next->SetPrevNext(prev, next->GetNext());
        }
        if (this->mpFirstMonoBehavior == current)
        {
            this->mpFirstMonoBehavior = next;
        }
        current->SetPrevNext(nullptr, nullptr);
        delete current;
    }
    return 0;
}

void BehaviorNode::SetPrevNext(BehaviorNode *prev, BehaviorNode *next)
{
    this->mpPrev = prev;
    this->mpNext = next;
}

BehaviorNode *BehaviorNode::GetPrev() const
{
    return this->mpPrev;
}

BehaviorNode *BehaviorNode::GetNext() const
{
    return this->mpNext;
}
