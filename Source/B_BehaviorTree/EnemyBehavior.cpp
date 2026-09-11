#include "EnemyBehavior.h"

#include "../A_GameObject/GameObjectManager.h"
#include "../A_GameObject/Enemy.h"
#include "../A_GameObject/SoundVisualizer.h"
#include "../C_Component/BaseComponentList.h"
#include "../C_Component/CharacterLife.h"
#include "../C_Component/OtherPlayerController.h"
#include "../S_Collision/BaseCollision.h"
#include "../S_Collision/BaseCollisionList.h"
#include "../U_Sound/Audio3D.h"
#include "../Z_Except/Master.h"

Enemy *GetEnemy(GameObject *obj)
{
    return static_cast<Enemy *>(obj);
}

EnemyState_Search::EnemyState_Search(GameObject *myObject) :
    mnSearch2Random(),
    MonoBehavior(myObject)
{

}

EnemyState_Search::~EnemyState_Search()
{

}

int EnemyState_Search::Initialize()
{
    // 自身のオブジェクトをEnemyにキャストしたものを取得する
    auto myEnemy = GetEnemy(this->GetMyObject());
    if (myEnemy != nullptr)
    {

    }

    mnSearch2Random.Initialize(10);
    return 0;
}

int EnemyState_Search::Finalize()
{
    // 自身のオブジェクトをEnemyにキャストしたものを取得する
    auto myEnemy = GetEnemy(this->GetMyObject());
    if (myEnemy != nullptr)
    {
        myEnemy->SetLastState(EnemyMono::EnemyMono_Search);
    }
    return 0;
}

BehaviorAccessData EnemyState_Search::Update(const BehaviorAccessData &myAccessData)
{
    // @Debug
    // エネミーを動かしたくない時に使ってください。
    // return myAccessData;

    auto myEnemy = GetEnemy(this->GetMyObject());
    if (myEnemy == nullptr)
    {
        return myAccessData;
    }

    // 攻撃後の避難中なら特別な処理を行う処理
    if (myEnemy->GetAway2HitCount().Update(false) == false)
    {
        // 適当な方向に変える
        return GetBehaviorAccessData(EnemyStateNode::EnemyStateNode_Normal, EnemyStateTag::EnemyStateTag_Random);
    }

    // 作業用変数
    VECTOR3D currentPos;
    VECTOR3D my2current;
    VECTOR3D myPos = myEnemy->GetPosition();
    VECTOR3D myForword = myEnemy->GetForwordVec();
    VECTOR3D newrVisualPos;
    bool target = false;

    // 攻撃のインターバルのやつ
    myEnemy->GetAttackCount().Update(false);

    // サウンドビジュアライザーの情報を取得する
    auto objectManager = Master::GetGameObjectManagerNow();
    if (objectManager == nullptr)
    {
        return myAccessData;
    }

    auto objectBox = objectManager->SearchGameObjectsTag(GOT_SoundVisualizer);
    for (unsigned long i = 0; i < objectBox.Count(); i++)
    {
        auto visualizer = static_cast<SoundVisualizer *>(objectBox[i]);
        if (visualizer != nullptr)
        {
            // 自分のオブジェクトと一致していたら処理を飛ばす
            if (visualizer->GetEnemyObject() == this->GetMyObject())
            {
                continue;
            }

            // 敵が見つけられないものは処理を飛ばす
            if (visualizer->GetEnemySearch() == false)
            {
                continue;
            }

            // VECTORの計算をする
            currentPos = visualizer->GetPosition();
            my2current = currentPos - myPos;

            // 自身の背中側にサウンドビジュアライザーがある場合は処理を行わない
            float dot = VECTOR3D::Dot(my2current, myForword);
            if (dot <= 0.0f)
            {
                continue;
            }

            // 目標とするビジュアライザーを一度でも見つけたという証拠
            if (target)
            {
                // 一番近いビジュアライザーを探す
                if (my2current.MagnitudeNoSqrt() < (newrVisualPos - myPos).MagnitudeNoSqrt())
                {
                    newrVisualPos = currentPos;
                }
            }
            else
            {
                newrVisualPos = currentPos;
                target = true;
            }
        }
    }

    // 一度でもサウンドビジュアライザーを見つけていたら処理を行う
    if (target)
    {
        // 次の目標地点を設定する
        myEnemy->SetTargetPos(newrVisualPos);

        // 目標地点まで向かう状態に変更する
        return GetBehaviorAccessData(EnemyStateNode::EnemyStateNode_Normal, EnemyStateTag::EnemyStateTag_Target);
    }
    // ビジュアライザーを探した回数を更新し、処理するべきタイミングになったら処理を行う
    else if (this->mnSearch2Random.Update(false))
    {
        // 全然見つからないので、ランダムに移動する状態に変更する
        return GetBehaviorAccessData(EnemyStateNode::EnemyStateNode_Normal, EnemyStateTag::EnemyStateTag_Random);
    }

    // 自分の正面方向を変える
    myEnemy->SetForword(-myForword, 0.0f);
    
    // もう一度探す
    return myAccessData;
}

