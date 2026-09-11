#include "BitFlag.h"
#define _WINSOCKAPI_
#include <stdlib.h>
#include <windows.h>

#define OneByteBitCount (8)
#define targetBit2byteIndex (targetBit)((targetBit) / OneByteBitCount)
#define targetBit2bitIndex (targetBit)((targetBit) % OneByteBitCount)

ByteParam::ByteParam(FlagByte *bitFlag, unsigned char useBit, bool param) :
    mpByte(bitFlag),
    mnUseBit(useBit),
    mbParam(param)
{
}

ByteParam::ByteParam(const FlagByte *bitFlag, unsigned char useBit, bool param) :
    mpByte(const_cast<FlagByte *>(bitFlag)),
    mnUseBit(useBit),
    mbParam(param)
{
}

ByteParam::~ByteParam()
{
    this->mpByte = nullptr;
    this->mnUseBit = 0;
    this->mbParam = false;
}

ByteParam::operator bool()const
{
    return this->mbParam;
}

ByteParam &ByteParam::operator=(bool flag)
{
    this->mpByte->SetFlag(this->mnUseBit, flag);
    this->mbParam = flag;
    return *this;
}

FlagByte::FlagByte() :
    mcOneByte(0x00)
{

}

FlagByte::FlagByte(unsigned char bitData) :
    mcOneByte(bitData)
{

}

FlagByte::FlagByte(const FlagByte &byte) :
    mcOneByte(byte.mcOneByte)
{

}

FlagByte::~FlagByte()
{
    this->mcOneByte = 0x00;
}

unsigned long FlagByte::CountOnFlag()
{
    unsigned long counter = 0;
    for (unsigned long i = 0; i < OneByteBitCount; i++)
    {
        if ((this->mcOneByte & (1 << i)) != 0)
        {
            counter++;
        }
    }
    return counter;
}

FlagByte &FlagByte::AllOn()
{
    this->mcOneByte = 0xff;
    return *this;
}

FlagByte &FlagByte::AllOff()
{
    this->mcOneByte = 0x00;
    return *this;
}

FlagByte &FlagByte::OnFlag(unsigned char useBit)
{
    if (useBit <= OneByteBitCount )
    {
        this->mcOneByte |= 1 << useBit;
    }
    return *this;
}

FlagByte &FlagByte::OffFlag(unsigned char useBit)
{
    if (useBit <= OneByteBitCount)
    {
        this->mcOneByte &= ~(1 << useBit);
    }
    return *this;
}

FlagByte &FlagByte::SetFlag(unsigned char useBit, bool flag)
{
    if (flag)
    {
        this->OnFlag(useBit);
    }
    else
    {
        this->OffFlag(useBit);
    }
    return *this;
}

ByteParam FlagByte::GetFlag(unsigned char useBit)
{
    return ByteParam(this, useBit, (this->mcOneByte & (1 << useBit)) != 0);
}

const ByteParam FlagByte::GetFlag(unsigned char useBit) const
{
    return ByteParam(this, useBit, (this->mcOneByte & (1 << useBit)) != 0);
}

FlagByte FlagByte::MoveL(unsigned char moveCount)
{
    return FlagByte(this->mcOneByte >> moveCount);
}

FlagByte FlagByte::MoveR(unsigned char moveCount)
{
    return FlagByte(this->mcOneByte >> moveCount);
}

// ビットを変更する
FlagByte &FlagByte::operator =(const FlagByte &flagByte)
{
    this->mcOneByte = flagByte.mcOneByte;
    return *this;
}

// ビットを足し合わせる
FlagByte &FlagByte::operator +=(const FlagByte &flagByte)
{
    this->mcOneByte |= flagByte.mcOneByte;
    return *this;
}

// ビットを一斉に消す
FlagByte &FlagByte::operator -=(const FlagByte &flagByte)
{
    this->mcOneByte &= ~(flagByte.mcOneByte);
    return *this;
}

// ビットを足し合わせる
FlagByte &FlagByte::operator |=(const FlagByte &flagByte)
{
    this->mcOneByte |= flagByte.mcOneByte;
    return *this;
}

