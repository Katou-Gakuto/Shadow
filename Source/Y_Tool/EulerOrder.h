#pragma once

#include "UseDeclaration.h"
#include <string>

#ifdef USE_MY_ROTATION

// オイラー角の回転順を表したenum
enum EulerOrder : unsigned char
{
    XYZ = 0,
    XZY,
    YXZ,
    YZX,
    ZXY,
    ZYX,

    EulerOrder_Null,
};

// 今回のゲームの各軸回転順
extern const EulerOrder globalOrder;

EulerOrder GetEulerOrder(std::string capitalOrder);
#endif // USE_MY_ROTATION