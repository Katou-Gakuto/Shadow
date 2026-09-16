#pragma once

#include <cstdint>
#include <vector>

class LightLineNode;

class VECTOR2D;
class BaseCollision2D;
class Ray2D;
struct CollisionCheckResult2D;

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

    // 
    bool CheckInArea(const VECTOR2D &pos) const;

    // 
    bool GetOffLight() const;

    // 
    bool OnLight();

    // 
    bool OffLight();

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
    bool mbOffLight;
};