// ビットの食い違いを探す
FlagByte &FlagByte::operator ^=(const FlagByte &flagByte)
{
    this->mcOneByte ^= flagByte.mcOneByte;
    return *this;
}

// ビットを厳選する
FlagByte &FlagByte::operator &=(const FlagByte &flagByte)
{
    this->mcOneByte &= flagByte.mcOneByte;
    return *this;
}

// ノット演算(ビット構成を自身の真逆にする)
FlagByte FlagByte::operator ~() const
{
    return FlagByte(~(this->mcOneByte));
}

// 指定のビットへのアクセスを許す
ByteParam FlagByte::operator[](unsigned char useBit)
{
    return this->GetFlag(useBit);
}

// 指定のビットへのアクセスを許す
ByteParam FlagByte::operator[](unsigned char useBit) const
{
    return this->GetFlag(useBit);
}

// ビットを足し合わせる
FlagByte operator +(const FlagByte &flagByteA, const FlagByte &flagByteB)
{
    return FlagByte(*(char *)(FlagByteBoxOperater::Add(&flagByteA, &flagByteB, 1).data()));
}

// ビットを一斉に消す
FlagByte operator -(const FlagByte &flagByteA, const FlagByte &flagByteB)
{
    return FlagByte(*(char *)(FlagByteBoxOperater::Sub(&flagByteA, &flagByteB, 1).data()));
}

// ビットを左へ移動させる
FlagByte operator <<(const FlagByte &flagByte, unsigned char bitCount)
{
    return FlagByte(*(char *)(FlagByteBoxOperater::MoveL(&flagByte, bitCount, 1).data()));
}

// ビットを右へ移動させる
FlagByte operator >>(const FlagByte &flagByte, unsigned char bitCount)
{
    return FlagByte(*(char *)(FlagByteBoxOperater::MoveR(&flagByte, bitCount, 1).data()));
}

// オア演算(どちらかでも1であれば1にする)
FlagByte operator |(const FlagByte &flagByteA, const FlagByte &flagByteB)
{
    return FlagByte(*(char *)(FlagByteBoxOperater::OR(&flagByteA, &flagByteB, 1).data()));
}

// Xオア演算(両者が違っていれば1にする)
FlagByte operator ^(const FlagByte &flagByteA, const FlagByte &flagByteB)
{
    return FlagByte(*(char *)(FlagByteBoxOperater::XOR(&flagByteA, &flagByteB, 1).data()));
}

// アンド演算(両者1の場合のみ1にする)
FlagByte operator &(const FlagByte &flagByteA, const FlagByte &flagByteB)
{
    return FlagByte(*(char *)(FlagByteBoxOperater::AND(&flagByteA, &flagByteB, 1).data()));
}

void FlagByteBoxOperater::BitCount2Index(unsigned long bitCount, unsigned long &index, unsigned long &bitIndex)
{
    index = bitCount / OneByteBitCount;
    bitIndex = bitCount % OneByteBitCount;
}

unsigned long FlagByteBoxOperater::CountOnFlag(FlagByte *bytes, unsigned long byteNum)
{
    unsigned long counter = 0;
    for (unsigned long i = 0; i < byteNum; i++)
    {
        counter += bytes[i].CountOnFlag();
    }
    return counter;
}

FlagByte *FlagByteBoxOperater::Assignment(FlagByte *destBytes, const FlagByte *srcBytes, unsigned long destByte)
{
    // デストのバイト分コピーをする
    for (unsigned long i = 0; i < destByte; i++)
    {
        destBytes[i] = srcBytes[i];
    }
    return destBytes;
}

FlagByte *FlagByteBoxOperater::Assignment(FlagByte *destBytes, const std::vector<FlagByte> &srcBytes, unsigned long destByte)
{
    // デストのバイト分コピーをする
    for (unsigned long i = 0; i < destByte; i++)
    {
        destBytes[i] = srcBytes[i];
    }
    return destBytes;
}

FlagByte *FlagByteBoxOperater::AllOn(FlagByte *bytes, unsigned long byteNum)
{
    for (unsigned long i = 0; i < byteNum; i++)
    {
        bytes[i].AllOn();
    }
    return bytes;
}

