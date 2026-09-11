#pragma once

#include "MallocData.h"
#include "MYTMP.h"

// テンプレートクラスのリスト
// VoidListがテンプレートになっただけです
template <class Type>
class TypeList
{
private:
    class TypeNode
	{
    private:
        Type* mpData;
        unsigned long mnNodeNum;
        bool mbDeleteFlag;
        TypeNode* mpPrev;
        TypeNode* mpNext;

    public:
        TypeNode(Type* data, unsigned long nodeNum) :
            mpData(data),
            mnNodeNum(nodeNum),
            mbDeleteFlag(false),
            mpPrev(nullptr),
            mpNext(nullptr)
        {
        }

        ~TypeNode()
        {
        }

        Type* GetDataPtr() const { return this->mpData; }

        void SetNum(unsigned long num) { this->mnNodeNum = num; }
        void SetDeleteFlag(bool flag) { this->mbDeleteFlag = flag; }
        unsigned long GetNum() const { return this->mnNodeNum; }
        bool GetDeleteFlag() const { return this->mbDeleteFlag; }

        void SetPrev(TypeNode* prev) { this->mpPrev = prev; }
        void SetNext(TypeNode* next) { this->mpNext = next; }
        TypeNode* GetPrev() const { return this->mpPrev; }
        TypeNode* GetNext() const { return this->mpNext; }
	};

	TypeNode* mpFirstNode;

