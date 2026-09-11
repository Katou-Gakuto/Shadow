#include "MyTimer.h"

#include <iostream>
#include <algorithm>
#include <ctime>

using namespace std::chrono;

const long long NS_PER_SEC =    1000000000;
const long long NS_PER_MS =     1000000;
const long long NS_PER_US =     1000;

system_clock::time_point chrono2time_point(const time_point<system_clock, nanoseconds> &src)
{
    return system_clock::time_point(duration_cast<system_clock::duration>(src.time_since_epoch()));
}

void chrono2AllParts(const time_point<system_clock, nanoseconds> &src, std::tm *tm, long long *rem_ns, long long *ms, long long *us, long long *ns)
{
    // std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>をstd::tmに変換する
    system_clock::time_point src_time_point = chrono2time_point(src);

    // 秒未満は別で管理しているため、ここでは秒未満を切り捨てる
    time_t src_time_t = system_clock::to_time_t(src_time_point);

    // 値を取得 & 設定
    std::tm src_stdtm = {};

    // time_tをUTC(std::tm)に変換
#if defined(_WIN32)
    gmtime_s(&src_stdtm, &src_time_t);
#else
    gmtime_r(&src_time_t, &src_stdtm);
#endif

    // std::tmが必要な場合は出力
    if (tm != nullptr)
    {
        *tm = src_stdtm;
    }

    // Sourceのすべての値をナノ秒換算で取得
    long long src_ns_total = duration_cast<nanoseconds>(src.time_since_epoch()).count();

    // 秒未満の値を、ナノ秒単位で取得
    long long src_rem_ns = src_ns_total % NS_PER_SEC;
    if (src_rem_ns < 0)
    {
        src_rem_ns += NS_PER_SEC;
    }

    // 秒未満の値がナノ秒単位で必要な場合は出力
    if (rem_ns != nullptr)
    {
        *rem_ns = src_rem_ns;
    }
    // ミリ秒が必要な場合は出力
    if (ms != nullptr)
    {
        // ミリ秒
        *ms = src_rem_ns / NS_PER_MS;
    }
    if (us != nullptr)
    {
        // マイクロ秒
        *us = (src_rem_ns / NS_PER_US) % NS_PER_US;
    }
    if (ns != nullptr)
    {
        // ナノ秒
        *ns = src_rem_ns % NS_PER_US;
    }
}

static std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> TmToChronoUtc(const std::tm &tm, long long ns)
{
    std::tm tmp = tm;

#if defined(_WIN32)
    time_t tt = _mkgmtime(&tmp);   // UTC
#else
    time_t tt = timegm(&tmp);      // UTC
#endif

    auto tp = std::chrono::system_clock::from_time_t(tt);
    return std::chrono::time_point_cast<std::chrono::nanoseconds>(tp)
        + std::chrono::nanoseconds(ns);
}

int DaysInMonth(int year, int month) // month: 0-11
{
    static const int days[] =
    { 31,28,31,30,31,30,31,31,30,31,30,31 };

    if (month == 1) // February
    {
        bool leap =
            (year % 4 == 0 && year % 100 != 0) ||
            (year % 400 == 0);
        return leap ? 29 : 28;
    }
    return days[month];
}

void SwapTimeParam(TimeParam &a, TimeParam &b)
{
    TimeParam temp = a;
    a = b;
    b = temp;
}

TimeParam::TimeParam() :
    mnSetTime()
{
}

TimeParam::TimeParam(const TP &tp) :
    mnSetTime(tp)
{
}

void TimeParam::Initialize()
{
    using clock = std::chrono::system_clock;
    using namespace std::chrono;

    // 現在時刻（ナノ秒）
    auto now = time_point_cast<nanoseconds>(clock::now());

    // 
    mnSetTime = now;
}

void TimeParam::Initialize(
    int targetYear,
    int targetMonth,
    int targetMDay,
    int targetHour,
    int targetMin,
    int targetSec,
    int targetMs,
    int targetUs,
    int targetNs)
{
    using namespace std::chrono;

    std::tm tm{};
    tm.tm_year = targetYear - 1900;
    tm.tm_mon = targetMonth - 1;
    tm.tm_mday = targetMDay;
    tm.tm_hour = targetHour;
    tm.tm_min = targetMin;
    tm.tm_sec = targetSec;

#if defined(_WIN32)
    std::time_t tt = _mkgmtime(&tm);
#else
    std::time_t tt = timegm(&tm);
#endif

    auto tp = time_point_cast<nanoseconds>(
        std::chrono::system_clock::from_time_t(tt));

    tp += milliseconds(targetMs) + microseconds(targetUs) + nanoseconds(targetNs);

    // 精度で丸める
    mnSetTime = tp;
}