FlagByte *FlagByteBoxOperater::AllOff(FlagByte *bytes, unsigned long byteNum)
{
    for (unsigned long i = 0; i < byteNum; i++)
    {
        bytes[i].AllOff();
    }
    return bytes;
}

FlagByte *FlagByteBoxOperater::OnFlag(FlagByte *bytes, unsigned long byteNum, unsigned long targetBit)
{
    unsigned long byteIndex = 0;
    unsigned long bitIndex = 0;
    FlagByteBoxOperater::BitCount2Index(targetBit, byteIndex, bitIndex);
    bytes[byteIndex].OnFlag(bitIndex);
    return bytes;
}

FlagByte *FlagByteBoxOperater::OffFlag(FlagByte *bytes, unsigned long byteNum, unsigned long targetBit)
{
    unsigned long byteIndex = 0;
    unsigned long bitIndex = 0;
    FlagByteBoxOperater::BitCount2Index(targetBit, byteIndex, bitIndex);
    bytes[byteIndex].OffFlag(bitIndex);
    return bytes;
}

FlagByte *FlagByteBoxOperater::SetFlag(FlagByte *bytes, unsigned long byteNum, unsigned long targetBit, bool flag)
{
    unsigned long byteIndex = 0;
    unsigned long bitIndex = 0;
    FlagByteBoxOperater::BitCount2Index(targetBit, byteIndex, bitIndex);
    bytes[byteIndex].SetFlag(bitIndex, flag);
    return bytes;
}

ByteParam FlagByteBoxOperater::GetFlag(FlagByte *bytes, unsigned long byteNum, unsigned long targetBit)
{
    unsigned long byteIndex = 0;
    unsigned long bitIndex = 0;
    FlagByteBoxOperater::BitCount2Index(targetBit, byteIndex, bitIndex);
    return bytes[byteIndex].GetFlag(bitIndex);
}

std::vector<FlagByte> FlagByteBoxOperater::Add(const FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum)
{
    std::vector<FlagByte> temp(byteNum);
    if (byteNum == 0)
    {
        return temp;
    }
    char *lData = (char *)(byteL);
    char *rData = (char *)(byteR);
    for (unsigned long i = 0; i < byteNum; i++)
    {
        temp[i] = lData[i] | rData[i];
    }
    return temp;
}

std::vector<FlagByte> FlagByteBoxOperater::Sub(const FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum)
{
    std::vector<FlagByte> temp(byteNum);
    if (byteNum == 0)
    {
        return temp;
    }
    char *lData = (char *)(byteL);
    char *rData = (char *)(byteR);
    for (unsigned long i = 0; i < byteNum; i++)
    {
        temp[i] = lData[i] & ~(rData[i]);
    }
    return temp;
}

std::vector<FlagByte> FlagByteBoxOperater::AND(const FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum)
{
    std::vector<FlagByte> temp(byteNum);
    if (byteNum == 0)
    {
        return temp;
    }
    char *lData = (char *)(byteL);
    char *rData = (char *)(byteR);
    for (unsigned long i = 0; i < byteNum; i++)
    {
        temp[i] = lData[i] & rData[i];
    }
    return temp;
}

std::vector<FlagByte> FlagByteBoxOperater::OR(const FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum)
{
    std::vector<FlagByte> temp(byteNum);
    if (byteNum == 0)
    {
        return temp;
    }
    char *lData = (char *)(byteL);
    char *rData = (char *)(byteR);
    for (unsigned long i = 0; i < byteNum; i++)
    {
        temp[i] = lData[i] & rData[i];
    }
    return temp;
}

std::vector<FlagByte> FlagByteBoxOperater::XOR(const FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum)
{
    std::vector<FlagByte> temp(byteNum);
    if (byteNum == 0)
    {
        return temp;
    }
    char *lData = (char *)(byteL);
    char *rData = (char *)(byteR);
    for (unsigned long i = 0; i < byteNum; i++)
    {
        temp[i] = lData[i] & rData[i];
    }
    return temp;
}

