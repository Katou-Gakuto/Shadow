#pragma once

// 
class CollisionHandle
{
public:
    // 
    CollisionHandle();

    // 
    ~CollisionHandle();

    // ƒRƒs[‹Ö~
    CollisionHandle(const CollisionHandle &) = delete;

    // ƒRƒs[‹Ö~
    CollisionHandle &operator=(const CollisionHandle &) = delete;

    // ƒ€[ƒu‹Ö~
    CollisionHandle(CollisionHandle &&) = delete;

    // ƒ€[ƒu‹Ö~
    CollisionHandle &operator=(CollisionHandle &&) = delete;

    // ”äŠr‰‰Zq
    bool operator==(const CollisionHandle &right);

    // ”äŠr‰‰Zq
    bool operator!=(const CollisionHandle &right);

private:
    unsigned long long mnHandle;
};
