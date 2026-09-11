#include "MyFunctions.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <limits.h>
#include <float.h>

bool MyFunctions::EqualInRangeF(float target, float base, float range)
{
    return ((base - range <= target) && (target <= base + range));
}

float MyFunctions::GetTargetNewrParam(float target, float f1, float f2)
{
    float temp = f1;
    float targetMin1 = f1 - target;
    float targetMin2 = f2 - target;
    if (targetMin2 < targetMin1)
    {
        temp = f2;
    }
    return temp;
}

float MyFunctions::GetRatio(float paramA, float paramB, float paramC)
{
    float returnValue = 0.0f;
    if (MyFunctions::EqualInRangeF(paramB, 0.0f, 0.00001f) == false)
    {
        float temp = paramA / paramB;
        returnValue = paramC * temp;
    }
    return returnValue;
}

float MyFunctions::GetFloatLerp(float start, float end, float rate)
{
    // 補間率を調整する
    if (rate < 0.0f)
    {
        rate = 0.0f;
    }
    if (1.0f < rate)
    {
        rate = 1.0f;
    }

    // 返り値
    float temp = 0.0f;

    // startからtargetまでの差分を取得する
    float distanceF = end - start;

    // 差分に倍率(補間率)をかけ、差分から戻すためにstartを足す
    temp = (distanceF * rate) + start;

    // 値を返す
    return temp;
}

float MyFunctions::GetClamp(float target, float min, float max)
{
    if (target < min)
    {
        return min;
    }
    if (max < target)
    {
        return max;
    }
    return target;
}

float MyFunctions::GetClamp01(float target)
{
    if (target < 0.0f)
    {
        return 0.0f;
    }
    if (1.0f < target)
    {
        return 1.0f;
    }
    return target;
}

float MyFunctions::Rad2Deg(float radian)
{
    return (radian / M_PI) * 180;
}

float MyFunctions::Deg2Rad(float degree)
{
    return (degree / 180) * M_PI;

}

float MyFunctions::GetMaxRad()
{
    return M_PI;
}

float MyFunctions::GetMinRad()
{
    return -(M_PI);
}

float MyFunctions::GetAllRad()
{
    return MyFunctions::GetMaxRad() - MyFunctions::GetMinRad();
}

float MyFunctions::GetArrangeRad(float radian)
{
    while ((MyFunctions::GetMinRad() <= radian && radian <= MyFunctions::GetMaxRad()) == false)
    {
        if (radian < MyFunctions::GetMinRad())
        {
            radian += MyFunctions::GetAllRad();
        }
        else
        {
            radian -= MyFunctions::GetAllRad();
        }
    }

    return radian;
}

float MyFunctions::GetMaxDeg()
{
    return 180.0f;
}

float MyFunctions::GetMinDeg()
{
    return -180.0f;
}

float MyFunctions::GetAllDeg()
{
    return MyFunctions::GetMaxDeg() - MyFunctions::GetMinDeg();
}

float MyFunctions::GetArrangeDeg(float degree)
{
    while ((MyFunctions::GetMinDeg() <= degree && degree <= MyFunctions::GetMaxDeg()) == false)
    {
        if (degree < MyFunctions::GetMinDeg())
        {
            degree += MyFunctions::GetAllDeg();
        }
        else
        {
            degree -= MyFunctions::GetAllDeg();
        }
    }

    return degree;
}

float MyFunctions::GetPI()
{
    return M_PI;
}

float MyFunctions::GetFastAtan2InigoQuilez(float x, float y)
{
    float t0, t1, t2, t3;

    t0 = fabsf(x);
    t1 = fabsf(y);
    t2 = max(t0, t1);
    t1 = min(t0, t1);
    t3 = t1 / (t2 + 1e-10f);
    float t3_sq = t3 * t3;
    float result = (((-0.046496f * t3_sq + 0.159314f) * t3_sq - 0.327622f) * t3_sq) * t3 + t3;
    if (t0 < t1)
    {
        result = (M_PI * 0.5f) - result;
    }
    if (x < 0)
    {
        result = M_PI - result;
    }
    if (y < 0)
    {
        result = -result;
    }
    return result;
}

signed int MyFunctions::GetMinInt()
{
    return INT_MIN;
}

signed char MyFunctions::GetMinChar()
{
    return CHAR_MIN;
}

signed short int MyFunctions::GetMinS()
{
    return SHRT_MIN;
}

signed long int MyFunctions::GetMinL()
{
    return LONG_MIN;
}

signed long long int MyFunctions::GetMinLL()
{
    return LLONG_MIN;
}

signed int MyFunctions::GetMaxInt()
{
    return INT_MAX;
}

signed char MyFunctions::GetMaxChar()
{
    return CHAR_MAX;
}

signed short int MyFunctions::GetMaxS()
{
    return SHRT_MAX;
}

signed long int MyFunctions::GetMaxL()
{
    return LONG_MAX;
}

signed long long int MyFunctions::GetMaxLL()
{
    return LLONG_MAX;
}

unsigned int MyFunctions::GetMaxU()
{
    return UINT_MAX;
}

unsigned char MyFunctions::GetMaxUC()
{
    return UCHAR_MAX;
}

unsigned short int MyFunctions::GetMaxUS()
{
    return USHRT_MAX;
}

unsigned long int MyFunctions::GetMaxUL()
{
    return ULONG_MAX;
}

unsigned long long int MyFunctions::GetMaxULL()
{
    return ULLONG_MAX;
}

