#include <map>
#include <string>
#include <vector>

#include "ResourceData.h"

#include "Master.h"

#include "EndManager.h"
#include "ResourceBase.h"
#include "ResourceGraph.h"

#ifdef _DEBUG
#include "DebugLogs/DebugLog.h"
#endif

ResourceGraph::ResourceGraph()
{
}

ResourceGraph::~ResourceGraph()
{
}

void ResourceGraph::Initilize()
{
	mmSettingHandleFlagByProcess[RESOURCE_BASE_HANDLE_FLAG_SETTING_TYPE::GENERATION_RESOURCE]  = HANDLE_FLAG::ZERO_LOOK;
	mmSettingHandleFlagByProcess[RESOURCE_BASE_HANDLE_FLAG_SETTING_TYPE::DUPLICATION_RESOURCE] = HANDLE_FLAG::ZERO_LOOK;
	mmSettingHandleFlagByProcess[RESOURCE_BASE_HANDLE_FLAG_SETTING_TYPE::REDUCE_RESOURCE]      = HANDLE_FLAG::ZERO_LOOK;
	// mpGraphHandleContainer = new HandleContainer<std::string>();
	// mpGraphHandleContainer->SetHandleFlag(HANDLE_FLAG::ZERO_LOOK);
	// mmDivGraphHandle.clear();
	// msDivGraphFileNames.clear();
}

void ResourceGraph::Finalize()
{
	for (std::pair<std::string, std::vector<int>> graphHandle : mclHandleContainer.GetHandleMap())
	{
		for (int i = 0; i < graphHandle.second.size(); i++)
		{
			DeleteGraph(graphHandle.second[i]);
#ifdef _DEBUG
			DEBUG::SaveText("画像ハンドル削除(最終) : " + std::to_string(graphHandle.second[i]) + " : " + graphHandle.first + " : HandleNumber <=" + std::to_string(mclHandleContainer.GetHandleCount(graphHandle.second[i])) + '\n', DEBUG::DEBUG_MAP_TYPE::DEBUG_GRAPH);
#endif
		}
	}
}
// リソース本体作成
int ResourceGraph::CreateResource(const std::string& fileName, int* plusData)
{
	return LoadGraph(fileName.c_str());
}

// リソース複製
int ResourceGraph::ResourceDuplication(const std::string& fileName, int* plusData)
{
	SetHandleFlag(RESOURCE_BASE_HANDLE_FLAG_SETTING_TYPE::DUPLICATION_RESOURCE);

	return 0;
}

// リソース削除
void ResourceGraph::ResourceDelete(const std::vector<int>& handles)
{
	for (int i = 0; i < handles.size(); i++)
	{
		DeleteGraph(handles[i]);
	}
}
// {
// 	for (std::pair<std::string, std::vector<int>> graphHandle : mpGraphHandleContainer->GetHandleMap())
// 	{
// 		for (int i = 0; i < graphHandle.second.size(); i++)
// 		{
// 			DeleteGraph(graphHandle.second[i]);
// #ifdef _DEBUG
// 			DEBUG::SaveText("画像ハンドル削除(最終) : " + std::to_string(graphHandle.second[i]) + " : " + graphHandle.first + " : HandleNumber <=" + std::to_string(mpGraphHandleContainer->GetHandleCount(graphHandle.second[i])) + '\n', DEBUG::DEBUG_MAP_TYPE::DEBUG_GRAPH);
// #endif
// 		}
// 	}
// 	delete mpGraphHandleContainer;
// 	mpGraphHandleContainer = nullptr;

// 	for (const std::pair<std::string, DIV_GRAPH_DATA>& divHandle : mmDivGraphHandle)
// 	{
// 		for (int i = 0; i < divHandle.second.allNum; i++)
// 		{
// 			DeleteGraph(*(divHandle.second.handle + i));
// #ifdef _DEBUG
// 			DEBUG::SaveText("DIV画像ハンドル削除(最終) : " + std::to_string(*(divHandle.second.handle + i)) + '\n');
// #endif
// 		}
// 	}
// 	mmDivGraphHandle.clear();
// 	msDivGraphFileNames.clear();
// }

// int ResourceGraph::GetGraphHandle(std::string fileName)
// {
// 	if (mpGraphHandleContainer->CheckFileName(fileName))
// 	{
// #ifdef _DEBUG
// 		int resultHandle = mpGraphHandleContainer->RegisterHandle(0);
// 		DEBUG::SaveText("画像ハンドル取得 : " + fileName + " : " + std::to_string(resultHandle) + '\n', DEBUG::DEBUG_MAP_TYPE::DEBUG_GRAPH);
// 		return resultHandle;
// #endif
// 		return mpGraphHandleContainer->RegisterHandle(0);
// 	}

