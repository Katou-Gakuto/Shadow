#pragma once

// シーンマネージャー
class BaseSceneManager;

// 
class Master
{
public:
    // シーンのManager
    static BaseSceneManager *mpBaseSceneManager;

    // そのほかManager



    // Masterの各メンバをnewする関数
    static int Initialize();

    // Masterの各メンバをdeleteする関数
    static int Finalize();
};