#include <vector>

#include "DrawData.h"
#include "ResourceData.h"

#include "Master.h"

#include "DrawManager.h"
#include "ResourceGraph.h"
#include "ResourceManager.h"

/*--------*/
/*y‹¤’Êz*/
/*--------*/

// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
DrawManager::DrawManager()
: mnDrawDataID(0)
{
}

// ƒfƒXƒgƒ‰ƒNƒ^
DrawManager::~DrawManager()
{
}

// ‰Šú‰»
void DrawManager::Initilize()
{
    // •`‰æî•ñ‚Ì‰Šú‰»
    for (int orderIt = 0; orderIt < DRAW_ORDER_MAX; ++orderIt) {
        mstDrawData[orderIt].clear();
    }
    mnDrawDataID = 0;
}

// I—¹
void DrawManager::Finalize()
{
    // •`‰æî•ñíœ
    DeleteDrawData();
}

// •`‰æ
void DrawManager::Draw()
{
    for (int orderIt = 0; orderIt < DRAW_ORDER_MAX; orderIt++)
    {
        for (auto drawData : mstDrawData[orderIt])
        {

            // •`‰æ
            DrawData_Draw(&drawData);
            // if (!drawData->drawFlag)
            // {
            //     continue;
            // }

            // switch (drawData->drawManagerDrawType)
            // {
            // case DRAW_MANAGER_DRAW_TYPE::GRAPH:
            //     SetDrawBlendMode(drawData->drawConfigData.blendMode, drawData->drawConfigData.blendParameter);

            //     Master::mpResourceManager->DrawData_Graph(drawData->drawGraphData);

            //     SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
            //     break;
            // }
        }
    }
}

// •`‰æ
void DrawManager::Draw(DRAW_GRAPH_DATA drawGraphData, DRAW_CONFIG_DATA drawConfigData)
{
}

// •`‰æ‡‚ğ•ÏX‚·‚é
bool DrawManager::ChangeDrawOrder(int changeDataID, int orderNumber)
{
    if ((orderNumber < 0) || (DRAW_ORDER_MAX <= orderNumber))
    {
        return true;    // ¸”sƒtƒ‰ƒO
    }

    for (int orderIt = 0; orderIt < DRAW_ORDER_MAX; orderIt++)
    {
        for (int dataIt = 0; dataIt < mstDrawData[orderIt].size(); dataIt++)
        {
            if (mstDrawData[orderIt][dataIt].drawID == changeDataID)
            {
                mstDrawData[orderNumber].push_back(mstDrawData[orderIt][dataIt]);
                mstDrawData[orderIt].erase(mstDrawData[orderIt].begin() + dataIt);
                return false;   // ¸”sƒtƒ‰ƒO
            }
        }
    }

    return true;    // ¸”sƒtƒ‰ƒO
}

// •`‰æî•ñ’Ç‰Á
DRAW_DATA DrawManager::AddDrawData(DRAW_DATA *drawData, int orderNumber)
{
    if ((orderNumber < 0) ||
        (DRAW_ORDER_MAX <= orderNumber))
    {
        orderNumber = DRAW_ORDER_CENTER;
    }

    // •`‰æî•ñ’Ç‰Áˆ—
    drawData->drawID = mnDrawDataID;
    mnDrawDataID += 1;
    mstDrawData[orderNumber].push_back(*drawData);

    return mstDrawData[orderNumber][mstDrawData[orderNumber].size() - 1];
}

// •`‰æî•ñIDw’èíœ
void DrawManager::DeleteDrawData_ID(int id)
{
    // •`‰æî•ñíœ
    for (auto& oneOrderData : mstDrawData)
    {
        for (auto it = oneOrderData.begin(); it != oneOrderData.end(); )
        {
            if (it->drawID == id)
            {
                switch (it->drawManagerDrawType)
                {
                case DRAW_MANAGER_DRAW_TYPE::GRAPH:
                    Master::mpResourceManager->GetGraphResource()->ReduceResourceHandle(it->drawGraphData.handle);
                    break;
                }

                it = oneOrderData.erase(it);
                return;
            }
            else
            {
                ++it;
            }
        }
    }
}

