#define _CRT_SECURE_NO_WARNINGS

#include "BinFileOperaterDirect.h"

// コンストラクタ
BinFileOperaterDirect::BinFileOperaterDirect()
{
}

// デストラクタ
BinFileOperaterDirect::~BinFileOperaterDirect()
{
    // 
    if (this->mpFile != nullptr)
    {
        // 
        this->Close();
    }
}

// ファイルを新規作成する関数
// ※既にファイルがある場合は失敗を返します。wbでファイルを生成した場合はOpenのflagで対応してください
bool BinFileOperaterDirect::Create(const char *filePath)
{
    // ファイルパスが無効な場合 ||
    // 既にファイルを開いている場合 は閉じる
    if (filePath == nullptr ||
        this->mpFile != nullptr)
    {
        return false;
    }

    // 読み込み専用のモードでそのファイルを開いてみる
    this->mpFile = fopen(filePath, "rb");

    // 開くのに成功した場合は処理を行う
    if (this->mpFile != nullptr)
    {
        // ファイルを閉じる
        this->Close();

        // 新規生成に失敗したことを返す
        return false;
    }

    // "wb"：バイナリ形式で新規作成
    this->mpFile = fopen(filePath, "wb");

    // 新規作成に失敗した場合は処理を行う
    if (this->mpFile == nullptr)
    {
        // 新規生成に失敗したことを返す
        return false;
    }

    // 一度ファイルを閉じる
    this->Close();

    // 成功したことを返す
    return true;
}

// ファイルを削除する関数
// ※このオペレータがファイルを開いている場合は失敗を返します
bool BinFileOperaterDirect::Delete(const char *filePath)
{
    // ファイルパスが無効な場合は処理を行う
    if (filePath == nullptr)
    {
        // 失敗したことを返す
        return false;
    }

    // 開いているファイルと同じファイルを削除しようとする場合は処理を行う
    if (mpFile != nullptr)
    {
        // 持っているファイルを消す可能性があるため失敗を返す
        return false;

    }

    // std::remove() は成功すると 0 を返す
    if (remove(filePath) != 0)
    {
        // 失敗したことを返す
        return false;
    }

    // 成功したことを返す
    return true;
}

// ファイルを開く関数
// ※overWrite == trueで"wb+"で開くことも可能ですが、おすすめはしません
bool BinFileOperaterDirect::Open(const char *filePath, bool overWrite)
{
    // ファイルパスが無効、または既にファイルを開いている場合は失敗
    if (filePath == nullptr ||
        this->mpFile != nullptr)
    {
        return false;
    }

    if (overWrite)
    {
        // 既存ファイルを保持したまま読み書き可能で開く
        this->mpFile = fopen(filePath, "rb+");
    }
    else
    {
        // 新規作成して読み書き可能で開く
        this->mpFile = fopen(filePath, "wb+");
    }

    if (this->mpFile == nullptr)
    {
        return false;
    }

    return true;
}

// ファイルを閉じる関数
// ※このオペレータが開いているファイルを持っていない場合は失敗を返します
bool BinFileOperaterDirect::Close()
{
    // 
    if (this->mpFile == nullptr)
    {
        // 
        return false;
    }

    // 
    int temp = fclose(this->mpFile);

    // 
    if (temp != 0)
    {
        // 
        return false;
    }

    // 
    this->mpFile = nullptr;

    // 
    return true;
}

// ファイルから現在のシーク位置から指定されたByteだけ読み込みを行う関数
bool BinFileOperaterDirect::Read(void *dst, uint64_t size)
{
    // 
    if (dst == nullptr)
    {
        // 
        return false;
    }

    // 
    size_t readSize = fread(dst, 1, size, this->mpFile);

    // 
    if (readSize != size)
    {
        // 
        return false;
    }

    // 
    return true;
}

// ファイルから現在のシーク位置から指定されたByteだけ書き込みを行う関数
bool BinFileOperaterDirect::Write(const void *src, uint64_t size)
{
    // 
    if (src == nullptr)
    {
        // 
        return false;
    }

    // 
    size_t writeSize = fwrite(src, 1, size, this->mpFile);

    // 
    if (writeSize != size)
    {
        // 
        return false;
    }

    // 
    return true;
}

// ファイルの現在のシーク位置を動かす関数
bool BinFileOperaterDirect::Seek(BinFileSeekPos seekPos, int64_t offset)
{
    // 
    int temp = -1;

    // 
    switch (seekPos)
    {
    case BinFileSeekPos::BinFileSeekPos_Start:
        // 
        temp = fseek(this->mpFile, offset, SEEK_SET);
        break;
    case BinFileSeekPos::BinFileSeekPos_Current:
        // 
        temp = fseek(this->mpFile, offset, SEEK_CUR);
        break;
    case BinFileSeekPos::BinFileSeekPos_End:
        // 
        temp = fseek(this->mpFile, offset, SEEK_END);
        break;
    default:
        break;
    }

    // 
    if (temp != 0)
    {
        // 
        return false;
    }

    // 
    return true;
}

// このオペレータがファイルを既に開いているかを確認する関数
bool BinFileOperaterDirect::CheckOpened() const
{
    // 
    return this->mpFile != nullptr;
}

// このバイナリファイルの大きさを取得する関数
bool BinFileOperaterDirect::GetFileSize(uint64_t *out) const
{
    if (out == nullptr ||
        this->mpFile == nullptr)
    {
        return false;
    }

    // 現在位置を取得
    const long currentPosition = ftell(this->mpFile);
    if (currentPosition < 0)
    {
        return false;
    }

    // ファイル末尾へ移動
    if (fseek(this->mpFile, 0, SEEK_END) != 0)
    {
        return false;
    }

    // ファイルサイズを取得
    const long fileSize = ftell(this->mpFile);

    // 元の位置へ戻す
    const int restoreResult = fseek(this->mpFile, currentPosition, SEEK_SET);

    if (fileSize < 0 ||
        restoreResult != 0)
    {
        return false;
    }

    // 
    *out = static_cast<uint32_t>(fileSize);

    // 
    return true;
}

// コンストラクタ
BinFileDataLayout::BinFileDataLayout() :
    mlLayoutBox(),
    mnDeactiveStartIndex(0)
{
}

// デストラクタ
BinFileDataLayout::~BinFileDataLayout()
{
}

// 新しい要素をレイアウトの末尾に追加する関数
bool BinFileDataLayout::AddElement(const BinFileDataElement& layoutElement)
{
    // 
    if (this->mnDeactiveStartIndex < this->mlLayoutBox.size())
    {
        // 自身のレイアウトを設定する
        this->mlLayoutBox[this->mnDeactiveStartIndex] = layoutElement;
    }
    else if (this->mnDeactiveStartIndex == this->mlLayoutBox.size())
    {
        // 自身のレイアウトを設定する
        this->mlLayoutBox.push_back(layoutElement);
    }
    else
    {
        // 
        return false;
    }

    // 
    mnDeactiveStartIndex++;

    // 成功したことを返す
    return true;
}