// #ifdef _DEBUG
// 	int handle = mpGraphHandleContainer->RegisterHandle(LoadGraph(fileName.c_str()));
// 	DEBUG::SaveText("画像ハンドル(原本)取得 : " + fileName + " : " + std::to_string(handle) + '\n', DEBUG::DEBUG_MAP_TYPE::DEBUG_GRAPH);
// 	return handle;
// #endif
// 	return mpGraphHandleContainer->RegisterHandle(LoadGraph(fileName.c_str()));
// }

// void ResourceGraph::ReduceGraphHandle(int handle)
// {
// 	std::vector<int> deleteHandle = mpGraphHandleContainer->DeleteHandle(handle);

// #ifdef _DEBUG
// 	std::string text = "画像ハンドル削除 : " + std::to_string(handle) + "(NOT_DELETE)";
// 	for (int i = 0; i < deleteHandle.size(); i++)
// 	{
// 		text = text + " : " + std::to_string(deleteHandle[i]);
// 	}
// 	text = text + '\n';
// 	DEBUG::SaveText(text, DEBUG::DEBUG_MAP_TYPE::DEBUG_GRAPH);
// #endif

// 	for (int i = 0; i < deleteHandle.size(); i++)
// 	{
// 		DeleteGraph(deleteHandle[i]);
// 	}
// }

// void ResourceGraph::GetDivGraphHandle(std::string fileName, DIV_GRAPH_DATA* graphData)
// {
// 	if (mmDivGraphHandle.find(fileName) != mmDivGraphHandle.end())
// 	{
// 		*graphData = mmDivGraphHandle[fileName];
// 		graphData->count += 1;
// 		return;
// 	}

// 	if (LoadDivGraph(fileName.c_str(), graphData->allNum, graphData->xNum, graphData->yNum, graphData->xSize, graphData->ySize, graphData->handle) == -1)
// 	{
// 		Master::mpEndManager->SetEndFlag(true, END_FLAG_NUMBER::RESOURCE_FLAG);
// 		return;
// 	}
// 	mmDivGraphHandle[fileName] = *graphData;
// 	msDivGraphFileNames.push_back(fileName);
// 	graphData->count += 1;
// 	graphData->number = ((int)msDivGraphFileNames.size() - 1);
// }

// void ResourceGraph::ReduceDivGraphHandle(int number)
// {
// 	if ((mmDivGraphHandle[msDivGraphFileNames[number]].count -= 1) <= 0)
// 	{
// 		for (int i = 0; i < mmDivGraphHandle[msDivGraphFileNames[number]].allNum; i++)
// 		{
// 			DeleteGraph(*(mmDivGraphHandle[msDivGraphFileNames[number]].handle + i));
// 		}
// 		mmDivGraphHandle.erase(msDivGraphFileNames[number]);
// 		msDivGraphFileNames.erase(msDivGraphFileNames.begin() + number);
// 	}
// }

// void ResourceGraph::DrawData_Graph(DRAW_GRAPH_DATA drawData)
// {
// 	switch (drawData.drawType)
// 	{
// 	case DRAW_GRAPH_TYPE::NORMAL:
// 		DrawGraph(drawData.pos.x, drawData.pos.y, drawData.handle, drawData.transFlag);
// 		break;

// 	case DRAW_GRAPH_TYPE::TURN:
// 		DrawTurnGraph(drawData.pos.x, drawData.pos.y, drawData.handle, drawData.transFlag);
// 		break;

// 	case DRAW_GRAPH_TYPE::EXTEND:
// 		DrawExtendGraph(drawData.pos.x, drawData.pos.y, drawData.extPos.x, drawData.extPos.y, drawData.handle, drawData.transFlag);
// 		break;

// 	case DRAW_GRAPH_TYPE::SIZE:
// 		DrawExtendGraph(drawData.pos.x, drawData.pos.y, drawData.pos.x + drawData.size.x, drawData.pos.y + drawData.size.y, drawData.handle, drawData.transFlag);
// 		break;

// 	case DRAW_GRAPH_TYPE::ROTA:
// 		DrawRotaGraph(drawData.pos.x, drawData.pos.y, drawData.extRate.z, drawData.angle, drawData.handle, drawData.transFlag, drawData.turnFlag.x, drawData.turnFlag.y);
// 		break;

// 	case DRAW_GRAPH_TYPE::ROTA_CENTER:
// 		DrawRotaGraph2(drawData.pos.x, drawData.pos.y, drawData.centerPos.x, drawData.centerPos.y, drawData.extRate.z, drawData.angle, drawData.handle, drawData.transFlag, drawData.turnFlag.x, drawData.turnFlag.y);
// 		break;

