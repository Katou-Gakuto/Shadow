#pragma once

// 小数点的なカウント方法なので、nowがmaxである瞬間は0を迎えた一瞬だけです。
// 逆に0である期間は0 <= now < 1 なので長めです
class CountDownSystem
{
private:
    unsigned long mnTrueSpan;
    unsigned long mnCountNow;

    void Loop();

public:
    CountDownSystem();
    ~CountDownSystem();

    int Initialize(unsigned long span);
    int Initialize(unsigned long span, unsigned long startParam);
    int Deactivate();
    bool Update(bool loop);
    int Restart();
    int Restart(unsigned long startParam);
    bool CheckNow() const;
    bool CheckActive() const;
    float GetRatioProgress(bool zeroStop) const;                    // カウントの進み具合を取得する
    float GetRatioRemaining(bool zeroStop) const;                   // カウントの残り具合を取得する


    unsigned long GetSpan() const{ return this->mnTrueSpan;}
    unsigned long GetCountNow() const{ return this->mnCountNow;}
};

// ↓ここでスタート              ↓ここでもう一回
// 5, 4, 3, 2, 1, 0, 4, 3, 2, 1, 0, 4, 3, 2, 1, 0, 4, 3, 2, 1, 0, 
//                ↑ここでもう一回　　　　　　  ↑ここでもう一回