void TimeParam::Initialize(
    const TimeParam &src,
    int diffYear,
    int diffMonth,
    int diffMDay,
    int diffHour,
    int diffMin,
    int diffSec,
    int diffMs,
    int diffUs,
    int diffNs)
{
    TimeDiff diff;
    diff.mnYear = diffYear;
    diff.mnMonth = diffMonth;
    diff.mnDay = diffMDay;
    diff.mnHour = diffHour;
    diff.mnMin = diffMin;
    diff.mnSec = diffSec;
    diff.mnNs =
        static_cast<long long>(diffMs) * NS_PER_MS
        + static_cast<long long>(diffUs) * 1'000
        + diffNs;

    this->Initialize(src, diff);
}

// srcからの各時間単位の差分を加算し、精度を指定するもの
void TimeParam::Initialize(
    const TimeParam &src,
    const TimeDiff &diff)
{
    using namespace std::chrono;

    auto tp = src.mnSetTime;

    // 年月日は tm に変換して処理
    std::tm tm{};
    long long rem_ns{};
    chrono2AllParts(tp, &tm, &rem_ns, nullptr, nullptr, nullptr);

    tm.tm_year += diff.mnYear;
    tm.tm_mon += diff.mnMonth;
    tm.tm_mday += diff.mnDay;
    tm.tm_hour += diff.mnHour;
    tm.tm_min += diff.mnMin;
    tm.tm_sec += diff.mnSec;

#if defined(_WIN32)
    std::time_t tt = _mkgmtime(&tm);
#else
    std::time_t tt = timegm(&tm);
#endif

    tp = time_point_cast<nanoseconds>(std::chrono::system_clock::from_time_t(tt));

    tp += nanoseconds(rem_ns + diff.mnNs);

    mnSetTime = tp;
}

// このTimeParamに設定されてる年を取得する関数
int TimeParam::GetYear() const
{
    // 時間についてまとめられている構造体
    std::tm tm{};

    // 既存ヘルパーで分解
    chrono2AllParts(mnSetTime, &tm, nullptr, nullptr, nullptr, nullptr);

    // 年を返す
    return tm.tm_year;
}

// このTimeParamに設定されてる月を取得する関数
int TimeParam::GetMonth() const
{
    // 時間についてまとめられている構造体
    std::tm tm{};

    // 既存ヘルパーで分解
    chrono2AllParts(mnSetTime, &tm, nullptr, nullptr, nullptr, nullptr);

    // 年を返す
    return tm.tm_mon;
}

// このTimeParamに設定されてる日を取得する関数
int TimeParam::GetDay() const
{
    // 時間についてまとめられている構造体
    std::tm tm{};

    // 既存ヘルパーで分解
    chrono2AllParts(mnSetTime, &tm, nullptr, nullptr, nullptr, nullptr);

    // 年を返す
    return tm.tm_mday;
}

// このTimeParamに設定されてる時間を取得する関数
int TimeParam::GetHour() const
{
    // 時間についてまとめられている構造体
    std::tm tm{};

    // 既存ヘルパーで分解
    chrono2AllParts(mnSetTime, &tm, nullptr, nullptr, nullptr, nullptr);

    // 年を返す
    return tm.tm_hour;
}

// このTimeParamに設定されてる分を取得する関数
int TimeParam::GetMinute() const
{
    // 時間についてまとめられている構造体
    std::tm tm{};

    // 既存ヘルパーで分解
    chrono2AllParts(mnSetTime, &tm, nullptr, nullptr, nullptr, nullptr);

    // 年を返す
    return tm.tm_min;
}

// このTimeParamに設定されてる秒を取得する関数
int TimeParam::GetSecond() const
{
    // 時間についてまとめられている構造体
    std::tm tm{};

    // 既存ヘルパーで分解
    chrono2AllParts(mnSetTime, &tm, nullptr, nullptr, nullptr, nullptr);

    // 年を返す
    return tm.tm_sec;
}

