#include "BaseComponentList.h"

#include "BaseComponent.h"

int BaseComponentList::DeleteAll()
{
    if (this->mpFirstComponent == nullptr)
    {
        return 0;
    }
    BaseComponent *current = this->mpFirstComponent;
    BaseComponent *next = this->mpFirstComponent;
    do
    {
        next = current->GetNext();
        this->IsolateTarget(current);
        delete current;
        current = next;
    } while (current != nullptr);
    this->mpFirstComponent = nullptr;
    return 0;
}

int BaseComponentList::ConnectTarget(BaseComponent *prev, BaseComponent *target, BaseComponent *next)
{
    if (target == nullptr)
    {
        return -1;
    }
    if (prev != nullptr)
    {
        prev->SetPrevNext(prev->GetPrev(), target);
    }
    if (next != nullptr)
    {
        next->SetPrevNext(target, next->GetNext());
    }
    if (this->mpFirstComponent == next)
    {
        this->mpFirstComponent = target;
    }
    target->SetPrevNext(prev, next);
    return 0;
}

int BaseComponentList::IsolateTarget(BaseComponent *target)
{
    if (target == nullptr)
    {
        return -1;
    }
    BaseComponent *prev = target->GetPrev();
    BaseComponent *next = target->GetNext();
    if (prev != nullptr)
    {
        prev->SetPrevNext(prev->GetPrev(), next);
    }
    if (next != nullptr)
    {
        next->SetPrevNext(prev, next->GetNext());
    }
    if (this->mpFirstComponent == target)
    {
        this->mpFirstComponent = next;
    }
    target->SetPrevNext(nullptr, nullptr);
    return 0;
}

// Addの度にSortを行うので、結果的に変える場所は一つのみになります
// よって今回行うのは挿入ソートで、最後尾のデータの挿入箇所を探すコードのみを書きます
int BaseComponentList::Sort()
{
    // リスト内に最後尾を含めて二つ以上有効なノードがない場合はソートのしようがないので関数終了
    if (this->mpFirstComponent == nullptr ||
        this->mpFirstComponent->GetNext() == nullptr)
    {
        return 0;
    }
    
    // 最後尾を見つける
    BaseComponent *last = this->mpFirstComponent;
    while (last->GetNext() != nullptr)
    {
        last = last->GetNext();
    }

    // 挿入箇所を探すループ文(lastよりも遅いもののひとつ前まで進める)
    for (BaseComponent *current = last->GetPrev(); current != nullptr; current = current->GetPrev())
    {
        // 着目ノードは最後尾より処理順が遅いのが前提なので、それが合っていたら次のノードに移る
        if (last->GetTagAndOrder() < current->GetTagAndOrder())
        {
            continue;
        }

        // 最後尾のノードを孤立させる
        this->IsolateTarget(last);

        // 挿入箇所に挿入する
        this->ConnectTarget(current, last, current->GetNext());

        // ソートを行うのは最後尾の一つのみなのでこれでok
        break;
    }

    // 関数が正常終了したことを返す
    return 0;
}

int BaseComponentList::Swap(BaseComponent *N1, BaseComponent *N2)
{
    // 詳しくはVoidListのSwapを見てください
    if (N1 == nullptr || N2 == nullptr)
    {
        return -1;
    }
    bool N1nextN2 = (N1->GetNext() == N2);
    bool N2nextN1 = (N2->GetNext() == N1);
    if (N1nextN2 == false && N2nextN1 == false)
    {
        BaseComponent *N1prev = N1->GetPrev();
        BaseComponent *N1next = N1->GetNext();
        BaseComponent *N2prev = N2->GetPrev();
        BaseComponent *N2next = N2->GetNext();
        this->IsolateTarget(N1);
        this->IsolateTarget(N2);
        this->ConnectTarget(N2prev, N1, N2next);
        this->ConnectTarget(N1prev, N2, N1next);
    }
    else
    {
        if (N1nextN2 == true)
        {
            BaseComponent *N0 = N1->GetPrev();
            BaseComponent *N3 = N2->GetNext();
            this->IsolateTarget(N1);
            this->IsolateTarget(N2);
            this->ConnectTarget(N0, N2, N1);
            this->ConnectTarget(N2, N1, N3);
        }
        else
        {
            BaseComponent *N0 = N2->GetPrev();
            BaseComponent *N3 = N1->GetNext();
            this->IsolateTarget(N1);
            this->IsolateTarget(N2);
            this->ConnectTarget(N0, N1, N2);
            this->ConnectTarget(N1, N2, N3);
        }
    }
    return 0;
}

