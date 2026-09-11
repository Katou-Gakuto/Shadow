#include "BaseScene.h"

#include "../A_GameObject/GameObjectManager.h"

BaseScene::BaseScene(SceneTag tag) :
    mnST(tag),
    mbCreateFlag(true),
    mpGameObjectManager(nullptr)
{

}

BaseScene::~BaseScene()
{
    if (this->mpGameObjectManager != nullptr)
    {
        delete this->mpGameObjectManager;
        this->mpGameObjectManager = nullptr;
    }
}

int BaseScene::BaseInitalize()
{
    if (this->mbCreateFlag)
    {
        this->mpGameObjectManager = new GameObjectManager;
        this->Create();
        this->mbCreateFlag = false;
    }

    this->Initialize();

    return 0;
}