// このTimeParamに設定されてるミリ秒を取得する関数
int TimeParam::GetMillSecond() const
{
    // ミリ秒
    long long ms = 0;

    // 既存ヘルパーで分解
    chrono2AllParts(mnSetTime, nullptr, nullptr, &ms, nullptr, nullptr);

    // ミリ秒を返す
    return ms;    
}

// このTimeParamに設定されてるマイクロ秒を取得する関数
int TimeParam::GetMicroSecond() const
{
    // ミリ秒
    long long us = 0;

    // 既存ヘルパーで分解
    chrono2AllParts(mnSetTime, nullptr, nullptr, nullptr, &us, nullptr);

    // ミリ秒を返す
    return us;
}

// このTimeParamに設定されてるナノ秒を取得する関数
int TimeParam::GetNanoSecond() const
{
    // ミリ秒
    long long ns = 0;

    // 既存ヘルパーで分解
    chrono2AllParts(mnSetTime, nullptr, nullptr, nullptr, nullptr, &ns);

    // ミリ秒を返す
    return ns;
}

TimeParam TimeParam::operator+(const TimeDiff &diff) const
{
    std::tm tm{};
    long long rem_ns = 0;

    // 既存ヘルパーで分解
    chrono2AllParts(mnSetTime, &tm, &rem_ns, nullptr, nullptr, nullptr);

    // 日付・時刻を加算（tm は後で正規化される）
    tm.tm_year += diff.mnYear;
    tm.tm_mon += diff.mnMonth;
    tm.tm_mday += diff.mnDay;
    tm.tm_hour += diff.mnHour;
    tm.tm_min += diff.mnMin;
    tm.tm_sec += diff.mnSec;

    // ナノ秒加算
    long long ns = rem_ns + diff.mnNs;

    // 秒への繰り上がり・繰り下がり
    tm.tm_sec += static_cast<int>(ns / NS_PER_SEC);
    ns %= NS_PER_SEC;
    if (ns < 0)
    {
        ns += NS_PER_SEC;
        tm.tm_sec -= 1;
    }

    TimeParam result(TmToChronoUtc(tm, ns));
    return result;
}

TimeParam TimeParam::operator-(const TimeDiff &diff) const
{
    TimeDiff inv = diff;
    inv.mnYear *= -1;
    inv.mnMonth *= -1;
    inv.mnDay *= -1;
    inv.mnHour *= -1;
    inv.mnMin *= -1;
    inv.mnSec *= -1;
    inv.mnNs *= -1;

    return *this + inv;
}

TimeDiff TimeParam::operator-(const TimeParam &other) const
{
    // A >= B になるように並び替え（符号管理を簡単にする）
    bool negative = false;
    const TimeParam *a = this;
    const TimeParam *b = &other;

    if (mnSetTime < other.mnSetTime)
    {
        negative = true;
        a = &other;
        b = this;
    }

    // 分解
    std::tm tmA{}, tmB{};
    long long nsA = 0, nsB = 0;

    chrono2AllParts(a->mnSetTime, &tmA, &nsA, nullptr, nullptr, nullptr);
    chrono2AllParts(b->mnSetTime, &tmB, &nsB, nullptr, nullptr, nullptr);

    TimeDiff diff{};

    // ナノ秒
    diff.mnNs = nsA - nsB;
    if (diff.mnNs < 0)
    {
        diff.mnNs += NS_PER_SEC;
        tmA.tm_sec--;
    }

    // 秒
    diff.mnSec = tmA.tm_sec - tmB.tm_sec;
    if (diff.mnSec < 0)
    {
        diff.mnSec += 60;
        tmA.tm_min--;
    }

    // 分
    diff.mnMin = tmA.tm_min - tmB.tm_min;
    if (diff.mnMin < 0)
    {
        diff.mnMin += 60;
        tmA.tm_hour--;
    }

    // 時
    diff.mnHour = tmA.tm_hour - tmB.tm_hour;
    if (diff.mnHour < 0)
    {
        diff.mnHour += 24;
        tmA.tm_mday--;
    }

    // 日
    diff.mnDay = tmA.tm_mday - tmB.tm_mday;
    if (diff.mnDay < 0)
    {
        tmA.tm_mon--;
        if (tmA.tm_mon < 0)
        {
            tmA.tm_mon = 11;
            tmA.tm_year--;
        }
        diff.mnDay += DaysInMonth(
            tmA.tm_year + 1900,
            tmA.tm_mon);
    }

    // 月
    diff.mnMonth = tmA.tm_mon - tmB.tm_mon;
    if (diff.mnMonth < 0)
    {
        diff.mnMonth += 12;
        tmA.tm_year--;
    }

    // 年
    diff.mnYear = tmA.tm_year - tmB.tm_year;

    // 符号を戻す
    if (negative)
    {
        diff.mnYear *= -1;
        diff.mnMonth *= -1;
        diff.mnDay *= -1;
        diff.mnHour *= -1;
        diff.mnMin *= -1;
        diff.mnSec *= -1;
        diff.mnNs *= -1;
    }
    return diff;
}

