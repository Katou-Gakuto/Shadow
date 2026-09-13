#include "NetworkScene.h"
#define _WINSOCKAPI_
#include <string.h>

#include "../E_Scene/BaseSceneManager.h"    // windowsナシ
#include "../R_Input/Input.h"               // windows.h
#include "../T_Model/Model2D.h"             // windowsナシ
#include "../V_Display/System.h"            // windows.h
#include "../W_Network/UDPData.h"           // ネットワーク系だけど無罪。Resource.hみたいなもん
#include "../W_Network/TCPData.h"
#include "../W_Network/OtherPerson.h"
#include "../W_Network/NetworkHome.h"

#include "../Y_Tool/MyFunctions.h"
#include "../Z_Except/Resource.h"
#include "../Z_Except/Master.h"

int NetworkScene::UpdateHostOrGest()
{
    // オンライン状態にする
    Master::mpNetworkHome->NetworkOnline();

    // 自身のプレイヤー識別番号を取得する
    mnMyNumber = Master::mpNetworkHome->GetMyNumber();

    // 既に通信相手がいる場合は処理を行う
    if (mnMyNumber != 0)
    {
        // nキーが長押しされていたら、そのことを全員に通知する
        if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Key_N))
        {
            // 送信する文章を生成する
            std::string string = mnMyNumber + "PがEnterキーを入力しました";

            // データを送信する
            Master::mpNetworkHome->SendGameDataUDP(
                UDPDataType::PDT_H2G_NetworkTest_HitKeyEnter,
                (void *)(string.data()),
                string.size(),
                false);
        }
    }

    // ゲストでなければ処理を行う
    if (mnMyNumber == 0)
    {
        // マウスの左クリックが押されていたら処理を行う
        if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Mouse_LeftCrick))
        {
            if (this->mdButtonHost.CheckInCursor())
            {
                this->mnPhase = ConnectMode::ConnectMode_Host_InputIPv4;
            }
            else if (this->mdButtonGest.CheckInCursor())
            {
                // 接続待ちの状態に移行する
                Master::mpNetworkHome->ConnectWait();

                // 次から新しい奴にする
                this->mnPhase = ConnectMode::ConnectMode_Gest_ConnectWait;
            }
            else if (this->mdButtonBack.CheckInCursor())
            {
                // シーンをタイトルに戻す
                Master::mpSceneManager->ChangeScene(SceneTag::ST_Title);
            }
        }
    }
    else if (mnMyNumber == 1)
    {
        // マウスの左クリックが押されていたら処理を行う
        if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Mouse_LeftCrick))
        {
            if (this->mdButtonHost.CheckInCursor())
            {
                this->mnPhase = ConnectMode::ConnectMode_Host_InputIPv4;
            }
            else if (this->mdButtonBack.CheckInCursor())
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
        }
    }
    else
    {
        // ホストからマルチプレイを始めるよっていうデータを取得し、取得出来たら処理を行う
        auto gameStartDataList = Master::mpNetworkHome->PullGameDataTCP(TCPDataType::PDT_H2G_Network_GameStart);
        if (0 < gameStartDataList.GetSize())
        {
            // データを取得する
            TCPData_GameStart *gameStartData = (TCPData_GameStart *)gameStartDataList.SearchDataPtrIndex(0);

            // 新しいゲームを始める準備を行う
            *(TimeParam *)Master::mpSceneManager->GetCSD(CSDN::CSDN_TimeParam_GameStartTime) = gameStartData->mdStartTime;

            // マルチゲームのシーンに移動する
            Master::mpSceneManager->ChangeScene(ST_MultiBattle);
        }
    }

    return 0;
}