// 目的の要素をレイアウトから削除する関数
bool BinFileDataLayout::DeleteElement(uint32_t target)
{
    // メモリを移動させる
    std::memmove(
        &this->mlLayoutBox[target],
        &this->mlLayoutBox[target + 1],
        this->mnDeactiveStartIndex - target);

    // 
    mnDeactiveStartIndex--;

    // 削除に成功したことを返す
    return true;
}

// レイアウトの各要素の情報を取得する関数
const BinFileDataElement* BinFileDataLayout::GetLayoutBox() const
{
    // 配列の先頭ポインタを返す
    return this->mlLayoutBox.data();
}

// レイアウトの要素数を取得する関数
uint32_t BinFileDataLayout::GetLayoutCount() const
{
    // size_tをuint32_tに変換して、値を返す
    return static_cast<uint32_t>(this->mlLayoutBox.size());
}

// このレイアウトのバイト数を取得する関数
// 大きさが動的な場合は0が返ってきます
uint32_t BinFileDataLayout::GetLayoutSize() const
{
    // このレイアウトのバイト数
    uint32_t sizeSum = 0;

    // 作業用変数
    uint32_t temp = 0;

    // 全ての要素を走査する
    for (auto element : this->mlLayoutBox)
    {
        // この要素の大きさが動的な場合は処理を行う
        if (element.mnSubData == 0)
        {
            // 合計Byte数を、動的を意味する0に戻す
            sizeSum = 0;

            // ループを終える
            break;
        }

        // Type自体の大きさを取得する
        temp = BinFileDataLayout::GetByteSizeFromType(element.mnDataType);

        // Typeの大きさ自体が動的の場合は処理を行う
        if (temp == 0)
        {
            // SumDataの値をそのまま合計Byte数に加算する
            sizeSum += element.mnSubData;
        }
        else
        {
            // 型の大きさ * 要素数を合計Byte数に加算する
            sizeSum += temp * element.mnSubData;
        }
    }

    // 計算で出されたバイト数を返す
    return sizeSum;
}

// 大きさが動的(要するにSubData == 0)の要素が何個あるかを返す関数
uint32_t BinFileDataLayout::GetDynamicSubDataCount() const
{
    uint32_t count = 0;
    for (auto element : this->mlLayoutBox)
    {
        // SubData == 0、つまり動的配列 or 動的サイズになる場合は処理を行う
        if (element.mnSubData == 0)
        {
            // 動的な場合は動的なSubDataを動的なデータの前に書く必要があるので、そのカウントを行う
            count++;
        }
    }

    // 結果を返す
    return count;
}

// 
bool BinFileDataLayout::CheckElementDynamic(uint32_t elementIndex) const
{
    return this->mlLayoutBox[elementIndex].mnSubData == 0;
}

// 目的の要素が何個目の動的な要素かを取得する関数
bool BinFileDataLayout::ElementIndex2DynamicIndex(uint32_t elementIndex, uint32_t* out) const
{
    // 
    if (!this->CheckElementDynamic(elementIndex) || 
        out == nullptr)
    {
        return false;
    }

    // 
    uint32_t dynamicIndex = 0;

    // 
    for (uint32_t i = 0; i < elementIndex; i++)
    {
        // 
        if (this->mlLayoutBox[i].mnSubData == 0)
        {
            // 
            dynamicIndex++;
        }
    }

    // 
    *out = dynamicIndex;

    // 
    return true;
}

// 
BinFileDataLayout &BinFileDataLayout::operator =(const BinFileDataLayout &src)
{
    // 
    this->mlLayoutBox.resize(src.mlLayoutBox.size());

    // 
    for (uint16_t i = 0; i < this->mlLayoutBox.size(); i++)
    {
        // 
        this->mlLayoutBox[i] = src.mlLayoutBox[i];
    }

    // 
    return *this;
}

// BinFileDataTypeからそのtype1つ辺りのByte数を取得する関数
uint32_t BinFileDataLayout::GetByteSizeFromType(BinFileDataType type)
{
    // 初期値は動的を意味する0にしておく
    uint32_t size = 0;

    // typeによって処理を変える
    switch (type)
    {
    case BinFileDataType::BinFileDataType_Signed8:
        size = sizeof(int8_t);
        break;
    case BinFileDataType::BinFileDataType_Signed16:
        size = sizeof(int16_t);
        break;
    case BinFileDataType::BinFileDataType_Signed32:
        size = sizeof(int32_t);
        break;
    case BinFileDataType::BinFileDataType_Signed64:
        size = sizeof(int64_t);
        break;
    case BinFileDataType::BinFileDataType_Unsigned8:
        size = sizeof(uint8_t);
        break;
    case BinFileDataType::BinFileDataType_Unsigned16:
        size = sizeof(uint16_t);
        break;
    case BinFileDataType::BinFileDataType_Unsigned32:
        size = sizeof(uint32_t);
        break;
    case BinFileDataType::BinFileDataType_Unsigned64:
        size = sizeof(uint64_t);
        break;
    case BinFileDataType::BinFileDataType_Float16:
        // これに関してはあんまり使ってほしくないね
        size = sizeof(float) / 2;
        break;
    case BinFileDataType::BinFileDataType_Float32:
        size = sizeof(float);
        break;
    case BinFileDataType::BinFileDataType_Float64:
        size = sizeof(double);
        break;
    case BinFileDataType::BinFileDataType_StringUTF8:
    case BinFileDataType::BinFileDataType_Memory:
        // 明示的に、動的を表す1にする
        size = 1;
        break;
    default:
        break;
    }

    // 結果を返す
    return size;
}

// コンストラクタ
BinFileDataTransformer::BinFileDataTransformer()
{
}

// デストラクタ
BinFileDataTransformer::~BinFileDataTransformer()
{
}

