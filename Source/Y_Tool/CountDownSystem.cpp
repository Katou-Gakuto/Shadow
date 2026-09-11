#include "CountDownSystem.h"

void CountDownSystem::Loop()
{
    this->mnCountNow = this->mnTrueSpan;
}

CountDownSystem::CountDownSystem() :
    mnTrueSpan(0),
    mnCountNow(0)
{

}

CountDownSystem::~CountDownSystem()
{

}

int CountDownSystem::Initialize(unsigned long span)
{
    this->mnTrueSpan = span;
    this->Restart();
    return 0;
}

int CountDownSystem::Initialize(unsigned long span, unsigned long startParam)
{
    this->mnTrueSpan = span;
    this->mnCountNow = startParam;
    return 0;
}

int CountDownSystem::Deactivate()
{
    this->mnCountNow = 0;
    this->mnTrueSpan = 0;
    return 0;
}

bool CountDownSystem::Update(bool loop)
{
    if (this->CheckActive() == false)
    {
        return false;
    }
    bool timming0 = false;
    if (loop)
    {
        if (this->mnCountNow == 0)
        {
            timming0 = true;
            this->Loop();
        }
        this->mnCountNow--;
    }
    else
    {
        if (0 == this->mnCountNow)
        {
            timming0 = true;
        }
        else
        {
            this->mnCountNow--;
        }
    }
    return timming0;
}

int CountDownSystem::Restart()
{
    this->mnCountNow = this->mnTrueSpan - 1;
    return 0;
}

int CountDownSystem::Restart(unsigned long startParam)
{
    this->mnCountNow = startParam;
    return 0;
}

bool CountDownSystem::CheckNow() const
{
    return (0 < this->mnTrueSpan) && (this->mnCountNow == 0);
}

bool CountDownSystem::CheckActive() const
{
    return 0 < this->mnTrueSpan;
}

float CountDownSystem::GetRatioProgress(bool zeroStop) const
{
    // カウントダウンが初期化されているなら処理を行う
    if (this->CheckActive())
    {
        // trueのタイミングだったら100%進んでいる
        if (this->mnCountNow == 0)
        {
            return 1.0f;
        }

        // 作業用の値を取得
        long long temp = (long long)(this->mnCountNow) + 1;

        // マイナスに行かない設定の時に、tempがスパン以上かどうかで処理を変える
        if (zeroStop && this->mnTrueSpan <= temp)
        {
            // マイナスに行かないので0.0fを返す
            return 0.0f;
        }
        else
        {
            // 進捗を返す(マイナスに行く可能性もあります)
            return (float)((long long)this->mnTrueSpan - temp) / (float)this->mnTrueSpan;
        }
    }

    // そもそもカウントをしていないので0.0fを返す
    return 0.0f;
}

float CountDownSystem::GetRatioRemaining(bool zeroStop) const
{
    // カウントダウンが初期化されているなら処理を行う
    if (this->CheckActive())
    {
        // trueのタイミングだったら1%も残っていない
        if (this->mnCountNow == 0)
        {
            return 0.0f;
        }

        // 作業用の値を取得
        long long temp = (long long)(this->mnCountNow) + 1;

        // 残り具合を返す
        return (float)temp / (float)this->mnTrueSpan;
    }

    // そもそもカウントをしていないので0.0fを返す
    return 0.0f;
}
