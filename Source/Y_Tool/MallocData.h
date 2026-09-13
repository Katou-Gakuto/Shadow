#pragma once

#include <stdlib.h>

#include "MYTMP.h"

// 自作のstd::vectorみたいなもん
// 
// [index]のコードをする場合は
// 必ず
// 必ず
// 必ず
// 必ず
// 必ず
// index < sizeであること。
// 結局は"配列"だからね。
template <class Type>
class MallocData
{
private:
    // 実メモリ
    Type *mpData;

    // 実メモリの大きさ
    unsigned long mnMemSize;

    // 有効な要素の数(0から詰めている前提)
    unsigned long mnSetParamCount;

    // 自身のメモリを確保する関数
    void MemCreate()
    {
        // 自身がすでにメモリを管理している場合は処理を行う
        if (this->mpData != nullptr)
        {
            // 管理しているメモリを安全に破棄する
            this->MemFree();
        }

        // 設定された新しいメモリの数が1以上であるかで処理を変える
        if (0 < this->mnMemSize)
        {
            // 指定された個数分のメモリを確保する
            mpData = (Type *)malloc(sizeof(Type) * mnMemSize);

            // メモリの確保に失敗した場合は処理を行う
            if (this->mpData == nullptr)
            {
                // 管理しているメモリの大きさが0だと記憶しておく
                this->mnMemSize = 0;
            }
        }
        else
        {
            // メモリをnullptrにする
            this->mpData = nullptr;
        }
    }

    // 自身の管理するメモリを開放する関数
    void MemFree()
    {
        // 自身がメモリを管理している場合は処理を行う
        if (this->mpData != nullptr)
        {
            // メンバーのデストラクタを呼ぶ関数
            this->MemberDestructor();

            // メモリを開放する
            free(this->mpData);

            // メモリを参照しないようにする
            this->mpData = nullptr;
        }

        // メモリの大きさを0にする
        this->mnMemSize = 0;

        // メモリ内の有効な要素数を0にする
        this->mnSetParamCount = 0;
    }

    // 自身の管理するメモリの大きさを増やす関数
    void MemAdd(unsigned long addSize)
    {
        // 新しいメモリを取得
        Type *nextPtr = (Type *)malloc(sizeof(Type) * this->mnMemSize + addSize);

        // 新しいメモリを取得できていない場合は処理を行う
        if (nextPtr == nullptr)
        {
            // 失敗したので即終了
            return;
        }

        // 新しいメモリに、既存のメモリにあったデータを丸ごと移す
        memcpy(nextPtr, this->mpData, sizeof(Type) * this->mnSetParamCount);

        // 既存のメモリを破棄する
        free(this->mpData);

        // 自身の値を更新する
        this->mnMemSize += addSize;
        this->mpData = nextPtr;
    }

    // 全ての要素を引数と同じにする
    void SetAllParam(const Type &src)
    {
        // 有効な要素のみ走査をする
        for (unsigned long i = 0; i < this->mnSetParamCount; i++)
        {
            // 引数と同じデータを代入する
            this->mpData[i] = src;
        }
    }

    // 全ての要素を引数と同じにする
    void SetAllParam(Type &&src)
    {
        // 有効な要素のみ走査をする
        for (unsigned long i = 0; i < this->mnSetParamCount; i++)
        {
            // 引数と同じデータを代入する
            this->mpData[i] = src;
        }
    }

    // メンバ配列の各要素にデストラクタを呼ぶ関数
    void MemberDestructor() noexcept
    {
        // 既に中身が作られた要素のみを操作する
        for (unsigned long i = 0; i < this->mnSetParamCount; i++)
        {
            // このTypeがデストラクタを呼ぶべきかを判断して関数を呼び出す
            this->MemberDestroy(&this->mpData[i], std::is_trivially_destructible<Type>{});
        }
    }

    // trivial型：何もしない
    void MemberDestroy(Type *p, std::true_type) noexcept
    {
        // no-op
    }