int NetworkScene::UpdateHostInputIPv4()
{
    // マウスの左クリックをしていた場合は処理を行う
    if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Mouse_LeftCrick))
    {
        // 0のところのボタンを押してたら
        if (this->mdButton0.CheckInCursor())
        {
            // 3桁目までの数字を入れようとしている場合なら処理を行う
            if (this->mnAddNumberCount < 3)
            {
                // 文字列に0を足す
                this->msIPv4Str += '0';

                // 文字を追加する
                this->mnAddNumberCount++;
            }
        }
        // 1のところのボタンを押してたら
        if (this->mdButton1.CheckInCursor())
        {
            // 3桁目までの数字を入れようとしている場合なら処理を行う
            if (this->mnAddNumberCount < 3)
            {
                // 文字列に0を足す
                this->msIPv4Str += '1';

                // 文字を追加する
                this->mnAddNumberCount++;
            }
        }
        // 2のところのボタンを押してたら
        if (this->mdButton2.CheckInCursor())
        {
            // 3桁目までの数字を入れようとしている場合なら処理を行う
            if (this->mnAddNumberCount < 3)
            {
                // 文字列に0を足す
                this->msIPv4Str += '2';

                // 文字を追加する
                this->mnAddNumberCount++;
            }
        }
        // 3のところのボタンを押してたら
        if (this->mdButton3.CheckInCursor())
        {
            // 2桁目までの数字を入れようとしている場合なら処理を行う
            if (this->mnAddNumberCount < 3)
            {
                // 文字列に0を足す
                this->msIPv4Str += '3';

                // 文字を追加する
                this->mnAddNumberCount++;
            }
        }
        // 4のところのボタンを押してたら
        if (this->mdButton4.CheckInCursor())
        {
            // 3桁目までの数字を入れようとしている場合なら処理を行う
            if (this->mnAddNumberCount < 3)
            {
                // 文字列に0を足す
                this->msIPv4Str += '4';

                // 文字を追加する
                this->mnAddNumberCount++;
            }
        }
        // 5のところのボタンを押してたら
        if (this->mdButton5.CheckInCursor())
        {
            // 3桁目までの数字を入れようとしている場合なら処理を行う
            if (this->mnAddNumberCount < 3)
            {
                // 文字列に0を足す
                this->msIPv4Str += '5';

                // 文字を追加する
                this->mnAddNumberCount++;
            }
        }
        // 6のところのボタンを押してたら
        if (this->mdButton6.CheckInCursor())
        {
            // 3桁目までの数字を入れようとしている場合なら処理を行う
            if (this->mnAddNumberCount < 3)
            {
                // 文字列に0を足す
                this->msIPv4Str += '6';

                // 文字を追加する
                this->mnAddNumberCount++;
            }
        }
        // 7のところのボタンを押してたら
        if (this->mdButton7.CheckInCursor())
        {
            // 3桁目までの数字を入れようとしている場合なら処理を行う
            if (this->mnAddNumberCount < 3)
            {
                // 文字列に0を足す
                this->msIPv4Str += '7';

                // 文字を追加する
                this->mnAddNumberCount++;
            }
        }
        // 8のところのボタンを押してたら
        if (this->mdButton8.CheckInCursor())
        {
            // 3桁目までの数字を入れようとしている場合なら処理を行う
            if (this->mnAddNumberCount < 3)
            {
                // 文字列に0を足す
                this->msIPv4Str += '8';

                // 文字を追加する
                this->mnAddNumberCount++;
            }
        }
        // 9のところのボタンを押してたら
        if (this->mdButton9.CheckInCursor())
        {
            // 3桁目までの数字を入れようとしている場合なら処理を行う
            if (this->mnAddNumberCount < 3)
            {
                // 文字列に0を足す
                this->msIPv4Str += '9';

                // 文字を追加する
                this->mnAddNumberCount++;
            }
        }
        // .のところのボタンを押してたら
        if (this->mdButtonDot.CheckInCursor())
        {
            // 一つでも数字が入っている場合なら処理を行う
            if (0 < this->mnAddNumberCount)
            {
                // 文字列に0を足す
                this->msIPv4Str += '.';

                // 直近の文字の追加が0ってことで
                this->mnAddNumberCount = 0;

                // ドットを追加した
                this->mnAddDotCount++;
            }
        }
        // ←のところのボタンを押してたら
        if (this->mdButtonBack.CheckInCursor())
        {
            // 何らかの文字が入っていた場合のみ処理を行う
            if (this->msIPv4Str.empty())
            {
                this->mnPhase = ConnectMode::ConnectMode_HostOrGest;
            }
            else
            {
                // 末尾一文字を取得し、stringのコンテナから末尾一文字を削除する
                char last = this->msIPv4Str.back();
                this->msIPv4Str.pop_back();

                // 末尾一文字がDotかどうかで処理を変える
                if (last == '.')
                {
                    // 末尾から数字を数え、'.'になったら回数の確認をやめる
                    int digitCount = 0;
                    for (int i = (int)this->msIPv4Str.size() - 1; i >= 0; --i)
                    {
                        if (std::isdigit((unsigned char)this->msIPv4Str[i]))
                        {
                            digitCount++;
                        }
                        else
                        {
                            break;
                        }
                    }

                    // 数えた桁数を代入する
                    this->mnAddNumberCount = digitCount;

                    // ドットが減ったってことで
                    this->mnAddDotCount--;
                }
                else
                {
                    // 数字を追加した回数を一回減らす
                    this->mnAddNumberCount--;
                }
            }
        }
        // Sendのところのボタンを押してたら
        if (this->mdButtonSend.CheckInCursor())
        {
            // 接続確認を開始する
            Master::mpNetworkHome->ConnectChallenge(this->msIPv4Str);

            // 通信相手との接続を開始する
            this->mnPhase = ConnectMode::ConnectMode_Host_ConnectWait;

            //
            this->mnAddDotCount = 0;
            this->mnAddNumberCount = 0;
            this->msIPv4Str.clear();
        }
    }
    return 0;
}