// メモリ上のデータをファイル上のデータに変換する関数
bool BinFileDataTransformer::GetFileData(const BinFileDataLayout* layout, const void** elementBox, const uint32_t* dynamicSubDataBox, uint8_t* fileData)
{
    // nullptrチェック
    if (layout == nullptr ||
        elementBox == nullptr ||
        fileData == nullptr)
    {
        // 失敗したことを返す
        return false;
    }

    // レイアウトの本体
    auto layoutBox = layout->GetLayoutBox();

    // レイアウトが何個の要素で構成されているか
    uint32_t layoutCount = layout->GetLayoutCount();

    // 動的なSubDataを見つけた回数
    uint32_t dynamicSubDataCount = 0;

    // データタイプの要素数
    uint32_t nowSubData = 0;

    // データタイプのByte数
    uint32_t typeSize = 0;

    // ファイルデータをどこまで書き終えたのか
    uint32_t fileDataIndex = 0;

    // 要素を一つずつ見ていく
    for (uint32_t layoutIndex = 0; layoutIndex < layoutCount; layoutIndex++)
    {
        // 要素を一つずつ見ていく
        if (elementBox[layoutIndex] != nullptr)
        {
            // SubDataが動的な場合は処理を行う
            if (layout->CheckElementDynamic(layoutIndex))
            {
                // 動的なサブデータを取得する
                nowSubData = dynamicSubDataBox[dynamicSubDataCount];

                // 動的なSubDataをファイルデータに書き込む
                memcpy(fileData + fileDataIndex, &nowSubData, sizeof(uint32_t));

                // ファイルデータのシークを動かす
                fileDataIndex += sizeof(uint32_t);

                // 動的なSubDataを見つけた回数を増やす
                dynamicSubDataCount++;
            }
            else
            {
                // 
                nowSubData = layoutBox[layoutIndex].mnSubData;
            }

            // この要素の1データ当たりのByte数を取得する
            typeSize = BinFileDataLayout::GetByteSizeFromType(layoutBox[layoutIndex].mnDataType);

            // ファイルデータに書き込む
            memcpy(fileData + fileDataIndex, elementBox[layoutIndex], typeSize * nowSubData);

            // ファイルデータのシークを動かす
            fileDataIndex += typeSize * nowSubData;
        }
        else
        {
            // 
            return false;
        }
    }

    // 成功したことを返す
    return true;
}

// レイアウトと動的なSubDataを見てFileDataにした際の大きさを取得する関数
uint32_t BinFileDataTransformer::GetFileDataSize(const BinFileDataLayout* layout, const uint32_t* dynamicSubDataBox)
{
    // 
    uint32_t fileDataSize = layout->GetLayoutSize();

    // 
    if (fileDataSize != 0)
    {
        return fileDataSize;
    }

    // 作業用変数
    const BinFileDataElement* layoutBox = layout->GetLayoutBox();
    uint32_t layoutCount = layout->GetLayoutCount();
    uint32_t dynamicCount = 0;
    uint32_t nowSubData = 0;

    // 全てのレイアウトの要素を走査する
    for (uint32_t layoutIndex = 0; layoutIndex < layoutCount; layoutIndex++)
    {
        // この要素が動的かを調べる
        if (layout->CheckElementDynamic(layoutIndex))
        {
            // 動的なサブデータを取得する
            nowSubData = dynamicSubDataBox[dynamicCount];

            // 動的なものがあった回数を加算する
            dynamicCount++;
        }
        else
        {
            // 静的なサブデータを入れる
            nowSubData = layoutBox[layoutIndex].mnSubData;
        }

        // そもそもの型の大きさ * サブデータをファイルサイズに加算する
        fileDataSize += BinFileDataLayout::GetByteSizeFromType(layoutBox[layoutIndex].mnDataType) * nowSubData;
    }

    // 結果を返す
    return fileDataSize;
}

// ファイル上のデータをメモリ上のデータに変換する関数
bool BinFileDataTransformer::GetElementData(const BinFileDataLayout* layout, const uint8_t* fileData, const uint32_t* dynamicSubDataBox, void** elementBox)
{
    // nullptrチェック
    if (layout == nullptr ||
        elementBox == nullptr ||
        fileData == nullptr)
    {
        // 失敗したことを返す
        return false;
    }

    // 動的なレイアウトにもかかわらず、dynamicSubDataBoxがnullptrの場合は失敗だよね
    if (layout->GetLayoutSize() == 0 &&
        dynamicSubDataBox == nullptr)
    {
        // 失敗したことを返す
        return false;
    }

    // レイアウトの本体
    auto layoutBox = layout->GetLayoutBox();

    // レイアウトが何個の要素で構成されているか
    uint32_t layoutCount = layout->GetLayoutCount();

    // 動的なSubDataを見つけた回数
    uint32_t dynamicSubDataCount = 0;

    // データタイプの要素数
    uint32_t nowSubData = 0;

    // データタイプのByte数
    uint32_t typeSize = 0;

    // ファイルデータをどこまで書き終えたのか
    uint32_t fileDataIndex = 0;

    // 要素を一つずつ見ていく
    for (uint32_t layoutIndex = 0; layoutIndex < layoutCount; layoutIndex++)
    {
        // 要素を一つずつ見ていく
        if (elementBox[layoutIndex] != nullptr)
        {
            // SubDataが動的な場合は処理を行う
            if (layout->CheckElementDynamic(layoutIndex))
            {
                // 動的なサブデータを取得する
                nowSubData = dynamicSubDataBox[dynamicSubDataCount];

                // ファイルデータのシークを動かす
                fileDataIndex += sizeof(uint32_t);

                // 動的なSubDataを見つけた回数を増やす
                dynamicSubDataCount++;
            }
            else
            {
                // 
                nowSubData = layoutBox[layoutIndex].mnSubData;
            }

            // この要素の1データ当たりのByte数を取得する
            typeSize = BinFileDataLayout::GetByteSizeFromType(layoutBox[layoutIndex].mnDataType);

            // ファイルデータに書き込む
            memcpy(elementBox[layoutIndex], fileData + fileDataIndex, typeSize * nowSubData);

            // ファイルデータのシークを動かす
            fileDataIndex += typeSize * nowSubData;
        }
        else
        {
            // 失敗したことを返す
            return false;
        }
    }

    // 成功したことを返す
    return true;
}

// ファイル上のデータをメモリ上のデータに変換する関数
bool BinFileDataTransformer::GetDynamicSubData(const BinFileDataLayout* layout, const uint8_t* fileData, uint32_t* dynamicSubDataBox)
{
    // nullptrチェック
    if (layout == nullptr ||
        fileData == nullptr ||
        dynamicSubDataBox == nullptr)
    {
        // 失敗したことを返す
        return false;
    }

    // レイアウトの本体
    auto layoutBox = layout->GetLayoutBox();

    // レイアウトが何個の要素で構成されているか
    uint32_t layoutCount = layout->GetLayoutCount();

    // 動的なSubDataを見つけた回数
    uint32_t dynamicSubDataCount = 0;

    // データタイプの要素数
    uint32_t nowSubData = 0;

    // データタイプのByte数
    uint32_t typeSize = 0;

    // ファイルデータをどこまで書き終えたのか
    uint32_t fileDataIndex = 0;

    // 要素を一つずつ見ていく
    for (uint32_t layoutIndex = 0; layoutIndex < layoutCount; layoutIndex++)
    {
        // SubDataが動的な場合は処理を行う
        if (layout->CheckElementDynamic(layoutIndex))
        {
            // 動的なサブデータを取得する
            nowSubData = *reinterpret_cast<const uint32_t*>(fileData + fileDataIndex);

            // 動的なSubDataをファイルデータに書き込む
            memcpy(&dynamicSubDataBox[dynamicSubDataCount], &nowSubData, sizeof(uint32_t));

            // ファイルデータのシークを動かす
            fileDataIndex += sizeof(uint32_t);

            // 動的なSubDataを見つけた回数を増やす
            dynamicSubDataCount++;
        }
        else
        {
            // 
            nowSubData = layoutBox[layoutIndex].mnSubData;
        }

        // この要素の1データ当たりのByte数を取得する
        typeSize = BinFileDataLayout::GetByteSizeFromType(layoutBox[layoutIndex].mnDataType);

        // ファイルデータのシークを動かす
        fileDataIndex += typeSize * nowSubData;
    }

    // 成功したことを返す
    return true;
}

