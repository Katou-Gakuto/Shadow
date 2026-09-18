#include "DXModelAnim.h"
#include "../A_GameObject/GameObject2D.h"
#include "../Z_Except/Master.h"

int DXAnimModel::Initialize()
{
	return 0;
}

int DXAnimModel::Update()
{
	return 0;
}

int DXAnimModel::Draw()
{
    auto model = static_cast<GameObject2D *>(this->GetUp());
    VECTOR2D pos = model->GetPosition();

	Master::DrawGraphAnim(
		VECTOR2D(pos.GetX() - (Master::PlayerSizeXY / 2), pos.GetY() - (Master::PlayerSizeXY / 2)),
		VECTOR2D(pos.GetX() + (Master::PlayerSizeXY / 2), pos.GetY() + (Master::PlayerSizeXY / 2)),
		uvLeftUp,
		uvRightDown,
		this->GetModelNumber());
	return 0;
}

int DXAnimModel::Finalize()
{
	return 0;
}