int NetworkScene::UpdateHostConnectWait()
{
    // 接続確認の進捗状況を取得する
    auto state = Master::mpNetworkHome->GetConnectingState();

    // 接続に成功した場合
    if (state == ConnectingState::ConnectingState_Complate)
    {
        // 接続完了画面に移行する
        this->mnPhase = ConnectMode::ConnectMode_Host_ConnectComplate;
    }
    // 異常終了した場合
    else if (state == ConnectingState::ConnectingState_Error)
    {
        // 接続開始画面に移行する
        this->mnPhase = ConnectMode::ConnectMode_Host_ConnectFailed;
    }
    // まだ処理中の場合
    else
    {
        // 左クリックしてたら処理をする
        if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Mouse_LeftCrick))
        {
            // カーソルがボタンに当たっていたら処理を行う
            if (this->mdButtonBack.CheckInCursor())
            {
                // 接続確認を中断する
                Master::mpNetworkHome->ConnectChallengeCancel();

                // IPアドレス入力のところまで戻る
                this->mnPhase = ConnectMode::ConnectMode_Host_InputIPv4;
            }
        }
    }

    // 関数が正常終了したことを返す
    return 0;
}

int NetworkScene::UpdateHostConnectComplate()
{
    if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Mouse_LeftCrick))
    {
        if (this->mdButtonBack.CheckInCursor())
        {
            this->mnPhase = ConnectMode::ConnectMode_HostOrGest;
        }
    }
    return 0;
}

int NetworkScene::UpdateHostConnectFailed()
{
    if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Mouse_LeftCrick))
    {
        if (this->mdButtonBack.CheckInCursor())
        {
            this->mnPhase = ConnectMode::ConnectMode_HostOrGest;
        }
    }
    return 0;
}