// コンストラクタ
BinFileDataSupporter::BinFileDataSupporter(const BinFileDataLayout* layout) :
    mpLayout(layout),
    mpElementBox(nullptr),
    mpDynamicSubDataBox(nullptr),
    mpDynamicElementBox(nullptr)
{
    // 失敗してもしーらね
    this->Initialize();
}

// デストラクタ
BinFileDataSupporter::~BinFileDataSupporter()
{
    if (this->mpLayout != nullptr)
    {
        if (this->mpElementBox != nullptr)
        {
            // 
            auto layoutBox = this->mpLayout->GetLayoutBox();

            // 
            auto layoutCount = this->mpLayout->GetLayoutCount();

            // 
            for (uint32_t i = 0; i < layoutCount; i++)
            {
                // 
                switch (layoutBox[i].mnDataType)
                {
                case BinFileDataType::BinFileDataType_Signed8:
                    this->DeleteSigned8(i);
                    break;
                case BinFileDataType::BinFileDataType_Signed16:
                    this->DeleteSigned16(i);
                    break;
                case BinFileDataType::BinFileDataType_Signed32:
                    this->DeleteSigned32(i);
                    break;
                case BinFileDataType::BinFileDataType_Signed64:
                    this->DeleteSigned64(i);
                    break;
                case BinFileDataType::BinFileDataType_Unsigned8:
                    this->DeleteSigned8(i);
                    break;
                case BinFileDataType::BinFileDataType_Unsigned16:
                    this->DeleteSigned16(i);
                    break;
                case BinFileDataType::BinFileDataType_Unsigned32:
                    this->DeleteSigned32(i);
                    break;
                case BinFileDataType::BinFileDataType_Unsigned64:
                    this->DeleteSigned64(i);
                    break;
                case BinFileDataType::BinFileDataType_Float16:
                    this->DeleteFloat16(i);
                    break;
                case BinFileDataType::BinFileDataType_Float32:
                    this->DeleteFloat32(i);
                    break;
                case BinFileDataType::BinFileDataType_Float64:
                    this->DeleteFloat64(i);
                    break;
                case BinFileDataType::BinFileDataType_StringUTF8:
                    this->DeleteCrahacterArray(i);
                    break;
                case BinFileDataType::BinFileDataType_Memory:
                    this->DeleteMemory(i);
                    break;
                default:
                    break;
                }
            }

            // 
            if (this->mpDynamicElementBox != nullptr)
            {
                delete[] this->mpDynamicElementBox;
            }

            // 
            if (this->mpDynamicSubDataBox != nullptr)
            {
                delete[] this->mpDynamicSubDataBox;
            }

            // 
            delete[] this->mpElementBox;
        }
    }
}

// 
const BinFileDataLayout& BinFileDataSupporter::GetLayout() const
{
    return *this->mpLayout;
}

// 
void** BinFileDataSupporter::GetElementBox()
{
    // 外向けの配列を返す
    return this->mpElementBox;
}

// 
void* const * BinFileDataSupporter::GetElementBox() const
{
    // 外向けの配列を返す
    return this->mpElementBox;
}

// 
int8_t* BinFileDataSupporter::GetElementPtrSigned8(uint32_t elementIndex)
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Signed8)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<int8_t*>(this->mpElementBox[elementIndex]);
}

// 
int16_t* BinFileDataSupporter::GetElementPtrSigned16(uint32_t elementIndex)
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Signed16)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<int16_t*>(this->mpElementBox[elementIndex]);
}

// 
int32_t* BinFileDataSupporter::GetElementPtrSigned32(uint32_t elementIndex)
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Signed32)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<int32_t*>(this->mpElementBox[elementIndex]);
}

// 
int64_t* BinFileDataSupporter::GetElementPtrSigned64(uint32_t elementIndex)
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Signed64)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<int64_t*>(this->mpElementBox[elementIndex]);
}

// 
uint8_t* BinFileDataSupporter::GetElementPtrUnsigned8(uint32_t elementIndex)
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Unsigned8)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<uint8_t*>(this->mpElementBox[elementIndex]);
}

// 
uint16_t* BinFileDataSupporter::GetElementPtrUnsigned16(uint32_t elementIndex)
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Unsigned16)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<uint16_t*>(this->mpElementBox[elementIndex]);
}

// 
uint32_t* BinFileDataSupporter::GetElementPtrUnsigned32(uint32_t elementIndex)
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Unsigned32)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<uint32_t*>(this->mpElementBox[elementIndex]);
}

// 
uint64_t* BinFileDataSupporter::GetElementPtrUnsigned64(uint32_t elementIndex)
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Unsigned64)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<uint64_t*>(this->mpElementBox[elementIndex]);
}

// 
int16_t *BinFileDataSupporter::GetElementPtrFloat16(uint32_t elementIndex)
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Float16)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<int16_t*>(this->mpElementBox[elementIndex]);
}

// 
float *BinFileDataSupporter::GetElementPtrFloat32(uint32_t elementIndex)
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Float32)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<float*>(this->mpElementBox[elementIndex]);
}

// 
double *BinFileDataSupporter::GetElementPtrFloat64(uint32_t elementIndex)
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Float64)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<double*>(this->mpElementBox[elementIndex]);
}

// 
char *BinFileDataSupporter::GetElementPtrCharacterArray(uint32_t elementIndex)
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_StringUTF8)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<char*>(this->mpElementBox[elementIndex]);
}

// 
unsigned char *BinFileDataSupporter::GetElementPtrMemory(uint32_t elementIndex)
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Memory)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<unsigned char*>(this->mpElementBox[elementIndex]);
}

// 
const int8_t* BinFileDataSupporter::GetElementPtrSigned8(uint32_t elementIndex) const
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Signed8)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<const int8_t*>(this->mpElementBox[elementIndex]);
}

// 
const int16_t* BinFileDataSupporter::GetElementPtrSigned16(uint32_t elementIndex) const
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Signed16)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<const int16_t*>(this->mpElementBox[elementIndex]);
}

// 
const int32_t* BinFileDataSupporter::GetElementPtrSigned32(uint32_t elementIndex) const
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Signed32)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<const int32_t*>(this->mpElementBox[elementIndex]);
}