BaseComponentList::BaseComponentList() :
    mpFirstComponent(nullptr)
{

}

BaseComponentList::~BaseComponentList()
{
    this->DeleteAll();
}

int BaseComponentList::Initialize()
{
    int temp = 0;
    for (BaseComponent *current = this->mpFirstComponent; current != nullptr; current = current->GetNext())
    {
        temp = current->BaseInitialize();
        if (temp != 0)
        {
            break;
        }
    }
    return temp;
}

int BaseComponentList::Finalize()
{
    int temp = 0;
    for (BaseComponent *current = this->mpFirstComponent; current != nullptr; current = current->GetNext())
    {
        temp = current->Finalize();
        if (temp != 0)
        {
            break;
        }
    }
    return temp;
}

int BaseComponentList::EarlyUpdate()
{
    int temp = 0;
    for (BaseComponent *current = this->mpFirstComponent; current != nullptr; current = current->GetNext())
    {
        if (current->GetInitializedFlag() == false ||
            current->GetActiveFlag() == false)
        {
            continue;
        }
        temp = current->EarlyUpdate();
        if (temp != 0)
        {
            break;
        }
    }
    return temp;
}

int BaseComponentList::Update()
{
    int temp = 0;

    // Initializeが終わっていないコンポーネントがあるかもしれないので
    temp = this->Initialize();

    // すべてのコンポーネントのUpdateを行う
    for (BaseComponent *current = this->mpFirstComponent; current != nullptr; current = current->GetNext())
    {
        if (current->GetInitializedFlag() == false ||
            current->GetActiveFlag() == false)
        {
            continue;
        }
        temp = current->Update();
        if (temp != 0)
        {
            break;
        }
    }
    return temp;
}

int BaseComponentList::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    int temp = 0;
    for (BaseComponent *current = this->mpFirstComponent; current != nullptr; current = current->GetNext())
    {
        if (current->GetInitializedFlag() == false ||
            current->GetActiveFlag() == false)
        {
            continue;
        }
        temp = current->HitOnCollision(myCollision, hitCollision);
        if (temp != 0)
        {
            break;
        }
    }
    return temp;
}

int BaseComponentList::LateUpdate()
{
    int temp = 0;
    for (BaseComponent *current = this->mpFirstComponent; current != nullptr; current = current->GetNext())
    {
        if (current->GetInitializedFlag() == false ||
            current->GetActiveFlag() == false)
        {
            continue;
        }
        temp = current->LateUpdate();
        if (temp != 0)
        {
            break;
        }
    }
    return temp;
}

int BaseComponentList::Draw()
{
    int temp = 0;
    for (BaseComponent *current = this->mpFirstComponent; current != nullptr; current = current->GetNext())
    {
        if (current->GetInitializedFlag() == false ||
            current->GetActiveFlag() == false)
        {
            continue;
        }
        temp = current->Draw();
        if (temp != 0)
        {
            break;
        }
    }
    return temp;
}

int BaseComponentList::Add(BaseComponent *target)
{
    if (target == nullptr)
    {
        return -1;
    }
    if (this->mpFirstComponent == nullptr)
    {
        this->mpFirstComponent = target;
        return 0;
    }
    BaseComponent *current = this->mpFirstComponent;
    while (current->GetNext() != nullptr)
    {
        current = current->GetNext();
    }
    this->ConnectTarget(current, target, nullptr);
    this->Sort();
    return 0;
}

int BaseComponentList::DeleteToFlag()
{
    if (this->mpFirstComponent == nullptr)
    {
        return 0;
    }
    BaseComponent *current = this->mpFirstComponent;
    BaseComponent *next = this->mpFirstComponent;
    do
    {
        next = current->GetNext();
        if (current->GetDeleteFlag())
        {
            this->IsolateTarget(current);
            delete current;
        }
        current = next;
    } while (current != nullptr);
    return 0;
}

std::vector<BaseComponent *> BaseComponentList::SearchComponent(ComponentTagAndOrder ctao) const
{
    unsigned long boxSize = 0;
    VoidList list;
    for (BaseComponent *current = this->mpFirstComponent; current != nullptr; current = current->GetNext())
    {
        if (current->GetTagAndOrder() == ctao)
        {
            boxSize++;
        }
    }
    std::vector < BaseComponent *>temp(boxSize);
    boxSize = 0;
    for (BaseComponent *current = this->mpFirstComponent; current != nullptr; current = current->GetNext())
    {
        if (current->GetTagAndOrder() == ctao)
        {
            temp[boxSize] = current;
            boxSize++;
        }
    }
    return temp;
}