int NetworkScene::UpdateGestConnectWait()
{
    // 接続の進捗を取得
    auto state = Master::mpNetworkHome->GetConnectingState();

    // 接続に成功したら処理を行う
    if (state == ConnectingState::ConnectingState_Complate)
    {
        this->mnPhase = ConnectMode::ConnectMode_Gest_ConnectComplate;
    }
    else if (state == ConnectingState::ConnectingState_Error)
    {
        this->mnPhase = ConnectMode::ConnectMode_Gest_ConnectFailed;
    }
    else if (state == ConnectingState::ConnectingState_Processing)
    {
        if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Mouse_LeftCrick) &&
            this->mdButtonBack.CheckInCursor())
        {
            // 接続確認待ちを解除する
            Master::mpNetworkHome->ConnectWaitCancel();

            // 
            this->mnPhase = ConnectMode::ConnectMode_Gest_ConnectFailed;
        }
    }
    return 0;
}

int NetworkScene::UpdateGestConnectComplate()
{
    if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Mouse_LeftCrick))
    {
        if (this->mdButtonBack.CheckInCursor())
        {
            this->mnPhase = ConnectMode::ConnectMode_HostOrGest;
        }
    }
    return 0;
}

int NetworkScene::UpdateGestConnectFailed()
{
    if (Master::mpInput->CheckInputKeyDown(InputTag::InputTag_Mouse_LeftCrick))
    {
        if (this->mdButtonBack.CheckInCursor())
        {
            this->mnPhase = ConnectMode::ConnectMode_HostOrGest;
        }
    }
    return 0;
}

int NetworkScene::DrawHostOrGest()
{
    // 描画する文字列
    std::wstring drawStr;

    // 作業用変数
    std::string tempStr;

    // 通信相手がいる場合は処理を行う
    if (this->mnMyNumber == 0)
    {
        drawStr =   L"hostかgestかマウスの左クリックで選んでください\n"
                    L"終了する場合は「←」をマウスの左クリックで選んでください";

    }
    else if (this->mnMyNumber == 1)
    {
        // 
        drawStr =   L"貴方はhostです\n"
                    L"さらに招待する場合はhostを終了する場合は←を選んでください";
    }
    else
    {
        // 
        drawStr = L"貴方はgestです\n"
                  L"hostが通信相手を招待し終わるまで待っていてください";

        // 
        // auto dataBox = Master::mpNetworkHome->SearchGameData(PacketDataType::PacketDataType_H2G_NetworkTest_HitKeyEnterString);
    }

    this->mdButtonBack.Draw();
    this->mdButtonGest.Draw();
    this->mdButtonHost.Draw();

    Master::mpSystem->DrawText2D(drawStr, 300.0f, 100.0f, Material2D(1.0f, 1.0f, 1.0f, 1.0f));

    return 0;
}
 
int NetworkScene::DrawHostInputIPv4()
{
    Master::mpSystem->DrawText2D(
        L"招待相手のIPアドレスを入力してください\n"
        L"IPアドレス : " + MyFunctions::UTF8toUTF16(this->msIPv4Str),
        300.0f, 100.0f, Material2D(1.0f, 1.0f, 1.0f, 1.0f));

    this->mdButtonBack.Draw();
    this->mdButton1.Draw();
    this->mdButton2.Draw();
    this->mdButton3.Draw();
    this->mdButton4.Draw();
    this->mdButton5.Draw();
    this->mdButton6.Draw();
    this->mdButton7.Draw();
    this->mdButton8.Draw();
    this->mdButton9.Draw();
    this->mdButton0.Draw();
    this->mdButtonDot.Draw();
    this->mdButtonSend.Draw();

    return 0;
}

int NetworkScene::DrawHostConnectWait()
{
    Master::mpSystem->DrawText2D(
        L"接続を試みています……\n"
        L"接続の確認を中断する場合は「←」を選んでください",
        300.0f, 100.0f, Material2D(1.0f, 1.0f, 1.0f, 1.0f));
    this->mdButtonBack.Draw();
    return 0;
}

int NetworkScene::DrawHostConnectComplate()
{
    Master::mpSystem->DrawText2D(
        L"接続完了が完了しました\n"
        L"「←」を選んでください",
        300.0f, 100.0f, Material2D(1.0f, 1.0f, 1.0f, 1.0f));
    this->mdButtonBack.Draw();

    return 0;
}

