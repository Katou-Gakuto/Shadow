#include "VoidList.h"

class VoidList::VoidNode
{
private:
    void *mpData;
    unsigned long mnNodeNum;
    bool mbDeleteFlag;
    VoidNode *mpPrev;
    VoidNode *mpNext;

public:
    VoidNode(void *data, unsigned long nodeNum) :
        mpData(data),
        mnNodeNum(nodeNum),
        mbDeleteFlag(false),
        mpPrev(nullptr),
        mpNext(nullptr)
    {

    }

    ~VoidNode()
    {

    }

    void *GetDataPtr() const { return this->mpData; }

    void SetNum(unsigned long num) { this->mnNodeNum = num; }
    void SetDeleteFlag(bool flag) { this->mbDeleteFlag = flag; }
    unsigned long GetNum() const { return this->mnNodeNum; }
    bool GetDeleteFlag() const { return this->mbDeleteFlag; }

    void SetPrev(VoidNode *prev) { this->mpPrev = prev; }
    void SetNext(VoidNode *next) { this->mpNext = next; }
    VoidNode *GetPrev() const { return this->mpPrev; }
    VoidNode *GetNext() const { return this->mpNext; }
};

void VoidList::Swap(VoidNode *N1, VoidNode *N2)
{
    // VoidNode1の略がN1でVoidNode2の略がN2です

    /* Swapするにあたっての確認 */
    if (N1 == nullptr || N2 == nullptr)
    {
        // 「そもそもSwapする対象がNULLって、どうゆーこっちゃねん！」ということ
        return;
    }

    // これがtrueの場合はN1とN2が隣り合っているということ
    bool N1nextN2 = (N1->GetNext() == N2);

    // これがtrueの場合はN2とN1が隣り合っているということ
    bool N2nextN1 = (N2->GetNext() == N1);

    // N1とN2が隣り合っていない場合はtrue
    if (N1nextN2 == false && N2nextN1 == false)
    {
        /* Swapの準備 */
        VoidNode *N1prev = N1->GetPrev();
        VoidNode *N1next = N1->GetNext();
        VoidNode *N2prev = N2->GetPrev();
        VoidNode *N2next = N2->GetNext();

        /* Swapの処理 */
        // N2prev←→N1←→N2next
        this->ConnectTarget(N2prev, N1, N2next);

        // N1prev←→N2←→N1next
        this->ConnectTarget(N1prev, N2, N1next);
    }
    else
    {
        // N1→N2の順番だったらtrue
        if (N1nextN2 == true)
        {
            /* Swapの準備 */
            VoidNode *N0 = N1->GetPrev();
            VoidNode *N3 = N2->GetNext();

            /* 現状「N0←→N1←→N2←→N3」であるものを「N0←→N2←→N1←→N3」に変える処理をする */

            /* Swapの処理 */
            // N0←→N2←→N1
            this->ConnectTarget(N0, N2, N1);

            // N2←→N1←→BC3
            this->ConnectTarget(N2, N1, N3);
        }
        else
        {
            /* Swapの準備 */
            VoidNode *N0 = N2->GetPrev();
            VoidNode *N3 = N1->GetNext();

            /* 現状「N0←→N2←→N1←→N3」であるものを「N0←→N1←→N2←→N3」に変える処理をする */

            /* Swapの処理 */
            // BC0←→N1←→N2
            this->ConnectTarget(N0, N1, N2);

            // N1←→N2←→BC3
            this->ConnectTarget(N1, N2, N3);
        }
    }

    //考え方
    /*
    今回、N1とN2で引数を作りましたが、わかりやすくするためにABCDの大文字アルファベットにします

    (隣り合っていない場合の処理)
    A→←B→←C→←D→←E
    ↓
    C→←B→←A→←D→←E
    ってさせたい。

    SwapするA, Bから別のところに飛ぶとき(Swap前)
    Aprev←A→Anext
    Bprev←B→Bnext

    SwapするA, Bに別のところから飛ぶとき(Swap前)
    Aprev→A←Anext
    Bprev→B←Bnext

    SwapするA, Bから別のところに飛ぶとき(Swap後)
    Aprev←B→Anext
    Bprev←A→Bnext

    SwapするA, Bに別のところから飛ぶとき(Swap後)
    Aprev→B←Anext
    Bprev→A←Bnext

    (隣り合っている場合の処理)
    ただし、上のコードをN1とN2が隣り合っている状態でやるとエラーを起こします。

       nullptr == Aprev
             A == Bprev
    Anext == B == Cprev
    Bnext == C == Dprev
    Cnext == D
    ↑の図の通り、AnextとBの本体が同じなので、BのNextにAnextを入れてしまうと、ループになってしまう。
    なので

    N1→N2 == B→Cの状態でつながっている場合は
    A == N1->GetPrev();
    B == N1;
    C == N2;
    D == N2->GetNext();
    とし、そのうえで、
    A←→C←→B
    と
    C←→B←→D
    にわけてくっつけなおします

    ちなみにACBDでつながっている状態からBとCをSwapさせる場合もあるかもしれないので、その時は
    A←→B←→C
    と
    B←→C←→D
    にわけてくっつけなおします
    */
}

