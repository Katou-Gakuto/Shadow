#pragma once

#include <vector>

// 縛りのある可変長引数関数を生成するためのヘッダ
#include "MYTMP.h"

// そのフラグたちを管理するために何Byte必要かを計算する
#define GetFlagBoxMax(useBitCount) (((useBitCount) % 8 == 0) ? ((useBitCount) / 8) : (((useBitCount) / 8) + 1))

/*
https://qiita.com/drken/items/7c6ff2aa4d8fce1c9361
↑ビットに詳しいQuitaのページ
*/

class FlagByte;

class ByteParam
{
    FlagByte *mpByte;
    unsigned char mnUseBit;
    bool mbParam;

public:
    ByteParam(FlagByte *bitFlag, unsigned char useBit, bool param);
    ByteParam(const FlagByte *bitFlag, unsigned char useBit, bool param);
    ~ByteParam();

    operator bool()const;
    ByteParam &operator=(bool flag);
};

class FlagByte
{
private:
    char mcOneByte;

public:
    FlagByte();
    FlagByte(unsigned char bitData);
    FlagByte(const FlagByte &byte);

    ~FlagByte();

    unsigned long CountOnFlag();

    FlagByte &AllOn();
    FlagByte &AllOff();
    FlagByte &OnFlag(unsigned char useBit);
    FlagByte &OffFlag(unsigned char useBit);
    FlagByte &SetFlag(unsigned char useBit, bool flag);
    ByteParam GetFlag(unsigned char useBit);
    const ByteParam GetFlag(unsigned char useBit) const;

    FlagByte MoveL(unsigned char moveCount);
    FlagByte MoveR(unsigned char moveCount);

    // ビットを変更する
    FlagByte &operator =(const FlagByte &flagByte);

    // ビットを足し合わせる
    FlagByte &operator +=(const FlagByte &flagByte);

    // ビットを一斉に消す
    FlagByte &operator -=(const FlagByte &flagByte);

    // ビットを足し合わせる
    FlagByte &operator |=(const FlagByte &flagByte);

    // ビットの食い違いを探す
    FlagByte &operator ^=(const FlagByte &flagByte);

    // ビットを厳選する
    FlagByte &operator &=(const FlagByte &flagByte);

    // ノット演算(ビット構成を自身の真逆にする)
    FlagByte operator ~() const;

    // 指定のビットへのアクセスを許す
    ByteParam operator[](unsigned char useBit);

    // 指定のビットへのアクセスを許す
    ByteParam operator[](unsigned char useBit) const;
};

// ビットを足し合わせる
FlagByte operator +(const FlagByte &flagByteA, const FlagByte &flagByteB);

// ビットを一斉に消す
FlagByte operator -(const FlagByte &flagByteA, const FlagByte &flagByteB);

// ビットを左へ移動させる
FlagByte operator <<(const FlagByte &flagByte, unsigned char bitCount);

// ビットを右へ移動させる
FlagByte operator >>(const FlagByte &flagByte, unsigned char bitCount);

// オア演算(どちらかでも1であれば1にする)
FlagByte operator |(const FlagByte &flagByteA, const FlagByte &flagByteB);

// Xオア演算(両者が違っていれば1にする)
FlagByte operator ^(const FlagByte &flagByteA, const FlagByte &flagByteB);

// アンド演算(両者1の場合のみ1にする)
FlagByte operator &(const FlagByte &flagByteA, const FlagByte &flagByteB);

class FlagByteBoxOperater
{
private:
    static void BitCount2Index(unsigned long bitCount, unsigned long &index, unsigned long &bitIndex);
    
public:
    // 条件クラス
    template <class TargetType>
    class TypeFilter : public MYTMP_Kit_TypeCheckerBase<
        (MYTMP_TC_RCVR_unsigned<TargetType>::value &&MYTMP_TC_RCVR_int<TargetType>::value && !MYTMP_TC_RCVR_Equal<TargetType, bool>::value) ||
        (MYTMP_TC_RCVR_UnsignedEnum<TargetType>::value)>
    {
    };

    enum test :unsigned char
    {
        a,
    };

