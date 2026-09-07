#include "ResourceData.h"

// DIV_GRAPH_DATAのIDカウント初期化
int DIV_GRAPH_DATA::IdCount = 0;

/*--------------------------------*/
/*【上下左右を起点に割合座標取得】*/
/*--------------------------------*/
// 左を起点にした割合(0～1)から横軸の座標を取得
int DisplaySize::Left_RatioWidth(float ratio)
{
    return static_cast<int>(x * ratio);
}

// 上を起点にした割合(0～1)から縦軸の座標を取得
int DisplaySize::Up_RatioHeight(float ratio)
{
    return static_cast<int>(y * ratio);
}

// 右を起点にした割合(0～1)から横軸の座標を取得
int DisplaySize::Right_RatioWidth(float ratio)
{
    return static_cast<int>(x * (1.0f - ratio));
}

// 下を起点にした割合(0～1)から横軸の座標を取得
int DisplaySize::Down_RatioHeight(float ratio)
{
    return static_cast<int>(y * (1.0f - ratio));
}

// 左上を起点にした割合(0～1)から座標を取得
Vector2_Int DisplaySize::LeftUp_Ratio(Vector2 ratio)
{
    Vector2_Int pos;
    pos.x = Left_RatioWidth(ratio.x);
    pos.y = Up_RatioHeight(ratio.y);
    return pos;
}

// 右上を起点にした割合(0～1)から座標を取得
Vector2_Int DisplaySize::RightUp_Ratio(Vector2 ratio)
{
    Vector2_Int pos;
    pos.x = Right_RatioWidth(ratio.x);
    pos.y = Up_RatioHeight(ratio.y);
    return pos;
}

// 左下を起点にした割合(0～1)から座標を取得
Vector2_Int DisplaySize::LeftDown_Ratio(Vector2 ratio)
{
    Vector2_Int pos;
    pos.x = Left_RatioWidth(ratio.x);
    pos.y = Down_RatioHeight(ratio.y);
    return pos;
}

// 右下を起点にした割合(0～1)から座標を取得
Vector2_Int DisplaySize::RightDown_Ratio(Vector2 ratio)
{
    Vector2_Int pos;
    pos.x = Right_RatioWidth(ratio.x);
    pos.y = Down_RatioHeight(ratio.y);
    return pos;
}


/*----------------------------------*/
/*【上下左右から離れた割合座標取得】*/
/*----------------------------------*/
// 左を起点にした離れた割合(0～1)から横軸の座標を取得
int DisplaySize::Left_SeparateRatioWidth(float startRatio, float endRatio, bool plusFlag)
{
    if (plusFlag)
    {
        endRatio += startRatio;
    }
    return static_cast<int>((x * endRatio)) - static_cast<int>(x * startRatio);
}

// 上を起点にした離れた割合(0～1)から縦軸の座標を取得
int DisplaySize::Up_SeparateRatioHeight(float startRatio, float endRatio, bool plusFlag)
{
    if (plusFlag)
    {
        endRatio += startRatio;
    }
    return static_cast<int>((y * endRatio)) - static_cast<int>(y * startRatio);
}

// 右を起点にした離れた割合(0～1)から横軸の座標を取得
int DisplaySize::Right_SeparateRatioWidth(float startRatio, float endRatio, bool plusFlag)
{
    if (plusFlag)
    {
        endRatio += startRatio;
    }
    return static_cast<int>((x * (1.0f - startRatio))) - static_cast<int>(x * (1.0f - endRatio));
}

// 下を起点にした離れた割合(0～1)から縦軸の座標を取得
int DisplaySize::Down_SeparateRatioHeight(float startRatio, float endRatio, bool plusFlag)
{
    if (plusFlag)
    {
        endRatio += startRatio;
    }
    return static_cast<int>((y * (1.0f - startRatio))) - static_cast<int>(y * (1.0f - endRatio));
}

// 左上を起点にした離れた割合(0～1)から座標を取得
Vector2_Int DisplaySize::LeftUp_SeparateRatio(Vector2 startRatio, Vector2 endRatio, bool plusFlag)
{
    Vector2_Int pos;
    pos.x = Left_SeparateRatioWidth(startRatio.x, endRatio.x, plusFlag);
    pos.y = Up_SeparateRatioHeight(startRatio.y, endRatio.y, plusFlag);
    return pos;
}

// 右上を起点にした離れた割合(0～1)から座標を取得
Vector2_Int DisplaySize::RightUp_SeparateRatio(Vector2 startRatio, Vector2 endRatio, bool plusFlag)
{
    Vector2_Int pos;
    pos.x = Right_SeparateRatioWidth(startRatio.x, endRatio.x, plusFlag);
    pos.y = Up_SeparateRatioHeight(startRatio.y, endRatio.y, plusFlag);
    return pos;
}

// 左下を起点にした離れた割合(0～1)から座標を取得
Vector2_Int DisplaySize::LeftDown_SeparateRatio(Vector2 startRatio, Vector2 endRatio, bool plusFlag)
{
    Vector2_Int pos;
    pos.x = Left_SeparateRatioWidth(startRatio.x, endRatio.x, plusFlag);
    pos.y = Down_SeparateRatioHeight(startRatio.y, endRatio.y, plusFlag);
    return pos;
}

// 右下を起点にした離れた割合(0～1)から座標を取得
Vector2_Int DisplaySize::RightDown_SeparateRatio(Vector2 startRatio, Vector2 endRatio, bool plusFlag)
{
    Vector2_Int pos;
    pos.x = Right_SeparateRatioWidth(startRatio.x, endRatio.x, plusFlag);
    pos.y = Down_SeparateRatioHeight(startRatio.y, endRatio.y, plusFlag);
    return pos;
}