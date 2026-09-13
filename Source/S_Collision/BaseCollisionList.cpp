#include "BaseCollisionList.h"

#include "BaseCollision.h"
#include "BaseCollision2D.h"
#include "BaseCollision2DManager.h"
#include "BaseCollision3D.h"
#include "BaseCollision3DManager.h"

#include "../Z_Except/Master.h"


BaseCollisionList::BaseCollisionList() : 
    mpFirstBaseCollision(nullptr),
    mpGameObject(nullptr)
{

}

BaseCollisionList::BaseCollisionList(GameObject *object) :
    mpFirstBaseCollision(nullptr),
    mpGameObject(object)
{

}

BaseCollisionList::~BaseCollisionList()
{
    this->DeleteAll();
}

int BaseCollisionList::SetCollisionMoveVec(CollisionDimension dimension, void *moveData)
{
    BaseCollision *current = this->mpFirstBaseCollision;
    while (current != nullptr)
    {
        if (current->GetCollisionDimension() == dimension)
        {
            current->SetMoveVec(moveData);
        }
        current = current->GetNextList();
    }

    return 0;
}

int BaseCollisionList::SetCollisionNextPos(CollisionDimension dimension, void *posData)
{
    BaseCollision *current = this->mpFirstBaseCollision;
    while (current != nullptr)
    {
        if (current->GetCollisionDimension() == dimension)
        {
            current->SetMoveVec(posData);
        }
        current = current->GetNextList();
    }

    return 0;
}

int BaseCollisionList::SetCollisionPosCollisionMoveVec()
{
    BaseCollision *current = this->mpFirstBaseCollision;
    while (current != nullptr)
    {
        current->SetPosToMoveVec();
        current = current->GetNextList();
    }

    return 0;
}

int BaseCollisionList::Initialize()
{
    return 0;
}

int BaseCollisionList::Finalize()
{
    return 0;
}

int BaseCollisionList::ListCollisionActivate()
{
    BaseCollision *current = this->mpFirstBaseCollision;
    while (current != nullptr)
    {
        current->SetActiveFlag(true);
        current = current->GetNextList();
    }
    return 0;
}

int BaseCollisionList::ListCollisionDeactivate()
{
    BaseCollision *current = this->mpFirstBaseCollision;
    while (current != nullptr)
    {
        current->SetActiveFlag(false);
        current = current->GetNextList();
    }
    return 0;
}

int BaseCollisionList::Add(BaseCollision *target, unsigned long listNum)
{
    // そもそもtargetがnullの場合
    if (target == nullptr)
    {
        return -1;
    }

    // mpActionFirstすらない場合
    if (mpFirstBaseCollision == nullptr)
    {
        // リストの最初の当たり判定をtargetにする
        mpFirstBaseCollision = target;
        
        // 番号を設定する
        target->SetListNum(listNum);

        // 上手くいったことを返す
        return 0;
    }

    // 最後尾を探しつつ、同じ番号を持った当たり判定がないかも探す
    BaseCollision *current = mpFirstBaseCollision;
    while (current->GetNextList() != nullptr)
    {
        // 着目中の当たり判定が同じ番号を持っていたら関数をその時点で終了しfalseを返す
        if (current->GetListNum() == listNum)
        {
            return 1;
        }

        // 着目当たり判定を次に進める
        current = current->GetNextList();
    }

    // リストに加える
    BaseCollisionList::ConnectTarget(current, target, nullptr);

    // 番号を設定する
    target->SetListNum(listNum);

    // 上手くいったことを返す
    return 0;
}

int BaseCollisionList::DeleteTarget(BaseCollision *target)
{
    if (target == nullptr ||
        mpFirstBaseCollision == nullptr)
    {
        return 0;
    }

    // ワールドから孤立させる
    target->WorldIsolateMySelf();

    // 自身の線形リストから孤立させる
    BaseCollisionList::IsolateTarget(target);

    // 削除
    delete target;
    target = nullptr;

    // 成功したことを返す
    return 0;
}

int BaseCollisionList::DeleteToFlag()
{
    if (this->mpFirstBaseCollision == nullptr)
    {
        return 0;
    }

    BaseCollision *current = this->mpFirstBaseCollision;
    BaseCollision *next = current->GetNextList();
    while (true)
    {
        if (current->GetDeleteFlag())
        {
            DeleteTarget(current);
        }
        current = next;
        if (current != nullptr)
        {
            next = current->GetNextList();
        }
        else
        {
            break;
        }
    }

    return 0;
}

int BaseCollisionList::DeleteAll()
{
    if (this->mpFirstBaseCollision == nullptr)
    {
        return 0;
    }

    BaseCollision *target = mpFirstBaseCollision;
    BaseCollision *next = target->GetNextList();

    while (true)
    {
        DeleteTarget(target);
        target = next;
        if (target == nullptr)
        {
            break;
        }
        else
        {
            next = target->GetNextList();
        }
    }

    return 0;
}

int BaseCollisionList::ConnectTarget(BaseCollision *prev, BaseCollision *target, BaseCollision *next)
{
    if (prev != nullptr)
    {
        prev->SetNextList(target);
    }
    if (next != nullptr)
    {
        next->SetPrevList(target);
    }

    target->SetPrevList(prev);
    target->SetNextList(next);

    // まだリストに何もないか、nextがの最初のコントローラーだった場合にtrue
    if (this->mpFirstBaseCollision == nullptr ||
        next == this->mpFirstBaseCollision)
    {
        // targetをリストの最初のコントローラーに再設定する
        this->mpFirstBaseCollision = target;
    }

    return 0;
}

int BaseCollisionList::IsolateTarget(BaseCollision *target)
{
    if (this->mpFirstBaseCollision == nullptr)
    {
        return 0;
    }

    // 作業用変数を用意
    BaseCollision *prev = target->GetPrevList();
    BaseCollision *next = target->GetNextList();

    if (prev != nullptr)
    {
        // prevがnullでなかったら、nextがnullだろうとセットする。(targetが最後尾のコリジョンの場合など)
        prev->SetNextList(next);
    }
    if (next != nullptr)
    {
        // nextがnullでなかったら、prevがnullだろうとセットする。(target == mpCollisionの場合など)
        next->SetPrevList(prev);
    }

    if (target == this->mpFirstBaseCollision)
    {
        // targetが最初のコリジョンだった場合は一つ後のコリジョンを最初のコリジョンとする
        this->mpFirstBaseCollision = next;
    }

    target->SetPrevList(nullptr);
    target->SetNextList(nullptr);

    return 0;
}

BaseCollision *BaseCollisionList::SearchCollisionNum(unsigned long collisionNum)
{
    BaseCollision *temp = nullptr;

    if (mpFirstBaseCollision == nullptr)
    {
        return temp;
    }

    BaseCollision *current = mpFirstBaseCollision;
    while (current != nullptr)
    {
        if (current->GetListNum() == collisionNum)
        {
            temp = current;
            break;
        }

        current = current->GetNextList();
    }

    return temp;
}
