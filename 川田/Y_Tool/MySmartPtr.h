#pragma once
#define _WINSOCKAPI_
#include <windows.h>

// ヒープ領域にあるオブジェクトの管理を行う、自作のスマートポインタ
template <class Type>
class MySmartPtr
{
private:
    // オブジェクトへのポインタ。ヒープ領域のやつを渡してね。
    // ↓この方法を使うと内部のType *のデストラクタを呼ぶときに怪しい挙動になるので泣く泣くこうしています
    // char *mpPtr
    Type *mpTypePtr;

    // 参照カウント
    unsigned long *mpRefCountPtr;

    // 参照カウント用のヒープ領域を作成し、ptrの内容を抜き取る関数
    int CreateMem(Type *&ptr)
    {
        int temp = -1;
        if (this->CheckRef0())
        {
            // 参照カウント用のメモリをヒープ領域で生成する
            this->mpRefCountPtr = new unsigned long;

            // 参照カウントを0にする
            *this->mpRefCountPtr = 0;

            // オブジェクトのアドレス用のメモリを設定する
            this->mpTypePtr = ptr;

            // 参照カウントを増やす
            (*this->mpRefCountPtr)++;

            // 返り値を0に設定する
            temp = 0;
        }

        // 結果を返す
        return temp;
    }

    // 参照カウントのヒープ領域を削除し、オブジェクトも削除する関数
    int ClearMem()
    {
        if (this->mpRefCountPtr != nullptr)
        {
            delete this->mpRefCountPtr;
            this->mpRefCountPtr = nullptr;
        }
        if (this->mpTypePtr != nullptr)
        {
            delete this->mpTypePtr;
            this->mpTypePtr = nullptr;
        }
        return 0;
    }

    // 参照カウントを増やす関数
    int IncrementRefCount()
    {
        (*this->mpRefCountPtr)++;
        return 0;
    }

    // 参照カウントを減らす関数
    int DecrementRefCount()
    {
        (*this->mpRefCountPtr)--;
        return 0;
    }

    // 参照カウントが0や、そもそも無効な値が入っていた場合にtrueを返す関数
    bool CheckRef0() const
    {
        bool temp = false;
        if (this->mpTypePtr == nullptr ||
            this->mpRefCountPtr == nullptr ||
            (*this->mpRefCountPtr) == 0)
        {
            temp = true;
        }
        return temp;
    }

public:
    // デフォルトコンストラクタ
    // 内部の値は全て無効なものになっています
    MySmartPtr() :
        mpTypePtr(nullptr),
        mpRefCountPtr(nullptr)
    {
    }

    // 変換コンストラクタ
    // スマートポインタの生成に成功した場合は内部の値も有効になっています
    // この関数は右辺値参照であり、名前のある永続的なオブジェクトの所有権を受け取ります
    MySmartPtr(Type *&ptr) :
        mpTypePtr(nullptr),
        mpRefCountPtr(nullptr)
    {
        // ptrがnullptrじゃなかったら処理を行う
        if (ptr != nullptr)
        {
            // ptrの内容を抜き取る
            this->CreateMem(ptr);

            // 所有権を移したのでptrをnullptrにする
            ptr = nullptr;
        }
    }

    // 変換コンストラクタ
    // スマートポインタの生成に成功した場合は内部の値も有効になっています
    // この関数は左辺値参照であり、名前のない瞬間的なオブジェクトを所有権ごと受け取ります
    MySmartPtr(Type *&&ptr) :
        mpTypePtr(nullptr),
        mpRefCountPtr(nullptr)
    {
        // ptrがnullptrじゃなかったら処理を行う
        if (ptr != nullptr)
        {
            // ptrの内容を抜き取る
            this->CreateMem(ptr);

            // 所有権を移したのでptrをnullptrにする
            ptr = nullptr;
        }
    }

    // コピーコンストラクタ
    // 参照を共有し、参照カウントを増やします
    MySmartPtr(const MySmartPtr &src) :
        mpTypePtr(nullptr),
        mpRefCountPtr(nullptr)
    {
        // コピーを行う
        this->Copy(src);
    }

    // ムーブコンストラクタ
    // 参照を引き継ぎます
    MySmartPtr(MySmartPtr *&&other) noexcept :
        mpTypePtr(nullptr),
        mpRefCountPtr(nullptr)
    {
        // コピーを行う
        this->Move(other);
    }

    // デストラクタ
    // 参照カウントを一つ減らし、参照カウントが0になった場合はオブジェクトごと削除します
    ~MySmartPtr()
    {
        // 自身の参照を減らす
        this->DecrementRefCount();

        // 参照が0になった場合は処理を行う
        if (this->CheckRef0())
        {
            // メモリを破棄する
            this->ClearMem();
        }
    }

