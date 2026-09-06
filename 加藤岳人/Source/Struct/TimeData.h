#pragma once
#include <cstdint>

// 時間情報
struct TIME_DATA
{
    constexpr TIME_DATA()
    : Full(0llu)
    {}
    constexpr TIME_DATA(uint64_t full)
    : Full(full)
    {}

    union
    {
        struct
        {
            uint32_t Time;  // 時間
            uint32_t Count; // カウント
        };

        uint64_t Full;
    };

    TIME_DATA operator+(unsigned int time) const
    {
        return TIME_DATA(this->Full + time);
    }
    TIME_DATA& operator+=(unsigned int time)
    {
        this->Full += time;
        return *this;
    }

    TIME_DATA operator+(const TIME_DATA& time) const
    {
        return TIME_DATA(this->Full + time.Full);
    }
    TIME_DATA& operator+=(const TIME_DATA& time)
    {
        this->Full += time.Full;
        return * this;
    }


    TIME_DATA operator-(unsigned int time) const
    {
        return TIME_DATA(this->Full - time);
    }
    TIME_DATA& operator-=(unsigned int time)
    {
        this->Full -= time;
        return *this;
    }

    TIME_DATA operator-(const TIME_DATA& time) const
    {
        return TIME_DATA(this->Full - time.Full);
    }
    TIME_DATA& operator-=(const TIME_DATA& time)
    {
        this->Full -= time.Full;
        return *this;
    }

    bool operator<(const TIME_DATA& time) const
    {
        return this->Full < time.Full;
    }

    bool operator<=(const TIME_DATA& time) const
    {
        return this->Full <= time.Full;
    }

    bool operator>(const TIME_DATA& time) const
    {
        return this->Full > time.Full;
    }

    bool operator>=(const TIME_DATA& time) const
    {
        return this->Full >= time.Full;
    }

    bool operator==(const TIME_DATA& time) const
    {
        return this->Full == time.Full;
    }
};

// フレーム情報
struct FRAME_DATA
{
    constexpr FRAME_DATA()
    : Frame(0u)
    {}
    constexpr FRAME_DATA(unsigned int frame)
    : Frame(frame)
    {
    }
    
    unsigned int Frame; // フレーム

    FRAME_DATA& operator++()
    {
        ++this->Frame;
        return *this;
    }

    FRAME_DATA operator+(unsigned int frame) const
    {
        return FRAME_DATA(this->Frame + frame);
    }
    FRAME_DATA& operator+=(unsigned int frame)
    {
        this->Frame += frame;
        return *this;
    }

    bool operator<(const FRAME_DATA& time) const
    {
        return this->Frame < time.Frame;
    }
    bool operator<=(const FRAME_DATA& time) const
    {
        return this->Frame <= time.Frame;
    }

    bool operator>(const FRAME_DATA& time) const
    {
        return this->Frame > time.Frame;
    }
    bool operator>=(const FRAME_DATA& time) const
    {
        return this->Frame >= time.Frame;
    }

    bool operator==(const FRAME_DATA& time) const
    {
        return this->Frame == time.Frame;
    }
};