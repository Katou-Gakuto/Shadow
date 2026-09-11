#include "TestEmitterController.h"

#include "../A_GameObject/TestEmitter.h"
#include "../R_Input/Input.h"
#include "../Z_Except/Master.h"

TestEmitterController::TestEmitterController(GameObject *myObject) :
    BaseComponent(myObject, CTAO::CTAO_TestEmitterController)
{
}

TestEmitterController::~TestEmitterController()
{
}

int TestEmitterController::Create()
{
    return 0;
}

int TestEmitterController::Initialize()
{
    return 0;
}

int TestEmitterController::Finalize()
{
    return 0;
}

int TestEmitterController::EarlyUpdate()
{
    return 0;
}

int TestEmitterController::Update()
{
    if (Master::mpInput->CheckInputKeyDown(InputTag_Key_I))
    {
        TestEmitter *obj = static_cast<TestEmitter *>(this->GetMyObject());
        obj->CreateSoundVisualizer();
    }

    return 0;
}

int TestEmitterController::HitOnCollision(BaseCollision *myCollision, BaseCollision *hitCollision)
{
    return 0;
}

int TestEmitterController::LateUpdate()
{
    return 0;
}

int TestEmitterController::Draw()
{
    return 0;
}
