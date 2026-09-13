#include "CharacterLife.h"

#include "../A_GameObject/GameObject.h"

#include "../E_Scene/SoloBattleScene.h"
#include "../E_Scene/MultiBattleScene.h"
#include "../E_Scene/BaseScene.h"
#include "../E_Scene/BaseSceneManager.h"

#include "../Z_Except/Master.h"

CharacterLife::CharacterLife(GameObject *myObject) :
    BaseComponent(myObject, ComponentTagAndOrder::CTAO_CharacterLife),
    mnLifeCountDown()
{
    this->mnLifeCountDown.Initialize(3);
}

CharacterLife::~CharacterLife()
{
}

int CharacterLife::Hit()
{
    // 残りライフを一つ減らす
    if (this->mnLifeCountDown.Update(false))
    {
        // 現在のシーンを取得する
        auto soloBattleScene = dynamic_cast<SoloBattleScene *>(Master::mpSceneManager->SearchSceneNow());
        if (soloBattleScene != nullptr)
        {
            // 自身を持っているオブジェクトが負けたことをマネージャーに報告する
            soloBattleScene->SetLoseObject(this->GetMyObject());
        }
        else
        {
            // 現在のシーンを取得する
            auto multiBattleScene = dynamic_cast<MultiBattleScene *>(Master::mpSceneManager->SearchSceneNow());
            if (multiBattleScene != nullptr)
            {
                // 自身を持っているオブジェクトが負けたことをマネージャーに報告する

            }
        }
    }

    // 関数が正常終了したことを返す
    return 0;
}

int CharacterLife::GetLifeCount() const
{
    // 残りライフを返す
    return this->mnLifeCountDown.GetCountNow();
}