// ‘S•`‰æî•ñíœ
void DrawManager::DeleteDrawData()
{
    // •`‰æî•ñíœ
    for (int orderIt = 0; orderIt < DRAW_ORDER_MAX; ++orderIt) {
        for (auto drawData : mstDrawData[orderIt])
        {
            switch (drawData.drawManagerDrawType)
            {
            case DRAW_MANAGER_DRAW_TYPE::GRAPH:
                Master::mpResourceManager->GetGraphResource()->ReduceResourceHandle(drawData.drawGraphData.handle);
                break;
            }
        }

        mstDrawData[orderIt].clear();
    }

    mnDrawDataID = 0;
}


// •`‰æî•ñ‚ğ•`‰æ
void DrawManager::DrawData_Draw(DRAW_DATA *drawData, bool absoluteDrawFlag)
{
    if (!drawData->drawFlag && !absoluteDrawFlag)
    {
        return;
    }

    switch (drawData->drawManagerDrawType)
    {
    case DRAW_MANAGER_DRAW_TYPE::GRAPH:
        SetDrawBlendMode(drawData->drawConfigData.blendMode, drawData->drawConfigData.blendParameter);

		DrawData_Graph(drawData->drawGraphData);

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
        break;
    }
}

// •`‰æî•ñæ“¾
DRAW_DATA DrawManager::GetDrawData(int drawId)
{
    for (int orderIt = 0; orderIt < DRAW_ORDER_MAX; ++orderIt) {
        for (int drawDataIndex = 0; drawDataIndex < mstDrawData[orderIt].size(); drawDataIndex++)
        {
            if (mstDrawData[orderIt][drawDataIndex].drawID == drawId)
            {
                return mstDrawData[orderIt][drawDataIndex];
            }
        }
    }

    return DRAW_DATA();
}

// •`‰æî•ñİ’è
void DrawManager::SetDrawData(DRAW_DATA* drawData, int drawId)
{
    for (int orderIt = 0; orderIt < DRAW_ORDER_MAX; ++orderIt) {
        for (int drawDataIndex = 0; drawDataIndex < mstDrawData[orderIt].size(); drawDataIndex++)
        {
            if (mstDrawData[orderIt][drawDataIndex].drawID == drawId)
            {
                mstDrawData[orderIt][drawDataIndex] = *drawData;
                return;
            }
        }
    }
}

// ƒ‚ƒfƒ‹•`‰æ
void DrawManager::DrawModelHandle(int modelHandle)
{
	MV1DrawModel(modelHandle);
}


// ƒ‚ƒfƒ‹•`‰æ(’¸“_)
void DrawManager::ModelDraw_Indexed(const std::vector<IndexedData>& modelVertexData)
{
    for (int i = 0; i < modelVertexData.size(); i++)
    {
		DrawIndexed(modelVertexData[i].vertex.data(), (int)modelVertexData[i].vertex.size(),
                    &modelVertexData[i].index[0].v1,   (int)modelVertexData[i].index.size(),
            		modelVertexData[i].textureHandle,
            		modelVertexData[i].transFlag);
    }
}

// ’¸“_î•ñ‚É‚æ‚é•`‰æ
void DrawManager::DrawIndexed(const VERTEX3D* VertexArray, int VertexNum, const unsigned short* IndexArray, int PolygonNum, int GrHandle, int TransFlag)
{
	DrawPolygonIndexed3D(VertexArray, VertexNum, IndexArray, PolygonNum, GrHandle, TransFlag);
}

