#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <stdio.h>

// BinFileOperater::Seekに渡すenum
enum BinFileSeekPos
{
    BinFileSeekPos_Start = 0,
    BinFileSeekPos_Current,
    BinFileSeekPos_End,
};

// BinFileを操作するクラス(PageとかCashとかなく、直にBinFileのIOを行う)
// ※ファイルを開くにはOpen()が必要ですが、閉じる際はデストラクタ内で自動で行います。明示的に閉じることも可能です。
class BinFileOperaterDirect
{
public:
    // コンストラクタ
    BinFileOperaterDirect();

    // デストラクタ
    ~BinFileOperaterDirect();

    // ファイルを新規作成する関数
    // ※既にファイルがある場合は失敗を返します。wbでファイルを生成した場合はOpenのflagで対応してください
    bool Create(const char *filePath);

    // ファイルを削除する関数
    // ※このオペレータがファイルを開いている場合は失敗を返します
    bool Delete(const char *filePath);

    // ファイルを開く関数
    // ※overWrite == trueで"wb+"で開くことも可能ですが、おすすめはしません
    bool Open(const char *filePath, bool overWrite = false);

    // ファイルを閉じる関数
    // ※このオペレータが開いているファイルを持っていない場合は失敗を返します
    bool Close();

    // ファイルから現在のシーク位置から指定されたByteだけ読み込みを行う関数
    bool Read(void *dst, uint64_t size);

    // ファイルから現在のシーク位置から指定されたByteだけ書き込みを行う関数
    bool Write(const void *src, uint64_t size);

    // ファイルの現在のシーク位置を動かす関数
    bool Seek(BinFileSeekPos seekPos, int64_t offset);

    // このオペレータがファイルを既に開いているかを確認する関数
    bool CheckOpened() const;

    // このバイナリファイルの大きさを取得する関数
    bool GetFileSize(uint64_t *out) const;

private:
    // C言語ライクのファイルポインタ
    FILE *mpFile;
};


// バイナリファイル上でメモリをどのような形式で扱えばいいかを指定する列挙体
enum BinFileDataType
{
    BinFileDataType_Signed8 = 0,                // Signed Integer : 8Bit
    BinFileDataType_Signed16,                   // Signed Integer : 16Bit
    BinFileDataType_Signed32,                   // Signed Integer : 32Bit
    BinFileDataType_Signed64,                   // Signed Integer : 64Bit
    BinFileDataType_Unsigned8,                  // Unsigned Integer : 8Bit
    BinFileDataType_Unsigned16,                 // Unsigned Integer : 16Bit
    BinFileDataType_Unsigned32,                 // Unsigned Integer : 32Bit
    BinFileDataType_Unsigned64,                 // Unsigned Integer : 64Bit
    BinFileDataType_Float16,                    // Floating Number : 32Bit
    BinFileDataType_Float32,                    // Floating Number : 32Bit
    BinFileDataType_Float64,                    // Floating Number : 64Bit
    BinFileDataType_StringUTF8,                 // String : Byte Size Unknown
    BinFileDataType_Memory,                     // Memory : Byte Size Unknown
};

// バイナリファイルで操作するデータのレイアウトの1要素
struct BinFileDataElement
{
    // データをどの形式で使用すればいいかを記憶しておく変数
    BinFileDataType mnDataType;

    // Variables Whose Meaning Changes Depending on ElementType
    // Signed, Unsinged, Float : mnSubData == (0 ? Dynamic Array : Static Array Count)
    // CharacterArray, Memory : mnSubData == (0 ? Dynamic Size : Static Size By Byte)
    uint32_t mnSubData;
};

// データベースに登録するデータのレイアウトを表現するクラス
class BinFileDataLayout
{
public:
    // コンストラクタ
    BinFileDataLayout();

    // デストラクタ
    ~BinFileDataLayout();

    // 新しい要素をレイアウトの末尾に追加する関数
    bool AddElement(const BinFileDataElement& layoutElement);

    // 目的の要素をレイアウトから削除する関数
    bool DeleteElement(uint32_t target);