    // クラス制限付きの可変長引数関数の実装例
    template<class... TargetType>
    static typename MYTMP_Kit_FuncCreater<std::vector<FlagByte>, TypeFilter, TargetType...>::return_type CreateFlagByteBox(unsigned long useByte, TargetType&&... target)
    {
        constexpr bool testParam0 = (MYTMP_TC_RCVR_unsigned<test>::value && MYTMP_TC_RCVR_int<test>::value && !MYTMP_TC_RCVR_Equal<test, bool>::value);
        constexpr bool testParam1 = MYTMP_TC_RCVR_UnsignedEnum<test>::value;
        constexpr bool testParam2 = TypeFilter<test>::value;
        constexpr bool testParam3 = MYTMP_TypeFilter<>::value;
        constexpr bool testParam4 = MYTMP_TypeFilter<TypeFilter<test>>::value;
        constexpr bool testParam5 = MYTMP_TypeFilter<TypeFilter<test>, TypeFilter<test>>::value;
        constexpr bool testParam6 = MYTMP_TypeFilter<TypeFilter<MYTMP_TG_Decay<test>>>::value;

        using testType2 = MYTMP_Kit_FuncCreater<std::vector<FlagByte>, TypeFilter, test>::return_type;

        /*
        using testType2 = MYTMP_FuncCreater<MyTypeFilter<TypeFilter<MYTMP_TG_Decay<test>>>::value, std::vector<FlagByte>>;
        bool testParam8 = MyTypeFilter<TypeFilter<test>, TypeFilter<test>>::value;
        bool testParam9 = MyTypeFilter<TypeFilter<test>, TypeFilter<test>>::value;
        */



        // 作業空間を取得する
        std::vector<FlagByte> glWorkSpace(useByte);

        // ループに入り、フラグを整える
        MYTMP_Kit_FuncLoopLambda(
            [&glWorkSpace](auto x)
            {
                FlagByteBoxOperater::OnFlag(glWorkSpace.data(), glWorkSpace.size(), static_cast<unsigned long>(x));
            },
            std::forward<TargetType>(target)...);

        // 結果を返す
        return glWorkSpace;
    }
    /*
    template<class... TargetType,
        typename = InWorkKitPartsSpace::MYTMP_FuncCreater<
        (MYTMP_Kit_TypeFilterCreater<
            (MYTMP_TC_RCVR_UnsignedEnum<TargetType>::value ||
                (MYTMP_TC_RCVR_int<TargetType>::value &&
                    MYTMP_TC_RCVR_unsigned<TargetType>::value &&
                    !MYTMP_TC_RCVR_Equal<TargetType, bool>::value)
                )>::value
            ), void>...>
    static std::vector<FlagByte> CreateFlagByteBox(unsigned long useByte, TargetType&&... target)
    {
        std::vector<FlagByte> glWorkSpace(useByte);

        MYTMP_Kit_FuncLoopLambda(
            [&glWorkSpace](auto x)
            {
                FlagByteBoxOperater::OnFlag(glWorkSpace.data(), glWorkSpace.size(), static_cast<unsigned long>(x));
            },
            std::forward<TargetType>(target)...);

        return glWorkSpace;
    }
    */

    static unsigned long CountOnFlag(FlagByte *bytes, unsigned long byteNum);

    static FlagByte *Assignment(FlagByte *destBytes, const FlagByte *srcBytes, unsigned long destByte);
    static FlagByte *Assignment(FlagByte *destBytes, const std::vector<FlagByte> &srcBytes, unsigned long destByte);

    static FlagByte *AllOn(FlagByte *bytes, unsigned long byteNum);
    static FlagByte *AllOff(FlagByte *bytes, unsigned long byteNum);

    static FlagByte *OnFlag(FlagByte *bytes, unsigned long byteNum, unsigned long targetBit);
    static FlagByte *OffFlag(FlagByte *bytes, unsigned long byteNum, unsigned long targetBit);
    static FlagByte *SetFlag(FlagByte *bytes, unsigned long byteNum, unsigned long targetBit, bool flag);
    static ByteParam GetFlag(FlagByte *bytes, unsigned long byteNum, unsigned long targetBit);