std::vector<FlagByte> FlagByteBoxOperater::NOT(const FlagByte *byte, unsigned long byteNum)
{
    std::vector<FlagByte> temp(byteNum);
    if (byteNum == 0)
    {
        return temp;
    }
    char *lData = (char *)(byte);
    for (unsigned long i = 0; i < byteNum; i++)
    {
        temp[i] = ~lData[i];
    }
    return temp;
}

std::vector<FlagByte> FlagByteBoxOperater::MoveL(const FlagByte *byte, unsigned long moveCount, unsigned long byteNum)
{
    std::vector<FlagByte> temp(byteNum, 0x00);
    if (byteNum == 0)
    {
        return temp;
    }

    char *data = (char *)(byte);
    unsigned long dataSkip = 0;
    unsigned long bitSkip;

    FlagByteBoxOperater::BitCount2Index(moveCount, dataSkip, bitSkip);

    if (byteNum <= dataSkip)
    {
        return temp;
    }

    for (unsigned long i = 0; i < byteNum - moveCount; i++)
    {
        temp[i + dataSkip] = data[i];
    }

    // 
    if (bitSkip != 0)
    {
        unsigned short param = 0x0000;
        unsigned short overNow = 0x0000;
        unsigned short overOld = 0x0000;
        for (unsigned long i = dataSkip; i < byteNum; i++)
        {
            // 2バイトの領域に着目データを移す
            param = *(char *)(temp.data() + i);

            // 1バイト目は0, 1バイト目は1のデータと&演算をし、2バイト目に漏れ出た分のみ取得する
            overNow = 0xff00 & (param << bitSkip);

            // 1バイト目のデータを移す
            temp[i] = param << bitSkip;

            // 前回のバイトからあふれた分を今回のデータに入れる
            temp[i] |= overOld;

            // 今回溢れた分を記憶しておく
            overOld = overNow;
        }
    }

    // 結果を返す
    return temp;
}

std::vector<FlagByte> FlagByteBoxOperater::MoveR(const FlagByte *byte, unsigned long moveCount, unsigned long byteNum)
{
    std::vector<FlagByte> temp(byteNum, 0x00);
    if (byteNum == 0)
    {
        return temp;
    }

    char *data = (char *)(byte);
    unsigned long dataSkip = 0;
    unsigned long bitSkip;

    FlagByteBoxOperater::BitCount2Index(moveCount, dataSkip, bitSkip);

    if (byteNum <= dataSkip)
    {
        return temp;
    }

    for (unsigned long i = 0; i < byteNum - moveCount; i++)
    {
        temp[i + dataSkip] = data[i];
    }

    // 
    if (bitSkip != 0)
    {
        unsigned char param[2] = { 0x00, 0x00 };
        unsigned short overNow = 0x0000;
        unsigned short overOld = 0x0000;
        for (unsigned long i = byteNum - 1; dataSkip <= i; i++)
        {
            // 2バイトの領域に着目データを移す
            param[1] = *(char *)(temp.data() + i);

            // bitSkip分だけ右に動いたデータを再度paramに移す
            *(unsigned short *)(param) =(*(unsigned short *)(param) >> bitSkip);

            // 1バイト目は1, 2バイト目は0のデータと&演算をし、1バイト目に漏れ出た値を抽出する
            overNow = 0x00ff & param[0];

            // 2バイト目に残っているデータをtempに代入する
            temp[i] = param[1];

            // 前回のバイトからあふれた分を今回のデータに入れる
            temp[i] |= overOld;

            // 今回溢れた分を記憶しておく
            overOld = param[0];

            // 1バイト目を0に直しておく
            param[0] = 0x00;
        }
    }

    // 結果を返す
    return temp;
}

FlagByte *FlagByteBoxOperater::AddAss(FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum)
{
    auto result = FlagByteBoxOperater::Add(byteL, byteR, byteNum);
    for (unsigned long i = 0; i < byteNum; i++)
    {
        byteL[i] = result[i];
    }
    return byteL;
}

FlagByte *FlagByteBoxOperater::SubAss(FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum)
{
    auto result = FlagByteBoxOperater::Sub(byteL, byteR, byteNum);
    for (unsigned long i = 0; i < byteNum; i++)
    {
        byteL[i] = result[i];
    }
    return byteL;
}

