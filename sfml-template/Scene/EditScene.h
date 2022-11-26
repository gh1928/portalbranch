#pragma once
#include "SFML/Graphics.hpp"
#include "Scene.h"
#include "../FrameWork/Utils.h"
#include <vector>
#include <array>
#include "../Objects/WireableObject.h"
#include "../Objects/WirePointArrows.h"
#include "../Objects/Wire.h"
#include "../Objects/NumBox.h"

class SpriteObj;
//class WireableObject;
//class WirePointArrows;
class EditScene : public Scene
{
public:
	EditScene();
	~EditScene();

	virtual void Init();	// ����
	virtual void Release();		// ����

	virtual void Enter();
	virtual void Exit();

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
public:
	void DrawWorldView(RenderWindow& window);
	void DrawUiView(RenderWindow& window);

	void InitMapTool();	
	void ReleaseMapTool();

	void SetMapToolPos();
	void SetMapToolSize();
	void FillMapTool();
	void UpdateMapTool(float dt);
	void DrawOutLine(RenderWindow& window);

	void InitWireMod();
	void UpdateWireMod(float dt);
	void FillNumBox();
	void DrawWireMod(RenderWindow& window);
	void DrawWireModMouseBox(RenderWindow& window);
	void LoadDataToWireableList();
	void RelaseWireableList();

	void Input(float dt);

	void InitUiTool();	
	void ReleaseUiTool();
	void UpdateUiTool(float dt);
	
	void FillUiToolBox();
	void SetUiToolPos(Vector2f pos);

	void MouseSpriteBoxUpdate();

	void UpdateSaveString();
	void LoadMapList();
	void DrawMapList(RenderWindow& window);

	void Reset();
	void Save();
	void Load();

private:	
	int rowNum;
	int colNum;
	Vector2f startPos;	

	array<array<pair<list<SpriteObj*>, SpriteObj*>, 100>, 100> mapTool;
	list<WireableObject*> wireableList;
	list<NumBox*> numBox;

	RectangleShape topLine;
	RectangleShape leftLine;
	RectangleShape rightLine;
	RectangleShape bottomLine;

	Sprite background;
	Sprite shadow;
	
	array<array<pair<SpriteObj*, Sprite>, 4>, 8> uiTool;
	
	Sprite uiOutLine;
	RectangleShape uiBackGround;
	Sprite uiMove;
	Sprite uiOpenClose;
	RectangleShape wiringModGui;

	Sprite uiTool2;
	
	Sprite saveMap;
	Sprite loadMap;
	Sprite resetMap;
	Sprite playMap;	
	vector<Sprite*> uiTool2List;


	SpriteObj* mouseBoxSprite;
	Sprite link;

	SpriteObj* mapToolCheckBox;
	SpriteObj* uiToolCheckBox;
	SpriteObj* uiTool2CheckBox;
	SpriteObj* crossUiCheckBox;

	bool isScenePlay;

	int zoomCount;

	bool isGridOn;
	bool isUiMoving;
	bool isUiOpen;
	bool mouseOnUi;	
	bool isWiring;

	bool isSaving;	

	String saveString;
	Text saveText;
	Sprite saveScreen;

	Text saveMsg;
	float msgTime;
	int msgTrasp;

	bool isLoading;

	string loadString;
	Sprite loadScreen;
	View loadView;
	list<pair<RectangleShape*, Text*>> loadList;
};

