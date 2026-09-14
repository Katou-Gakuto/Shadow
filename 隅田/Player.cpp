#include "Player.h"

Player::Create()
{
    Pos = OldPos;
}


Player::Initialize()
{
}


Player::Finalize()
{
}


Player::EarlyUpdate(KeyState keystate)
{
    OldPos = Pos;
}


Player::Update()
{
}


Player::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    // Ž©g‚ÌUŒ‚‚Ì“–‚½‚è”»’è‚ª‘ŠŽè‚Ì‘Ì‚Ì“–‚½‚è”»’è‚É‚ ‚½‚Á‚Ä‚¢‚éê‡‚Íˆ—‚ðs‚¤
    if (myCollision->GetCollisionTag() == CollisionTag::CollisionTag_Attack &&
        hitCollision->GetCollisionTag() == CollisionTag::CollisionTag_CharaBody &&
        myCollision->GetMyObject() != hitCollision->GetMyObject())
    {

    }
    return 0;
}


Player::LateUpdate()
{
    if (moveVec.x != 0)
        Pos.x += moveVec.x;
    if (moveVec.y != 0)
        Pos.y += moveVec.y;
}


Player::Draw()
{
    DrawBox(Pos.x; Pos.y, Pos.x + 30, Pos.y + 60, 0xFFFFFF, true);
}