// 
const int64_t* BinFileDataSupporter::GetElementPtrSigned64(uint32_t elementIndex) const
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Signed64)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<const int64_t*>(this->mpElementBox[elementIndex]);
}

// 
const uint8_t* BinFileDataSupporter::GetElementPtrUnsigned8(uint32_t elementIndex) const
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Unsigned8)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<const uint8_t*>(this->mpElementBox[elementIndex]);
}

// 
const uint16_t* BinFileDataSupporter::GetElementPtrUnsigned16(uint32_t elementIndex) const
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Unsigned16)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<const uint16_t*>(this->mpElementBox[elementIndex]);
}

// 
const uint32_t* BinFileDataSupporter::GetElementPtrUnsigned32(uint32_t elementIndex) const
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Unsigned32)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<const uint32_t*>(this->mpElementBox[elementIndex]);
}

// 
const uint64_t* BinFileDataSupporter::GetElementPtrUnsigned64(uint32_t elementIndex) const
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Unsigned64)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<const uint64_t*>(this->mpElementBox[elementIndex]);
}

// 
const int16_t* BinFileDataSupporter::GetElementPtrFloat16(uint32_t elementIndex) const
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Float16)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<const int16_t*>(this->mpElementBox[elementIndex]);
}

// 
const float* BinFileDataSupporter::GetElementPtrFloat32(uint32_t elementIndex) const
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Float32)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<const float*>(this->mpElementBox[elementIndex]);
}

// 
const double* BinFileDataSupporter::GetElementPtrFloat64(uint32_t elementIndex) const
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Float64)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<const double*>(this->mpElementBox[elementIndex]);
}

// 
const char* BinFileDataSupporter::GetElementPtrCharacterArray(uint32_t elementIndex) const
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_StringUTF8)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<const char*>(this->mpElementBox[elementIndex]);
}

// 
const unsigned char* BinFileDataSupporter::GetElementPtrMemory(uint32_t elementIndex) const
{
    // 
    if (this->mpLayout == nullptr ||
        this->mpLayout->GetLayoutBox()[elementIndex].mnDataType != BinFileDataType::BinFileDataType_Memory)
    {
        // 
        return nullptr;
    }

    // 
    return reinterpret_cast<const unsigned char*>(this->mpElementBox[elementIndex]);
}

// 
uint32_t BinFileDataSupporter::GetNowSubData(uint32_t elementIndex) const
{
    // 
    if (this->mpLayout == nullptr)
    {
        // 
        return 0;
    }

    // 
    uint32_t nowSubData = 0;

    // 
    if (this->mpLayout->CheckElementDynamic(elementIndex))
    {
        // 
        uint32_t dynamicIndex = 0;
        if (this->mpLayout->ElementIndex2DynamicIndex(elementIndex, &dynamicIndex))
        {
            // 
            return 0;
        }

        // 
        nowSubData = this->mpDynamicSubDataBox[dynamicIndex];
    }
    else
    {
        // 
        nowSubData = this->mpLayout->GetLayoutBox()[elementIndex].mnSubData;
    }

    // 
    return nowSubData;
}

// 
bool BinFileDataSupporter::SetNowSubData(uint32_t elementIndex, uint32_t newSubData)
{
    // 
    uint32_t dynamicIndex = 0;

    // 
    if (!this->mpLayout->ElementIndex2DynamicIndex(elementIndex, &dynamicIndex))
    {
        // 
        return false;
    }

    // 
    this->mpDynamicSubDataBox[dynamicIndex] = newSubData;
    
    // 
    bool temp = false;

    // 
    switch (this->mpLayout->GetLayoutBox()[elementIndex].mnDataType)
    {
    case BinFileDataType::BinFileDataType_Signed8:
        temp = this->ChangeSubDataSigned8(elementIndex, dynamicIndex);
        break;
    case BinFileDataType::BinFileDataType_Signed16:
        temp = this->ChangeSubDataSigned16(elementIndex, dynamicIndex);
        break;
    case BinFileDataType::BinFileDataType_Signed32:
        temp = this->ChangeSubDataSigned32(elementIndex, dynamicIndex);
        break;
    case BinFileDataType::BinFileDataType_Signed64:
        temp = this->ChangeSubDataSigned64(elementIndex, dynamicIndex);
        break;
    case BinFileDataType::BinFileDataType_Unsigned8:
        temp = this->ChangeSubDataUnsigned8(elementIndex, dynamicIndex);
        break;
    case BinFileDataType::BinFileDataType_Unsigned16:
        temp = this->ChangeSubDataUnsigned16(elementIndex, dynamicIndex);
        break;
    case BinFileDataType::BinFileDataType_Unsigned32:
        temp = this->ChangeSubDataUnsigned32(elementIndex, dynamicIndex);
        break;
    case BinFileDataType::BinFileDataType_Unsigned64:
        temp = this->ChangeSubDataUnsigned64(elementIndex, dynamicIndex);
        break;
    case BinFileDataType::BinFileDataType_Float16:
        temp = this->ChangeSubDataFloat16(elementIndex, dynamicIndex);
        break;
    case BinFileDataType::BinFileDataType_Float32:
        temp = this->ChangeSubDataFloat32(elementIndex, dynamicIndex);
        break;
    case BinFileDataType::BinFileDataType_Float64:
        temp = this->ChangeSubDataFloat64(elementIndex, dynamicIndex);
        break;
    case BinFileDataType::BinFileDataType_StringUTF8:
        temp = this->ChangeSubDataCrahacterArray(elementIndex, dynamicIndex);
        break;
    case BinFileDataType::BinFileDataType_Memory:
        temp = this->ChangeSubDataMemory(elementIndex, dynamicIndex);
        break;
    default:
        break;
    }

    // 
    return temp;
}

// そのデータタイプがどれだけ処理がしづらいかを返す関数
// 値が低いほど処理がデータタイプということです
uint32_t BinFileDataSupporter::GetDataTypeCost(BinFileDataType type)
{
    // 
    uint32_t temp = false;

    // 
    switch (type)
    {
    case BinFileDataType::BinFileDataType_Signed8:
        temp = 1;
        break;
    case BinFileDataType::BinFileDataType_Signed16:
        temp = 2;
        break;
    case BinFileDataType::BinFileDataType_Signed32:
        temp = 4;
        break;
    case BinFileDataType::BinFileDataType_Signed64:
        temp = 8;
        break;
    case BinFileDataType::BinFileDataType_Unsigned8:
        temp = 1;
        break;
    case BinFileDataType::BinFileDataType_Unsigned16:
        temp = 2;
        break;
    case BinFileDataType::BinFileDataType_Unsigned32:
        temp = 4;
        break;
    case BinFileDataType::BinFileDataType_Unsigned64:
        temp = 8;
        break;
    case BinFileDataType::BinFileDataType_Float16:
        temp = 2 + 1;
        break;
    case BinFileDataType::BinFileDataType_Float32:
        temp = 4 + 1;
        break;
    case BinFileDataType::BinFileDataType_Float64:
        temp = 8 + 1;
        break;
    case BinFileDataType::BinFileDataType_StringUTF8:
        temp = 10;
        break;
    case BinFileDataType::BinFileDataType_Memory:
        temp = 3;
        break;
    default:
        temp = 0xffffffff;
        break;
    }

    // 
    return temp;
}