    // レイアウトの各要素の情報を取得する関数
    const BinFileDataElement* GetLayoutBox() const;

    // レイアウトの要素数を取得する関数
    uint32_t GetLayoutCount() const;

    // このレイアウトのバイト数を取得する関数
    // 大きさが動的な場合は0が返ってきます
    uint32_t GetLayoutSize() const;

    // 大きさが動的(要するにSubData == 0)の要素が何個あるかを返す関数
    uint32_t GetDynamicSubDataCount() const;

    // この要素のSubDataが動的を意味する0かを確認する関数
    bool CheckElementDynamic(uint32_t elementIndex) const;

    // 目的の要素が何個目の動的な要素かを取得する関数
    bool ElementIndex2DynamicIndex(uint32_t elementIndex, uint32_t *out) const;

    // 代入演算子(コピー)
    BinFileDataLayout &operator =(const BinFileDataLayout &src);

    // BinFileDataTypeからそのtype1つ辺りのByte数を取得する関数
    static uint32_t GetByteSizeFromType(BinFileDataType type);

private:
    // レイアウト情報の配列
    std::vector<BinFileDataElement> mlLayoutBox;

    // 
    size_t mnDeactiveStartIndex;
};

// BinFileDataLayoutを活用して処理を行う関数をまとめたクラス
class BinFileDataTransformer
{
public:
    // コンストラクタ
    BinFileDataTransformer();

    // デストラクタ
    ~BinFileDataTransformer();

    // メモリ上のデータをファイル上のデータに変換する関数
    bool GetFileData(const BinFileDataLayout* layout, const void** elementBox, const uint32_t* dynamicSubDataBox, uint8_t* fileData);

    // レイアウトと動的なSubDataを見てFileDataにした際の大きさを取得する関数
    uint32_t GetFileDataSize(const BinFileDataLayout* layout, const uint32_t* dynamicSubDataBox);

    // ファイル上のデータをメモリ上のデータに変換する関数
    bool GetElementData(const BinFileDataLayout* layout, const uint8_t* fileData, const uint32_t* dynamicSubDataBox, void** elementBox);

    // ファイル上のデータをメモリ上のデータに変換する関数
    bool GetDynamicSubData(const BinFileDataLayout* layout, const uint8_t* fileData, uint32_t* dynamicSubDataBox);

protected:
};

// 
class BinFileDataSupporter
{
public:
    // コンストラクタ
    BinFileDataSupporter(const BinFileDataLayout* layout);

    // デストラクタ
    ~BinFileDataSupporter();

    // 
    const BinFileDataLayout& GetLayout() const;

    // 
    void** GetElementBox();

    // 
    void *const *GetElementBox() const;

    // 
    int8_t* GetElementPtrSigned8(uint32_t elementIndex);

    // 
    int16_t* GetElementPtrSigned16(uint32_t elementIndex);

    // 
    int32_t* GetElementPtrSigned32(uint32_t elementIndex);

    // 
    int64_t* GetElementPtrSigned64(uint32_t elementIndex);

    // 
    uint8_t* GetElementPtrUnsigned8(uint32_t elementIndex);

    // 
    uint16_t* GetElementPtrUnsigned16(uint32_t elementIndex);

    // 
    uint32_t* GetElementPtrUnsigned32(uint32_t elementIndex);

    // 
    uint64_t* GetElementPtrUnsigned64(uint32_t elementIndex);

    // 
    int16_t* GetElementPtrFloat16(uint32_t elementIndex);

    // 
    float *GetElementPtrFloat32(uint32_t elementIndex);

    // 
    double* GetElementPtrFloat64(uint32_t elementIndex);

    // 
    char *GetElementPtrCharacterArray(uint32_t elementIndex);

    // 
    unsigned char *GetElementPtrMemory(uint32_t elementIndex);

    // 
    const int8_t* GetElementPtrSigned8(uint32_t elementIndex) const;

    // 
    const int16_t* GetElementPtrSigned16(uint32_t elementIndex) const;