FlagByte *FlagByteBoxOperater::ANDAss(FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum)
{
    auto result = FlagByteBoxOperater::AND(byteL, byteR, byteNum);
    for (unsigned long i = 0; i < byteNum; i++)
    {
        byteL[i] = result[i];
    }
    return byteL;
}

FlagByte *FlagByteBoxOperater::ORAss(FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum)
{
    auto result = FlagByteBoxOperater::OR(byteL, byteR, byteNum);
    for (unsigned long i = 0; i < byteNum; i++)
    {
        byteL[i] = result[i];
    }
    return byteL;
}

FlagByte *FlagByteBoxOperater::XORAss(FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum)
{
    auto result = FlagByteBoxOperater::XOR(byteL, byteR, byteNum);
    for (unsigned long i = 0; i < byteNum; i++)
    {
        byteL[i] = result[i];
    }
    return byteL;
}

FlagByte *FlagByteBoxOperater::NOTAss(FlagByte *byte, unsigned long byteNum)
{
    auto result = FlagByteBoxOperater::NOT(byte, byteNum);
    for (unsigned long i = 0; i < byteNum; i++)
    {
        byte[i] = result[i];
    }
    return byte;
}

FlagByte *FlagByteBoxOperater::MoveLAss(FlagByte *byte, unsigned long moveCount, unsigned long byteNum)
{
    auto result = FlagByteBoxOperater::MoveL(byte, moveCount, byteNum);
    for (unsigned long i = 0; i < byteNum; i++)
    {
        byte[i] = result[i];
    }
    return byte;
}

FlagByte *FlagByteBoxOperater::MoveRAss(FlagByte *byte, unsigned long moveCount, unsigned long byteNum)
{
    auto result = FlagByteBoxOperater::MoveR(byte, moveCount, byteNum);
    for (unsigned long i = 0; i < byteNum; i++)
    {
        byte[i] = result[i];
    }
    return byte;
}

bool FlagByteBoxOperater::Equal(const FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum)
{
    return !(FlagByteBoxOperater::NotEqual(byteL, byteR, byteNum));
}

bool FlagByteBoxOperater::NotEqual(const FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum)
{
    bool notEqual = false;
    char *lData = (char *)(byteL);
    char *rData = (char *)(byteR);

    for (unsigned long i = 0; i < byteNum; i++)
    {
        if (lData[i] != rData[i])
        {
            notEqual = true;
            break;
        }
    }
    return notEqual;
}

BitParam::BitParam(BitFlag *bitFlag, unsigned long useBit, bool param) :
    mpBitFlag(bitFlag),
    mnUseBit(useBit),
    mbParam(param)
{
}

BitParam::~BitParam()
{
    this->mpBitFlag = nullptr;
    this->mnUseBit = 0;
    this->mbParam = false;
}

BitParam::operator bool()const
{
    return this->mbParam;
}

BitParam &BitParam::operator=(bool flag)
{
    if (this->mpBitFlag != nullptr)
    {
        if (flag)
        {
            this->mpBitFlag->OnFlag(this->mnUseBit);
        }
        else
        {
            this->mpBitFlag->OffFlag(this->mnUseBit);
        }
    }
    this->mbParam = flag;
    return *this;
}

void BitFlag::Deactivate()
{
    // 初期化をする前に自身を無効化しておく
    if (this->mpMemory != nullptr)
    {
        free(this->mpMemory);
    }
    this->mnSize = 0;
    this->mbActive = false;
}

BitFlag::BitFlag() :
    mpMemory(nullptr),
    mnSize(0),
    mbActive(false)
{

}

BitFlag::BitFlag(unsigned long useBit) :
    mpMemory(nullptr),
    mnSize(0),
    mbActive(false)
{
    // 使用するバイト数
    unsigned long useByte = 0;

    // 1byte == 8bitなので
    if ((useBit % OneByteBitCount) == 0)
    {
        useByte = useBit / OneByteBitCount;
    }
    else
    {
        useByte = (useBit / OneByteBitCount) + 1;
    }

    // 使用するバイト数を自分で持っておく
    this->mnSize = useByte;

    // メモリの確保
    this->mpMemory = (char *)malloc(sizeof(char) * this->mnSize);
    if (this->mpMemory != nullptr)
    {
        // メモリ内部の初期化
        memset(this->mpMemory, 0, sizeof(char) * this->mnSize);

        // 有効であることを記憶させる
        this->mbActive = true;
    }
}