void VoidList::ConnectTarget(VoidNode *prev, VoidNode *target, VoidNode *next)
{
    if (target == nullptr)
    {
        return;
    }
    if (prev != nullptr)
    {
        prev->SetNext(target);
    }
    if (next != nullptr)
    {
        next->SetPrev(target);
    }
    if (this->mpFirstNode == next)
    {
        this->mpFirstNode = target;
    }
    target->SetPrev(prev);
    target->SetNext(next);
}

void VoidList::IsolateTarget(VoidNode *target)
{
    if (target == nullptr)
    {
        return;
    }

    VoidNode *prev = target->GetPrev();
    VoidNode *next = target->GetNext();
    if (prev != nullptr)
    {
        prev->SetNext(next);
    }
    if (next != nullptr)
    {
        next->SetPrev(prev);
    }
    if (this->mpFirstNode == target)
    {
        this->mpFirstNode = next;
    }
    target->SetPrev(nullptr);
    target->SetNext(nullptr);
}

VoidList::VoidNode *VoidList::SearchNodeNum(unsigned long num) const
{
    VoidNode *current = this->mpFirstNode;
    while (current != nullptr)
    {
        if (current->GetNum() == num)
        {
            break;
        }
        current = current->GetNext();
    }
    return current;
}

VoidList::VoidNode *VoidList::SearchNodeIndex(unsigned long index) const
{
    VoidNode *current = this->mpFirstNode;
    unsigned long counter = 0;
    while (current != nullptr)
    {
        if (counter == index)
        {
            break;
        }
        current = current->GetNext();
        counter++;
    }
    return current;
}

VoidList::VoidList() :
    mpFirstNode(nullptr)
{

}

VoidList::~VoidList()
{
    this->GetIsolateAll();
}

bool VoidList::Add(void *dataPtr, unsigned long num)
{
    if (dataPtr == nullptr &&
        this->SearchNodeNum(num) != nullptr)
    {
        return false;
    }
    VoidNode *newNode = new VoidNode(dataPtr, num);
    if (newNode == nullptr)
    {
        return false;
    }

    if (this->mpFirstNode == nullptr)
    {
        this->mpFirstNode = newNode;
        return  true;
    }

    VoidNode *current = this->mpFirstNode;
    while (current->GetNext() != nullptr)
    {
        current = current->GetNext();
    }

    this->ConnectTarget(current, newNode, nullptr);
    return  true;
}

// 同じnumを持ったデータがある場合でもリストに追加する関数です。この関数を使用するのはあまりお勧めしません。
bool VoidList::AddForced(void *dataPtr)
{
    if (dataPtr == nullptr)
    {
        return false;
    }
    VoidNode *newNode = new VoidNode(dataPtr, 0);
    if (newNode == nullptr)
    {
        return false;
    }

    if (this->mpFirstNode == nullptr)
    {
        this->mpFirstNode = newNode;
        return  true;
    }

    VoidNode *current = this->mpFirstNode;
    while (current->GetNext() != nullptr)
    {
        current = current->GetNext();
    }

    this->ConnectTarget(current, newNode, nullptr);
    return  true;
}