// 初期化を行う
bool BinFileDataSupporter::Initialize()
{
    // 
    if (this->mpLayout == nullptr)
    {
        // 
        return false;
    }

    // 
    auto layoutBox = this->mpLayout->GetLayoutBox();

    // 
    uint32_t layoutCount = this->mpLayout->GetLayoutCount();

    // 
    uint32_t dynamicSubDataCount = this->mpLayout->GetDynamicSubDataCount();

    // 
    this->mpElementBox = new void* [layoutCount];

    // 
    if (dynamicSubDataCount == 0)
    {
        // std::vectorの大きさをもって置けるやつ
        this->mpDynamicSubDataBox = new uint32_t[dynamicSubDataCount];

        // std::vectorも許容できるようにしています
        this->mpDynamicElementBox = new void* [dynamicSubDataCount];
    }

    // 
    uint32_t dynamicSubDataIndex = 0;

    // 
    for (uint32_t i = 0; i < layoutCount; i++)
    {
        // 
        switch (layoutBox[i].mnDataType)
        {
        case BinFileDataType::BinFileDataType_Signed8:
            if (layoutBox[i].mnSubData == 0)
            {
                this->mpDynamicElementBox[dynamicSubDataIndex] = new std::vector<int8_t>;
                this->SetNowSubData(i, 0);
            }
            else
            {
                this->mpElementBox[i] = new int8_t[layoutBox[i].mnSubData];
            }
            break;
        case BinFileDataType::BinFileDataType_Signed16:
            if (layoutBox[i].mnSubData == 0)
            {
                this->mpDynamicElementBox[i] = new std::vector<int16_t>;
                this->SetNowSubData(i, 0);
            }
            else
            {
                this->mpElementBox[i] = new int16_t[layoutBox[i].mnSubData];
            }
            break;
        case BinFileDataType::BinFileDataType_Signed32:
            if (layoutBox[i].mnSubData == 0)
            {
                this->mpDynamicElementBox[i] = new std::vector<int32_t>;
                this->SetNowSubData(i, 0);
            }
            else
            {
                this->mpElementBox[i] = new int32_t[layoutBox[i].mnSubData];
            }
            break;
        case BinFileDataType::BinFileDataType_Signed64:
            if (layoutBox[i].mnSubData == 0)
            {
                this->mpDynamicElementBox[i] = new std::vector<int64_t>;
                this->SetNowSubData(i, 0);
            }
            else
            {
                this->mpElementBox[i] = new int64_t[layoutBox[i].mnSubData];
            }
            break;
        case BinFileDataType::BinFileDataType_Unsigned8:
            if (layoutBox[i].mnSubData == 0)
            {
                this->mpDynamicElementBox[i] = new std::vector<uint8_t>;
                this->SetNowSubData(i, 0);
            }
            else
            {
                this->mpElementBox[i] = new uint8_t[layoutBox[i].mnSubData];
            }
            break;
        case BinFileDataType::BinFileDataType_Unsigned16:
            if (layoutBox[i].mnSubData == 0)
            {
                this->mpDynamicElementBox[i] = new std::vector<uint16_t>;
                this->SetNowSubData(i, 0);
            }
            else
            {
                this->mpElementBox[i] = new uint16_t[layoutBox[i].mnSubData];
            }
            break;
        case BinFileDataType::BinFileDataType_Unsigned32:
            if (layoutBox[i].mnSubData == 0)
            {
                this->mpDynamicElementBox[i] = new std::vector<uint32_t>;
                this->SetNowSubData(i, 0);
            }
            else
            {
                this->mpElementBox[i] = new uint32_t[layoutBox[i].mnSubData];
            }
            break;
        case BinFileDataType::BinFileDataType_Unsigned64:
            if (layoutBox[i].mnSubData == 0)
            {
                this->mpDynamicElementBox[i] = new std::vector<uint64_t>;
                this->SetNowSubData(i, 0);
            }
            else
            {
                this->mpElementBox[i] = new uint64_t[layoutBox[i].mnSubData];
            }
            break;
        case BinFileDataType::BinFileDataType_Float16:
            if (layoutBox[i].mnSubData == 0)
            {
                this->mpDynamicElementBox[i] = new std::vector<int16_t>;
                this->SetNowSubData(i, 0);
            }
            else
            {
                this->mpElementBox[i] = new int16_t[layoutBox[i].mnSubData];
            }
            break;
        case BinFileDataType::BinFileDataType_Float32:
            if (layoutBox[i].mnSubData == 0)
            {
                this->mpDynamicElementBox[i] = new std::vector<float>;
                this->SetNowSubData(i, 0);
            }
            else
            {
                this->mpElementBox[i] = new float[layoutBox[i].mnSubData];
            }
            break;
        case BinFileDataType::BinFileDataType_Float64:
            if (layoutBox[i].mnSubData == 0)
            {
                this->mpDynamicElementBox[i] = new std::vector<double>;
                this->SetNowSubData(i, 0);
            }
            else
            {
                this->mpElementBox[i] = new double[layoutBox[i].mnSubData];
            }
            break;
        case BinFileDataType::BinFileDataType_StringUTF8:
            if (layoutBox[i].mnSubData == 0)
            {
                this->mpDynamicElementBox[i] = new std::vector<char>;
                this->SetNowSubData(i, 0);
            }
            else
            {
                this->mpElementBox[i] = new char[layoutBox[i].mnSubData];
            }
            break;
        case BinFileDataType::BinFileDataType_Memory:
            if (layoutBox[i].mnSubData == 0)
            {
                this->mpDynamicElementBox[i] = new std::vector<unsigned char>;
                this->SetNowSubData(i, 0);
            }
            else
            {
                this->mpElementBox[i] = new unsigned char[layoutBox[i].mnSubData];
            }
            break;
        default:
            return false;
            break;
        }
    }

}

// 
bool BinFileDataSupporter::ChangeSubDataSigned8(uint32_t elementIndex, uint32_t dynamicIndex)
{
    // 
    std::vector<int8_t>* target = (std::vector<int8_t> *)(this->mpDynamicElementBox[dynamicIndex]);

    // 
    if (target->size() < this->mpDynamicSubDataBox[dynamicIndex])
    {
        // 要素数を拡張する
        target->resize(this->mpDynamicSubDataBox[dynamicIndex]);
    }

    // 改めて配列へのポインタを設定する
    this->mpElementBox[elementIndex] = target->data();

    // 成功したことを返す
    return true;
}

