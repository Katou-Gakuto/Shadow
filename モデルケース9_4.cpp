
class MapData
{
	// マップの情報が入っている.binのパスを渡すことでマップを自動生成する関数
};

class Scene
{
	MapData* data;
};

class SceneManager
{
	Scene* memberArray;
};

class GameObject
{
	// Updateを毎フレーム呼ぶか
	// trueはキャラクター
	// falseはオブジェクト

	// 座標
};

class GameObjectManager
{
	// 
	GameObject* member;


	// 座標から操作できるButtonObjectを走査する関数。引数はキャラクターの座標
};

class GameCharacter : GameObject
{

};

class CharacterShadow : GameCharacter
{

};

class CharacterLight : GameCharacter
{

};

class MapObject : GameObject
{
	// Update()は空
	// 
	// Simulation()を用意しこのSimulationに触るのはButtonだけ
	// ↑この中身の変更方法で悩み中
	// ・Buttonが渡す引数に処理を変更させる
	// ・MapObjectを継承させまくって1オブジェクト1クラスにする
	// ・ラムダを入れる
	// ・関数ポインタを持たせる	
	// 
	// SimulationChange(SimulationResult &result);
	// 
	// 

};

class LightObject : MapObject
{
	// bool 光ってるか
	// 
	// color 光の色
	// 
	// std::vector<VECTOR2D> 光範囲の頂点を管理する動的配列
};

class AllAngleLight : LightObject
{

};

class AngleLight : LightObject
{

};

class WallObject : MapObject
{

};

class ButtonObject : MapObject
{
	// ActionStart()
	// ↑これでMapObjectのAction()を呼ぶ
	// 
	// 
	// 


};