BitFlag::BitFlag(const BitFlag &bitFlag) :
    mpMemory(nullptr),
    mnSize(bitFlag.mnSize),
    mbActive(false)
{
    // メモリサイズの確認
    if (0 < this->mnSize)
    {
        // メモリの確保
        this->mpMemory = (char *)malloc(sizeof(char) * this->mnSize);
        if (this->mpMemory != nullptr &&
            bitFlag.mpMemory != nullptr)
        {
            // メモリ内部のコピー
            memcpy(this->mpMemory, bitFlag.mpMemory, sizeof(char) * this->mnSize);

            // 有効であることを記憶させる
            this->mbActive = true;
        }
    }
}

BitFlag::~BitFlag()
{
    this->Deactivate();
}

bool BitFlag::CheckActive() const
{
    return this->mbActive;
}

bool BitFlag::CheckZero() const
{
    bool temp = true;
    if (this->mbActive &&
        this->mpMemory != nullptr)
    {
        for (unsigned long i = 0; i < this->mnSize; i++)
        {
            if (this->mpMemory[i] != 0)
            {
                temp = false;
                break;
            }
        }
    }
    else
    {
        temp = false;
    }
    return temp;
}

bool BitFlag::CheckFlag(unsigned long bitCount) const
{
    if (this->mbActive == false)
    {
        return false;
    }

    bool temp = false;
    unsigned long index = bitCount / OneByteBitCount;
    unsigned long currentBit = bitCount % OneByteBitCount;

    temp = (0 != (this->mpMemory[index] & (1 << currentBit)));

    return temp;
}

int BitFlag::OnFlag(unsigned long bitCount)
{
    int temp = -1;
    if (this->mbActive)
    {
        unsigned long index = bitCount / OneByteBitCount;
        unsigned long currentBit = bitCount % OneByteBitCount;
        if (index < this->mnSize)
        {
            this->mpMemory[index] |= (1 << currentBit);
            temp = 0;
        }
    }
    if (temp == -1)
    {
        this->mbActive = false;
    }
    return temp;
}

int BitFlag::OffFlag(unsigned long bitCount)
{
    int temp = -1;
    if (this->mbActive)
    {
        unsigned long index = bitCount / OneByteBitCount;
        unsigned long currentBit = bitCount % OneByteBitCount;
        this->mpMemory[index] &= ~(1 << currentBit);
        temp = 0;
    }
    else
    {
        this->mbActive = false;
    }
    return temp;
}

unsigned long BitFlag::GetSize() const
{
    return this->mnSize;
}

void BitFlag::ResetSize(unsigned long byteSize)
{
    // 初期化をする前に自身を無効化しておく
    this->Deactivate();

    // 確保すべきメモリサイズを取得
    this->mnSize = byteSize;

    // メモリの確保
    this->mpMemory = (char *)malloc(sizeof(char) * this->mnSize);
    if (this->mpMemory != nullptr)
    {
        // メモリ内部の初期化
        memset(this->mpMemory, 0, sizeof(char) * this->mnSize);

        // 有効であることを記憶させる
        this->mbActive = true;
    }
}

void BitFlag::MoveL()
{
    if (this->mbActive == true &&
        this->mpMemory != nullptr)
    {
        for (unsigned long i = 0; i < this->mnSize; i++)
        {
            // まず<<をして、最後尾ビットを0にしておく
            this->mpMemory[i] = this->mpMemory[i] << 1;

            // i + 1をして不正アクセスが起こらなければ処理を続ける
            if (i + 1 < this->mnSize)
            {
                // 次のバイトの最前ビットが1だった場合はさらに処理を行う
                if (this->mpMemory[i + 1] & 1 << OneByteBitCount - 1)
                {
                    // 現在の最後尾ビットを1にする
                    this->mpMemory[i] |= 1 << 0;
                }
            }
        }
    }
    else
    {
        // 自身を無効化しておく
        this->Deactivate();
    }
}

