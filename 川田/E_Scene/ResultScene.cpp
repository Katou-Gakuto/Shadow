#include "ResultScene.h"

#include "../E_Scene/BaseSceneManager.h"
#include "../R_Input/Input.h"
#include "../V_Display/System.h"
#include "../W_Network/NetworkHome.h"
#include "../W_Network/TCPData.h"

#include "../Z_Except/Master.h"

int ResultScene::UpdateSolo()
{
    // プレイヤーが勝ったかどうか
    if (this->mbPlayerWin)
    {
        // あなたの勝ち！
    }
    else
    {
        // あなたの負け……
    }

    // 再戦する
    if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Key_Enter))
    {
        Master::mpSceneManager->ChangeScene(SceneTag::ST_SoloBattle);
    }
    // 再選しない
    if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Key_BackSpace))
    {
        Master::mpSceneManager->ChangeScene(SceneTag::ST_Title);
    }

    // 関数が正常終了したことを返す
    return 0;
}

int ResultScene::UpdateMulti()
{
    // 自分のプレイヤー識別番号が1なら、自分はホストということ
    bool myHost = Master::mpNetworkHome->GetMyNumber() == 1;

    // 1P(ホスト)の勝ちか、2P(ゲスト)の勝ちかを判断する
    if (myHost == this->mbPlayerWin)
    {
        // ホストが勝ったので……
    }
    else
    {
        // ゲストが勝ったので……
    }

    if (myHost)
    {
        // 勝敗結果と再戦の意思の有無
        if (this->mnPhase == ResultScenePhase::ResultScenePhase_Multi_Result)
        {
            // 再戦する
            if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Key_Enter))
            {
                // 再戦待ち
                this->mnPhase = ResultScenePhase::ResultScenePhase_Multi_Wait_Battle;
            }
            // 再選しない
            if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Key_BackSpace))
            {
                // 再戦しない意思を伝える
                TCPData_BattleRetry data;
                data.mbRetry = false;
                Master::mpNetworkHome->SendGameDataTCP(TCPDataType::PDT_ITR_Network_BattleRetry, &data, sizeof(data));

                // ネットルームに移動する
                Master::mpSceneManager->ChangeScene(ST_Network);
            }
        }
        // 通信相手も再戦希望かを待つ
        else
        {
            // 再戦希望のデータが届いたら、ゲームスタートのデータを送信する
            auto retryDataList = Master::mpNetworkHome->PullGameDataTCP(TCPDataType::PDT_ITR_Network_BattleRetry);
            if (0 < retryDataList.GetSize())
            {
                TCPData_BattleRetry *retryData = (TCPData_BattleRetry *)retryDataList.SearchDataPtrIndex(0);
                if (retryData->mbRetry == true)
                {
                    // 新しいゲームを始める準備を行う
                    TCPData_GameStart gameStartData;
                    TimeParam nowTime;
                    TimeParam startTime;

                    // ミリ秒単位で現在時刻(UTCを取得)
                    nowTime.Initialize();

                    // 計算に使用する時間差を取得
                    TimeDiff diff;

                    // 現在時刻から5秒後に試合を始めることを設定する
                    diff.mnSec = 5;
                    startTime.Initialize(nowTime, diff);

                    // 試合開始のタイミングを設定し、送信する
                    gameStartData.mdStartTime = startTime;
                    Master::mpNetworkHome->SendGameDataTCP(TCPDataType::PDT_H2G_Network_GameStart, &gameStartData, sizeof(gameStartData));

                    // 試合開始のタイミングを自分でも持っておく
                    *(TimeParam *)Master::mpSceneManager->GetCSD(CSDN::CSDN_TimeParam_GameStartTime) = startTime;

                    // 通信対戦のシーンに移動する
                    Master::mpSceneManager->ChangeScene(ST_MultiBattle);
                }
                else
                {
                    // 自分もネットルームに移動する
                    Master::mpSceneManager->ChangeScene(ST_Network);
                }
            }
        }
    }
    // ゲスト
    else
    {
        // 勝敗結果と再戦の意思の有無
        if (this->mnPhase == ResultScenePhase::ResultScenePhase_Multi_Result)
        {
            // 送信するデータ
            TCPData_BattleRetry data;

            // 再戦する
            if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Key_Enter))
            {
                // 再戦したい意思を伝える
                data.mbRetry = true;
                Master::mpNetworkHome->SendGameDataTCP(TCPDataType::PDT_ITR_Network_BattleRetry, &data, sizeof(data));

                // 再戦待ち
                this->mnPhase = ResultScenePhase::ResultScenePhase_Multi_Wait_Battle;
            }
            // 再選しない
            if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Key_BackSpace))
            {
                // 再戦しない意思を伝える
                data.mbRetry = false;
                Master::mpNetworkHome->SendGameDataTCP(TCPDataType::PDT_ITR_Network_BattleRetry, &data, sizeof(data));

                // ネットルームに移動する
                Master::mpSceneManager->ChangeScene(ST_Network);
            }
        }
        // 通信相手も再戦希望かを待つ
        else
        {
            // ゲームを始めるというデータが届いたら、新しいゲームを始める準備を行う
            auto retryDataList = Master::mpNetworkHome->PullGameDataTCP(TCPDataType::PDT_H2G_Network_GameStart);
            if (0 < retryDataList.GetSize())
            {
                // データを取得する
                TCPData_GameStart *gameStartData = (TCPData_GameStart *)retryDataList.SearchDataPtrIndex(0);

                // 新しいゲームを始める準備を行う
                *(TimeParam *)Master::mpSceneManager->GetCSD(CSDN::CSDN_TimeParam_GameStartTime) = gameStartData->mdStartTime;

                // マルチゲームのシーンに移動する
                Master::mpSceneManager->ChangeScene(ST_MultiBattle);
            }

            // ゲームを終了するというデータが届いたら、ネットルームに移動する
            auto endDataList = Master::mpNetworkHome->PullGameDataTCP(TCPDataType::PDT_ITR_Network_BattleRetry);
            if (0 < endDataList.GetSize())
            {
                // 自分もネットルームに移動する
                Master::mpSceneManager->ChangeScene(ST_Network);
            }
        }
    }

    // 関数が正常終了したことを返す
    return 0;
}