    static std::vector<FlagByte> Add(const FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum);
    static std::vector<FlagByte> Sub(const FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum);
    static std::vector<FlagByte> AND(const FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum);
    static std::vector<FlagByte> OR(const FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum);
    static std::vector<FlagByte> XOR(const FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum);
    static std::vector<FlagByte> NOT(const FlagByte *byte, unsigned long byteNum);
    static std::vector<FlagByte> MoveL(const FlagByte *byteL, unsigned long moveCount, unsigned long byteNum);
    static std::vector<FlagByte> MoveR(const FlagByte *byteL, unsigned long moveCount, unsigned long byteNum);

    static FlagByte *AddAss(FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum);
    static FlagByte *SubAss(FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum);
    static FlagByte *ANDAss(FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum);
    static FlagByte *ORAss(FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum);
    static FlagByte *XORAss(FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum);
    static FlagByte *NOTAss(FlagByte *byte, unsigned long byteNum);
    static FlagByte *MoveLAss(FlagByte *byteL, unsigned long moveCount, unsigned long byteNum);
    static FlagByte *MoveRAss(FlagByte *byteL, unsigned long moveCount, unsigned long byteNum);

    static bool Equal(const FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum);
    static bool NotEqual(const FlagByte *byteL, const FlagByte *byteR, unsigned long byteNum);
};



class BitFlag;

// BitFlagで使用するクラス
class BitParam
{
private:
    BitFlag *mpBitFlag;
    unsigned long mnUseBit;
    bool mbParam;

public:
    BitParam(BitFlag *bitFlag, unsigned long useBit, bool param);
    ~BitParam();

    operator bool()const;
    BitParam &operator=(bool flag);
};

// Bit単位でフラグ管理を行ってくれるお手軽クラス
// 将来はフラグ専用の関数置き場みたいにしてもいいですねぇ
class BitFlag
{
private:
    char *mpMemory;
    unsigned long mnSize;
    bool mbActive;

    void Deactivate();

public:
    BitFlag();
    BitFlag(unsigned long useBit);
    BitFlag(const BitFlag &bitFlag);
    ~BitFlag();

    bool CheckActive() const;
    bool CheckZero() const;
    bool CheckFlag(unsigned long bitCount) const;
    int OnFlag(unsigned long bitCount);
    int OffFlag(unsigned long bitCount);

    unsigned long GetSize() const;
    void ResetSize(unsigned long byteSize);
    void MoveL();
    void MoveR();

    static unsigned long GetOnBitCount(void *src, unsigned long  byteSize);

    // ビットを変更する
    void operator =(const BitFlag &bitFlag);

    // ビットを足し合わせる
    void operator +=(const BitFlag &bitFlag);

    // ビットを一斉に消す
    void operator -=(const BitFlag &bitFlag);

    // ビットを足し合わせる
    void operator |=(const BitFlag &bitFlag);

    // ビットの食い違いを探す
    void operator ^=(const BitFlag &bitFlag);

    // ビットを厳選する
    void operator &=(const BitFlag &bitFlag);

    // ノット演算(ビット構成を自身の真逆にする)
    BitFlag operator ~() const;

    // 指定のビットへの取得のみ許す
    bool operator[](unsigned long useBit) const;

    // 指定のビットへのアクセスを許す
    BitParam operator[](unsigned long useBit);
};

// ビットを足し合わせる
BitFlag operator +(const BitFlag &bitFlagA, const BitFlag &bitFlagB);

// ビットを一斉に消す
BitFlag operator -(const BitFlag &bitFlagA, const BitFlag &bitFlagB);

// ビットを左へ移動させる
BitFlag operator <<(const BitFlag &bitFlag, unsigned long bitCount);

// ビットを右へ移動させる
BitFlag operator >>(const BitFlag &bitFlag, unsigned long bitCount);

// オア演算(どちらかでも1であれば1にする)
BitFlag operator |(const BitFlag &bitFlagA, const BitFlag &bitFlagB);

// Xオア演算(両者が違っていれば1にする)
BitFlag operator ^(const BitFlag &bitFlagA, const BitFlag &bitFlagB);

// アンド演算(両者1の場合のみ1にする)
BitFlag operator &(const BitFlag &bitFlagA, const BitFlag &bitFlagB);
