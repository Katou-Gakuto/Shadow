#include "CollisionHandle.h"

// 
CollisionHandle::CollisionHandle() :
    mnHandle(0)
{
}

// 
CollisionHandle::~CollisionHandle()
{
}

// ”äŠr‰‰ŽZŽq
bool CollisionHandle::operator==(const CollisionHandle &right)
{
    // 
    return this->mnHandle == right.mnHandle;
}

// ”äŠr‰‰ŽZŽq
bool CollisionHandle::operator!=(const CollisionHandle &right)
{
    // 
    return this->mnHandle != right.mnHandle;
}