int NetworkScene::DrawHostConnectFailed()
{
    Master::mpSystem->DrawText2D(
        L"接続が確認できませんでした\n"
        L"「←」を選んでください",
        300.0f, 100.0f, Material2D(1.0f, 1.0f, 1.0f, 1.0f));
    this->mdButtonBack.Draw();

    return 0;
}

int NetworkScene::DrawGestConnectWait()
{
    std::wstring str = L"貴方のIPアドレス : ";
    str += this->mdMyIPv4.GetIPv4WStr();
    str +=  L"\n招待を待っています……\n"
            L"招待の確認を中断する場合は「←」を選んでください";
    Master::mpSystem->DrawText2D(str, 300.0f, 100.0f, Material2D(1.0f, 1.0f, 1.0f, 1.0f));
    this->mdButtonBack.Draw();
    return 0;
}

int NetworkScene::DrawGestConnectComplate()
{
    Master::mpSystem->DrawText2D(
        L"招待を確認しました\n"
        L"「←」を選んでください",
        300.0f, 100.0f, Material2D(1.0f, 1.0f, 1.0f, 1.0f));
    this->mdButtonBack.Draw();
    return 0;
}

int NetworkScene::DrawGestConnectFailed()
{
    Master::mpSystem->DrawText2D(
        L"招待が確認できませんでした\n"
        L"「←」を選んでください",
        300.0f, 100.0f, Material2D(1.0f, 1.0f, 1.0f, 1.0f));
    this->mdButtonBack.Draw();
    return 0;
}

NetworkScene::NetworkScene() :
    msResult(),
    mnPhase(ConnectMode::ConnectMode_HostOrGest),
    mnAddNumberCount(0),
    mnAddDotCount(0),
    mpNewOtherPerson(nullptr),
    mnSendACK(0),
    mnMyNumber(0),
    BaseScene(SceneTag::ST_Network)
{

}

NetworkScene::~NetworkScene()
{

}

