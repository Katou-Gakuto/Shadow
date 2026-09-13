#include "FlagByte.h"
#define _WINSOCKAPI_
#include <stdlib.h>
#include <windows.h>

#define OneByteBitCount (8)
#define targetBit2byteIndex (targetBit)((targetBit) / OneByteBitCount)
#define targetBit2bitIndex (targetBit)((targetBit) % OneByteBitCount)

FlagByteParam::FlagByteParam(FlagByte *bitFlag, unsigned char useBit, bool param) :
    mpByte(bitFlag),
    mnUseBit(useBit),
    mbParam(param)
{
}

FlagByteParam::FlagByteParam(const FlagByte *bitFlag, unsigned char useBit, bool param) :
    mpByte(const_cast<FlagByte *>(bitFlag)),
    mnUseBit(useBit),
    mbParam(param)
{
}

FlagByteParam::~FlagByteParam()
{
    this->mpByte = nullptr;
    this->mnUseBit = 0;
    this->mbParam = false;
}

FlagByteParam::operator bool()const
{
    return this->mbParam;
}

FlagByteParam &FlagByteParam::operator=(bool flag)
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

FlagByteParam FlagByte::GetFlag(unsigned char useBit)
{
    return FlagByteParam(this, useBit, (this->mcOneByte & (1 << useBit)) != 0);
}

const FlagByteParam FlagByte::GetFlag(unsigned char useBit) const
{
    return FlagByteParam(this, useBit, (this->mcOneByte & (1 << useBit)) != 0);
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
FlagByteParam FlagByte::operator[](unsigned char useBit)
{
    return this->GetFlag(useBit);
}

// 指定のビットへのアクセスを許す
FlagByteParam FlagByte::operator[](unsigned char useBit) const
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

FlagByteParam FlagByteBoxOperater::GetFlag(FlagByte *bytes, unsigned long byteNum, unsigned long targetBit)
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
