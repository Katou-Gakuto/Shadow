#pragma once

#include "../Y_Tool/VECTOR.h"

#include "../S_Collision/Ray2D.h"

#include <vector>

// 
class LightLineNode
{
public:
    // コンストラクタ
    LightLineNode(void *parent);

    // デストラクタ
    ~LightLineNode();

    // 自身の光の終了地点を計算し、設定する関数
    // 次のノードにつながる場合はtrueを返す
    bool CalculateEndPos(
        const VECTOR2D &startPos,
        const VECTOR2D &toEndNorm,
        const std::vector<BaseCollision2D *> &objectCollBox,
        const std::vector<BaseCollision2D *> &mirrorCollBox,
        float &stockMagunitude,
        Ray2D &tempRay,
        CollisionCheckResult2D &tempResultCurrent,
        CollisionCheckResult2D &tempResultNewr);

    // 自身のノードのひとつ前の終了地点、つまり自身のスタート地点を返す関数
    // mpPrevがnullptrの場合はライトの根元を返す
    const VECTOR2D &GetMyStartPos() const;

    // このライトの根元地点を返す関数
    const VECTOR2D &GetListStartPos() const;

    // 自身のノードの終了地点を返す関数
    const VECTOR2D &GetMyEndPos() const;

    // 前のノードに移る関数
    // Listの頭にこのノードがあり、mpPrevがnullptrの場合もoutにはnullptrが格納され、trueが返ります
    bool AccessPrev(LightLineNode **out);

    // 次のノードに移る関数
    // mpNextがtrueの場合はnewを行い、自身のmpNextにヒープのインスタンスを記憶してから、そのポインタを返します
    bool AccessNext(LightLineNode **out);

    // 次のノードが有効かを確認する関数
    bool CheckNext() const;

private:
    // 
    void *mpParentLight;

    // 光が終わる座標
    VECTOR2D mvEndPos;

    // 次のノードに行くか
    bool mbNext;

    // ひとつ前の光
    LightLineNode *mpPrev;

    // ひとつ後の光
    LightLineNode *mpNext;
};

// 
class LightObjectController;

// 
class LightArea
{
public:
    // 
    LightArea(void *parentLightController);

    // 
    ~LightArea();

    // 自身の光のレイの角度を設定する関数
    void SetLightLineAngleBox(uint32_t lineCount, float maxAngle);

    // 自身の光の終了地点を計算し、設定する関数
    // 次のノードにつながる場合はtrueを返す
    bool CalculateNode(
        float baseAngle,
        float stockMagunitude,
        const VECTOR2D &startPos,
        const std::vector<BaseCollision2D *> &objectCollBox,
        const std::vector<BaseCollision2D *> &mirrorCollBox,
        Ray2D &tempRay,
        CollisionCheckResult2D &tempResultCurrent,
        CollisionCheckResult2D &tempResultNewr);

    // 
    uint32_t GetAllNodeCount() const;

    // 
    LightLineNode **GetFirstNodeBox();

    // 
    LightLineNode *const *GetFirstNodeBox() const;

    // 
    VECTOR2D *GetLineEndPosBox();

    // 
    const VECTOR2D *GetLineEndPosBox() const;

    // 
    uint32_t GetLightLineCount() const;

    // 
    void SetLightLineCount(uint32_t count);

private:
    // とりあえず用意しておこうか
    std::vector<LightLineNode *> mlLightFirstNode;

    // 
    std::vector<VECTOR2D> mlLineEndPosBox;

    // 
    std::vector<float> mlLineAngleBox;

    // 
    void *mpParentLight;

    // 
    uint32_t mnLightLineCount;

    // 
    LightObjectController *GetLightController() const;
};
