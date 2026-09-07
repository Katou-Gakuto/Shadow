#include <map>
#include <string>
#include <vector>

#include "DxLib.h"

#include "HandleContainer.h"
#include "ResourceBase.h"
#include "ResourceMovie.h"

#ifdef _DEBUG
#include "DebugLogs/DebugLog.h"
#endif

ResourceMovie::ResourceMovie()
{
}

ResourceMovie::~ResourceMovie()
{
}

void ResourceMovie::Initilize()
{
	mmSettingHandleFlagByProcess[RESOURCE_BASE_HANDLE_FLAG_SETTING_TYPE::GENERATION_RESOURCE]  = HANDLE_FLAG::NONE;
	mmSettingHandleFlagByProcess[RESOURCE_BASE_HANDLE_FLAG_SETTING_TYPE::DUPLICATION_RESOURCE] = HANDLE_FLAG::NONE;
	mmSettingHandleFlagByProcess[RESOURCE_BASE_HANDLE_FLAG_SETTING_TYPE::REDUCE_RESOURCE]      = HANDLE_FLAG::NONE;
}

void ResourceMovie::Finalize()
{
	for (std::pair<std::string, std::vector<int>> moveiHandle : mclHandleContainer.GetHandleMap())
	{
		for (int i = 0; i < moveiHandle.second.size(); i++)
		{
			DeleteGraph(moveiHandle.second[i]);
#ifdef _DEBUG
			DEBUG::SaveText("動画ハンドル削除(最終) : " + std::to_string(moveiHandle.second[i]) + " : " + moveiHandle.first + " : HandleNumber <=" + std::to_string(mclHandleContainer.GetHandleCount(moveiHandle.second[i])) + '\n');
#endif
		}
	}
}

// int ResourceMovie::GetMovieHandle(std::string fileName)
// {
// #ifdef _DEBUG
// 	int handle = mclHandleContainer.RegisterHandle(LoadGraph(fileName.c_str()), fileName);
// 	DEBUG::SaveText("動画ハンドル取得 : " + fileName + " : " + std::to_string(handle) + '\n');
// 	return handle;
// #endif
// 	return mclHandleContainer.RegisterHandle(LoadGraph(fileName.c_str()), fileName);
// }

// void ResourceMovie::ReduceMovie(int handle)
// {
// 	std::vector<int> deleteHandle = mclHandleContainer.DeleteHandle(handle);

// #ifdef _DEBUG
// 	std::string text = "動画ハンドル削除 : " + std::to_string(handle) + "(NOT_DELETE)";
// 	for (int i = 0; i < deleteHandle.size(); i++)
// 	{
// 		text = text + " : " + std::to_string(deleteHandle[i]);
// 	}
// 	text = text + '\n';
// 	DEBUG::SaveText(text);
// #endif

// 	for (int i = 0; i < deleteHandle.size(); i++)
// 	{
// 		DeleteGraph(deleteHandle[i]);
// 	}
// }

void ResourceMovie::PlayMovie(int handle)
{
	PlayMovieToGraph(handle);
}

void ResourceMovie::StopMovie(int handle)
{
	PauseMovieToGraph(handle);
}

void ResourceMovie::MovieReset(int handle)
{
	SeekMovieToGraph(handle, 0);
}

void ResourceMovie::MovieLoop(int handle)
{
	if (GetMovieStateToGraph(handle) != 1)
	{
		MovieReset(handle);
		PlayMovie(handle);
	}
}


// リソース本体作成
int ResourceMovie::CreateResource(const std::string& fileName, int* plusData)
{
	return LoadGraph(fileName.c_str());
}

// リソース複製
int ResourceMovie::ResourceDuplication(const std::string& fileName, int* plusData)
{
	SetHandleFlag(RESOURCE_BASE_HANDLE_FLAG_SETTING_TYPE::DUPLICATION_RESOURCE);

	return LoadGraph(fileName.c_str());
}

// リソース削除
void ResourceMovie::ResourceDelete(const std::vector<int>& handles)
{
	for (int i = 0; i < handles.size(); i++)
	{
		DeleteGraph(handles[i]);
	}
}