int NetworkScene::Create()
{
    Model2D *modelHost = new Model2D(TextureHandle::TextureHandle_Network_Host, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);
    Model2D *modelGest = new Model2D(TextureHandle::TextureHandle_Network_Gest, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);

    float modelSize = 300.0f;
    float hostGestUp = 400.0f;
    float hostLeft = 300.0f;
    float gestLeft = 600.0f;
    float BottunSize = 200.0f;

    this->mdButtonHost.SetParam(
        modelHost,
        VECTOR2D(hostLeft, hostGestUp),
        VECTOR2D(hostLeft + BottunSize, hostGestUp + BottunSize),
        VECTOR2D(hostLeft + modelSize, hostGestUp + modelSize));
    this->mdButtonGest.SetParam(
        modelGest,
        VECTOR2D(gestLeft, hostGestUp),
        VECTOR2D(gestLeft + BottunSize, hostGestUp + BottunSize),
        VECTOR2D(gestLeft + modelSize, hostGestUp + modelSize));

    Model2D *modelBS = new Model2D(TextureHandle::TextureHandle_Network_Back, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);
    Model2D *model1 = new Model2D(TextureHandle::TextureHandle_Network_Number1, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);
    Model2D *model2 = new Model2D(TextureHandle::TextureHandle_Network_Number2, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);
    Model2D *model3 = new Model2D(TextureHandle::TextureHandle_Network_Number3, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);
    Model2D *model4 = new Model2D(TextureHandle::TextureHandle_Network_Number4, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);
    Model2D *model5 = new Model2D(TextureHandle::TextureHandle_Network_Number5, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);
    Model2D *model6 = new Model2D(TextureHandle::TextureHandle_Network_Number6, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);
    Model2D *model7 = new Model2D(TextureHandle::TextureHandle_Network_Number7, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);
    Model2D *model8 = new Model2D(TextureHandle::TextureHandle_Network_Number8, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);
    Model2D *model9 = new Model2D(TextureHandle::TextureHandle_Network_Number9, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);
    Model2D *model0 = new Model2D(TextureHandle::TextureHandle_Network_Number0, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);
    Model2D *modelDot = new Model2D(TextureHandle::TextureHandle_Network_Dot, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);
    Model2D *modelSend = new Model2D(TextureHandle::TextureHandle_Network_Send, nullptr, nullptr, ScreenNumber::ScreenNumber_2DObject);

    VECTOR2D startPos = VECTOR2D(150.0f, 450.0f);
    float areaOneSize = 50.0f;
    this->mdButtonBack.SetParam(
        modelBS,
        startPos + VECTOR2D(areaOneSize * 0, areaOneSize * 0),
        startPos + VECTOR2D(areaOneSize * 1, areaOneSize * 1),
        startPos + VECTOR2D(areaOneSize * 0 + modelSize, areaOneSize * 0 + modelSize));
    this->mdButton1.SetParam(
        model1,
        startPos + VECTOR2D(areaOneSize * 1, areaOneSize * 0),
        startPos + VECTOR2D(areaOneSize * 2, areaOneSize * 1),
        startPos + VECTOR2D(areaOneSize * 1 + modelSize, areaOneSize * 0 + modelSize));
    this->mdButton2.SetParam(
        model2,
        startPos + VECTOR2D(areaOneSize * 2, areaOneSize * 0),
        startPos + VECTOR2D(areaOneSize * 3, areaOneSize * 1),
        startPos + VECTOR2D(areaOneSize * 2 + modelSize, areaOneSize * 0 + modelSize));
    this->mdButton3.SetParam(
        model3,
        startPos + VECTOR2D(areaOneSize * 3, areaOneSize * 0),
        startPos + VECTOR2D(areaOneSize * 4, areaOneSize * 1),
        startPos + VECTOR2D(areaOneSize * 3 + modelSize, areaOneSize * 0 + modelSize));
    this->mdButton4.SetParam(
        model4,
        startPos + VECTOR2D(areaOneSize * 4, areaOneSize * 0),
        startPos + VECTOR2D(areaOneSize * 5, areaOneSize * 1),
        startPos + VECTOR2D(areaOneSize * 4 + modelSize, areaOneSize * 0 + modelSize));
    this->mdButton5.SetParam(
        model5,
        startPos + VECTOR2D(areaOneSize * 5, areaOneSize * 0),
        startPos + VECTOR2D(areaOneSize * 6, areaOneSize * 1),
        startPos + VECTOR2D(areaOneSize * 5 + modelSize, areaOneSize * 0 + modelSize));
    this->mdButton6.SetParam(
        model6,
        startPos + VECTOR2D(areaOneSize * 6, areaOneSize * 0),
        startPos + VECTOR2D(areaOneSize * 7, areaOneSize * 1),
        startPos + VECTOR2D(areaOneSize * 6 + modelSize, areaOneSize * 0 + modelSize));
    this->mdButton7.SetParam(
        model7,
        startPos + VECTOR2D(areaOneSize * 7, areaOneSize * 0),
        startPos + VECTOR2D(areaOneSize * 8, areaOneSize * 1),
        startPos + VECTOR2D(areaOneSize * 7 + modelSize, areaOneSize * 0 + modelSize));
    this->mdButton8.SetParam(
        model8,
        startPos + VECTOR2D(areaOneSize * 8, areaOneSize * 0),
        startPos + VECTOR2D(areaOneSize * 9, areaOneSize * 1),
        startPos + VECTOR2D(areaOneSize * 8 + modelSize, areaOneSize * 0 + modelSize));
    this->mdButton9.SetParam(
        model9,
        startPos + VECTOR2D(areaOneSize * 9, areaOneSize * 0),
        startPos + VECTOR2D(areaOneSize * 10, areaOneSize * 1),
        startPos + VECTOR2D(areaOneSize * 9 + modelSize, areaOneSize * 0 + modelSize));
    this->mdButton0.SetParam(
        model0,
        startPos + VECTOR2D(areaOneSize * 10, areaOneSize * 0),
        startPos + VECTOR2D(areaOneSize * 11, areaOneSize * 1),
        startPos + VECTOR2D(areaOneSize * 10 + modelSize, areaOneSize * 0 + modelSize));
    this->mdButtonDot.SetParam(
        modelDot,
        startPos + VECTOR2D(areaOneSize * 11, areaOneSize * 0),
        startPos + VECTOR2D(areaOneSize * 12, areaOneSize * 1),
        startPos + VECTOR2D(areaOneSize * 11 + modelSize, areaOneSize * 0 + modelSize));
    this->mdButtonSend.SetParam(
        modelSend,
        startPos + VECTOR2D(areaOneSize * 12, areaOneSize * 0),
        startPos + VECTOR2D(areaOneSize * 13, areaOneSize * 1),
        startPos + VECTOR2D(areaOneSize * 12 + modelSize, areaOneSize * 0 + modelSize));

    return 0;
}