BehaviorAccessData EnemyState_Search::HitOnCollision(const BehaviorAccessData &myAccessData, BaseCollision *myCollision, BaseCollision *hitCollision)
{
    // 自身の攻撃の当たり判定が相手の体の当たり判定にあたっている場合は処理を行う
    if (myCollision->GetCollisionTag() == CollisionTag::CollisionTag_Attack &&
        hitCollision->GetCollisionTag() == CollisionTag::CollisionTag_CharaBody &&
        myCollision->GetMyObject() != hitCollision->GetMyObject())
    {
        auto life = (CharacterLife *)hitCollision->GetMyObject()->GetBaseComponentList()->SearchComponent(CTAO_CharacterLife)[0];
        if (life != nullptr)
        {
            // 残りライフを減らす
            life->Hit();
        }
    }

    return myAccessData;
}

EnemyState_Target::EnemyState_Target(GameObject *myObject) :
    mbCollisionHit(false),
    MonoBehavior(myObject)
{

}

EnemyState_Target::~EnemyState_Target()
{

}

int EnemyState_Target::Initialize()
{
    // 自身のオブジェクトをEnemyにキャストしたものを取得する
    auto myEnemy = static_cast<Enemy *>(this->GetMyObject());
    if (myEnemy != nullptr)
    {
        EnemyMono lastState = myEnemy->GetLastState();
        if (lastState == EnemyMono::EnemyMono_Idol ||
            lastState == EnemyMono::EnemyMono_Search)
        {
            // 0フレーム目から音が出るように設定する
            myEnemy->GetFootstepsCount().Restart(0);
        }
    }

    mbCollisionHit = false;
    return 0;
}

int EnemyState_Target::Finalize()
{
    // 自身のオブジェクトをEnemyにキャストしたものを取得する
    auto myEnemy = static_cast<Enemy *>(this->GetMyObject());
    if (myEnemy != nullptr)
    {
        myEnemy->SetLastState(EnemyMono::EnemyMono_Target);
    }
    return 0;
}

BehaviorAccessData EnemyState_Target::Update(const BehaviorAccessData &myAccessData)
{
    // 自身のオブジェクトをEnemyにキャストしたものを取得する
    auto myEnemy = static_cast<Enemy *>(this->GetMyObject());
    if (myEnemy == nullptr)
    {
        return myAccessData;
    }

    // 攻撃のカウントダウンのやつ
    myEnemy->GetAttackCount().Update(false);

    // 足音のカウントを進めて、サウンドビジュアライザーを生成していたらif文の中に入る
    SoundVisualizer *newVisual = myEnemy->CreateFootstepsToCount();
    if (newVisual != nullptr)
    {
        // 5%の確率で自分の足音を参照して移動する状態に切り替える
        int random = rand() % 100;
        if (random <= 5)
        {
            myEnemy->SetMyVisual(newVisual);

            // 状態を切り替える
            return GetBehaviorAccessData(EnemyNode::EnemyNode_Normal, EnemyMono::EnemyMono_Random);
        }
    }

    // 目標地点に到着した場合は処理を行う
    if (myEnemy->GetPosition() == myEnemy->GetTargetPos())
    {
        // もう攻撃ができる状態なら攻撃を行う
        if (myEnemy->GetAttackCount().CheckNow())
        {
            // 攻撃を行う
            myEnemy->Attack();

            // 避難を開始する
            myEnemy->GetAway2HitCount().Restart();
        }

        // 状態を切り替える
        return GetBehaviorAccessData(EnemyNode::EnemyNode_Normal, EnemyMono::EnemyMono_Search);
    }

    // エネミーの現在地点からターゲットまでの距離を取得
    VECTOR3D now2target = myEnemy->GetTargetPos() - myEnemy->GetPosition();
    if (now2target.MagnitudeNoSqrt() < myEnemy->GetSpeed() * myEnemy->GetSpeed())
    {
        myEnemy->SetNextPos(myEnemy->GetTargetPos());
        myEnemy->SetCurrentDirection(now2target.Normalize());

        // 体の当たり判定を取得する
        BaseCollision *body = myEnemy->GetBaseCollisionList()->SearchCollisionNum(0);

        // 体の当たり判定の移動量も設定しておく
        body->SetMoveVec(&now2target);

    }
    else
    {
        VECTOR3D now2targetNorm = now2target.Normalize();
        VECTOR3D moveVec = now2targetNorm * myEnemy->GetSpeed();
        myEnemy->SetMoveVec3D(moveVec);
        myEnemy->SetCurrentDirection(now2targetNorm);

        // 体の当たり判定を取得する
        BaseCollision *body = myEnemy->GetBaseCollisionList()->SearchCollisionNum(0);

        // 体の当たり判定の移動量も設定しておく
        body->SetMoveVec(&moveVec);
    }

    // この状態のまま当たり判定の計算に入る
    return myAccessData;
}