// ‰æ‘œ•`‰æ
void DrawManager::DrawData_Graph(DRAW_GRAPH_DATA drawData)
{
	switch (drawData.drawType)
	{
	case DRAW_GRAPH_TYPE::NORMAL:
		DrawGraph(drawData.pos.x, drawData.pos.y, drawData.handle, drawData.transFlag);
		break;

	case DRAW_GRAPH_TYPE::TURN:
		DrawTurnGraph(drawData.pos.x, drawData.pos.y, drawData.handle, drawData.transFlag);
		break;

	case DRAW_GRAPH_TYPE::EXTEND:
		DrawExtendGraph(drawData.pos.x, drawData.pos.y, drawData.extPos.x, drawData.extPos.y, drawData.handle, drawData.transFlag);
		break;

	case DRAW_GRAPH_TYPE::SIZE:
		DrawExtendGraph(drawData.pos.x, drawData.pos.y, drawData.pos.x + drawData.size.x, drawData.pos.y + drawData.size.y, drawData.handle, drawData.transFlag);
		break;

	case DRAW_GRAPH_TYPE::ROTA:
		DrawRotaGraph(drawData.pos.x, drawData.pos.y, drawData.extRate.z, drawData.angle, drawData.handle, drawData.transFlag, drawData.turnFlag.x, drawData.turnFlag.y);
		break;

	case DRAW_GRAPH_TYPE::ROTA_CENTER:
		DrawRotaGraph2(drawData.pos.x, drawData.pos.y, drawData.centerPos.x, drawData.centerPos.y, drawData.extRate.z, drawData.angle, drawData.handle, drawData.transFlag, drawData.turnFlag.x, drawData.turnFlag.y);
		break;

	case DRAW_GRAPH_TYPE::ROTA_EXTEND_XY:
		DrawRotaGraph3(drawData.pos.x, drawData.pos.y, drawData.centerPos.x, drawData.centerPos.y, drawData.extRate.x, drawData.extRate.y, drawData.angle, drawData.handle, drawData.transFlag, drawData.turnFlag.x, drawData.turnFlag.y);
		break;

	case DRAW_GRAPH_TYPE::FREE:
		DrawModiGraph(drawData.upLeft.x, drawData.upLeft.y, drawData.upRight.x, drawData.upRight.y, drawData.downRight.x, drawData.downRight.y, drawData.downLeft.x, drawData.downLeft.y, drawData.handle, drawData.transFlag);
		break;

	case DRAW_GRAPH_TYPE::RECT:
		DrawRectGraph(drawData.pos.x, drawData.pos.y, drawData.graphPos.x, drawData.graphPos.y, drawData.size.x, drawData.size.y, drawData.handle, drawData.transFlag, drawData.turnFlag.x, drawData.turnFlag.y);
		break;

	case DRAW_GRAPH_TYPE::RECT_EXTEND:
		DrawRectExtendGraph(drawData.pos.x, drawData.pos.y, drawData.extPos.x, drawData.extPos.y, drawData.graphPos.x, drawData.graphPos.y, drawData.graphSize.x, drawData.graphSize.y, drawData.handle, drawData.transFlag);
		break;

	case DRAW_GRAPH_TYPE::RECT_EXTEND_SIZE:
		DrawRectExtendGraph(drawData.pos.x, drawData.pos.y, drawData.pos.x + drawData.size.x, drawData.pos.y + drawData.size.y, drawData.graphPos.x, drawData.graphPos.y, drawData.graphSize.x, drawData.graphSize.y, drawData.handle, drawData.transFlag);
		break;
	}
}


// •`‰æî•ñæ“¾
DRAW_GRAPH_DATA DrawManager::GetDrawGraphData(int handle, int x, int y)
{
	DRAW_GRAPH_DATA drawData;
	drawData.drawType = DRAW_GRAPH_TYPE::NORMAL;
	if (handle != -1)
	{
		drawData.handle = handle;
	}
	drawData.transFlag = TRUE;

	drawData.pos.x = x;
	drawData.pos.y = y;

	return drawData;
}

// •`‰æî•ñæ“¾
DRAW_GRAPH_DATA DrawManager::GetDrawGraphData(int handle, int x, int y, int sizeX, int sizeY)
{
	DRAW_GRAPH_DATA drawData;
	drawData.drawType = DRAW_GRAPH_TYPE::SIZE;
	if (handle != -1)
	{
		drawData.handle = handle;
	}
	drawData.transFlag = TRUE;

	drawData.pos.x = x;
	drawData.pos.y = y;

	drawData.size.x = sizeX;
	drawData.size.y = sizeY;

	return drawData;
}

