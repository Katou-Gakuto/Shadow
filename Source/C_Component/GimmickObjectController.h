
#include "BaseComponent.h"

#include <cstdint>
#include <vector>

#include "../F_PuzzleGimmick/PuzzleGimmickData.h"

#include "../Y_Tool/VECTOR.h"

/*
GameManagerにAddする順番は
Button→GimmickObject
にしてくださいな。
*/

/*
鏡は基本的には普通の壁と同じようにComponentを持たせる
工夫する部分はGameObject::Create()内で生成する当たり判定がただの四角じゃなくて
本来なら四角の当たり判定の部分を、4個のRayにし『鏡面判定のRay + 普通の壁のRay * 3』にするところ
*/

// 
class GimmickObjectController : public BaseComponent
{
public:
    /// TODO: これ中身簡易的に作成したから本来の用途とちがうと思う
    // 
    GimmickObjectController() :BaseComponent(nullptr, ComponentTagAndOrder::CTAO_PlayerController){}

    //
    ~GimmickObjectController() {}

    int Create() override { return 0; }
    int Initialize() override { return 0; }
    int Finalize() override { return 0; }
    int EarlyUpdate() override { return 0; }
    int Update() override { return 0; }
    int HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision) override { return 0; }
    int LateUpdate() override { return 0; }
    int Draw() override { return 0; }

private:
    // 
    std::vector<PuzzleGimmickData> mlGimmickList;
};