void BitFlag::MoveR()
{
    if (this->mbActive == true &&
        this->mpMemory != nullptr)
    {
        for (unsigned long i = this->mnSize - 1; 0 < i; i--)
        {
            // まず>>をして、最前ビットを0にしておく
            this->mpMemory[i] = this->mpMemory[i] >> 1;

            // i - 1をして不正アクセスが起こらなければ処理を続ける
            if (0 <= i - 1)
            {
                // 次のバイトの最前ビットが1だった場合はさらに処理を行う
                if (this->mpMemory[i - 1] & 1 << 0)
                {
                    // 現在の最後尾ビットを1にする
                    this->mpMemory[i] |= 1 << OneByteBitCount - 1;
                }
            }
        }
    }
    else
    {
        // 自身を無効化しておく
        this->Deactivate();
    }
}

unsigned long BitFlag::GetOnBitCount(void *src, unsigned long byteSize)
{
    unsigned long counter = 0;
    char *current = (char *)(src);
    for (unsigned long i = 0; i < byteSize; i++)
    {
        for (unsigned long j = 0; j < 8; j++)
        {
            // 各ビットを走査し、ビットがOnだった場合はカウントを増やす
            if ((current[i] & 1 << j) != 0)
            {
                counter++;
            }
        }
    }
    return counter;
}

void BitFlag::operator =(const BitFlag &bitFlag)
{
    // パラメータを代入するまえに、メモリの解放と自身の無効化をしておく
    this->Deactivate();

    // パラメータを代入する
    this->mnSize = bitFlag.mnSize;

    // メモリサイズの確認
    if (0 < this->mnSize)
    {
        // メモリの確保
        this->mpMemory = (char *)malloc(sizeof(char) * this->mnSize);
        if (this->mpMemory != nullptr &&
            bitFlag.mpMemory != nullptr)
        {
            // メモリ内部のコピー
            memcpy(this->mpMemory, bitFlag.mpMemory, sizeof(char) * this->mnSize);

            // 有効であることを記憶させる
            this->mbActive = true;
        }
    }
}

void BitFlag::operator +=(const BitFlag &bitFlag)
{
    if (this->mbActive == true &&
        bitFlag.mbActive == true &&
        this->mnSize == bitFlag.mnSize)
    {
        for (unsigned long i = 0; i < this->mnSize; i++)
        {
            // フラグが片方でも1であれば、自身の着目フラグを1にする
            this->mpMemory[i] = this->mpMemory[i] | bitFlag.mpMemory[i];
        }
    }
    else
    {
        // 自身を無効化しておく
        this->Deactivate();
    }
}

void BitFlag::operator -=(const BitFlag &bitFlag)
{
    if (this->mbActive == true &&
        bitFlag.mbActive == true &&
        this->mnSize == bitFlag.mnSize)
    {
        // 引数bitFlagで1になっているビットを全て0にする
        this->operator&=(~bitFlag);
    }
    else
    {
        // 自身を無効化しておく
        this->Deactivate();
    }
}

// ビットを足し合わせる
void BitFlag::operator |=(const BitFlag &bitFlag)
{
    if (this->mbActive == true &&
        bitFlag.mbActive == true &&
        this->mnSize == bitFlag.mnSize)
    {
        for (unsigned long i = 0; i < this->mnSize; i++)
        {
            // フラグが片方でも1であれば、自身の着目フラグを1にする
            this->mpMemory[i] = this->mpMemory[i] | bitFlag.mpMemory[i];
        }
    }
    else
    {
        // 自身を無効化しておく
        this->Deactivate();
    }
}

// ビットの食い違いを探す
void BitFlag::operator ^=(const BitFlag &bitFlag)
{
    if (this->mbActive == true &&
        bitFlag.mbActive == true &&
        this->mnSize == bitFlag.mnSize)
    {
        for (unsigned long i = 0; i < this->mnSize; i++)
        {
            // フラグが片方でも1であれば、自身の着目フラグを1にする
            this->mpMemory[i] = this->mpMemory[i] ^ bitFlag.mpMemory[i];
        }
    }
    else
    {
        // 自身を無効化しておく
        this->Deactivate();
    }
}

