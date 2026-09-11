#include "EulerOrder.h"

#ifdef USE_MY_ROTATION

// ç°âÒÇÃÉQÅ[ÉÄÇÃäeé≤âÒì]èá
const EulerOrder globalOrder = EulerOrder::YXZ;

EulerOrder GetEulerOrder(std::string capitalOrder)
{
    EulerOrder temp = EulerOrder::EulerOrder_Null;
    int orderNum = -1;
    const int orderMax = 6;
    std::string orderBox[orderMax] =
    {
        "XYZ",
        "XZY",
        "YXZ",
        "YZX",
        "ZXY",
        "ZYX",
    };

    for (int i = 0; i < orderMax; i++)
    {
        if (orderBox[i] == capitalOrder)
        {
            orderNum = i;
            break;
        }
    }

    switch (orderNum)
    {
    case 0:
        temp = EulerOrder::XYZ;
        break;
    case 1:
        temp = EulerOrder::XZY;
        break;
    case 2:
        temp = EulerOrder::YXZ;
        break;
    case 3:
        temp = EulerOrder::YZX;
        break;
    case 4:
        temp = EulerOrder::ZXY;
        break;
    case 5:
        temp = EulerOrder::ZYX;
        break;
    case -1:
        break;
    default:
        break;
    }

    return temp;
}

#endif // USE_MY_ROTATION