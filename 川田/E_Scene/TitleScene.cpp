#include "TitleScene.h"

#include "BaseSceneManager.h"

#include "../T_Model/Model2D.h"
#include "../D_UI/BaseUIManager.h"

#include "../R_Input/Input.h"
#include "../V_Display/System.h"
#include "../W_Network/NetworkHome.h"
#include "../Z_Except/Resource.h"
#include "../Z_Except/Master.h"

TitleScene::TitleScene() :
    mnCorsorNum(0),
    BaseScene(SceneTag::ST_Title)
{

}

TitleScene::~TitleScene()
{

}

int TitleScene::Create()
{
    Model2D *modelGameStart = new Model2D(TextureHandle::TextureHandle_Title_GameStart, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);
    Model2D *modelNetwork = new Model2D(TextureHandle::TextureHandle_Title_Network, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);
    Model2D *modelQuit = new Model2D(TextureHandle::TextureHandle_Title_Quit, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);

    VECTOR2D startPos(0.0f, 0.0f);

    float buttonSizeX = 400.0f;
    float buttonSizeY = 200.0f;
    float modelSize = 500.0f;

    mdButtonGameStart.SetParam(
        modelGameStart,
        startPos + VECTOR2D(0.0f, 0.0f),
        startPos + VECTOR2D(buttonSizeX, buttonSizeY),
        startPos + VECTOR2D(0.0f + modelSize, 0.0f + modelSize));
    mdButtonNetwork.SetParam(
        modelNetwork,
        startPos + VECTOR2D(0.0f, buttonSizeY + 50.0f),
        startPos + VECTOR2D(buttonSizeX, buttonSizeY + 50.0f + buttonSizeY),
        startPos + VECTOR2D(0.0f + modelSize, buttonSizeY + 50.0f + modelSize));
    mdButtonQuit.SetParam(
        modelQuit,
        startPos + VECTOR2D(0.0f, buttonSizeY + 50.0f + buttonSizeY + 50.0f),
        startPos + VECTOR2D(buttonSizeX, buttonSizeY + 50.0f + buttonSizeY + 50.0f + buttonSizeY),
        startPos + VECTOR2D(0.0f + modelSize, buttonSizeY + 50.0f + buttonSizeY + 50.0f + modelSize));

    // 関数が正常終了したことを返す
    return 0;
}

int TitleScene::Initialize()
{
    mnCorsorNum = 0;
    this->Create();

    return 0;
}

int TitleScene::Finalize()
{
    return 0;
}

int TitleScene::Update()
{
    if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Mouse_LeftCrick))
    {
        if (this->mdButtonGameStart.CheckInCursor())
        {
            // 自身がオフラインの状態であれば処理を行う
            unsigned long myNumber = Master::mpNetworkHome->GetMyNumber();
            if (myNumber == 0)
            {
                // 
                Master::mpSceneManager->ChangeScene(SceneTag::ST_SoloBattle);
            }
            else
            {
                // 
                Master::mpSceneManager->ChangeScene(SceneTag::ST_MultiBattle);

                // time関数で取得した時刻でバトルを始める事を伝える
                // で、バトルが始まる時に個人個人でカウントダウンをしてもらう

            }
        }
        else if (this->mdButtonNetwork.CheckInCursor())
        {
            // ネットワークのシーンに移行する
            Master::mpSceneManager->ChangeScene(SceneTag::ST_Network);
        }
        else if (this->mdButtonQuit.CheckInCursor())
        {
            // ゲームを終了するフラグをオンにする
            Master::QuitFlagOn();
        }
    }
    return 0;
}

int TitleScene::Draw()
{
    // Master::mpSystem->DrawText2D(L"The Stealthy", 0.0f, 0.0f, Material2D(0.5f, 0.5f, 0.5f, 1.0f));
    this->mdButtonGameStart.Draw();
    this->mdButtonNetwork.Draw();
    this->mdButtonQuit.Draw();
    return 0;
}