// 
bool BinFileDataSupporter::ChangeSubDataSigned16(uint32_t elementIndex, uint32_t dynamicIndex)
{
    // 
    std::vector<int16_t>* target = (std::vector<int16_t> *)(this->mpDynamicElementBox[dynamicIndex]);

    // 
    if (target->size() < this->mpDynamicSubDataBox[dynamicIndex])
    {
        // 要素数を拡張する
        target->resize(this->mpDynamicSubDataBox[dynamicIndex]);
    }

    // 改めて配列へのポインタを設定する
    this->mpElementBox[elementIndex] = target->data();

    // 成功したことを返す
    return true;
}

// 
bool BinFileDataSupporter::ChangeSubDataSigned32(uint32_t elementIndex, uint32_t dynamicIndex)
{
    // 
    std::vector<int32_t>* target = (std::vector<int32_t> *)(this->mpDynamicElementBox[dynamicIndex]);

    // 
    if (target->size() < this->mpDynamicSubDataBox[dynamicIndex])
    {
        // 要素数を拡張する
        target->resize(this->mpDynamicSubDataBox[dynamicIndex]);
    }

    // 改めて配列へのポインタを設定する
    this->mpElementBox[elementIndex] = target->data();

    // 成功したことを返す
    return true;
}

// 
bool BinFileDataSupporter::ChangeSubDataSigned64(uint32_t elementIndex, uint32_t dynamicIndex)
{
    // 
    std::vector<int64_t>* target = (std::vector<int64_t> *)(this->mpDynamicElementBox[dynamicIndex]);

    // 
    if (target->size() < this->mpDynamicSubDataBox[dynamicIndex])
    {
        // 要素数を拡張する
        target->resize(this->mpDynamicSubDataBox[dynamicIndex]);
    }

    // 改めて配列へのポインタを設定する
    this->mpElementBox[elementIndex] = target->data();

    // 成功したことを返す
    return true;
}

// 
bool BinFileDataSupporter::ChangeSubDataUnsigned8(uint32_t elementIndex, uint32_t dynamicIndex)
{
    // 
    std::vector<uint8_t>* target = (std::vector<uint8_t> *)(this->mpDynamicElementBox[dynamicIndex]);

    // 
    if (target->size() < this->mpDynamicSubDataBox[dynamicIndex])
    {
        // 要素数を拡張する
        target->resize(this->mpDynamicSubDataBox[dynamicIndex]);
    }

    // 改めて配列へのポインタを設定する
    this->mpElementBox[elementIndex] = target->data();

    // 成功したことを返す
    return true;
}

// 
bool BinFileDataSupporter::ChangeSubDataUnsigned16(uint32_t elementIndex, uint32_t dynamicIndex)
{
    // 
    std::vector<uint16_t>* target = (std::vector<uint16_t> *)(this->mpDynamicElementBox[dynamicIndex]);

    // 
    if (target->size() < this->mpDynamicSubDataBox[dynamicIndex])
    {
        // 要素数を拡張する
        target->resize(this->mpDynamicSubDataBox[dynamicIndex]);
    }

    // 改めて配列へのポインタを設定する
    this->mpElementBox[elementIndex] = target->data();

    // 成功したことを返す
    return true;
}

// 
bool BinFileDataSupporter::ChangeSubDataUnsigned32(uint32_t elementIndex, uint32_t dynamicIndex)
{
    // 
    std::vector<uint32_t>* target = (std::vector<uint32_t> *)(this->mpDynamicElementBox[dynamicIndex]);

    // 
    if (target->size() < this->mpDynamicSubDataBox[dynamicIndex])
    {
        // 要素数を拡張する
        target->resize(this->mpDynamicSubDataBox[dynamicIndex]);
    }

    // 改めて配列へのポインタを設定する
    this->mpElementBox[elementIndex] = target->data();

    // 成功したことを返す
    return true;
}

// 
bool BinFileDataSupporter::ChangeSubDataUnsigned64(uint32_t elementIndex, uint32_t dynamicIndex)
{
    // 
    std::vector<uint64_t>* target = (std::vector<uint64_t> *)(this->mpDynamicElementBox[dynamicIndex]);

    // 
    if (target->size() < this->mpDynamicSubDataBox[dynamicIndex])
    {
        // 要素数を拡張する
        target->resize(this->mpDynamicSubDataBox[dynamicIndex]);
    }

    // 改めて配列へのポインタを設定する
    this->mpElementBox[elementIndex] = target->data();

    // 成功したことを返す
    return true;
}

// 
bool BinFileDataSupporter::ChangeSubDataFloat16(uint32_t elementIndex, uint32_t dynamicIndex)
{
    // 
    std::vector<int16_t>* target = (std::vector<int16_t> *)(this->mpDynamicElementBox[dynamicIndex]);

    // 
    if (target->size() < this->mpDynamicSubDataBox[dynamicIndex])
    {
        // 要素数を拡張する
        target->resize(this->mpDynamicSubDataBox[dynamicIndex]);
    }

    // 改めて配列へのポインタを設定する
    this->mpElementBox[elementIndex] = target->data();

    // 成功したことを返す
    return true;
}

// 
bool BinFileDataSupporter::ChangeSubDataFloat32(uint32_t elementIndex, uint32_t dynamicIndex)
{
    // 
    std::vector<float>* target = (std::vector<float> *)(this->mpDynamicElementBox[dynamicIndex]);

    // 
    if (target->size() < this->mpDynamicSubDataBox[dynamicIndex])
    {
        // 要素数を拡張する
        target->resize(this->mpDynamicSubDataBox[dynamicIndex]);
    }

    // 改めて配列へのポインタを設定する
    this->mpElementBox[elementIndex] = target->data();

    // 成功したことを返す
    return true;
}

// 
bool BinFileDataSupporter::ChangeSubDataFloat64(uint32_t elementIndex, uint32_t dynamicIndex)
{
    // 
    std::vector<double>* target = (std::vector<double> *)(this->mpDynamicElementBox[dynamicIndex]);

    // 
    if (target->size() < this->mpDynamicSubDataBox[dynamicIndex])
    {
        // 要素数を拡張する
        target->resize(this->mpDynamicSubDataBox[dynamicIndex]);
    }

    // 改めて配列へのポインタを設定する
    this->mpElementBox[elementIndex] = target->data();

    // 成功したことを返す
    return true;
}

