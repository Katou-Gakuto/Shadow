#include "DxLib.h"

#include "ShineObject.h"

ShineObject::ShineObject()
{
}

ShineObject::~ShineObject()
{
}

void ShineObject::Init()
{
}

void ShineObject::Finalize()
{
}

void ShineObject::Update()
{
}

void ShineObject::Draw()
{
    DrawCircle(mv2Position.x, mv2Position.y, (mv2Size.x * mv2Size.y * 0.5f) * 0.5f, GetColor(0, 255, 0), TRUE);
}