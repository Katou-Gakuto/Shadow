#pragma once

#include <type_traits>

// 不特定多数から参照をカウントし、オブジェクトの寿命を管理するクラス
template<class T>
class ReferenceManager
{
private:
    unsigned long mnReferenceCount;
    T *mpManagimentObject;

public:

    // コンストラクタ
    ReferenceManager() :
        mnReferenceCount(0),
        mpManagimentObject(nullptr)
    {
    }

    // デストラクタ
    ~ReferenceManager()
    {
        // 参照を減らし、参照に合わせてオブジェクトの削除などを行う
        this->CountDown();
    }

    // コピー禁止
    ReferenceManager(const ReferenceManager &) = delete;
    ReferenceManager &operator= (const ReferenceManager &) = delete;

    // コンストラクタ(ムーブ)
    ReferenceManager(ReferenceManager &&other) noexcept
    {
        // ムーブを行う
        this->Move(other);
    }

    // ムーブを行う関数(右辺値参照)
    ReferenceManager<T> &Move(ReferenceManager &&other)
    {
        // まず参照を引き継ぐ
        this->mpManagimentObject = other.mpManagimentObject;
        this->mnReferenceCount = other.mnReferenceCount;

        // otherが何も参照していないようにする
        other.mpManagimentObject = nullptr;
        other.mnReferenceCount = 0;

        // 自身を返す
        return *this;
    }

    // 管理するオブジェクトを設定する関数
    // この関数内部で参照を一つ増やしているため、削除時に参照を1つ残すようにしてください
    ReferenceManager<T> &Set(T *manegimentObject)
    {
        // 既に管理しているオブジェクトがあった場合は処理を行う
        if (this->mpManagimentObject != nullptr)
        {
            // 既に管理しているオブジェクトがあることを返す
            return *this;
        }

        // オブジェクトのポインタを取得する
        this->mpManagimentObject = manegimentObject;

        // 参照を増やす
        this->mnReferenceCount++;

        // 引数の値からオブジェクトを参照できないようにする
        manegimentObject = nullptr;

        // 成功したことを返す
        return *this;
    }

    // 代入演算子(右辺値参照ムーブ)
    ReferenceManager<T> &operator= (ReferenceManager &&other) noexcept
    {
        // ムーブを行う
        return this->Move(std::move(other));
    }

    // 代入演算子(管理オブジェクトを設定する)
    ReferenceManager<T> &operator= (T *ptr)
    {
        // オブジェクトを設定する
        return this->Set(ptr);
    }

    // 参照を増やす関数
    int CountUp()
    {
        // 管理しているオブジェクトがない場合は処理を行う
        if (this->mpManagimentObject == nullptr)
        {
            // 管理しているオブジェクトがないことを返す
            return -1;
        }

        // 参照を増やす
        this->mnReferenceCount++;

        // 成功したことを返す
        return 0;
    }

    // 参照を減らし、オブジェクトの削除なども行う関数
    int CountDown()
    {
        // 管理しているオブジェクトがない場合は処理を行う
        if (this->mpManagimentObject == nullptr)
        {
            // 管理しているオブジェクトがないことを返す
            return -1;
        }

        // 参照を減らす
        this->mnReferenceCount--;

        // 参照がなくなった場合は処理を行う
        if (this->mnReferenceCount == 0)
        {
            // 管理しているオブジェクトを消去する
            delete this->mpManagimentObject;

            // 明示的にnullptrを入れる
            this->mpManagimentObject = nullptr;
        }

        // 成功したことを返す
        return 0;
    }

    // 現在の参照のカウントを取得する関数
    const unsigned long GetCount() const
    {
        // 参照のカウントを返す
        return this->mnReferenceCount;
    }

    // 管理しているオブジェクトを取得する関数
    T *GetManagimentObject() const
    {
        // オブジェクトを返す
        return this->mpManagimentObject;
    }

    // 管理しているオブジェクトがあるかを確認する
    bool CheckManagiment() const
    {
        // 管理しているオブジェクトがnullptrではないかを返す
        return this->mpManagimentObject != nullptr;
    }

    // 
    bool CheckLastReference() const
    {
        return this->mnReferenceCount == 1;
    }

    // 
    T *operator->()
    {
        return this->GetManagimentObject();
    }

    // 
    const T *operator->() const
    {
        return this->GetManagimentObject();
    }
};
