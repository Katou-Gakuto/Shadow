#pragma once

struct BehaviorAccessData
{
    unsigned long mnNodeNumber;         // 上層のアクセス番号
    unsigned long mnMonoNumber;         // 下層のアクセス番号
};

BehaviorAccessData GetBehaviorAccessDataZero();
BehaviorAccessData GetBehaviorAccessData(unsigned long node, unsigned long mono);

bool operator ==(const BehaviorAccessData &lData, const BehaviorAccessData &rData);
bool operator !=(const BehaviorAccessData &lData, const BehaviorAccessData &rData);