    // コピーを行う関数
    MySmartPtr &Copy(const MySmartPtr &src)
    {
        // まずアドレスを共有する
        this->mpTypePtr = src.mpTypePtr;
        this->mpRefCountPtr = src.mpRefCountPtr;

        // 自身が参照などを管理している場合は処理を行う
        if (this->CheckRef0())
        {
            // 参照を増やし、失敗した場合は処理を行う
            if (this->IncrementRefCount() == -1)
            {
                // メモリを一切合切破棄する
                this->ClearMem();
            }
        }

        // 自身を返す
        return *this;
    }

    // ムーブを行う関数
    MySmartPtr &Move(MySmartPtr &&other)
    {
        // まずアドレスを共有する
        this->mpTypePtr = other.mpTypePtr;
        this->mpRefCountPtr = other.mpRefCountPtr;

        // otherが管理できないようにする
        other.mpTypePtr = nullptr;
        other.mpRefCountPtr = nullptr;

        // 自身を返す
        return *this;
    }

    // 自身が管理するオブジェクトを設定する関数(左辺値参照)
    MySmartPtr &Set(Type *&ptr)
    {
        // 自身が既にオブジェクトを管理している
        if (this->CheckRef0() == false)
        {
            return *this;
        }

        // nullptrを代入している
        if (ptr == nullptr)
        {
            return *this;
        }

        // ptrの内容を抜き取る
        this->CreateMem(ptr);

        // 所有権を移したのでptrをnullptrにする
        ptr = nullptr;

        // 自身を返す
        return *this;
    }

    // 自身が管理するオブジェクトを設定する関数(右辺値参照)
    MySmartPtr &Set(Type *&&ptr)
    {
        // 自身が既にオブジェクトを管理している
        if (this->CheckRef0() == false)
        {
            return *this;
        }

        // nullptrを代入している
        if (ptr == nullptr)
        {
            return *this;
        }

        // ptrの内容を抜き取る
        this->CreateMem(ptr);

        // 念の為にptrをnullptrにする
        ptr = nullptr;

        // 自身を返す
        return *this;
    }

    // 代入演算子(コピー)
    MySmartPtr &operator=(const MySmartPtr &src)
    {
        // 自身を返す
        return this->Copy(src);
    }

    // 代入演算子(ムーブ)
    MySmartPtr &operator=(MySmartPtr &&src)
    {
        // 自身を返す
        return this->Move(std::move(src));
    }

    // 代入演算子(オブジェクト左辺値参照)
    MySmartPtr &operator=(Type &ptr)
    {
        // 自身を返す
        return this->Set(ptr);
    }

    // 代入演算子(オブジェクト右辺値参照)
    MySmartPtr &operator=(Type &&ptr)
    {
        // 自身を返す
        return this->Set(ptr);
    }

    // 現在の参照カウントを取得する関数
    unsigned long GetRefCount() const
    {
        unsigned long temp = 0;
        if (this->mpRefCountPtr != nullptr)
        {
            temp = *this->mpRefCountPtr;
        }
        return temp;
    }

    // 自身によるオブジェクトの管理を終了する関数
    // そのオブジェクトを管理しているのが自身のみの場合は管理オブジェクトを返し、そうでない場合はnullptrを返す
    // ※この関数はポインタの管理を放棄するという無責任な挙動をする関数です。極力使わないでください。
    Type *PopPtr()
    {
        // これからの処理を「自身が現在オブジェクトを管理している場合」のみに限定する
        if (this->CheckRef0())
        {
            return nullptr;
        }

        // 返り値(自分がそのオブジェクトを管理している最後のポインタだった場合のみ自分のオブジェクトを返す)
        Type *temp = nullptr;

        // 参照カウントを減らす
        this->DecrementRefCount();

        // 参照カウントが0になったかを確認する
        if (this->CheckRef0())
        {
            // 参照カウント0になったので、tempに管理オブジェクトを代入する
            temp = this->mpTypePtr;
        }

        // 自身の管理を終える
        this->mpRefCountPtr = nullptr;
        this->mpTypePtr = nullptr;

        // 結果を返す
        return temp;
    }

    // この記述をすることでptr = ～;は不可能になり、 *ptr = ～;は可能になる
    // 参照先が同じなだけの別のポインタを渡しているだけだから安全っちゃ安全なのか
    // 暗黙の型変換を可能にする記述だが、c++はconstなどの型修飾子のみでdeleteを禁止することが出来ない。
    // よって生ポインタを渡すこの関数は実装しません。
    operator Type *() const = delete;

    Type operator*() { return *((Type *)(this->mpTypePtr)); }
    const Type &operator*() const { return *((Type *)(this->mpTypePtr)); }
    Type *operator->() { return (Type *)(this->mpTypePtr); }
    const Type *operator->() const { return (Type *)(this->mpTypePtr); }
};

/*
union TestUnion
{
    int num;
    float param;
    char character[4];



};
*/