// •`‰æî•ñæ“¾
DRAW_GRAPH_DATA DrawManager::GetDrawGraphData(int handle, int x, int y, float sizeXRatio, float sizeYRatio)
{
	DRAW_GRAPH_DATA drawData;
	drawData.drawType = DRAW_GRAPH_TYPE::SIZE;
	if (handle != -1)
	{
		drawData.handle = handle;
	}
	drawData.transFlag = TRUE;

	drawData.pos.x = x;
	drawData.pos.y = y;

	drawData.size = ResourceManager::mstDisplaySize.LeftUp_Ratio(Vector2(sizeXRatio, sizeYRatio));

	return drawData;
}

// •`‰æî•ñæ“¾
DRAW_GRAPH_DATA DrawManager::GetDrawGraphData(int handle, float xRatio, float yRatio)
{
	DRAW_GRAPH_DATA drawData;
	drawData.drawType = DRAW_GRAPH_TYPE::NORMAL;
	if (handle != -1)
	{
		drawData.handle = handle;
	}
	drawData.transFlag = TRUE;

	drawData.pos = ResourceManager::mstDisplaySize.LeftUp_Ratio(Vector2(xRatio, yRatio));

	return drawData;
}

// •`‰æî•ñæ“¾
DRAW_GRAPH_DATA DrawManager::GetDrawGraphData(int handle, float xRatio, float yRatio, int sizeX, int sizeY)
{
	DRAW_GRAPH_DATA drawData;
	drawData.drawType = DRAW_GRAPH_TYPE::SIZE;
	if (handle != -1)
	{
		drawData.handle = handle;
	}
	drawData.transFlag = TRUE;

	drawData.pos = ResourceManager::mstDisplaySize.LeftUp_Ratio(Vector2(xRatio, yRatio));

	drawData.size.x = sizeX;
	drawData.size.y = sizeY;

	return drawData;
}

// •`‰æî•ñæ“¾
DRAW_GRAPH_DATA DrawManager::GetDrawGraphData(int handle, float xRatio, float yRatio, float sizeXRatio, float sizeYRatio)
{
	DRAW_GRAPH_DATA drawData;
	drawData.drawType = DRAW_GRAPH_TYPE::SIZE;
	if (handle != -1)
	{
		drawData.handle = handle;
	}
	drawData.transFlag = TRUE;

	drawData.pos = ResourceManager::mstDisplaySize.LeftUp_Ratio(Vector2(xRatio, yRatio));

	drawData.size = ResourceManager::mstDisplaySize.LeftUp_Ratio(Vector2(sizeXRatio, sizeYRatio));

	return drawData;
}

// •`‰æî•ñæ“¾
DRAW_GRAPH_DATA DrawManager::GetDrawGraphData(int handle, Vector2_Int pos)
{
	DRAW_GRAPH_DATA drawData;
	drawData.drawType = DRAW_GRAPH_TYPE::NORMAL;
	if (handle != -1)
	{
		drawData.handle = handle;
	}
	drawData.transFlag = TRUE;

	drawData.pos = pos;

	return drawData;
}

// •`‰æî•ñæ“¾
DRAW_GRAPH_DATA DrawManager::GetDrawGraphData(int handle, Vector2_Int pos, Vector2_Int size)
{
	DRAW_GRAPH_DATA drawData;
	drawData.drawType = DRAW_GRAPH_TYPE::SIZE;
	if (handle != -1)
	{
		drawData.handle = handle;
	}
	drawData.transFlag = TRUE;

	drawData.pos = pos;

	drawData.size = size;

	return drawData;
}

// •`‰æî•ñæ“¾
DRAW_GRAPH_DATA DrawManager::GetDrawGraphData(int handle, Vector2_Int leftUp, Vector2_Int rightUp, Vector2_Int leftDown, Vector2_Int rightDown)
{
	DRAW_GRAPH_DATA drawData;
	drawData.drawType = DRAW_GRAPH_TYPE::FREE;
	if (handle != -1)
	{
		drawData.handle = handle;
	}
	drawData.transFlag = TRUE;

	drawData.upLeft = leftUp;
	drawData.upRight = rightUp;
	drawData.downLeft = leftDown;
	drawData.downRight = rightDown;

	return drawData;
}