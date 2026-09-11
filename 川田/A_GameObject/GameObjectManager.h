#pragma once

#include <vector>

#include "GameObject.h"
#include "../Y_Tool/CreateFlag.h"
#include "../Y_Tool/MallocData.h"

enum UseCameraNumber : unsigned char
{
    UCN_MainCamera = 0,

    UCN_Max,
};

class BaseCamera;

// 2D、3Dを問わないゲームオブジェクトの線形リストを一括管理するクラス
class GameObjectManager
{
private:
    GameObject *mpFirstGameObject;
    BaseCamera *mlCameraBox[UseCameraNumber::UCN_Max];
    UseCameraNumber mnNowCameraNumber;
    CreateFlag mbCreateFlag;
    bool mbUpdateMatrixFlag;

    int UpdateMatrix();                                     // Draw関数の最初に呼び出し、現在使用カメラのアップデートから行列を変更する関数

public:
    GameObjectManager();
    ~GameObjectManager();

    int SetNowCamera(UseCameraNumber nextUseCamera);        // 使用するカメラを変更する
    BaseCamera *GetCameraNum(UseCameraNumber nextUseCamera);// 持っているカメラの中から合うものを取得する
    BaseCamera *GetCameraNow();                             // 現在使用しているカメラを取得する
    int Create();
    int Initialize();
    int Finalize();

    int ObjectCollisionActivate();
    int ObjectCollisionDeactivate();

    int ResetMoveVec();                                     // 移動ベクトルを0ベクトルに設定する
    int EarlyUpdate();                                      // 通常更新の前にしなければいけない何か
    int Update();                                           // 通常更新
    int SetPositionToMoveVec();                             // 当たり判定計算後のオブジェクトの座標の確定
    int SetCollisionPosToCollisionMoveVec();                // 当たり判定計算後の当たり判定の座標の確定
    int LateUpdate();                                       // 当たり判定計算後の更新
    int DeleteToFlagMember();                               // 削除フラグの立っている当たり判定を削除する
    int DeleteToFlag();                                     // 削除フラグの立っているオブジェクトを削除する
    int UpdateModel();                                      // モデルの更新
    int Draw();                                             // モデルの描画

    int Add(GameObject *target);
    int AddCamera(BaseCamera *target, UseCameraNumber cameraNum);
    int DeleteTarget(GameObject *target);
    int DeleteAll();
    int ConnectTarget(GameObject *prev, GameObject *target, GameObject *next);
    int IsolateTarget(GameObject *target);
    MallocData<GameObject *> SearchGameObjectsTag(GameObjectTag tag);
};