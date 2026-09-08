#include "ResourceManager.h"

#include "Master.h"

#include "ResourceGraph.h"
#include "ResourceMovie.h"
#include "ResourceSound.h"

std::string ResourceManager::msResourceFile = "";
DisplaySize ResourceManager::mstDisplaySize = DisplaySize();

ResourceManager::ResourceManager()
:mpGraphResource(nullptr)
, mpDivGraphResource(nullptr)
, mpMovieResource(nullptr)
, mpSoundResource(nullptr)
, mpBackSoundResource(nullptr)
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Initilize()
{
	mpGraphResource = new ResourceGraph();
	mpGraphResource->Initilize();
	mpDivGraphResource = new ResourceDivGraph();
	mpDivGraphResource->Initilize();

	mpMovieResource = new ResourceMovie();
	mpMovieResource->Initilize();

	mpSoundResource= new ResourceSound();
	mpSoundResource->Initilize();
	mpBackSoundResource= new ResourceBackSound();
	mpBackSoundResource->Initilize();

	Vector2_Int setDisplaySize;
	GetScreenState(&setDisplaySize.x, &setDisplaySize.y, &mstDisplaySize.colorBit);
	mstDisplaySize = setDisplaySize;

	SetGlobalAmbientLight(GetColorF(0.3f, 0.3f, 0.3f, 1.0f));
}

void ResourceManager::Finalize()
{	{
		if (mpGraphResource != nullptr)
		{
			mpGraphResource->Finalize();
			delete mpGraphResource;
			mpGraphResource = nullptr;
		}

		if (mpDivGraphResource != nullptr)
		{
			mpDivGraphResource->Finalize();
			delete mpDivGraphResource;
			mpDivGraphResource = nullptr;
		}
	}

	if (mpMovieResource != nullptr)
	{
		mpMovieResource->Finalize();
		delete mpMovieResource;
		mpMovieResource = nullptr;
	}

			
	{
		if (mpSoundResource != nullptr)
		{
			mpSoundResource->Finalize();
			delete mpSoundResource;
			mpSoundResource = nullptr;
		}

		if (mpBackSoundResource != nullptr)
		{
			mpBackSoundResource->Finalize();
			delete mpBackSoundResource;
			mpBackSoundResource = nullptr;
		}
	}
}

void ResourceManager::Update()
{
	// サウンドボリュームの更新
	{
		mpSoundResource->SoundUpdate();
		mpBackSoundResource->SoundUpdate();
	}
}

void ResourceManager::StartDraw()
{
}

void ResourceManager::MiddleDraw()
{
}

void ResourceManager::LastDraw()
{
}

void ResourceManager::DrawDataRelease()
{
}