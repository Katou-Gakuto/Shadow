#include "Model2D.h"

#include "Animation2D.h"
#include "Animation2DList.h"
#include "Pose2D.h"
#include "../A_GameObject/GameObject2D.h"
#include "../V_Display/System.h"
#include "../Z_Except/Master.h"
#include "../Z_Except/ResourceManager.h"

Model2D::Model2D(TextureHandle model2DNum, Animation2DList *animationList, GameObject2D *myObject, ScreenNumber drawScreen) :
    mnHandle(model2DNum),
    mpAnimation2DList(animationList),
    mvLeftUpPos(VECTOR2D::GetZero()),
    mvRightBottomPos(VECTOR2D::GetZero()),
    BaseModel(myObject, ModelType::ModelType_Model2D, drawScreen)
{
    // Ž©g‚Ìƒ‚ƒfƒ‹‚ðŽæ“¾‚·‚é
    if (Master::mpResourceManager->LoadModel2D(model2DNum) == false)
    {
        this->mnHandle = TextureHandle::TextureHandle_Max;
    }
}

Model2D::~Model2D()
{
    Master::mpResourceManager->DeleteModel2D(this->mnHandle);
    if (this->mpAnimation2DList != nullptr)
    {
        delete this->mpAnimation2DList;
    }
}

int Model2D::Initialize()
{
    if (this->mpAnimation2DList != nullptr)
    {
        return this->mpAnimation2DList->Initialize();
    }

    return 0;
}

int Model2D::Finalize()
{
    if (this->mpAnimation2DList != nullptr)
    {
        return this->mpAnimation2DList->Finalize();
    }

    return 0;
}

int Model2D::Update()
{
    if (this->mpAnimation2DList != nullptr)
    {
        return this->mpAnimation2DList->Update();
    }

    return 0;
}

int Model2D::Draw()
{
    if (Master::mpSystem->ChangeShaderMode(ShaderMode::ShaderMode_Index_2DTexture) != 0)
    {
        return -1;
    }

    // ‚¨”C‚¹‚Å•`‰æ‚·‚éƒtƒ‰ƒO
    bool autoDrawFlag = true;

    // ƒAƒjƒ[ƒVƒ‡ƒ“2DŠÖ˜A‚ÌÅ‰º‘w‚Ü‚Åö‚Á‚ÄUV‚Ì’l‚ðŽæ“¾‚µ‚Ä‚©‚ç•`‰æ‚ð‚·‚éBŽ¸”s‚µ‚½‚ç‚¨”C‚¹‚Å•`‰æ‚ðs‚¤
    if (this->mpAnimation2DList != nullptr)
    {
        auto nowAnimation = this->mpAnimation2DList->GetNowAnimation2D();
        if (nowAnimation != nullptr)
        {
            auto nowPose = nowAnimation->GetCurrentPose2D();
            if (nowPose != nullptr)
            {
                // Žæ“¾‚·‚é
                void *resource = Master::mpResourceManager->GetModel2D(this->mnHandle)->mpData;

                // ƒVƒXƒeƒ€‚Éƒ‚ƒfƒ‹‘S‘Ì‚Ì•`‰æ‚ð‚¨Šè‚¢‚·‚é
                Master::mpSystem->DrawSprite(
                    this->mnHandle,
                    this->mvLeftUpPos,
                    this->mvRightBottomPos,
                    nowPose->GetLeftUpUV(),
                    nowPose->GetRightBottomUV());

                // •`‰æ‚É¬Œ÷‚µ‚½‚Ì‚ÅA‚¨”C‚¹‚Å•`‰æ‚·‚éƒtƒ‰ƒO‚ðfalse‚É‚µ‚Ä‚¨‚­
                autoDrawFlag = false;
            }
        }
    }

    // ‚¨”C‚¹‚Å•`‰æ‚·‚éˆ—
    if (autoDrawFlag)
    {
        // ƒVƒXƒeƒ€‚Éƒ‚ƒfƒ‹‘S‘Ì‚Ì•`‰æ‚ð‚¨Šè‚¢‚·‚é
        Master::mpSystem->DrawSprite(
            this->mnHandle,
            this->mvLeftUpPos,
            this->mvRightBottomPos,
            VECTOR2D(0.0f, 0.0f),
            VECTOR2D(1.0f, 1.0f));
    }

    return 0;
}

void Model2D::SetPosition(const VECTOR2D &leftUpPos, const VECTOR2D &rightBottomPos)
{
    this->mvLeftUpPos = leftUpPos;
    this->mvRightBottomPos = rightBottomPos;
}