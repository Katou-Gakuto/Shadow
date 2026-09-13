#pragma once

#include "MallocData.h"

class VoidList
{
private:
    class VoidNode;
    VoidNode *mpFirstNode;

    void Swap(VoidNode *node1, VoidNode *node2);
    void ConnectTarget(VoidNode *prev, VoidNode *target, VoidNode *next);
    void IsolateTarget(VoidNode *target);

    VoidList::VoidNode *SearchNodeNum(unsigned long num) const;
    VoidList::VoidNode *SearchNodeIndex(unsigned long index) const;

public:
    VoidList();
    ~VoidList();

    bool Add(void *dataPtr, unsigned long num);         // 追加が成功したらtrueを返し、すでに同じnumを持ったデータなどがあればfalseを返します
    bool AddForced(void *dataPtr);                      // 同じnumを持ったデータがある場合でもリストに追加する関数です。この関数を使用するのはあまりお勧めしません。
    void *GetIsolateToNum(unsigned long num);           // 削除が成功したらtrueを返し、numを持ったデータを見つけられなかったらfalseを返します
    void *GetIsolateToIndex(unsigned long index);       // 削除が成功したらtrueを返し、numを持ったデータを見つけられなかったらfalseを返します
    MallocData<void *> GetIsolateToFlag();              // 削除が成功したらtrueを返します
    MallocData<void *> GetIsolateAll();                 // 削除が成功したらtrueを返します
    bool SortToNum();                                   // 整列が成功したらtrueを返し、途中でエラーが起こった場合は整列を中断してfalseを返します
    void *SearchDataNum(unsigned long num) const;       // 線形リストに登録したデータへのポインタ返します
    void *SearchDataIndex(unsigned long index) const;   // 線形リストに登録したデータへのポインタ返します
    unsigned long GetSize() const;                      // 線形リストに登録したデータの総数を返します



    void OnDeleteFlagNum(unsigned long num);
    void OnDeleteFlagIndex(unsigned long index);
};