void *VoidList::GetIsolateToNum(unsigned long num)
{
    void *temp = nullptr;
    VoidNode *target = this->SearchNodeNum(num);
    if (target != nullptr)
    {
        this->IsolateTarget(target);
        temp = target->GetDataPtr();
        delete target;
    }
    return temp;
}

void *VoidList::GetIsolateToIndex(unsigned long index)
{
    void *temp = nullptr;
    VoidNode *target = this->SearchNodeIndex(index);
    if (target != nullptr)
    {
        this->IsolateTarget(target);
        temp = target->GetDataPtr();
        delete target;
    }
    return temp;
}

MallocData<void *> VoidList::GetIsolateToFlag()
{
    if (this->mpFirstNode == nullptr)
    {
        return MallocData<void *>(0);
    }
    unsigned long dataCount = 0;
    VoidNode *current = this->mpFirstNode;
    while (current != nullptr)
    {
        if (current->GetDeleteFlag())
        {
            dataCount++;
        }
        current = current->GetNext();
    }
    MallocData<void *> temp(dataCount);
    dataCount = 0;
    current = this->mpFirstNode;
    VoidNode *next = nullptr;
    while(true)
    {
        next = current->GetNext();
        if (current->GetDeleteFlag())
        {
            this->IsolateTarget(current);
            temp[dataCount] = current->GetDataPtr();
            delete current;
            dataCount++;
        }
        if (next != nullptr)
        {
            current = next;
        }
        else
        {
            break;
        }
    }
    return temp;
}

MallocData<void *> VoidList::GetIsolateAll()
{
    if (this->mpFirstNode == nullptr)
    {
        return MallocData<void *>(0);
    }

    unsigned long dataCount = this->GetSize();
    MallocData<void *> temp(0);
    temp.Resize(dataCount);
    if (dataCount == 0)
    {
        return temp;
    }
    dataCount = 0;
    VoidNode *current = this->mpFirstNode;
    VoidNode *next = nullptr;
    while (current != nullptr)
    {
        next = current->GetNext();
        temp.AddParam(current->GetDataPtr());
        delete current;
        current = next;
        dataCount++;
    }

    this->mpFirstNode = nullptr;
    return temp;
}

bool VoidList::SortToNum()
{
    VoidNode *current = this->mpFirstNode;
    VoidNode *ptr = nullptr;
    VoidNode *changeNode = nullptr;
    while (current != nullptr)
    {
        ptr = current->GetNext();
        changeNode = nullptr;
        
        while (ptr != nullptr)
        {
            if (ptr->GetNum() < current->GetNum())
            {
                if (changeNode == nullptr)
                {
                    changeNode = ptr;
                }
                else
                {
                    if (ptr->GetNum() < changeNode->GetNum())
                    {
                        changeNode = ptr;
                    }
                }
            }

            ptr = ptr->GetNext();
        }

        // 交換したいものがあるなら処理を行う
        if (changeNode != nullptr)
        {
            // 交換する
            this->Swap(current, changeNode);

            // currentが後ろに追いやられて、反対にchangeNodeが前に出てきたので
            current = changeNode;
        }

        current = current->GetNext();
    }

    return true;
}

void *VoidList::SearchDataNum(unsigned long num) const
{
    void *temp = nullptr;
    VoidNode *current = this->SearchNodeNum(num);
    if (current != nullptr)
    {
        temp = current->GetDataPtr();
    }
    return temp;
}

void *VoidList::SearchDataIndex(unsigned long index) const
{
    void *temp = nullptr;
    VoidNode *current = this->SearchNodeIndex(index);
    if (current != nullptr)
    {
        temp = current->GetDataPtr();
    }
    return temp;
}

unsigned long VoidList::GetSize() const
{
    unsigned long counter = 0;
    VoidNode *current = this->mpFirstNode;
    while (current != nullptr)
    {
        counter++;
        current = current->GetNext();
    }
    return counter;
}

void VoidList::OnDeleteFlagNum(unsigned long num)
{
    this->SearchNodeNum(num)->SetDeleteFlag(true);
}

void VoidList::OnDeleteFlagIndex(unsigned long index)
{
    this->SearchNodeIndex(index)->SetDeleteFlag(true);
}