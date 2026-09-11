#pragma once

#include "BaseModel.h"
#include "../Y_Tool/VECTOR.h"
#include "../Z_Except/Resource.h"

class GameObject2D;
class Animation2DList;
class Model2D : public BaseModel
{
private:
    TextureHandle mnHandle;
    Animation2DList *mpAnimation2DList;
    VECTOR2D mvLeftUpPos;
    VECTOR2D mvRightBottomPos;

public:
    Model2D(TextureHandle model2DNum, Animation2DList *animationList, GameObject2D *myObject, ScreenNumber drawScreen);
    ~Model2D();

    int Initialize();
    int Finalize();
    int Update();
    int Draw();

    void SetPosition(const VECTOR2D &leftUpPos, const VECTOR2D &rightBottomPos);

    TextureHandle GetHandle() const { return this->mnHandle; }
    Animation2DList *GetAnimation2DList() { return this->mpAnimation2DList; }
    VECTOR2D GetLeftUpPos() const { return this->mvLeftUpPos; }
    VECTOR2D GetRightBottomPos() const { return this->mvRightBottomPos; }
};