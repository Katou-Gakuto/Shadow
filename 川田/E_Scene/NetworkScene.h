#pragma once

#include "BaseScene.h"

#include <string>

#include "../D_UI/QuadrangleButton2D.h"
#include "../W_Network/IPData.h"

enum ConnectMode : unsigned long
{
    ConnectMode_HostOrGest = 0,
    ConnectMode_Host_InputIPv4,
    ConnectMode_Host_ConnectWait,
    ConnectMode_Host_ConnectComplate,
    ConnectMode_Host_ConnectFailed,
    ConnectMode_Gest_ConnectWait,
    ConnectMode_Gest_ConnectComplate,
    ConnectMode_Gest_ConnectFailed,
};

class OtherPerson;

class NetworkScene : public BaseScene
{
private:
    std::string msIPv4Str;
    QuadrangleButton2D mdButtonGest;
    QuadrangleButton2D mdButtonHost;
    QuadrangleButton2D mdButton0;
    QuadrangleButton2D mdButton1;
    QuadrangleButton2D mdButton2;
    QuadrangleButton2D mdButton3;
    QuadrangleButton2D mdButton4;
    QuadrangleButton2D mdButton5;
    QuadrangleButton2D mdButton6;
    QuadrangleButton2D mdButton7;
    QuadrangleButton2D mdButton8;
    QuadrangleButton2D mdButton9;
    QuadrangleButton2D mdButtonBack;
    QuadrangleButton2D mdButtonDot;
    QuadrangleButton2D mdButtonSend;

    std::string msResult;
    ConnectMode mnPhase;
    int mnAddNumberCount;
    unsigned long mnAddDotCount;


    OtherPerson *mpNewOtherPerson;
    unsigned long mnSendACK;
    IPData mdMyIPv4;
    unsigned long mnMyNumber;

    int UpdateHostOrGest();
    int UpdateHostInputIPv4();
    int UpdateHostConnectWait();
    int UpdateHostConnectComplate();
    int UpdateHostConnectFailed();
    int UpdateGestConnectWait();
    int UpdateGestConnectComplate();
    int UpdateGestConnectFailed();

    int DrawHostOrGest();
    int DrawHostInputIPv4();
    int DrawHostConnectWait();
    int DrawHostConnectComplate();
    int DrawHostConnectFailed();
    int DrawGestConnectWait();
    int DrawGestConnectComplate();
    int DrawGestConnectFailed();

public:
    NetworkScene();
    ~NetworkScene();

    int Create() override;
    int Initialize() override;
    int Finalize() override;
    int Update() override;
    int Draw() override;

};