#pragma once
#define _WINSOCKAPI_
#include <windows.h>
#include <string>

// 便利関数まとめ
class MyFunctions
{
public:
    static bool EqualInRangeF(float f1, float f2, float range);
    static float GetTargetNewrParam(float target, float f1, float f2);
    static float GetRatio(float paramA, float ParamB, float paramC);
    static float GetFloatLerp(float start, float end, float rate);
    static float GetClamp(float target, float min, float max);
    static float GetClamp01(float target);
    static float Rad2Deg(float radian);
    static float Deg2Rad(float degree);
    static float GetMaxRad();
    static float GetMinRad();
    static float GetAllRad();
    static float GetArrangeRad(float radian);
    static float GetMaxDeg();
    static float GetMinDeg();
    static float GetAllDeg();
    static float GetArrangeDeg(float degree);
    static float GetPI();
    static float GetFastAtan2InigoQuilez(float x, float y);

    static signed int GetMaxInt();
    static signed char GetMaxChar();
    static signed short int GetMaxS();
    static signed long int GetMaxL();
    static signed long long int GetMaxLL();
    static signed int GetMinInt();
    static signed char GetMinChar();
    static signed short int GetMinS();
    static signed long int GetMinL();
    static signed long long int GetMinLL();

    static unsigned int GetMaxU();
    static unsigned char GetMaxUC();
    static unsigned short int GetMaxUS();
    static unsigned long int GetMaxUL();
    static unsigned long long int GetMaxULL();

    static float GetMinFloat();
    static double GetMinDouble();
    static long double GetMinLongDouble();
    static float GetMaxFloat();
    static double GetMaxDouble();
    static long double GetMaxLongDouble();



    static bool CheckTargetIsBitChecker(unsigned long target);                      // BitChecker……フラグを調べるときのchecker。0x0001や0x0040などの全ビットの中で一つだけ1になっているもの
    static unsigned long ShiftCount2BitChecker(unsigned long shiftCount);           // そのビットが0x0001を何回「<<」させたら得られる値かを取得する
    static unsigned long BitChecker2ShiftCount(unsigned long bitChecker);           // 0x0001を回数分「<<」したら得られる値を取得する

    static std::string LPCWSTR2String(LPCWSTR lpcwstr);                             // LPCWSTR(Unicode)をstd::string(MultiByte)に変換する関数
    static std::string UTF16toUTF8(const std::wstring &utf16str);                   // UTF-16で取得した文字列をUTF-8で表示する(Unicode→MultiByte)に変換している?
    static std::wstring UTF8toUTF16(const std::string &utf8str);                    // UTF-8で取得した文字列をUTF-16で表示する(MultiByte→Unicode)に変換している?

    static void GetDirectoryAndFileName(const std::string &src, std::string *directory, std::string *fileName);

    static signed long long str2ll(const std::string &param);
    static unsigned long long str2ull(const std::string &param);
    static float str2float(const std::string &param);
    static double str2double(const std::string &param);
};