#pragma once

#include <chrono>
#include <vector>

// このヘッダに書かれたクラスは UTC を基準とする絶対時刻を扱います
// よって、ローカルタイム・タイムゾーン・DST 等への変換は手動で行ってください。

#define Micro2Nano 1000LL;
#define Mill2Nano 1000000LL;
#define Sec2Nano 1000000000LL;

// 加算や減算に使用する構造体
class TimeDiff
{
public:
    int mnYear = 0;
    int mnMonth = 0;
    int mnDay = 0;
    int mnHour = 0;
    int mnMin = 0;
    int mnSec = 0;
    long long mnNs = 0;

    TimeDiff() :
        mnYear(0),
        mnMonth(0),
        mnDay(0),
        mnHour(0),
        mnMin(0),
        mnSec(0),
        mnNs(0)
    {
    }
    ~TimeDiff()
    {
    }
};

// 時刻と精度の構造体
class TimeParam
{
private:
    using TP = std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>;

    TP mnSetTime;

public:
    TimeParam();
    TimeParam(const TP &tp);
    ~TimeParam() = default;

    // 現在時刻を取得する関数
    void Initialize();

    // 年、月、日、時間、分、秒、ミリ秒、マイクロ秒、ナノ秒と精度を全て指定するもの
    void Initialize(
        int targetYear,
        int targetMonth,
        int targetMDay,
        int targetHour,
        int targetMin,
        int targetSec,
        int targetMs,
        int targetUs,
        int targetNs);

    // srcからの各時間単位の差分を加算し、精度を指定するもの
    void Initialize(
        const TimeParam &src,
        int diffYear,
        int diffMonth,
        int diffMDay,
        int diffHour,
        int diffMin,
        int diffSec,
        int diffMs,
        int diffUs,
        int diffNs);

    // srcからの各時間単位の差分を加算し、精度を指定するもの
    void Initialize(
        const TimeParam &src,
        const TimeDiff &diff);

    // このTimeParamに設定されてる年を取得する関数
    int GetYear() const;

    // このTimeParamに設定されてる月を取得する関数
    int GetMonth() const;

    // このTimeParamに設定されてる日を取得する関数
    int GetDay() const;

    // このTimeParamに設定されてる時間を取得する関数
    int GetHour() const;

    // このTimeParamに設定されてる分を取得する関数
    int GetMinute() const;

    // このTimeParamに設定されてる秒を取得する関数
    int GetSecond() const;

    // このTimeParamに設定されてるミリ秒を取得する関数
    int GetMillSecond() const;

    // このTimeParamに設定されてるマイクロ秒を取得する関数
    int GetMicroSecond() const;

    // このTimeParamに設定されてるナノ秒を取得する関数
    int GetNanoSecond() const;

    TimeParam operator+(const TimeDiff &diff) const;
    TimeParam operator-(const TimeDiff &diff) const;

    // 時刻差（必要なら）
    TimeDiff operator-(const TimeParam &other) const;

    static bool CheckOver(const TimeParam &oldTime, const TimeParam &newTime);
    static bool CheckEqualOrOver(const TimeParam &oldTime, const TimeParam &newTime);
};

bool operator<(const TimeParam &l, const TimeParam &r);
bool operator<=(const TimeParam &l, const TimeParam &r);

// 経過時間を記録するやつ
class MyTimer
{
private:
    std::chrono::steady_clock::time_point mnStart;
    std::chrono::steady_clock::time_point mnTemp;
    std::vector<std::chrono::steady_clock::time_point> mlPointList;

public:
    MyTimer();
    ~MyTimer();

    int Start();
    int ClearList();

    int RecordTemp();
    int RecordList();

    int ChangeStartTemp();
    int ChangeStartList(unsigned long recordIndex);

    long long GetRecordTemp() const;
    long long GetRecordList(unsigned long recordIndex) const;
};

// その時刻になったかを確認するクラス
class MyAlarm
{
private:
    std::vector<TimeParam> mlTimeParams;
    unsigned long mnNextOverIndex;

public:
    MyAlarm(unsigned long useSetTime);
    ~MyAlarm();

    int CheckOverIndex();

    // 追加でアラーム時刻を設定する関数
    // 返り値のboolは『新しく設定した時刻が現在時刻より前か』を返しています
    bool AddSetTime(const TimeParam &param);

    void Clear();
};
