#pragma once
#include <vector>

#include "DrawData.h"
#include "ResourceData.h"

class DrawManager
{
public:
    // •`‰æ‡”Ô Å‘å
    static constexpr int DRAW_ORDER_MAX = 3;

    // •`‰æ‡”Ô ’†‰›
    static constexpr int DRAW_ORDER_CENTER = DRAW_ORDER_MAX / 2;

private:
    // •`‰æî•ñ
    std::vector<DRAW_DATA> mstDrawData[DRAW_ORDER_MAX];

    // •`‰æÅ‘åID
    int mnDrawDataID;

public:
    DrawManager();
    ~DrawManager();

    /// <summary>‰Šú‰»</summary>
    void Initilize();
    /// <summary>I—¹</summary>
    void Finalize();

    /// <summary>•`‰æ</summary>
    void Draw();
    /// <summary>•`‰æ</summary>
    void Draw(DRAW_GRAPH_DATA drawGraphData, DRAW_CONFIG_DATA drawConfigData = DRAW_CONFIG_DATA());

    /// <summary>•`‰æ‡‚ğ•ÏX‚·‚é</summary>
    /// <param name="changeDataID">•ÏX‚·‚é•`‰æî•ñID</param>
    /// <param name="orderNumber">•`‰æ‡(0ˆÈãDRAW_ORDER_MAX–¢–)</param>
    /// <returns>¸”sƒtƒ‰ƒO</returns>
    bool ChangeDrawOrder(int changeDataID, int orderNumber = DRAW_ORDER_CENTER);

    /// <summary>•`‰æî•ñ’Ç‰Á¦vcector‚ÅÄŠm•Û‚·‚é‚Æ•Ï”‚Ì•Û‘¶êŠ‚ª‚¸‚ê‚é‚½‚ßƒoƒO‚é‚½‚ßupush_backv‚ªI‚í‚Á‚½Œã‚É‚â‚é</summary>
    /// <param name="drawData">’Ç‰Á‚·‚é•`‰æî•ñ</param>
    /// <param name="orderNumber">•`‰æ‡(0ˆÈãDRAW_ORDER_MAX–¢–)</param>
    /// <returns>•`‰æî•ñID</returns>
    DRAW_DATA AddDrawData(DRAW_DATA *drawData, int orderNumber = DRAW_ORDER_CENTER);

    /// <summary>•`‰æî•ñIDw’èíœ</summary>
    void DeleteDrawData_ID(int id);

    /// <summary>‘S•`‰æî•ñíœ</summary>
    void DeleteDrawData();

    /// <summary>•`‰æî•ñ‚ğ•`‰æ</summary>
    void DrawData_Draw(DRAW_DATA *drawData, bool absoluteDrawFlag = false);

    /// <summary>•`‰æî•ñæ“¾</summary>
    /// <param name="drawId">•`‰æID</param>
    /// <returns>•`‰æî•ñ</returns>
    DRAW_DATA GetDrawData(int drawId);

    /// <summary>•`‰æî•ñİ’è</summary>
    /// <param name="drawData">•`‰æî•ñ</param>
    /// <param name="drawId">•`‰æID</param>
    void SetDrawData(DRAW_DATA* drawData, int drawId);

    
    /*--------*/
    /*y•`‰æz*/
    /*--------*/
    
	/// <summary>ƒ‚ƒfƒ‹•`‰æ</summary>
	void DrawModelHandle(int modelHandle);
    
    /// <summary>ƒ‚ƒfƒ‹•`‰æ(’¸“_)</summary>
    void ModelDraw_Indexed(const std::vector<IndexedData>& modelVertexData);
	/// <summary>’¸“_î•ñ‚É‚æ‚é•`‰æ</summary>
	void DrawIndexed(const VERTEX3D* VertexArray, int VertexNum, const unsigned short* IndexArray, int PolygonNum, int GrHandle, int TransFlag);
	/// <summary>‰æ‘œ•`‰æ</summary>
	void DrawData_Graph(DRAW_GRAPH_DATA drawData);

    /// <summary>•`‰æî•ñæ“¾</summary>
	DRAW_GRAPH_DATA GetDrawGraphData(int handle, int x, 		 int y);
	DRAW_GRAPH_DATA GetDrawGraphData(int handle, int x, 		 int y, 	   int sizeX, 		 int sizeY);
	DRAW_GRAPH_DATA GetDrawGraphData(int handle, int x, 		 int y,		   float sizeXRatio, float sizeYRatio);
	DRAW_GRAPH_DATA GetDrawGraphData(int handle, float xRatio, float yRatio);
	DRAW_GRAPH_DATA GetDrawGraphData(int handle, float xRatio, float yRatio, int sizeX, 		 int sizeY);
	DRAW_GRAPH_DATA GetDrawGraphData(int handle, float xRatio, float yRatio, float sizeXRatio, float sizeYRatio);
	DRAW_GRAPH_DATA GetDrawGraphData(int handle, Vector2_Int pos);
	DRAW_GRAPH_DATA GetDrawGraphData(int handle, Vector2_Int pos, Vector2_Int size);
	DRAW_GRAPH_DATA GetDrawGraphData(int handle, Vector2_Int leftUp, Vector2_Int rightUp, Vector2_Int leftDown, Vector2_Int rightDown);
};