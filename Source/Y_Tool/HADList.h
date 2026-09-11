#pragma once

#include "MySmartPtr.h"

// もし全ての関数を使いたい場合は
// #define HADLIST_FUNCTION_ALLOPEN
// をコピペしてください

// "Heap Auto Delete List"
// このクラスの変数が削除されるとき、自動でリスト内のヒープ領域のデータ本体も削除するクラスです
// Search関数の使い捨ての返り値などにご使用ください
// ※このクラスのリスト以外にもリストや配列で管理しているデータを渡すのは非推奨です
// ※このクラスのテンプレート引数にポインタはつけないでください
template <class Type>
class HADList
{
private:
    // リスト部分
    class HADNodeManager
    {
        // ノード部分
        class HADNode
        {
        private:
            MySmartPtr<Type> mpDataPtr;
            unsigned long mnNodeNum;
            bool mbDeleteFlag;
            HADNode* mpPrev;
            HADNode* mpNext;

        public:
            HADNode(Type* data, unsigned long nodeNum) :
                mpDataPtr(data),
                mnNodeNum(nodeNum),
                mbDeleteFlag(false),
                mpPrev(nullptr),
                mpNext(nullptr)
            {
            }
            ~HADNode()
            {
                this->mpPrev = nullptr;
                this->mpNext = nullptr;

                // このデストラクタを出ようとすると、MySmartPtrのデストラクタに移行する
                // そしてMySmartPtrが管理オブジェクトを破壊する
            }


#ifdef HADLIST_FUNCTION_ALLOPEN
            Type &GetData() { return *(this->mpDataPtr); }
#endif // HADLIST_FUNCTION_ALLOPEN
            Type *GetDataPtr() const { return (Type *)(this->mpDataPtr.operator->()); }
#ifdef HADLIST_FUNCTION_ALLOPEN
            MySmartPtr<Type> GetDataSmartPtr() const { return this->mpDataPtr; }
#endif // HADLIST_FUNCTION_ALLOPEN
            void SetNum(unsigned long num) { this->mnNodeNum = num; }
            void SetDeleteFlag(bool flag) { this->mbDeleteFlag = flag; }
            unsigned long GetNum() const { return this->mnNodeNum; }
            bool GetDeleteFlag() const { return this->mbDeleteFlag; }

            void SetPrev(HADNode* prev) { this->mpPrev = prev; }
            void SetNext(HADNode* next) { this->mpNext = next; }
            HADNode* GetPrev() const { return this->mpPrev; }
            HADNode* GetNext() const { return this->mpNext; }
        };

        HADNode *mpFirstNode;