int ResultScene::DrawSolo(unsigned long myNumber)
{
    // プレイヤーが勝ったかどうか
    if (this->mbPlayerWin)
    {
        // あなたの勝ち！
        Master::mpSystem->DrawText2D(L"YOU WIN!\n\nRetry : Enter\nTitle : BackSpace\n", 50, 50, Material2D(1.0f, 1.0f, 1.0f, 1.0f));
    }
    else
    {
        // あなたの負け……
        Master::mpSystem->DrawText2D(L"YOU LOSE……\n\nRetry : Enter\nTitle : BackSpace\n", 50, 50, Material2D(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // 関数が正常終了したことを返す
    return 0;
}

int ResultScene::DrawMulti(unsigned long myNumber)
{
    // 
    std::wstring str;

    str += myNumber;
    str += L"P WIN!\n\nRetry→Enter\nTitle→BackSpace\n";

    // プレイヤーが勝ったかどうか
    if (this->mbPlayerWin)
    {
        // あなたの勝ち！
        Master::mpSystem->DrawText2D(str, 50, 50, Material2D(1.0f, 1.0f, 1.0f, 1.0f));
    }
    else
    {
        // あなたの負け……
        Master::mpSystem->DrawText2D(str, 50, 50, Material2D(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // 関数が正常終了したことを返す
    return 0;
}

ResultScene::ResultScene() :
    mnPhase(ResultScenePhase::ResultScenePhase_Solo_Result),
    mbPlayerWin(false),
    BaseScene(SceneTag::ST_Result)
{
}

ResultScene::~ResultScene()
{
}

int ResultScene::Create()
{
    // UIを作る


    return 0;
}

int ResultScene::Initialize()
{
    // 初期化(することねぇなぁ)

    // 前回のシーンがどのシーンだったかで、処理を変える
    SceneTag prevScene = *(SceneTag *)Master::mpSceneManager->GetCSD(CSDN::CSDN_SceneTag_BattleMode);
    if (prevScene == SceneTag::ST_SoloBattle)
    {
        this->mnPhase = ResultScenePhase_Solo_Result;
    }
    else if (prevScene == SceneTag::ST_MultiBattle)
    {
        this->mnPhase = ResultScenePhase_Multi_Result;
    }

    // 勝敗を覚えておく
    this->mbPlayerWin = *(bool *)Master::mpSceneManager->GetCSD(CSDN::CSDN_bool_PlayerWin);

    // 関数が正常終了したことを返す
    return 0;
}

int ResultScene::Finalize()
{


    return 0;
}

int ResultScene::Update()
{
    int temp = 0; 
    if (this->mnPhase == ResultScenePhase::ResultScenePhase_Solo_Result)
    {
        temp = this->UpdateSolo();
    }
    else 
    {
        temp = this->UpdateMulti();
    }
    return temp;
}

int ResultScene::Draw()
{
    int temp = 0;
    unsigned long myNumber = Master::mpNetworkHome->GetMyNumber();
    if (this->mnPhase == ResultScenePhase::ResultScenePhase_Solo_Result)
    {
        temp = this->DrawSolo(myNumber);
    }
    else
    {
        temp = this->DrawMulti(myNumber);
    }
    return temp;
}

void ResultScene::SetResult(bool playerWin, SceneTag prevScene)
{
    this->mbPlayerWin = playerWin;
    if (prevScene == SceneTag::ST_SoloBattle)
    {
        this->mnPhase = ResultScenePhase::ResultScenePhase_Solo_Result;
    }
    else if (prevScene == SceneTag::ST_MultiBattle)
    {
        this->mnPhase = ResultScenePhase::ResultScenePhase_Multi_Result;
    }
}