BehaviorAccessData EnemyState_Target::HitOnCollision(const BehaviorAccessData &myAccessData, BaseCollision *myCollision, BaseCollision *hitCollision)
{
    // 当たり判定の計算中に行う処理は特にない。
    if (hitCollision->GetCollisionTag() == CollisionTag::CollisionTag_Wall &&
        hitCollision->GetCollisionNorm() == CollisionNorm::CollisionNorm_Out)
    {
        mbCollisionHit = true;
    }
    // 自身の攻撃の当たり判定が相手の体の当たり判定にあたっている場合は処理を行う
    if (myCollision->GetCollisionTag() == CollisionTag::CollisionTag_Attack &&
        hitCollision->GetCollisionTag() == CollisionTag::CollisionTag_CharaBody &&
        myCollision->GetMyObject() != hitCollision->GetMyObject())
    {
        auto life = (CharacterLife *)hitCollision->GetMyObject()->GetBaseComponentList()->SearchComponent(CTAO_CharacterLife)[0];
        if (life != nullptr)
        {
            // 残りライフを減らす
            life->Hit();
        }
    }

    return myAccessData;
}

BehaviorAccessData EnemyState_Target::LateUpdate(const BehaviorAccessData &myAccessData)
{
    auto myEnemy = GetEnemy(this->GetMyObject());
    if (myEnemy == nullptr)
    {
        return myAccessData;
    }
    myEnemy->UpdateEmitterPos();

    // 当たり判定の計算を行ったのに進め無くなった場合は自分の足音を参照して移動する状態に切り替える
    if (mbCollisionHit &&
        myEnemy->GetMoveVec3D() == VECTOR3D::GetZero())
    {
        return GetBehaviorAccessData(EnemyNode::EnemyNode_Normal, EnemyMono::EnemyMono_Random);
    }

    // このまま次のフレームの処理に入る
    return myAccessData;
}

EnemyState_Random::EnemyState_Random(GameObject *myObject) :
    MonoBehavior(myObject)
{

}

EnemyState_Random::~EnemyState_Random()
{

}

int EnemyState_Random::Initialize()
{
    // 自身のオブジェクトをEnemyにキャストしたものを取得する
    auto myEnemy = static_cast<Enemy *>(this->GetMyObject());
    if (myEnemy != nullptr)
    {
        // 着目サウンドラインを設定する(delete後のオブジェクト見て怒られそう。嫌だなぁ)
        auto myVisual = myEnemy->GetMyVisual();
        if (myVisual != nullptr)
        {
            // アクティブのインデックスの最大数を取得
            unsigned long randomIndex = rand() % myVisual->GetLineCountNow();
            unsigned long index = 0;
            if (myVisual->GetActiveLineIndexToAllIndex(randomIndex, index))
            {
                myEnemy->SetNowIndex(index);
                VECTOR3D direction;
                myVisual->GetActiveDirection(direction, index);
                direction = direction.SetMagnitude(5.0f);
                myEnemy->SetCurrentDirection(direction);
            }
        }
        else
        {
            myEnemy->SetCurrentDirection(VECTOR3D::GetZero());
        }

        EnemyMono lastState = myEnemy->GetLastState();
        if (lastState == EnemyMono::EnemyMono_Idol ||
            lastState == EnemyMono::EnemyMono_Search)
        {
            // 0フレーム目から音が出るように設定する
            myEnemy->GetFootstepsCount().Restart(0);
        }
    }
    return 0;
}