float MyFunctions::GetMinFloat()
{
    return FLT_MIN;
}

double MyFunctions::GetMinDouble()
{
    return DBL_MIN;
}

long double MyFunctions::GetMinLongDouble()
{
    return LDBL_MIN;
}

float MyFunctions::GetMaxFloat()
{
    return FLT_MAX;
}

double MyFunctions::GetMaxDouble()
{
    return DBL_MAX;
}

long double MyFunctions::GetMaxLongDouble()
{
    return LDBL_MAX;
}

// BitChecker……フラグを調べるときのchecker。0x0001や0x0040などの全ビットの中で一つだけ1になっているもの
bool MyFunctions::CheckTargetIsBitChecker(unsigned long target)
{
    bool temp = false;

    unsigned long bitMax = 8 * 4;
    for (unsigned long i = 0; i < bitMax; i++)
    {
        if ((1 << i) & target)
        {
            if (temp == true)
            {
                temp = false;
                break;
            }
            else
            {
                temp = true;
            }
        }
    }

    return temp;
}

// そのビットが0x0001を何回「<<」させたら得られる値かを取得する
unsigned long MyFunctions::ShiftCount2BitChecker(unsigned long shiftCount)
{
    return (1 << shiftCount);
}


// 0x0001を回数分「<<」したら得られる値を取得する
unsigned long MyFunctions::BitChecker2ShiftCount(unsigned long bitChecker)
{
    unsigned long shiftCount = 0;
    if (MyFunctions::CheckTargetIsBitChecker(bitChecker))
    {
        while (((1 << shiftCount) & bitChecker) == false)
        {
            shiftCount++;
        }
    }
    else
    {
        // エラーの場合は返り値を一億にしています。そんなにビットあるわけねぇだろ！
        shiftCount = 100000000;
    }

    return shiftCount;
}

std::string MyFunctions::LPCWSTR2String(LPCWSTR lpcwstr)
{
    if (lpcwstr == nullptr) return std::string();

    // 変換後のバッファサイズを取得（マルチバイト文字数）
    int sizeNeeded = WideCharToMultiByte(
        CP_ACP,            // コードページ（ANSIコードページ）
        0,                 // 変換フラグ
        lpcwstr,           // 変換元ワイド文字列
        -1,                // 文字列長（-1でNULL終端まで）
        NULL,              // 出力バッファ（今回はサイズ取得のみ）
        0,
        NULL,
        NULL);

    if (sizeNeeded == 0)
    {
        // エラー処理
        return std::string();
    }

    // 出力用バッファを確保
    std::string result(sizeNeeded, 0);

    // 実際の変換実行
    int bytesConverted = WideCharToMultiByte(
        CP_ACP,
        0,
        lpcwstr,
        -1,
        &result[0],
        sizeNeeded,
        NULL,
        NULL);

    if (bytesConverted == 0)
    {
        // エラー処理
        return std::string();
    }

    // std::stringはNULL終端を含むので最後のNULLは除く
    result.resize(bytesConverted - 1);

    return result;
}

// UTF-16で取得した文字列をUTF-8で表示する(Unicode→MultiByte)に変換している?
std::string MyFunctions::UTF16toUTF8(const std::wstring &utf16str)
{
    // 何も自分のバッファが必要か調べる
    int utf8len = WideCharToMultiByte(CP_UTF8, 0, utf16str.c_str(), -1, nullptr, 0, nullptr, nullptr);

    // 必要量に応じたサイズのメモリを確保する
    std::string utf8str(utf8len, 0);

    // メモリの中に変換したデータを入れる
    WideCharToMultiByte(CP_UTF8, 0, utf16str.c_str(), -1, &utf8str[0], utf8len, nullptr, nullptr);

    // string型はnull文字が要らないので削除
    utf8str.pop_back();

    // 結果を返す
    return utf8str;
}

// UTF-8で取得した文字列をUTF-16で表示する
std::wstring MyFunctions::UTF8toUTF16(const std::string &utf8str)
{
    // 何も自分のバッファが必要か調べる
    int utf16len = MultiByteToWideChar(CP_UTF8, 0, utf8str.c_str(), -1, NULL, 0);

    // 必要量に応じたサイズのメモリを確保する
    std::wstring utf16str(utf16len, 0);

    // メモリの中に変換したデータを入れる
    MultiByteToWideChar(CP_UTF8, 0, utf8str.c_str(), -1, &utf16str[0], utf16len);

    // string型はnull文字が要らないので削除
    utf16str.pop_back();

    // 結果を返す
    return utf16str;
}

// 
void MyFunctions::GetDirectoryAndFileName(const std::string &src, std::string *directory, std::string *fileName)
{
    size_t pos = src.find_last_of("/\\");

    // パス区切り文字がない場合かどうかで処理を行う
    if (pos == std::string::npos)
    {
        if (directory != nullptr)
        {
            *directory = "";
        }
        if (fileName != nullptr)
        {
            *fileName = src;
        }
    }
    else
    {
        if (directory != nullptr)
        {
            *directory = src.substr(0, pos);
        }
        if (fileName != nullptr)
        {
            *fileName = src.substr(pos + 1);
        }
    }
}

signed long long MyFunctions::str2ll(const std::string &param)
{
    return std::stoll(param);
}

unsigned long long MyFunctions::str2ull(const std::string &param)
{
    return std::stoull(param);
}

float MyFunctions::str2float(const std::string &param)
{
    return std::stof(param);
}

double MyFunctions::str2double(const std::string &param)
{
    return std::stod(param);
}
