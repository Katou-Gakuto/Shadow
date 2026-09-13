#include "Model3D.h"

#include "../A_GameObject/GameObject3D.h"

Model3D::Model3D(GameObject3D *myObject, ScreenNumber drawScreen) :
    mvPosition(VECTOR3D::GetZero()),
    BaseModel(myObject, ModelType::ModelType_Model3D, drawScreen)
{

}

Model3D::~Model3D()
{

}

int Model3D::Initialize()
{
    return 0;
}

int Model3D::Finalize()
{
    return 0;
}

int Model3D::Update()
{
    return 0;
}

int Model3D::Draw()
{
    return 0;
}
