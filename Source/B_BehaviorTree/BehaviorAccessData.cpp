#include "BehaviorAccessData.h"

BehaviorAccessData GetBehaviorAccessDataZero()
{
    BehaviorAccessData data;
    data.mnNodeNumber = 0;
    data.mnMonoNumber = 0;
    return data;
}

BehaviorAccessData GetBehaviorAccessData(unsigned long node, unsigned long mono)
{
    BehaviorAccessData data;
    data.mnNodeNumber = node;
    data.mnMonoNumber = mono;
    return data;
}

bool operator ==(const BehaviorAccessData &lData, const BehaviorAccessData &rData)
{
    return (lData.mnNodeNumber == rData.mnNodeNumber && lData.mnMonoNumber == rData.mnMonoNumber);
}

bool operator !=(const BehaviorAccessData &lData, const BehaviorAccessData &rData)
{
    return !(lData == rData);
}