// ビットを厳選する
void BitFlag::operator &=(const BitFlag &bitFlag)
{
    if (this->mbActive == true &&
        bitFlag.mbActive == true &&
        this->mnSize == bitFlag.mnSize)
    {
        for (unsigned long i = 0; i < this->mnSize; i++)
        {
            // フラグが片方でも1であれば、自身の着目フラグを1にする
            this->mpMemory[i] = this->mpMemory[i] & bitFlag.mpMemory[i];
        }
    }
    else
    {
        // 自身を無効化しておく
        this->Deactivate();
    }
}


// ノット演算(ビット構成を自身の真逆にする)
BitFlag BitFlag::operator ~() const
{
    // 初期化されたメモリを作成する
    BitFlag temp(this->mnSize);

    // 内部のメモリを逆にしていく
    for (unsigned long i = 0; i < temp.mnSize; i++)
    {
        temp.mpMemory[i] = ~(this->mpMemory[i]);
    }

    // 逆になったメモリを返す
    return temp;
}

// 指定のビットへの取得のみ許す
bool BitFlag::operator[](unsigned long useBit) const
{
    return this->CheckFlag(useBit);
}

// 指定のビットへのアクセスを許す
BitParam BitFlag::operator[](unsigned long useBit)
{
    return BitParam(this, useBit, this->CheckFlag(useBit));
}

// ビットを足し合わせる
BitFlag operator +(const BitFlag &bitFlagA, const BitFlag &bitFlagB)
{
    return bitFlagA | bitFlagB;
}

// ビットを一斉に消す
BitFlag operator -(const BitFlag &bitFlagA, const BitFlag &bitFlagB)
{
    BitFlag temp;
    if (bitFlagA.CheckActive() == true &&
        bitFlagB.CheckActive() == true &&
        bitFlagA.GetSize() == bitFlagB.GetSize())
    {
        // tempをAと同じものにする
        temp = bitFlagA;

        // BのビットをDeleteする
        temp.operator-=(bitFlagB);
    }
    return temp;
}

// ビットを左へ移動させる
BitFlag operator <<(const BitFlag &bitFlag, unsigned long bitCount)
{
    BitFlag temp = bitFlag;
    for (unsigned long i = 0; i < bitCount; i++)
    {
        temp.MoveL();
    }
    return temp;
}

// ビットを右へ移動させる
BitFlag operator >>(const BitFlag &bitFlag, unsigned long bitCount)
{
    BitFlag temp = bitFlag;
    for (unsigned long i = 0; i < bitCount; i++)
    {
        temp.MoveR();
    }
    return temp;
}

// オア演算(どちらかでも1であれば1にする)
BitFlag operator |(const BitFlag &bitFlagA, const BitFlag &bitFlagB)
{
    BitFlag temp;
    if (bitFlagA.CheckActive() == true &&
        bitFlagB.CheckActive() == true &&
        bitFlagA.GetSize() == bitFlagB.GetSize())
    {
        temp = bitFlagA;
        temp |= bitFlagB;
    }
    return temp;
}

// Xオア演算(両者が違っていれば1にする)
BitFlag operator ^(const BitFlag &bitFlagA, const BitFlag &bitFlagB)
{
    BitFlag temp;
    if (bitFlagA.CheckActive() == true &&
        bitFlagB.CheckActive() == true &&
        bitFlagA.GetSize() == bitFlagB.GetSize())
    {
        temp = bitFlagA;
        temp ^= bitFlagB;
    }
    return temp;
}

// アンド演算(両者1の場合のみ1にする)
BitFlag operator &(const BitFlag &bitFlagA, const BitFlag &bitFlagB)
{
    BitFlag temp;
    if (bitFlagA.CheckActive() == true &&
        bitFlagB.CheckActive() == true &&
        bitFlagA.GetSize() == bitFlagB.GetSize())
    {
        temp = bitFlagA;
        temp.operator&=(bitFlagB);
    }
    return temp;
}