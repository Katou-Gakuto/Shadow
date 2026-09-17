#include "DxLib.h"

#include "TestObjectBase.h"
#include "WallObject.h"

WallObject::WallObject()
: TestObjectBase()
{
}

WallObject::~WallObject()
{
}

void WallObject::Init()
{
}

void WallObject::Finalize()
{
}

void WallObject::Update()
{
}

void WallObject::Draw()
{
    DrawBox(mv2Position.x - (mv2Size.x * 0.5f), mv2Position.y - (mv2Size.y * 0.5f), mv2Position.x + (mv2Size.x * 0.5f), mv2Position.y + (mv2Size.y * 0.5f), GetColor(0, 255, 0), TRUE);
}