    // 
    const int32_t* GetElementPtrSigned32(uint32_t elementIndex) const;

    // 
    const int64_t* GetElementPtrSigned64(uint32_t elementIndex) const;

    // 
    const uint8_t* GetElementPtrUnsigned8(uint32_t elementIndex) const;

    // 
    const uint16_t* GetElementPtrUnsigned16(uint32_t elementIndex) const;

    // 
    const uint32_t* GetElementPtrUnsigned32(uint32_t elementIndex) const;

    // 
    const uint64_t* GetElementPtrUnsigned64(uint32_t elementIndex) const;

    // 
    const int16_t* GetElementPtrFloat16(uint32_t elementIndex) const;

    // 
    const float* GetElementPtrFloat32(uint32_t elementIndex) const;

    // 
    const double* GetElementPtrFloat64(uint32_t elementIndex) const;

    // 
    const char* GetElementPtrCharacterArray(uint32_t elementIndex) const;

    // 
    const unsigned char* GetElementPtrMemory(uint32_t elementIndex) const;

    // 
    uint32_t GetNowSubData(uint32_t elementIndex) const;

    // 
    bool SetNowSubData(uint32_t elementIndex, uint32_t newSubData);

    // そのデータタイプがどれだけ処理がしづらいかを返す関数
    // 値が低いほど処理がデータタイプということです
    static uint32_t GetDataTypeCost(BinFileDataType type);

private:
    // 
    const BinFileDataLayout *mpLayout;

    // 
    void **mpElementBox;

    // 
    uint32_t* mpDynamicSubDataBox;

    // 
    void **mpDynamicElementBox;

    // 初期化を行う
    bool Initialize();

    // 
    bool ChangeSubDataSigned8(uint32_t elementIndex, uint32_t dynamicIndex);

    // 
    bool ChangeSubDataSigned16(uint32_t elementIndex, uint32_t dynamicIndex);

    // 
    bool ChangeSubDataSigned32(uint32_t elementIndex, uint32_t dynamicIndex);

    // 
    bool ChangeSubDataSigned64(uint32_t elementIndex, uint32_t dynamicIndex);

    // 
    bool ChangeSubDataUnsigned8(uint32_t elementIndex, uint32_t dynamicIndex);

    // 
    bool ChangeSubDataUnsigned16(uint32_t elementIndex, uint32_t dynamicIndex);

    // 
    bool ChangeSubDataUnsigned32(uint32_t elementIndex, uint32_t dynamicIndex);

    // 
    bool ChangeSubDataUnsigned64(uint32_t elementIndex, uint32_t dynamicIndex);

    // 
    bool ChangeSubDataFloat16(uint32_t elementIndex, uint32_t dynamicIndex);

    // 
    bool ChangeSubDataFloat32(uint32_t elementIndex, uint32_t dynamicIndex);

    // 
    bool ChangeSubDataFloat64(uint32_t elementIndex, uint32_t dynamicIndex);

    // 
    bool ChangeSubDataCrahacterArray(uint32_t elementIndex, uint32_t dynamicIndex);

    // 
    bool ChangeSubDataMemory(uint32_t elementIndex, uint32_t dynamicIndex);

    // 
    void DeleteSigned8(uint32_t elementIndex);

    // 
    void DeleteSigned16(uint32_t elementIndex);

    // 
    void DeleteSigned32(uint32_t elementIndex);

    // 
    void DeleteSigned64(uint32_t elementIndex);

    // 
    void DeleteUnsigned8(uint32_t elementIndex);

    // 
    void DeleteUnsigned16(uint32_t elementIndex);

    // 
    void DeleteUnsigned32(uint32_t elementIndex);

    // 
    void DeleteUnsigned64(uint32_t elementIndex);

    // 
    void DeleteFloat16(uint32_t elementIndex);

    // 
    void DeleteFloat32(uint32_t elementIndex);

    // 
    void DeleteFloat64(uint32_t elementIndex);

    // 
    void DeleteCrahacterArray(uint32_t elementIndex);

    // 
    void DeleteMemory(uint32_t elementIndex);
};