        void Swap(HADNode *N1, HADNode *N2)
        {
            // HADNode1の略がN1でHADNode2の略がN2です

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
                HADNode * N1prev = N1->GetPrev();
                HADNode * N1next = N1->GetNext();
                HADNode * N2prev = N2->GetPrev();
                HADNode * N2next = N2->GetNext();

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
                    HADNode * N0 = N1->GetPrev();
                    HADNode * N3 = N2->GetNext();

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
                    HADNode * N0 = N2->GetPrev();
                    HADNode * N3 = N1->GetNext();

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

        void ConnectTarget(HADNode * prev, HADNode * target, HADNode * next)
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

        void IsolateTarget(HADNode * target)
        {
            if (target == nullptr)
            {
                return;
            }

            HADNode * prev = target->GetPrev();
            HADNode * next = target->GetNext();
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

        HADList::HADNodeManager::HADNode * SearchNodeNum(unsigned long num) const
        {
            HADNode * current = this->mpFirstNode;
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

        HADList::HADNodeManager::HADNode * SearchNodeIndex(unsigned long index) const
        {
            HADNode * current = this->mpFirstNode;
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

        void DeleteAll()
        {
            HADNode *current = this->mpFirstNode;
            HADNode *next = nullptr;
            while (current != nullptr)
            {
                next = current->GetNext();
                delete current;
                current = next;
            }
        }

    public:
        HADNodeManager() :
            mpFirstNode(nullptr)
        {
        }

        ~HADNodeManager()
        {
            // この関数の中でNodeを消すとMySmartPtrが消える。
            // MySmartPtrが消えたら、管理中のデータも消える。
            this->DeleteAll();
        }

        // 追加が成功したらtrueを返し、すでに同じnumを持ったデータなどがあればfalseを返します
        bool Add(Type* dataPtr, unsigned long num)
        {
            if (dataPtr == nullptr &&
                this->SearchNodeNum(num) != nullptr)
            {
                return false;
            }
            HADNode * newNode = new HADNode(dataPtr, num);
            if (newNode == nullptr)
            {
                return false;
            }

            if (this->mpFirstNode == nullptr)
            {
                this->mpFirstNode = newNode;
                return  true;
            }

            HADNode * current = this->mpFirstNode;
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
            HADNode * newNode = new HADNode(dataPtr, 0);
            if (newNode == nullptr)
            {
                return false;
            }

            if (this->mpFirstNode == nullptr)
            {
                this->mpFirstNode = newNode;
                return  true;
            }

            HADNode * current = this->mpFirstNode;
            while (current->GetNext() != nullptr)
            {
                current = current->GetNext();
            }

            this->ConnectTarget(current, newNode, nullptr);
            return  true;
        }

        // 削除が成功したらtrueを返します
        bool DeleteOnFlag()
        {
            HADNode * current = this->mpFirstNode;
            HADNode * next = nullptr;
            while (current != nullptr)
            {
                next = current->GetNext();
                if (current->GetDeleteFlag())
                {
                    delete current;
                }
                current = next;
            }

            return true;
        }

        // 整列が成功したらtrueを返し、途中でエラーが起こった場合は整列を中断してfalseを返します
        bool SortToNum()
        {
            HADNode * current = this->mpFirstNode;
            HADNode * ptr = nullptr;
            HADNode * changeNode = nullptr;
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

        template<typename IfFunc, typename ActionFunc>
        void ProcessingLambda(IfFunc ifFunc, ActionFunc actionFunc)
        {
            HADNode *current = this->mpFirstNode;
            while (current != nullptr)
            {
                if (ifFunc(current->GetDataPtr()))
                {
                    actionFunc(current->GetDataPtr());
                }
                current = current->GetNext();
            }
        }

#ifdef HADLIST_FUNCTION_ALLOPEN
        // 線形リストに登録したデータへの参照を返します
        Type& SearchDataNum(unsigned long num) const
        {
            HADNode * current = this->SearchNodeNum(num);
            return current->GetData();
        }
        // 線形リストに登録したデータへの参照を返します
        Type& SearchDataIndex(unsigned long index) const
        {
            HADNode * current = this->SearchNodeIndex(index);
            return current->GetData();
        }
#endif // HADLIST_FUNCTION_ALLOPEN

        // 線形リストに登録したデータへの参照を返します
        Type *SearchDataPtrNum(unsigned long num) const
        {
            HADNode *current = this->SearchNodeNum(num);
            return current->GetDataPtr();
        }

        // 線形リストに登録したデータへの参照を返します
        Type *SearchDataPtrIndex(unsigned long index) const
        {
            HADNode *current = this->SearchNodeIndex(index);
            if (current != nullptr)
            {
                return current->GetDataPtr();
            }
            return nullptr;
        }

#ifdef HADLIST_FUNCTION_ALLOPEN
        // 線形リストに登録したデータへの参照を返します
        MySmartPtr<Type> SearchDataSmartPtrNum(unsigned long num) const
        {
            HADNode *current = this->SearchNodeNum(num);
            return current->GetDataSmartPtr();
        }

        // 線形リストに登録したデータへの参照を返します
        MySmartPtr<Type> SearchDataSmartPtrIndex(unsigned long index) const
        {
            HADNode *current = this->SearchNodeIndex(index);
            return current->GetDataSmartPtr();
        }
#endif // HADLIST_FUNCTION_ALLOPEN

        // 線形リストに登録したデータの総数を返します
        unsigned long GetSize() const
        {
            unsigned long counter = 0;
            HADNode * current = this->mpFirstNode;
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
    };

    // 唯一のメンバー変数
    MySmartPtr<HADNodeManager> mpManager;

    // スワップ？
    
public:
    HADList() :
        mpManager()
    {
        this->mpManager = new HADNodeManager;
    }
    HADList(const HADList& src)
    {
        this->mpManager = src.mpManager;
    }
    ~HADList()
    {
        // MySmartPtrでHADNodeManagerの削除が行われる
    }

    // 追加が成功したらtrueを返し、すでに同じnumを持ったデータなどがあればfalseを返します
    bool Add(Type* dataPtr, unsigned long num)
    {
        return this->mpManager->Add(dataPtr, num);
    }

    // 同じnumを持ったデータがある場合でもリストに追加する関数です。この関数を使用するのはあまりお勧めしません。
    bool AddForced(Type* dataPtr)
    {
        return this->mpManager->AddForced(dataPtr);
    }

    template<typename IfFunc, typename ActionFunc>
    void ProcessingLambda(IfFunc ifFunc, ActionFunc actionFunc)
    {
        this->mpManager->ProcessingLambda(ifFunc, actionFunc);
    }

    // 削除フラグが立っているもののみ、削除を行う
    bool DeleteOnFlag()
    {
        return this->mpManager->DeleteOnFlag();
    }
    
    // 整列が成功したらtrueを返し、途中でエラーが起こった場合は整列を中断してfalseを返します
    bool SortToNum()
    {
        return this->mpManager->SortToNum();
    }

#ifdef HADLIST_FUNCTION_ALLOPEN
    // 線形リストに登録したデータへの参照を返します
    Type& SearchDataNum(unsigned long num) const
    {
        return this->mpManager->SearchDataNum(num);
    }

    // 線形リストに登録したデータへの参照を返します
    Type& SearchDataIndex(unsigned long index) const
    {
        return this->mpManager->SearchDataIndex(index);
    }
#endif // HADLIST_FUNCTION_ALLOPEN
    // 線形リストに登録したデータへの参照を返します
    Type *SearchDataPtrNum(unsigned long num) const
    {
        return this->mpManager->SearchDataPtrNum(num);
    }

    // 線形リストに登録したデータへの参照を返します
    Type *SearchDataPtrIndex(unsigned long index) const
    {
        return this->mpManager->SearchDataPtrIndex(index);
    }

#ifdef HADLIST_FUNCTION_ALLOPEN
    // 線形リストに登録したデータへの参照を返します
    MySmartPtr<Type> SearchDataSmartPtrNum(unsigned long num) const
    {
        return this->mpManager->SearchDataSmartPtrNum(num);
    }

    // 線形リストに登録したデータへの参照を返します
    MySmartPtr<Type> SearchDataSmartPtrIndex(unsigned long index) const
    {
        return this->mpManager->SearchDataSmartPtrIndex(index);
    }
#endif // HADLIST_FUNCTION_ALLOPEN
    // 線形リストに登録したデータの総数を返します
    unsigned long GetSize() const
    {
        return this->mpManager->GetSize();
    }

    void OnIsolateFlagNum(unsigned long num)
    {
        this->mpManager->OnIsolateFlagNum(num);
    }

    void OnIsolateFlagIndex(unsigned long index)
    {
        this->mpManager->OnIsolateFlagNum(index);
    }

};