// 	case DRAW_GRAPH_TYPE::ROTA_EXTEND_XY:
// 		DrawRotaGraph3(drawData.pos.x, drawData.pos.y, drawData.centerPos.x, drawData.centerPos.y, drawData.extRate.x, drawData.extRate.y, drawData.angle, drawData.handle, drawData.transFlag, drawData.turnFlag.x, drawData.turnFlag.y);
// 		break;

// 	case DRAW_GRAPH_TYPE::FREE:
// 		DrawModiGraph(drawData.upLeft.x, drawData.upLeft.y, drawData.upRight.x, drawData.upRight.y, drawData.downRight.x, drawData.downRight.y, drawData.downLeft.x, drawData.downLeft.y, drawData.handle, drawData.transFlag);
// 		break;

// 	case DRAW_GRAPH_TYPE::RECT:
// 		DrawRectGraph(drawData.pos.x, drawData.pos.y, drawData.graphPos.x, drawData.graphPos.y, drawData.size.x, drawData.size.y, drawData.handle, drawData.transFlag, drawData.turnFlag.x, drawData.turnFlag.y);
// 		break;

// 	case DRAW_GRAPH_TYPE::RECT_EXTEND:
// 		DrawRectExtendGraph(drawData.pos.x, drawData.pos.y, drawData.extPos.x, drawData.extPos.y, drawData.graphPos.x, drawData.graphPos.y, drawData.graphSize.x, drawData.graphSize.y, drawData.handle, drawData.transFlag);
// 		break;

// 	case DRAW_GRAPH_TYPE::RECT_EXTEND_SIZE:
// 		DrawRectExtendGraph(drawData.pos.x, drawData.pos.y, drawData.pos.x + drawData.size.x, drawData.pos.y + drawData.size.y, drawData.graphPos.x, drawData.graphPos.y, drawData.graphSize.x, drawData.graphSize.y, drawData.handle, drawData.transFlag);
// 		break;
// 	}
// }

ResourceDivGraph::ResourceDivGraph()
{
}

ResourceDivGraph::~ResourceDivGraph()
{
}

void ResourceDivGraph::Initilize()
{
	mmSettingHandleFlagByProcess[RESOURCE_BASE_HANDLE_FLAG_SETTING_TYPE::GENERATION_RESOURCE]  = HANDLE_FLAG::ZERO_LOOK;
	mmSettingHandleFlagByProcess[RESOURCE_BASE_HANDLE_FLAG_SETTING_TYPE::DUPLICATION_RESOURCE] = HANDLE_FLAG::ZERO_LOOK;
	mmSettingHandleFlagByProcess[RESOURCE_BASE_HANDLE_FLAG_SETTING_TYPE::REDUCE_RESOURCE]      = HANDLE_FLAG::ZERO_LOOK;
}

void ResourceDivGraph::Finalize()
{
	for (std::pair<std::string, std::vector<DIV_GRAPH_DATA>> graphHandle : mclHandleContainer.GetHandleMap())
	{
		for (int i = 0; i < graphHandle.second.size(); i++)
		{
			for (int j = 0; j < graphHandle.second[i].allNum; j++)
			{
				DeleteGraph(*(graphHandle.second[i].handle + j));
#ifdef _DEBUG
				DEBUG::SaveText("画像ハンドル削除DIV(最終) : " + std::to_string(*(graphHandle.second[i].handle + j)) + '\n');
#endif
			}
		}
	}
}

// リソース本体作成
DIV_GRAPH_DATA ResourceDivGraph::CreateResource(const std::string& fileName, DIV_GRAPH_DATA* plusData)
{	
	if (plusData == nullptr)
	{
		Master::mpEndManager->SetEndFlag(true, END_FLAG_NUMBER::RESOURCE_FLAG);
		return DIV_GRAPH_DATA();
	}

	if (LoadDivGraph(fileName.c_str(), plusData->allNum, plusData->xNum, plusData->yNum, plusData->xSize, plusData->ySize, plusData->handle) == -1)
	{
		Master::mpEndManager->SetEndFlag(true, END_FLAG_NUMBER::RESOURCE_FLAG);
		return 0;
	}
	return *plusData;
}

// リソース複製
DIV_GRAPH_DATA ResourceDivGraph::ResourceDuplication(const std::string& fileName, DIV_GRAPH_DATA* plusData)
{
	return DIV_GRAPH_DATA(-1);// 何もないデータID増やさない
}

// リソース削除
void ResourceDivGraph::ResourceDelete(const std::vector<DIV_GRAPH_DATA>& handles)
{
	for (int i = 0; i < handles.size(); i++)
	{
		for (int j = 0; j < handles[i].allNum; j++)
		{
			DeleteGraph(*(handles[i].handle + j));
		}
	}
}