    void Swap(TypeNode*N1, TypeNode*N2)
    {
        // TypeNode1の略がN1でTypeNode2の略がN2です

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
            TypeNode* N1prev = N1->GetPrev();
            TypeNode* N1next = N1->GetNext();
            TypeNode* N2prev = N2->GetPrev();
            TypeNode* N2next = N2->GetNext();

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
                TypeNode* N0 = N1->GetPrev();
                TypeNode* N3 = N2->GetNext();

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
                TypeNode* N0 = N2->GetPrev();
                TypeNode* N3 = N1->GetNext();

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

    void ConnectTarget(TypeNode* prev, TypeNode* target, TypeNode* next)
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

    void IsolateTarget(TypeNode* target)
    {
        if (target == nullptr)
        {
            return;
        }

        TypeNode* prev = target->GetPrev();
        TypeNode* next = target->GetNext();
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

    TypeList::TypeNode* SearchNodeNum(unsigned long num) const
    {
        TypeNode* current = this->mpFirstNode;
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

    TypeList::TypeNode* SearchNodeIndex(unsigned long index) const
    {
        TypeNode* current = this->mpFirstNode;
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

public:
    TypeList() :
        mpFirstNode(nullptr)
    {
    }

    ~TypeList()
    {
        this->GetIsolateAll();
    }

    // 追加が成功したらtrueを返し、すでに同じnumを持ったデータなどがあればfalseを返します
    bool Add(Type* dataPtr, unsigned long num)
    {
        if (dataPtr == nullptr &&
            this->SearchNodeNum(num) != nullptr)
        {
            return false;
        }
        TypeNode* newNode = new TypeNode(dataPtr, num);
        if (newNode == nullptr)
        {
            return false;
        }

        if (this->mpFirstNode == nullptr)
        {
            this->mpFirstNode = newNode;
            return  true;
        }

        TypeNode* current = this->mpFirstNode;
        while (current->GetNext() != nullptr)
        {
            current = current->GetNext();
        }

        this->ConnectTarget(current, newNode, nullptr);
        return  true;
    }

    // 同じnumを持ったデータがある場合でもリストに追加する関数です。この関数を使用するのはあまりお勧めしません。
    bool AddForced(Type* dataPtr)
    {
        if (dataPtr == nullptr)
        {
            return false;
        }
        TypeNode* newNode = new TypeNode(dataPtr, 0);
        if (newNode == nullptr)
        {
            return false;
        }

        if (this->mpFirstNode == nullptr)
        {
            this->mpFirstNode = newNode;
            return  true;
        }

        TypeNode* current = this->mpFirstNode;
        while (current->GetNext() != nullptr)
        {
            current = current->GetNext();
        }

        this->ConnectTarget(current, newNode, nullptr);
        return  true;
    }

    // 1つ目のラムダ式で受け取った条件に当てはまるデータを、2つ目のラムダ式の通りに処理を行う関数
    template <typename Predicate, typename Action>
    void ProcessingRambdaForword(Predicate ifFunc, Action func)
    {
        TypeNode* current = this->mpFirstNode;
        while (current != nullptr)
        {
            if (ifFunc(current->GetDataPtr()))
            {
                func(current->GetDataPtr());
            }
            current = current->GetNext();
        }
    }

    // 1つ目のラムダ式で受け取った条件に当てはまるデータを、2つ目のラムダ式の通りに処理を行う関数
    template <typename Predicate, typename Action>
    void ProcessingRambdaBack(Predicate ifFunc, Action func)
    {
        // 先頭のノードを取得する
        TypeNode *current = this->mpFirstNode;

        // 先頭のノードがnullptrじゃないなら処理を行う
        if (current != nullptr)
        {
            // currentの次がnullptrになるまで処理を行う(要するに最後尾を見つける)
            while (current->GetNext() != nullptr)
            {
                // ノードを次に進める
                current = current->GetNext();
            }

            // 最後尾から先頭にかけて処理を行う
            while (current != nullptr)
            {
                // 渡されたラムダを使い、処理を行うかを判断する
                if (ifFunc(current->GetDataPtr()))
                {
                    // 渡されたラムダでデータに対して処理を行う
                    func(current->GetDataPtr());
                }

                // ひとつ前のノードに戻る
                current = current->GetPrev();
            }
        }
    }

    // 削除が成功したらtrueを返し、numを持ったデータを見つけられなかったらfalseを返します
    Type* GetIsolateToNum(unsigned long num)
    {
        Type * temp = nullptr;
        TypeNode* target = this->SearchNodeNum(num);
        if (target != nullptr)
        {
            this->IsolateTarget(target);
            temp = target->GetDataPtr();
            delete target;
        }
        return temp;
    }
    
    // 削除が成功したらtrueを返し、numを持ったデータを見つけられなかったらfalseを返します
    Type* GetIsolateToIndex(unsigned long index)
    {
        Type * temp = nullptr;
        TypeNode* target = this->SearchNodeIndex(index);
        if (target != nullptr)
        {
            this->IsolateTarget(target);
            temp = target->GetDataPtr();
            delete target;
        }
        return temp;
    }

    // 削除が成功したらtrueを返します
    MallocData<Type*> GetIsolateToFlag()
    {
        if (this->mpFirstNode == nullptr)
        {
            return MallocData<Type *>(0);
        }
        unsigned long dataCount = 0;
        TypeNode* current = this->mpFirstNode;
        while (current != nullptr)
        {
            if (current->GetDeleteFlag())
            {
                dataCount++;
            }
            current = current->GetNext();
        }
        MallocData<Type *> temp(dataCount);
        dataCount = 0;
        current = this->mpFirstNode;
        TypeNode* next = nullptr;
        while (true)
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

    // 削除が成功したらtrueを返します
    MallocData<Type*> GetIsolateAll()
    {
        if (this->mpFirstNode == nullptr)
        {
            return MallocData<Type *>(0);
        }

        unsigned long dataCount = this->GetSize();
        MallocData<Type *> temp(0);
        temp.Resize(dataCount);
        if (dataCount == 0)
        {
            return temp;
        }
        dataCount = 0;
        TypeNode* current = this->mpFirstNode;
        TypeNode* next = nullptr;
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

    // 整列が成功したらtrueを返し、途中でエラーが起こった場合は整列を中断してfalseを返します
    bool SortToNum()
    {
        TypeNode* current = this->mpFirstNode;
        TypeNode* ptr = nullptr;
        TypeNode* changeNode = nullptr;
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

    // 線形リストに登録したデータへのポインタ返します
    Type* SearchDataNum(unsigned long num) const
    {
        Type * temp = nullptr;
        TypeNode* current = this->SearchNodeNum(num);
        if (current != nullptr)
        {
            temp = current->GetDataPtr();
        }
        return temp;
    }
	
    // 線形リストに登録したデータへのポインタ返します
    Type* SearchDataIndex(unsigned long index) const
    {
        Type * temp = nullptr;
        TypeNode* current = this->SearchNodeIndex(index);
        if (current != nullptr)
        {
            temp = current->GetDataPtr();
        }
        return temp;
    }
    
    // 線形リストに登録したデータの総数を返します
    unsigned long GetSize() const
    {
        unsigned long counter = 0;
        TypeNode* current = this->mpFirstNode;
        while (current != nullptr)
        {
            counter++;
            current = current->GetNext();
        }
        return counter;
    }

    void OnIsolateFlagNum(unsigned long num)
    {
        this->SearchNodeNum(num)->SetDeleteFlag(true);
    }
	
    void OnIsolateFlagIndex(unsigned long index)
    {
        this->SearchNodeIndex(index)->SetDeleteFlag(true);
    }

    // ラムダ式で受け取った条件を使用して、削除フラグをオンにする関数
    template <typename Predicate>
    void OnIsolateFlagRambda(Predicate ifFunc)
    {
        TypeNode* current = this->mpFirstNode;
        while (current != nullptr)
        {
            if (ifFunc(current->GetDataPtr()))
            {
                current->SetDeleteFlag(true);
            }
            current = current->GetNext();
        }
    }
};