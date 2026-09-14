#include "Player.h"

Player::Create()
{
}


Player::Initialize()
{
}


Player::Finalize()
{
}


Player::EarlyUpdate(KeyState keystate)
{
    PDate.Pos.x
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
    
}


Player::Draw()
{
}