    // 非trivial型：デストラクタ呼ぶ
    void MemberDestroy(Type *p, std::false_type) noexcept
    {
        p->~T();
    }

public:
    // コンストラクタ(内部メモリはnullptr)
    MallocData() :
        mpData(nullptr),
        mnMemSize(0),
        mnSetParamCount(0)
    {
    }

    // コンストラクタ(size分のメモリを確保する)
    MallocData(unsigned long size) :
        mpData(nullptr),
        mnMemSize(size),
        mnSetParamCount(size)
    {
        // 自身が管理するメモリを生成する
        this->MemCreate();
    }

    // コンストラクタ(size分のメモリを確保し、initParamで各要素を初期化する)
    MallocData(unsigned long size, const Type &initParam) :
        mpData(nullptr),
        mnMemSize(size),
        mnSetParamCount(size)
    {
        // 自身が管理するメモリを生成する
        this->MemCreate();

        // 自信が管理するメモリを初期化する
        this->SetAllParam(initParam);
    }

    // コンストラクタ(size分のメモリを確保し、initParamで各要素を初期化する)
    MallocData(unsigned long size, Type &&initParam) :
        mpData(nullptr),
        mnMemSize(size),
        mnSetParamCount(size)
    {
        // 自身が管理するメモリを生成する
        this->MemCreate();

        // 自信が管理するメモリを初期化する
        this->SetAllParam(initParam);
    }

    // コピーコンストラクタ
    MallocData(const MallocData &src) :
        mpData(nullptr),
        mnMemSize(0),
        mnSetParamCount(0)
    {
        // 内部でCopy()を呼び出すだけ
        this->Copy(src);
    }

    // ムーブコンストラクタ
    MallocData(MallocData &&other) noexcept :
        mpData(nullptr),
        mnSetParamCount(0),
        mnMemSize(0)
    {
        // 内部でMove()を呼び出すだけ
        this->Move(other);
    }

    // コピー
    MallocData<Type> &operator=(const MallocData &src)
    {
        // 内部でCopy()を呼び出すだけ
        return this->Copy(src);
    }

    // ムーブ
    MallocData<Type> &operator=(MallocData &&other) noexcept
    {
        // 内部でMove()を呼び出すだけ
        return this->Move(other);
    }

    // デストラクタ
    ~MallocData()
    {
        // 管理しているメモリを安全に破棄する
        this->MemFree();
    }

    // 配列の要素を指定個数、引数で初期化する(初期化個数は必ず0から詰め詰めで処理します)
    void Initialize(unsigned long initCount, const MallocData &src)
    {
        // 有効な要素数が初期化数より小さい場合は処理を行う
        if (this->mnSetParamCount < initCount)
        {
            // メモリの大きさが初期化数より小さい場合は処理を行う
            if (this->mnMemSize < initCount)
            {
                // 新しい配列の大きさを計算し、記憶しておく
                this->mnMemSize = initCount * 2;

                // 新しい配列の大きさに沿ってデータを生成する
                this->MemCreate();
            }
        }

        // 有効な要素数を記憶しておく
        this->mnSetParamCount = initCount;

        // 初期化を行う個数だけ処理を行う
        this->SetAllParam(src);
    }

    // 配列の要素を指定個数、引数で初期化する
    void Initialize(unsigned long initCount, MallocData &&src)
    {
        // 有効な要素数が初期化数より小さい場合は処理を行う
        if (this->mnSetParamCount < initCount)
        {
            // メモリの大きさが初期化数より小さい場合は処理を行う
            if (this->mnMemSize < initCount)
            {
                // 新しい配列の大きさを計算し、記憶しておく
                this->mnMemSize = initCount * 2;

                // 新しい配列の大きさに沿ってデータを生成する
                this->MemCreate();
            }
        }

        // 有効な要素数を記憶しておく
        this->mnSetParamCount = initCount;

        // 初期化を行う個数だけ処理を行う
        this->SetAllParam(src);
    }

