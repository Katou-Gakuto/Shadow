#include "DotWeenEnum.h"
#include "DotWeenData.h"

#include "Master.h"

#include "StateDotWeenBase.h"
#include "StateDotWeen.h"
#include "TimeManager.h"

/*--------------------------*/
/*【バウンドアウトステート】*/
/*--------------------------*/
void StateOutBounce::Update(DOT_WEEN_DATA dotWeenData)
{
    float ratio = dotWeenData.DotWeenElapsedTime / dotWeenData.DotWeenTotalTime;

 /*   *dotWeenData.ChangeData = ((ratio) * (dotWeenData.EndData - dotWeenData.StartData)) + dotWeenData.StartData;
    return;*/
    if (ratio < (1.0f / 2.75f))
    {
        *dotWeenData.ChangeData = (7.5625f * ratio * ratio * (dotWeenData.EndData - dotWeenData.StartData)) + dotWeenData.StartData;
    }
    else if (ratio < (2.0f / 2.75f))
    {
        ratio -= (1.5f / 2.75f);
        *dotWeenData.ChangeData =  (((7.5625f * ratio * ratio) + 0.75f) * (dotWeenData.EndData - dotWeenData.StartData)) + dotWeenData.StartData;
    }
    else if (ratio < (2.5f / 2.75f))
    {
        ratio -= (2.25f / 2.75f);
        *dotWeenData.ChangeData = (((7.5625f * ratio * ratio) + 0.9375f) * (dotWeenData.EndData - dotWeenData.StartData)) + dotWeenData.StartData;
    }
    else
    {
        ratio -= (2.625f / 2.75f);
        *dotWeenData.ChangeData = (((7.5625f * ratio * ratio) + 0.984375f) * (dotWeenData.EndData - dotWeenData.StartData)) + dotWeenData.StartData;
    }

}

/*

    /// <summary>
    /// DOTween の Ease.OutBounce と同等の計算
    /// t: 現在の時間 (0.0?1.0)
    /// 戻り値: 補間後の値 (0.0?1.0)
    /// </summary>
    public static float OutBounce(float t)
    {
        if (t < (1f / 2.75f))
        {
            return 7.5625f * t * t;
        }
        else if (t < (2f / 2.75f))
        {
            t -= (1.5f / 2.75f);
            return 7.5625f * t * t + 0.75f;
        }
        else if (t < (2.5f / 2.75f))
        {
            t -= (2.25f / 2.75f);
            return 7.5625f * t * t + 0.9375f;
        }
        else
        {
            t -= (2.625f / 2.75f);
            return 7.5625f * t * t + 0.984375f;
        }
    }
*/

/*
#include <iostream>
#include <cmath>
#include <stdexcept>

// イージングの種類
enum class EaseType {
    Linear,
    InQuad,
    OutQuad,
    InOutQuad,
    OutBounce
};

// Ease 計算関数
double Ease(EaseType type, double t) {
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;

    switch (type) {
        case EaseType::Linear:
            return t;

        case EaseType::InQuad:
            return t * t;

        case EaseType::OutQuad:
            return t * (2 - t);

        case EaseType::InOutQuad:
            return (t < 0.5) ? (2 * t * t) : (-1 + (4 - 2 * t) * t);

        case EaseType::OutBounce: {
            // DOTween の OutBounce に近い計算
            const double n1 = 7.5625;
            const double d1 = 2.75;
            if (t < 1 / d1) {
                return n1 * t * t;
            } else if (t < 2 / d1) {
                t -= 1.5 / d1;
                return n1 * t * t + 0.75;
            } else if (t < 2.5 / d1) {
                t -= 2.25 / d1;
                return n1 * t * t + 0.9375;
            } else {
                t -= 2.625 / d1;
                return n1 * t * t + 0.984375;
            }
        }

        default:
            throw std::invalid_argument("Unknown EaseType");
    }
}

int main() {
    // 例: 0?1 の間を 0.1 刻みで計算
    for (double t = 0.0; t <= 1.0; t += 0.1) {
        double value = Ease(EaseType::OutBounce, t);
        std::cout << "t=" << t << " -> " << value << "\n";
    }
    return 0;
}

*/