// 
bool BinFileDataSupporter::ChangeSubDataCrahacterArray(uint32_t elementIndex, uint32_t dynamicIndex)
{
    // 
    std::vector<char>* target = (std::vector<char> *)(this->mpDynamicElementBox[dynamicIndex]);

    // 
    if (target->size() < this->mpDynamicSubDataBox[dynamicIndex])
    {
        // 要素数を拡張する
        target->resize(this->mpDynamicSubDataBox[dynamicIndex]);
    }

    // 改めて配列へのポインタを設定する
    this->mpElementBox[elementIndex] = target->data();

    // 成功したことを返す
    return true;
}

// 
bool BinFileDataSupporter::ChangeSubDataMemory(uint32_t elementIndex, uint32_t dynamicIndex)
{
    // 
    std::vector<unsigned char>* target = (std::vector<unsigned char> *)(this->mpDynamicElementBox[dynamicIndex]);

    // 
    if (target->size() < this->mpDynamicSubDataBox[dynamicIndex])
    {
        // 要素数を拡張する
        target->resize(this->mpDynamicSubDataBox[dynamicIndex]);
    }

    // 改めて配列へのポインタを設定する
    this->mpElementBox[elementIndex] = target->data();

    // 成功したことを返す
    return true;
}

// 
void BinFileDataSupporter::DeleteSigned8(uint32_t elementIndex)
{
    // 
    if (this->mpLayout->CheckElementDynamic(elementIndex))
    {
        // 削除
        delete[] reinterpret_cast<int8_t*>(this->mpElementBox[elementIndex]);
    }
    else
    {
        // 
        auto target = (std::vector<int8_t> *)(this->mpElementBox[elementIndex]);

        // 
        delete target;
    }
}

// 
void BinFileDataSupporter::DeleteSigned16(uint32_t elementIndex)
{
    // 
    if (this->mpLayout->CheckElementDynamic(elementIndex))
    {
        // 削除
        delete[] reinterpret_cast<int16_t*>(this->mpElementBox[elementIndex]);
    }
    else
    {
        // 
        auto target = (std::vector<int16_t> *)(this->mpElementBox[elementIndex]);

        // 
        delete target;
    }
}

// 
void BinFileDataSupporter::DeleteSigned32(uint32_t elementIndex)
{
    // 
    if (this->mpLayout->CheckElementDynamic(elementIndex))
    {
        // 削除
        delete[] reinterpret_cast<int32_t*>(this->mpElementBox[elementIndex]);
    }
    else
    {
        // 
        auto target = (std::vector<int32_t> *)(this->mpElementBox[elementIndex]);

        // 
        delete target;
    }
}

// 
void BinFileDataSupporter::DeleteSigned64(uint32_t elementIndex)
{
    // 
    if (this->mpLayout->CheckElementDynamic(elementIndex))
    {
        // 削除
        delete[] reinterpret_cast<int64_t*>(this->mpElementBox[elementIndex]);
    }
    else
    {
        // 
        auto target = (std::vector<int64_t> *)(this->mpElementBox[elementIndex]);

        // 
        delete target;
    }
}

// 
void BinFileDataSupporter::DeleteUnsigned8(uint32_t elementIndex)
{
    // 
    if (this->mpLayout->CheckElementDynamic(elementIndex))
    {
        // 削除
        delete[] reinterpret_cast<uint8_t*>(this->mpElementBox[elementIndex]);
    }
    else
    {
        // 
        auto target = (std::vector<uint8_t> *)(this->mpElementBox[elementIndex]);

        // 
        delete target;
    }
}

// 
void BinFileDataSupporter::DeleteUnsigned16(uint32_t elementIndex)
{
    // 
    if (this->mpLayout->CheckElementDynamic(elementIndex))
    {
        // 削除
        delete[] reinterpret_cast<uint16_t*>(this->mpElementBox[elementIndex]);
    }
    else
    {
        // 
        auto target = (std::vector<uint16_t> *)(this->mpElementBox[elementIndex]);

        // 
        delete target;
    }
}

// 
void BinFileDataSupporter::DeleteUnsigned32(uint32_t elementIndex)
{
    // 
    if (this->mpLayout->CheckElementDynamic(elementIndex))
    {
        // 削除
        delete[] reinterpret_cast<uint32_t*>(this->mpElementBox[elementIndex]);
    }
    else
    {
        // 
        auto target = (std::vector<uint32_t> *)(this->mpElementBox[elementIndex]);

        // 
        delete target;
    }
}

// 
void BinFileDataSupporter::DeleteUnsigned64(uint32_t elementIndex)
{
    // 
    if (this->mpLayout->CheckElementDynamic(elementIndex))
    {
        // 削除
        delete[] reinterpret_cast<uint64_t*>(this->mpElementBox[elementIndex]);
    }
    else
    {
        // 
        auto target = (std::vector<uint64_t> *)(this->mpElementBox[elementIndex]);

        // 
        delete target;
    }
}

// 
void BinFileDataSupporter::DeleteFloat16(uint32_t elementIndex)
{
    // 
    if (this->mpLayout->CheckElementDynamic(elementIndex))
    {
        // 削除
        delete[] reinterpret_cast<int16_t*>(this->mpElementBox[elementIndex]);
    }
    else
    {
        // 
        auto target = (std::vector<int16_t> *)(this->mpElementBox[elementIndex]);

        // 
        delete target;
    }
}

// 
void BinFileDataSupporter::DeleteFloat32(uint32_t elementIndex)
{
    // 
    if (this->mpLayout->CheckElementDynamic(elementIndex))
    {
        // 削除
        delete[] reinterpret_cast<float*>(this->mpElementBox[elementIndex]);
    }
    else
    {
        // 
        auto target = (std::vector<float> *)(this->mpElementBox[elementIndex]);

        // 
        delete target;
    }
}

// 
void BinFileDataSupporter::DeleteFloat64(uint32_t elementIndex)
{
    // 
    if (this->mpLayout->CheckElementDynamic(elementIndex))
    {
        // 削除
        delete[] reinterpret_cast<double*>(this->mpElementBox[elementIndex]);
    }
    else
    {
        // 
        auto target = (std::vector<double> *)(this->mpElementBox[elementIndex]);

        // 
        delete target;
    }
}

// 
void BinFileDataSupporter::DeleteCrahacterArray(uint32_t elementIndex)
{
    // 
    if (this->mpLayout->CheckElementDynamic(elementIndex))
    {
        // 削除
        delete[] reinterpret_cast<char*>(this->mpElementBox[elementIndex]);
    }
    else
    {
        // 
        auto target = (std::vector<char> *)(this->mpElementBox[elementIndex]);

        // 
        delete target;
    }
}

// 
void BinFileDataSupporter::DeleteMemory(uint32_t elementIndex)
{
    // 
    if (this->mpLayout->CheckElementDynamic(elementIndex))
    {
        // 削除
        delete[] reinterpret_cast<unsigned char*>(this->mpElementBox[elementIndex]);
    }
    else
    {
        // 
        auto target = (std::vector<unsigned char> *)(this->mpElementBox[elementIndex]);

        // 
        delete target;
    }
}