bool TimeParam::CheckOver(const TimeParam &oldTime, const TimeParam &newTime)
{
    // oldTimeの方が古かったらtrue
    return oldTime.mnSetTime < newTime.mnSetTime;
}

bool TimeParam::CheckEqualOrOver(const TimeParam &oldTime, const TimeParam &newTime)
{
    // oldTimeの方が古かったらtrue
    return oldTime.mnSetTime <= newTime.mnSetTime;
}

bool operator<(const TimeParam &l, const TimeParam &r)
{
    return TimeParam::CheckOver(l, r);
}

bool operator<=(const TimeParam &l, const TimeParam &r)
{
    return TimeParam::CheckEqualOrOver(l, r);
}

MyTimer::MyTimer()
{

}

MyTimer::~MyTimer()
{

}

int MyTimer::Start()
{
    this->mnStart = high_resolution_clock::now();
    this->mlPointList.clear();
    return 0;
}

int MyTimer::ClearList()
{
    this->mlPointList.clear();
    return 0;
}

int MyTimer::RecordTemp()
{
    this->mnTemp = high_resolution_clock::now();
    return 0;
}

int MyTimer::RecordList()
{
    this->mlPointList.push_back(high_resolution_clock::now());
    return 0;
}

int MyTimer::ChangeStartTemp()
{
    this->mnStart = this->mnTemp;
    return 0;
}

int MyTimer::ChangeStartList(unsigned long recordIndex)
{
    // 
    if (this->mlPointList.size() <= recordIndex)
    {
        return -1;
    }
    this->mnStart = this->mlPointList[recordIndex];
    return 0;
}

long long MyTimer::GetRecordTemp() const
{
    // 返り値
    long long temp = 0;

    // スタート地点からの時間差を確認する
    auto diff = this->mnTemp - this->mnStart;

    // どの単位で記録を取得するか
    temp = duration_cast<nanoseconds>(diff).count();

    // 
    return temp;
}

long long MyTimer::GetRecordList(unsigned long recordIndex) const
{
    // 
    if (this->mlPointList.size() <= recordIndex)
    {
        return 0;
    }

    // 返り値
    long long temp = 0;

    // スタート地点からの時間差を確認する
    auto diff = this->mlPointList[recordIndex] - this->mnStart;

    // 
    temp = duration_cast<nanoseconds>(diff).count();

    // 
    return temp;
}

MyAlarm::MyAlarm(unsigned long useSetTime) :
    mlTimeParams(0),
    mnNextOverIndex(0)
{
    mlTimeParams.reserve(useSetTime);
}

MyAlarm::~MyAlarm()
{
    this->mlTimeParams.clear();
}

int MyAlarm::CheckOverIndex()
{
    TimeParam now;
    now.Initialize();

    int lastOverIndex = (int)mnNextOverIndex - 1;

    while (mnNextOverIndex < mlTimeParams.size() &&
        mlTimeParams[mnNextOverIndex] <= now)
    {
        lastOverIndex = mnNextOverIndex;
        ++mnNextOverIndex;
    }

    return lastOverIndex;
}

bool MyAlarm::AddSetTime(const TimeParam &param)
{
    mlTimeParams.push_back(param);
    std::sort(mlTimeParams.begin(), mlTimeParams.end());

    TimeParam now;
    now.Initialize();
    return param <= now;
}

void MyAlarm::Clear()
{
    this->mlTimeParams.clear();
    this->mnNextOverIndex = 0;
}