int EnemyState_Random::Finalize()
{
    // 自身のオブジェクトをEnemyにキャストしたものを取得する
    auto myEnemy = static_cast<Enemy *>(this->GetMyObject());
    if (myEnemy != nullptr)
    {
        myEnemy->SetLastState(EnemyMono::EnemyMono_Random);
    }
    return 0;
}

BehaviorAccessData EnemyState_Random::Update(const BehaviorAccessData &myAccessData)
{
    auto myEnemy = static_cast<Enemy *>(this->GetMyObject());
    if (myEnemy == nullptr)
    {
        return myAccessData;
    }
    // 攻撃のカウントダウンのやつ
    myEnemy->GetAttackCount().Update(false);
    SoundVisualizer *newVisual = nullptr;

    // 自身のビジュアライザーがない場合とそうでない場合で処理を変える
    if (myEnemy->GetMyVisual() == nullptr)
    {
        while (newVisual == nullptr)
        {
            newVisual = myEnemy->CreateFootstepsToCount();
        }
    }
    else
    {
        newVisual = myEnemy->CreateFootstepsToCount();
    }

    // 新しいビジュアライザーがある場合は、処理を行う
    if (newVisual != nullptr)
    {
        myEnemy->SetMyVisual(newVisual);
    }

    // 
    if (myEnemy->GetCurrentDirection() == VECTOR3D::GetZero())
    {
        // 着目サウンドラインを設定する(delete後のオブジェクト見て怒られそう。嫌だなぁ)
        auto myVisual = myEnemy->GetMyVisual();
        if (myVisual != nullptr)
        {
            // アクティブのインデックスの最大数を取得
            unsigned long randomIndex = rand() % myVisual->GetLineCountNow();
            unsigned long index = 0;
            if (myVisual->GetActiveLineIndexToAllIndex(randomIndex, index))
            {
                myEnemy->SetNowIndex(index);
                VECTOR3D direction;
                myVisual->GetActiveDirection(direction, index);
                direction = direction.SetMagnitude(5.0f);
                myEnemy->SetCurrentDirection(direction);
            }
        }
    }

    // 移動ベクトルを取得する
    VECTOR3D moveVec = myEnemy->GetCurrentDirection();

    // 移動ベクトルをこれに設定する
    myEnemy->SetMoveVec3D(moveVec);

    // 体の当たり判定を取得する
    BaseCollision *body = myEnemy->GetBaseCollisionList()->SearchCollisionNum(0);

    // 体の当たり判定の移動量も設定しておく
    body->SetMoveVec(&moveVec);

    // 正面方向も設定する
    myEnemy->SetForword(myEnemy->GetCurrentDirection().Normalize(), 0.0f);

    // このままで大丈夫です
    return myAccessData;
}

BehaviorAccessData EnemyState_Random::HitOnCollision(const BehaviorAccessData &myAccessData, BaseCollision *myCollision, BaseCollision *hitCollision)
{
    // 自身の攻撃の当たり判定が相手の体の当たり判定にあたっている場合は処理を行う
    if (myCollision->GetCollisionTag() == CollisionTag::CollisionTag_Attack &&
        hitCollision->GetCollisionTag() == CollisionTag::CollisionTag_CharaBody &&
        myCollision->GetMyObject() != hitCollision->GetMyObject())
    {
        auto life = (CharacterLife *)hitCollision->GetMyObject()->GetBaseComponentList()->SearchComponent(CTAO_CharacterLife)[0];
        if (life != nullptr)
        {
            // 残りライフを減らす
            life->Hit();
        }
    }

    return myAccessData;
}

BehaviorAccessData EnemyState_Random::LateUpdate(const BehaviorAccessData &myAccessData)
{
    auto myEnemy = static_cast<Enemy *>(this->GetMyObject());
    if (myEnemy == nullptr)
    {
        return myAccessData;
    }
    myEnemy->UpdateEmitterPos();

    auto myVisual = myEnemy->GetMyVisual();
    if (myVisual != nullptr)
    {
        // 当たり判定の計算によって着目サウンドラインが無効化されていたら処理を行う
        if (myVisual->CheckActiveIndex(myEnemy->GetNowIndex()) == false ||
            myEnemy->GetMoveVec3D() == VECTOR3D::GetZero())
        {
            return GetBehaviorAccessData(EnemyNode::EnemyNode_Normal, EnemyMono::EnemyMono_Search);
        }
    }
    return myAccessData;
}