    // 大きさを設定しなおす(内部のデータは消去されません)
    void Resize(unsigned long size)
    {
        this->MemAdd(size);
    }

    // 配列末尾に要素を追加
    void AddParam(const Type &data)
    {
        // 入っている要素数が、既にサイズ以上になっている場合は処理を行う
        if (this->mnMemSize <= this->mnSetParamCount)
        {
            // 自身のサイズ分だけメモリを追加する
            this->MemAdd(this->mnMemSize);
        }

        // データを追加する
        this->mpData[this->mnSetParamCount] = data;

        // データを追加したことを記憶しておく
        this->mnSetParamCount++;
    }

    // 明示的にコピーを行う
    MallocData<Type> &Copy(const MallocData &src)
    {
        // 自身がメモリを管理している場合は処理を行う
        if (this->mpData != nullptr)
        {
            // 現在管理しているメモリの大きさが、コピー元の大きさより小さい場合は処理を行う
            if (src.mnMemSize < this->mnMemSize)
            {
                // 管理しているメモリを安全に破棄する
                this->MemFree();

                // 自身が管理する実メモリの大きさを更新する
                this->mnMemSize = src.mnMemSize;

                // 自身が管理するメモリを生成する
                this->MemCreate();
            }
        }

        // 有効な要素数をコピーする
        this->mnSetParamCount = src.mnSetParamCount;

        // 有効な要素の数だけコピーする
        for (unsigned long i = 0; i < this->mnSetParamCount; i++)
        {
            // コピー部分
            this->mpData[i] = src.mpData[i];
        }

        // 結果を返す
        return *this;
    }

    // 明示的にムーブを行う
    MallocData<Type> &Move(MallocData &other)
    {
        // 既にメモリを持っている場合は処理を行う
        if (this->mpData != nullptr)
        {
            // 管理しているメモリを安全に破棄する
            this->MemFree();
        }

        // コピー
        this->mpData = other.mpData;
        this->mnSetParamCount = other.mnSetParamCount;
        this->mnMemSize = other.mnMemSize;

        // otherからは参照させないようにする
        other.mpData = nullptr;
        other.mnSetParamCount = 0;
        other.mnMemSize = 0;

        // 結果を返す
        return *this;
    }

    // 明示的にムーブを行う
    MallocData<Type> &Move(MallocData &&other)
    {
        // 既にメモリを持っている場合は処理を行う
        if (this->mpData != nullptr)
        {
            // 管理しているメモリを安全に破棄する
            this->MemFree();
        }

        // コピー
        this->mpData = other.mpData;
        this->mnSetParamCount = other.mnSetParamCount;
        this->mnMemSize = other.mnMemSize;

        // otherからは参照させないようにする
        other.mpData = nullptr;
        other.mnSetParamCount = 0;
        other.mnMemSize = 0;

        // 結果を返す
        return *this;
    }

    // メモリを破棄する(内部的にはメモリはfreeしてません)
    void Clear()
    {
        this->mnSetParamCount = 0;
    }

    // "内部メモリを増やさず要素を追加できるのか"を確認する
    bool CheckAdd() const
    {
        return this->mnSetParamCount < this->mnMemSize;
    }

    // []オペレータ
    Type &operator[](unsigned long index)
    {
        return this->mpData[index];
    }

    // []オペレータ
    const Type &operator[](unsigned long index) const
    {
        return this->mpData[index];
    }

    // 要素の個数を取得する
    unsigned long Count() const
    {
        return this->mnSetParamCount;
    }

    // 実メモリの大きさを取得する
    unsigned long Size() const
    {
        return this->mnMemSize;
    }

    // 実メモリのアドレスを取得する
    Type *Memory() const
    {
        return this->mpData;
    }

    // メモリを管理していないかを確認する
    bool CheckMemEmpty() const
    {
        return (this->mpData == nullptr || this->mnMemSize == 0);
    }
};