int NetworkScene::Initialize()
{
    this->mdMyIPv4 = Master::mpNetworkHome->GetMyIPv4();
    this->msIPv4Str.clear();
    this->mnPhase = ConnectMode::ConnectMode_HostOrGest;
    this->mnAddNumberCount = 0;
    this->mnAddDotCount = 0;
    return 0;
}

int NetworkScene::Finalize()
{
    return 0;
}

int NetworkScene::Update()
{
    int temp = 0;
    switch (this->mnPhase)
    {
    case ConnectMode::ConnectMode_HostOrGest:
        temp = this->UpdateHostOrGest();
        break;
    case ConnectMode::ConnectMode_Host_InputIPv4:
        temp = this->UpdateHostInputIPv4();
        break;
    case ConnectMode::ConnectMode_Host_ConnectWait:
        temp = this->UpdateHostConnectWait();
        break;
    case ConnectMode::ConnectMode_Host_ConnectComplate:
        temp = this->UpdateHostConnectComplate();
        break;
    case ConnectMode::ConnectMode_Host_ConnectFailed:
        temp = this->UpdateHostConnectFailed();
        break;
    case ConnectMode::ConnectMode_Gest_ConnectWait:
        temp = this->UpdateGestConnectWait();
        break;
    case ConnectMode::ConnectMode_Gest_ConnectComplate:
        temp = this->UpdateGestConnectComplate();
        break;
    case ConnectMode::ConnectMode_Gest_ConnectFailed:
        temp = this->UpdateGestConnectFailed();
        break;
    default:
        break;
    }
    return temp;
}

int NetworkScene::Draw()
{
    if (Master::mpSystem->ChangeShaderMode(ShaderMode::ShaderMode_Index_2DTexture) != 0)
    {
        return 0;
    }

    int temp = 0;
    switch (this->mnPhase)
    {
    case ConnectMode::ConnectMode_HostOrGest:
        temp = this->DrawHostOrGest();
        break;
    case ConnectMode::ConnectMode_Host_InputIPv4:
        temp = this->DrawHostInputIPv4();
        break;
    case ConnectMode::ConnectMode_Host_ConnectWait:
        temp = this->DrawHostConnectWait();
        break;
    case ConnectMode::ConnectMode_Host_ConnectComplate:
        temp = this->DrawHostConnectComplate();
        break;
    case ConnectMode::ConnectMode_Host_ConnectFailed:
        temp = this->DrawHostConnectFailed();
        break;
    case ConnectMode::ConnectMode_Gest_ConnectWait:
        temp = this->DrawGestConnectWait();
        break;
    case ConnectMode::ConnectMode_Gest_ConnectComplate:
        temp = this->DrawGestConnectComplate();
        break;
    case ConnectMode::ConnectMode_Gest_ConnectFailed:
        temp = this->DrawGestConnectFailed();
        break;
    default:
        break;
    }
    return temp;
}

