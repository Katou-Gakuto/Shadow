#include "Pose2D.h"

Pose2D::Pose2D() :
    mvLeftUpUV(VECTOR2D::GetZero()),
    mvRightBottomUV(VECTOR2D::GetZero()),
    mpPrev(nullptr),
    mpNext(nullptr)
{

}

Pose2D::Pose2D(float left, float up, float right, float down) :
    mvLeftUpUV(VECTOR2D(left, up)),
    mvRightBottomUV(VECTOR2D(right, down)),
    mpPrev(nullptr),
    mpNext(nullptr)
{

}

Pose2D::~Pose2D()
{

}

int Pose2D::Initialize()
{
    return 0;
}

int Pose2D::Finalize()
{
    return 0;
}

int Pose2D::Update()
{
    return 0;
}

int Pose2D::Draw()
{
    return 0;
}