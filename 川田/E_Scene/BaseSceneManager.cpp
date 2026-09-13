#include "BaseSceneManager.h"

#include "BaseScene.h"
#include "TitleScene.h"
#include "ResultScene.h"
#include "NetworkScene.h"
#include "SoloBattleScene.h"
#include "MultiBattleScene.h"

#include "../Y_Tool/MyTimer.h"

BaseScene *BaseSceneManager::SearchSceneTag(SceneTag tag)
{
    BaseScene *temp = nullptr;
    for (unsigned char i = 0; i < SceneTag::ST_Max; i++)
    {
        if (this->mpSceneBox[i] != nullptr &&
            this->mpSceneBox[i]->GetTag() == tag)
        {
            temp = this->mpSceneBox[i];
            break;
        }
    }
    return temp;
}

BaseSceneManager::BaseSceneManager() :
    mpSceneBox(),
    mnSceneOld(SceneTag::ST_Max),
    mnSceneNow(SceneTag::ST_Max),
    mnChangeScene(SceneTag::ST_Title),
    mnResreveScene(SceneTag::ST_Max),
    mbFlag()
{
    for (unsigned char i = 0; i < SceneTag::ST_Max; i++)
    {
        this->mpSceneBox[i] = nullptr;
    }
}

BaseSceneManager::~BaseSceneManager()
{
    for (unsigned char i = 0; i < SceneTag::ST_Max; i++)
    {
        if (this->mpSceneBox[i] != nullptr)
        {
            delete this->mpSceneBox[i];
            this->mpSceneBox[i] = nullptr;
        }
    }

    // CSDの削除
    delete (SceneTag *)this->mlUniqueDataList.GetIsolateToNum(CSDN::CSDN_SceneTag_BattleMode);
    delete (bool *)this->mlUniqueDataList.GetIsolateToNum(CSDN::CSDN_bool_PlayerWin);
    delete (TimeParam *)this->mlUniqueDataList.GetIsolateToNum(CSDN::CSDN_TimeParam_GameStartTime);
    delete (unsigned long *)this->mlUniqueDataList.GetIsolateToNum(CSDN::CSDN_unsignedLong_OtherPlayerNum);
}

int BaseSceneManager::Create()
{
    if (this->mbFlag.GetFlag())
    {
        this->mpSceneBox[SceneTag::ST_Title] = new TitleScene();
        this->mpSceneBox[SceneTag::ST_Network] = new NetworkScene();
        this->mpSceneBox[SceneTag::ST_SoloBattle] = new SoloBattleScene();
        this->mpSceneBox[SceneTag::ST_MultiBattle] = new MultiBattleScene();
        this->mpSceneBox[SceneTag::ST_Result] = new ResultScene();

        // シーンで共有するデータ
        this->mlUniqueDataList.Add(new SceneTag, CSDN::CSDN_SceneTag_BattleMode);
        this->mlUniqueDataList.Add(new bool, CSDN::CSDN_bool_PlayerWin);
        this->mlUniqueDataList.Add(new TimeParam, CSDN::CSDN_TimeParam_GameStartTime);
        this->mlUniqueDataList.Add(new unsigned long, CSDN::CSDN_unsignedLong_OtherPlayerNum);


        this->mbFlag.CreateComplate();
    }

    return 0;
}

int BaseSceneManager::Initialize()
{
    this->Create();
    return 0;
}

int BaseSceneManager::Finalize()
{
    return 0;
}

int BaseSceneManager::Update()
{
    // シーンを変えるかを記憶しておくフラグ
    bool sceneChangeFlag = this->mnSceneNow != this->mnChangeScene;

    // シーンを変えるなら前回のシーンと現在のシーンを更新する
    if (sceneChangeFlag)
    {
        this->mnSceneOld = this->mnSceneNow;
        this->mnSceneNow = this->mnChangeScene;
    }

    // 前回のシーンと今回のシーンを取得する
    BaseScene *sceneOld = this->SearchSceneTag(this->mnSceneOld);
    BaseScene *sceneNow = this->SearchSceneTag(this->mnSceneNow);

    // 今回のシーンがnullptrでないなら処理をおコアぬ
    if (sceneNow != nullptr)
    {
        // シーンが変わったタイミングなら今回のシーンにInitializeをかける
        if (sceneChangeFlag)
        {
            sceneNow->BaseInitalize();
        }

        // 今回のシーンに更新をかける
        sceneNow->Update();
    }

    // シーンが変わったタイミングかつ、前回のシーンがnullptrでないなら前回のシーンにFinalizeをかける
    if (sceneChangeFlag && sceneOld != nullptr)
    {
        // シーンを変えたことを書いておく
        this->mnChangeScene = this->mnSceneNow;

        // 現在のシーンもnullptrではなく、さらにsceneOldとも違っていた場合
        if (sceneNow != nullptr && sceneNow!= sceneOld)
        {
            sceneOld->Finalize();
        }
    }

    // 処理の結果を返す
    return 0;
}

int BaseSceneManager::Draw()
{
    BaseScene *sceneNow = this->SearchSceneTag(this->mnSceneNow);
    if (sceneNow != nullptr)
    {
        sceneNow->Draw();
    }

    return 0;
}

BaseScene *BaseSceneManager::SearchSceneNow()
{
    BaseScene *temp = nullptr;
    for (unsigned char i = 0; i < SceneTag::ST_Max; i++)
    {
        if (this->mpSceneBox[i]->GetTag() == this->mnSceneNow)
        {
            temp = this->mpSceneBox[i];
            break;
        }
    }
    return temp;
}

void *BaseSceneManager::GetCSD(CSDN number)
{
    return this->mlUniqueDataList.SearchDataNum(number);
}
