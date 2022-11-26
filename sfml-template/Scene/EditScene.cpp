#include "EditScene.h"
#include "../FrameWork/stdafx.h"
#include "SceneMgr.h"
#include "../FrameWork/InputMgr.h"
#include "../Objects/SpriteObj.h"
#include "../Manager/ResourceMgr.h"
#include "../FrameWork/Const.h"
#include "../FrameWork/Framework.h"
#include "../FrameWork/Utils.h"
#include "../FrameWork/ObjectHeaders.h"
#include <fstream>

#include <filesystem>
namespace fs = std::filesystem;

EditScene::EditScene()
{
	topLine.setFillColor(Color::Black);
	leftLine.setFillColor(Color::Black);
	rightLine.setFillColor(Color::Black);
	bottomLine.setFillColor(Color::Black);

	background.setTexture(*RESOURCEMGR->GetTexture("Graphics/backgrounds/ruin2.png"));	
	shadow.setTexture(*RESOURCEMGR->GetTexture("Graphics/Ui/shadow.png"));
	

	uiBackGround.setFillColor(Color(229,232,233,255));
	uiOutLine.setTexture(*RESOURCEMGR->GetTexture("Graphics/Ui/uioutline.png"));
	uiMove.setTexture(*RESOURCEMGR->GetTexture("Graphics/Ui/cross.png"));
	Utils::SetOrigin(uiMove, Origins::MC);
	uiOpenClose.setTexture(*RESOURCEMGR->GetTexture("Graphics/Ui/cross2.png"));
	Utils::SetOrigin(uiOpenClose, Origins::MC);
	wiringModGui.setFillColor(Color(37, 255, 254, 50));	
	//wiringModGui.setTexture(RESOURCEMGR->GetTexture("Graphics/Ui/wiringmod.png"));

	SetTex(uiTool2, "Graphics/Ui/uitool2.png");	
	SetTex(saveMap, "Graphics/Ui/save.png");
	SetTex(loadMap, "Graphics/Ui/load.png");
	SetTex(resetMap, "Graphics/Ui/reset.png");
	SetTex(playMap, "Graphics/Ui/play.png");
	Utils::SetSpriteSize(saveMap, { 75.f,75.f });
	Utils::SetSpriteSize(loadMap, { 75.f,75.f });
	Utils::SetSpriteSize(resetMap, { 75.f,75.f });
	Utils::SetSpriteSize(playMap, { 75.f,75.f });

	uiTool2List.push_back(&saveMap);
	uiTool2List.push_back(&loadMap);
	uiTool2List.push_back(&resetMap);
	uiTool2List.push_back(&playMap);

	mapToolCheckBox = new SpriteObj;
	mapToolCheckBox->SetResourceTexture("Graphics/Ui/checkbox.png");	
	mapToolCheckBox->FitScale(TILE_SIZE);

	uiToolCheckBox = new SpriteObj;
	uiToolCheckBox->SetResourceTexture("Graphics/Ui/checkbox.png");
	uiToolCheckBox->FitScale(TILE_SIZE * 3.f);

	uiTool2CheckBox = new SpriteObj;
	uiTool2CheckBox->SetResourceTexture("Graphics/Ui/checkbox.png");
	uiTool2CheckBox->FitScale(TILE_SIZE * 3.f);

	crossUiCheckBox = new SpriteObj;
	crossUiCheckBox->SetResourceTexture("Graphics/Ui/checkbox.png");
	crossUiCheckBox->FitScale(40.f);
	crossUiCheckBox->SetOrigin(Origins::MC);	

	link.setTexture(*RESOURCEMGR->GetTexture("Graphics/Ui/link.png"));

	saveText.setFont(*RESOURCEMGR->GetFont("Fonts/D-DINCondensed-Bold.otf"));
	saveText.setCharacterSize(100);
	saveText.setFillColor(Color::Black);
	saveText.setPosition({ WINDOW_WIDTH / 2 - 330 ,WINDOW_HEIGHT / 2  - 60});

	saveMsg.setFont(*RESOURCEMGR->GetFont("Fonts/D-DINCondensed-Bold.otf"));	
	saveMsg.setPosition({ WINDOW_WIDTH / 2  ,WINDOW_HEIGHT / 2 });
	saveMsg.setCharacterSize(75);

	SetTex(saveScreen, "Graphics/Ui/savebg.png");	
	SetTex(loadScreen, "Graphics/Ui/loadbg.png");
}

EditScene::~EditScene()
{
	Release();
	delete mouseBoxSprite;
	delete mapToolCheckBox;
	delete uiToolCheckBox;
	delete uiTool2CheckBox;
	delete crossUiCheckBox;
}

void EditScene::Init()
{		
	isScenePlay = true;
	rowNum = 20;
	colNum = 20;	

	isUiOpen = true;
	isGridOn = true;
	isWiring = false;
	SpriteObj::OnOffWiringState(false);

	mapToolCheckBox->SetActive(false);
	uiToolCheckBox->SetActive(false);
	uiTool2CheckBox->SetActive(false);

	isSaving = false;
	isLoading = false;

	//Vector2u tileSize = RESOURCEMGR->GetTexture("Graphics/grid.png")->getSize();
	Vector2u tileSize = { TILE_SIZE, TILE_SIZE };
	auto winSize = (Vector2f)FRAMEWORK->GetWindowSize();

	startPos.x = (winSize.x - rowNum * tileSize.x) * 0.5f;
	startPos.y = (winSize.y - colNum * tileSize.y) * 0.5f + (colNum - 1) * tileSize.y;

	InitMapTool();	
	InitWireMod();
	zoomCount = 0;

	Vector2f uiViewSize = uiView.getSize();
	Vector2f uiStartPos;
	uiStartPos.x = 75.f;
	uiStartPos.y = (uiViewSize.y - uiOutLine.getGlobalBounds().height) * 0.5f;
	uiOutLine.setPosition(uiStartPos);

	auto size = (Vector2f)FRAMEWORK->GetWindowSize();
	loadView.setSize(size);
	loadView.setCenter(size * 0.5f);

	mouseBoxSprite = nullptr;
	isUiMoving = false;
	mouseOnUi = false;

	saveString.clear();
	saveText.setString(saveString);

	InitUiTool();
}

void EditScene::Release()
{
	ReleaseMapTool();	
	ReleaseUiTool();
}

void EditScene::Enter()
{	
	cout << "Enter EditScene\n";	

	ofstream txt("Map/temp.txt");

	Scene::SetWorldView();
	Scene::SetUiView();
	Init();
	Button::SetIsEditMap(true);
}

void EditScene::Exit()
{
	cout << "Exit EditScene\n";
	isScenePlay = false;
	Release();
	Button::SetIsEditMap(false);
}

void EditScene::Update(float dt)
{		
	if (!isScenePlay)
		return;
	
	Input(dt);
	UpdateMapTool(dt);	
	UpdateWireMod(dt);
	UpdateUiTool(dt);
	UpdateSaveString();	

	MouseSpriteBoxUpdate();	

	if (msgTime < 0.f)
		return;

	msgTime -= dt;
		
	if (msgTime < 2.f)
	{
		msgTrasp -= 100 * dt;
		saveMsg.setFillColor(Color(0, 0, 0, msgTrasp));
	}
}

void EditScene::Draw(RenderWindow& window)
{	
	DrawWorldView(window);
	DrawUiView(window);	
}

void EditScene::DrawWorldView(RenderWindow& window)
{
	window.setView(worldView);
	window.draw(shadow);
	window.draw(background);

	for (int i = 0; i < colNum; i++)
	{
		for (int j = 0; j < rowNum; j++)
		{
			if (!mapTool[i][j].first.empty())
			{
				for (auto array : mapTool[i][j].first)
				{
					if (array->GetActive() && array != nullptr)
						array->Draw(window);
				}
			}

			if (mapTool[i][j].second->GetActive() && isGridOn)
				mapTool[i][j].second->Draw(window);
		}
	}	
	DrawOutLine(window);

	if (mapToolCheckBox->GetActive())
		mapToolCheckBox->Draw(window);

	DrawWireMod(window);
}

void EditScene::DrawUiView(RenderWindow& window)
{
	window.setView(uiView);

	window.draw(uiBackGround);

	for (int i = 0; i < uiTool.size(); i++)
	{
		for (int j = 0; j < uiTool[i].size(); j++)
		{
			if (uiTool[i][j].first != nullptr && uiTool[i][j].first->GetActive())
				uiTool[i][j].first->Draw(window);

			window.draw(uiTool[i][j].second);
		}
	}

	if (isWiring)
		window.draw(wiringModGui);

	window.draw(uiOutLine);
	
	if (uiToolCheckBox->GetActive() && !isWiring)
		uiToolCheckBox->Draw(window);

	window.draw(uiTool2);
	window.draw(saveMap);
	window.draw(loadMap);
	window.draw(resetMap);
	window.draw(playMap);

	if (uiTool2CheckBox->GetActive() && !isWiring)
		uiTool2CheckBox->Draw(window);

	window.draw(uiMove);
	window.draw(uiOpenClose);

	if (crossUiCheckBox->GetActive())
		crossUiCheckBox->Draw(window);

	if (mouseBoxSprite != nullptr)
		mouseBoxSprite->Draw(window);

	DrawWireModMouseBox(window);

	if (isSaving)
	{
		window.draw(saveScreen);
		window.draw(saveText);
	}

	if (isLoading)
	{
		window.draw(loadScreen);
	}
		
	if (msgTime > 0 && !isSaving && !isLoading)
		window.draw(saveMsg);

	DrawMapList(window);
}

void EditScene::InitMapTool()
{
	for (int i = 0; i < mapTool.size(); i++)
	{
		for (int j = 0; j < mapTool[i].size(); j++)
		{
			mapTool[i][j].second = new SpriteObj;
			mapTool[i][j].second->SetResourceTexture("Graphics/grid.png");
			mapTool[i][j].second->FitScale(TILE_SIZE);
			mapTool[i][j].second->SetActive(false);
		}
	}

	for (int i = 0; i < colNum; i++)
	{
		for (int j = 0; j < rowNum; j++)
		{
			mapTool[i][j].second->SetActive(true);
		}
	}

	Vector2f reSize = { (float)TILE_SIZE * rowNum, (float)TILE_SIZE * colNum };
	Utils::SetSpriteSize(background, reSize);
	Utils::SetSpriteSize(shadow, reSize * 1.01f);

	SetMapToolPos();
}

void EditScene::ReleaseMapTool()
{
	for (int i = 0; i < mapTool.size(); i++)
	{
		for (int j = 0; j < mapTool[i].size(); j++)
		{
			delete mapTool[i][j].second;

			for (auto array : mapTool[i][j].first)
			{
				if (array != nullptr)
				{
					delete array;					
				}					
			}
			mapTool[i][j].first.clear();
		}
	}
}

void EditScene::SetMapToolPos()
{
	if (mapTool.empty())
		return;
	
	//Vector2u size = RESOURCEMGR->GetTexture("Graphics/grid.png")->getSize();	
	Vector2u tileSize = { TILE_SIZE, TILE_SIZE };

	for (int i = 0; i < colNum; i++)
	{
		for (int j = 0; j < rowNum; j++)
		{
			Vector2f pos = { (float)tileSize.x * j, (float)tileSize.y * (-i) };
			mapTool[i][j].second->SetPos(pos + startPos);
		}
	}

	background.setPosition(mapTool[colNum - 1][0].second->GetPos());
	Vector2f shadowPosFix{ 7.f, 7.f };
	shadow.setPosition(mapTool[colNum - 1][0].second->GetPos() + shadowPosFix);

	topLine.setPosition(mapTool[colNum - 1][0].second->GetPos());
	topLine.setSize({ (float)tileSize.x * rowNum , 1.f });
	leftLine.setPosition(topLine.getPosition());
	leftLine.setSize({ 1.f, (float)tileSize.y * colNum });

	Vector2f fixPos;
	fixPos = { (float)tileSize.x, 0.f };
	rightLine.setPosition(mapTool[colNum - 1][rowNum - 1].second->GetPos() + fixPos);
	rightLine.setSize(leftLine.getSize());	
	fixPos = { 0.f, (float)tileSize.y - 1.f};
	bottomLine.setPosition(mapTool[0][0].second->GetPos() + fixPos);
	bottomLine.setSize(topLine.getSize());
}

void EditScene::SetMapToolSize()
{
	for (int i = 0; i < mapTool.size(); i++)
	{
		for (int j = 0; j < mapTool[i].size(); j++)
		{
			mapTool[i][j].second->SetActive(false);

			for (auto array : mapTool[i][j].first)
			{
				array->SetActive(false);
			}
		}
	}

	for (int i = 0; i < colNum; i++)
	{
		for (int j = 0; j < rowNum; j++)
		{
			mapTool[i][j].second->SetActive(true);

			for (auto array : mapTool[i][j].first)
			{
				array->SetActive(true);
			}
		}
	}

	Vector2f reSize = { (float)TILE_SIZE * rowNum, (float)TILE_SIZE * colNum };
	Utils::SetSpriteSize(background, reSize);
	Utils::SetSpriteSize(shadow, reSize * 1.01f);

	SetMapToolPos();
}

void EditScene::FillMapTool()
{
	if (isWiring || isSaving || isLoading)
		return;

	Vector2f mousePos = ScreenToWorldPos((Vector2i)InputMgr::GetMousePos());
	Vector2f fixPos{ TILE_SIZE / 2,TILE_SIZE };
	
	for (int i = 0; i < colNum; i++)
	{
		for (int j = 0; j < rowNum; j++)
		{		
			if (mapTool[i][j].second->GetGlobalBounds().contains(mousePos) &&
				mouseBoxSprite != nullptr &&
				InputMgr::GetMouseButton(Mouse::Left) &&
				!mouseOnUi)
			{
	
				if (!mapTool[i][j].first.empty())
				{
					for (auto tool : mapTool[i][j].first)
					{
						if (mouseBoxSprite->GetId() == tool->GetId() &&
							mouseBoxSprite->GetRotation() == tool->GetRotation())
						{
							return;
						}
					}

					auto it = mapTool[i][j].first.begin();
					while (it != mapTool[i][j].first.end())
					{
						if (mouseBoxSprite->GetId() != (*it)->GetId() &&
							mouseBoxSprite->GetRotation() == (*it)->GetRotation())
						{
							SpriteObj* temp = *it;
							mapTool[i][j].first.erase(it);
							delete temp;
							break;
						}						
						else ++it;
					}

					if (mouseBoxSprite->GetObjSize() == ObjectSize::Big)
					{
						for (auto tool : mapTool[i][j].first)
						{
							delete tool;
						}
						mapTool[i][j].first.clear();
					}
				}		

				if (!mapTool[i][j].first.empty())
				{
					if (mapTool[i][j].first.front()->GetObjSize() == ObjectSize::Big)
					{
						delete mapTool[i][j].first.front();
						mapTool[i][j].first.pop_front();
					}
				}

				mapTool[i][j].first.push_front(mouseBoxSprite->NewThis());
				mapTool[i][j].first.front()->FitScale(TILE_SIZE);
				mapTool[i][j].first.front()->SetOrigin(Origins::BC);
				mapTool[i][j].first.front()->SetBoolInMapTool(true);
				mapTool[i][j].first.front()->SetPos(mapTool[i][j].second->GetPos() + fixPos);
				if (mapTool[i][j].first.front()->GetObjSize() == ObjectSize::Normal)
				{
					int rotNum = (int)mouseBoxSprite->GetRotation();
					mapTool[i][j].first.front()->SetRotationInBox(rotNum, TILE_SIZE, mapTool[i][j].second->GetPos());
				}

				if (mapTool[i][j].first.front()->GetObjType() == ObjectType::Trigger)
				{
					Button* temp = (Button*)mapTool[i][j].first.front();
					temp->AddNumBox(Button::GetButtonNum() - 1);
				}
				mapTool[i][j].first.front()->Init();

				break;
			}

			if (mapTool[i][j].second->GetGlobalBounds().contains(mousePos) &&
				mouseBoxSprite == nullptr &&
				InputMgr::GetMouseButton(Mouse::Right) &&
				!mapTool[i][j].first.empty())
			{			
				for (auto tool : mapTool[i][j].first)
				{
					delete tool;
				}
				mapTool[i][j].first.clear();
				break;
			}
		}
	}
}

void EditScene::UpdateMapTool(float dt)
{
	if (!isScenePlay)
		return;

	FillMapTool();

	Vector2f mousePos = ScreenToWorldPos((Vector2i)InputMgr::GetMousePos());
	mapToolCheckBox->SetActive(false);

	for (int i = 0; i < colNum; i++)
	{
		for (int j = 0; j < rowNum; j++)
		{
			if (mapTool[i][j].second->GetGlobalBounds().contains(mousePos) &&
				!mouseOnUi)
			{
				mapToolCheckBox->SetActive(true);
				mapToolCheckBox->SetPos(mapTool[i][j].second->GetPos());
				break;
			}
		}
	}
}

void EditScene::Input(float dt)
{
	if (InputMgr::GetKeyDown(Keyboard::Escape))
	{
		if(!isSaving && !isLoading)
			SCENE_MGR->ChangeScene(Scenes::GAMESTART);		
	}

	if (InputMgr::GetKeyDown(Keyboard::Tab))
	{
		exit(1);
	}

	if (InputMgr::GetMouseWheelState() == 1)
	{
		if (zoomCount > 20 || isLoading)
			return;
		zoomCount++;
		worldView.zoom(0.94f);		
	}
	if (InputMgr::GetMouseWheelState() == -1)
	{		
		if (zoomCount < -3 || isLoading)
			return;
		zoomCount--;
		worldView.zoom(1.06f);
	}

	if (InputMgr::GetKeyDown(Keyboard::Numpad6) ||
		InputMgr::GetKeyDown(Keyboard::Right))
	{
		rowNum++;
		SetMapToolSize();
	}
	if (InputMgr::GetKeyDown(Keyboard::Numpad4) ||
		InputMgr::GetKeyDown(Keyboard::Left))
	{
		if (rowNum <= 1)
			return;
		rowNum--;
		SetMapToolSize();
	}
	if (InputMgr::GetKeyDown(Keyboard::Numpad8) ||
		InputMgr::GetKeyDown(Keyboard::Up))
	{
		colNum++;
		SetMapToolSize();
	}
	if (InputMgr::GetKeyDown(Keyboard::Numpad2) ||
		InputMgr::GetKeyDown(Keyboard::Down))
	{
		if (colNum <= 1)
			return;
		colNum--;
		SetMapToolSize();
	}

	if (InputMgr::GetMouseButton(Mouse::Middle))
	{
		Vector2f pos = InputMgr::GetMousePosDisplacement();
		worldView.setCenter(worldView.getCenter() + pos);
	}

	if (InputMgr::GetMouseButtonDown(Mouse::Left)
		&& uiMove.getGlobalBounds().contains(InputMgr::GetMousePos()))
	{
		isUiMoving = true;
	}

	if (isUiMoving && InputMgr::GetMouseButton(Mouse::Left))
	{
		SetUiToolPos(InputMgr::GetMousePos());
	}

	if (InputMgr::GetMouseButtonUp(Mouse::Left))
	{
		isUiMoving = false;
	}

	if (InputMgr::GetKeyDown(Keyboard::F5))
	{
		isSaving = true;
	}

	if (InputMgr::GetKeyDown(Keyboard::F9))
	{
		isLoading = true;		
		LoadMapList();
	}

	if (InputMgr::GetKeyDown(Keyboard::R))
	{
		if (mouseBoxSprite != nullptr &&
			mouseBoxSprite->GetObjSize() == ObjectSize::Normal)
		{
			int rot = (int)mouseBoxSprite->GetRotation();
			mouseBoxSprite->SetRotationInBox((rot + 1) % 4, 50.f, InputMgr::GetMousePos());
		}
	}

	if (InputMgr::GetKeyDown(Keyboard::Num1))
	{
		isGridOn = !isGridOn;
	}
	if (InputMgr::GetKeyDown(Keyboard::Num2))
	{		
		SpriteObj::OnOffWiringState(isWiring = !isWiring);
		isWiring ? LoadDataToWireableList() : RelaseWireableList();
	}
	if (InputMgr::GetKeyDown(Keyboard::A) &&
		isWiring &&
		numBox.front()->GetNum() > 1)
	{
		numBox.front()->SetNum(numBox.front()->GetNum() - 1);
	}

	if (InputMgr::GetKeyDown(Keyboard::D) &&
		isWiring &&
		numBox.front()->GetNum() < Button::GetButtonNum())
	{
		numBox.front()->SetNum(numBox.front()->GetNum() + 1);
	}
}

void EditScene::DrawOutLine(RenderWindow& window)
{
	window.draw(topLine);
	window.draw(leftLine);
	window.draw(rightLine);
	window.draw(bottomLine);
}

void EditScene::InitWireMod()
{
	numBox.push_back(new NumBox);
	numBox.back()->SetNum(1);
}

void EditScene::UpdateWireMod(float dt)
{
	if (!isWiring || !isScenePlay)
		return;

	numBox.front()->SetPos(GetMouseWorldPos());
	FillNumBox();
}

void EditScene::FillNumBox()
{
	if (wireableList.empty())
		return;

	for (auto wlist : wireableList)
	{
		if (wlist->IsMouseIn()
			&& InputMgr::GetMouseButtonDown(Mouse::Left))
		{
			wlist->AddNumBox(numBox.front());
		}
	}
}

void EditScene::DrawWireMod(RenderWindow& window)
{
	if (!isWiring)
		return;

	for (auto i : numBox)
	{
		i->Draw(window);
	}
}

void EditScene::DrawWireModMouseBox(RenderWindow& window)
{
	if (!isWiring)
		return;


}

void EditScene::LoadDataToWireableList()
{
	for (int i = 0; i < colNum; i++)
	{
		for (int j = 0; j < rowNum; j++)
		{
			if (!mapTool[i][j].first.empty())
			{
				for (auto tool : mapTool[i][j].first)
				{
					if (tool->GetObjType() == ObjectType::Catcher)
					{
						wireableList.push_back((WireableObject*)tool);
					}
				}
			}
		}
	}
}

void EditScene::RelaseWireableList()
{
	wireableList.clear();
}

void EditScene::InitUiTool()
{
	FloatRect rect = uiOutLine.getLocalBounds();
	uiBackGround.setSize({ rect.width, rect.height });
	wiringModGui.setSize(uiBackGround.getSize());

	Vector2f uiStartPos = uiOutLine.getPosition();
	uiBackGround.setPosition(uiStartPos);
	wiringModGui.setPosition(uiStartPos);
	uiMove.setPosition(uiStartPos);
	
	Vector2f uiEndPos{ uiStartPos.x + rect.width, uiStartPos.y + rect.height };

	uiOpenClose.setPosition(uiEndPos);

	for (int i = 0; i < uiTool.size(); i++)
	{
		for (int j = 0; j < uiTool[i].size(); j++)
		{
			uiTool[i][j].first = nullptr;

			uiTool[i][j].second.setTexture(*RESOURCEMGR->GetTexture("Graphics/grid.png"));
			uiTool[i][j].second.setScale({ 1.5f,1.5f });			

			Vector2f setPos = { 75.f * j, 75.f * i };
			uiTool[i][j].second.setPosition(setPos + uiStartPos);
		}
	}

	FillUiToolBox();

	Vector2f fixPos = {75.f *0.5f, 75.f *0.5f};
	for (int i = 0; i < uiTool.size(); i++)
	{
		for (int j = 0; j < uiTool[i].size(); j++)
		{
			if (uiTool[i][j].first != nullptr)
			{
				uiTool[i][j].first->FitScale(75.f);
				uiTool[i][j].first->SetOrigin(Origins::MC);
				uiTool[i][j].first->SetPos(uiTool[i][j].second.getPosition() + fixPos);
			}
		}
	}

	Vector2u uitoolSize = uiTool2.getTexture()->getSize();
	Vector2f tool2FixPos = { 75.f,0.f };

	uiTool2.setPosition(WINDOW_WIDTH - uitoolSize.x - 10.f, WINDOW_HEIGHT - uitoolSize.y - 10.f);
	saveMap.setPosition(uiTool2.getPosition());	
	loadMap.setPosition(saveMap.getPosition() + tool2FixPos);
	resetMap.setPosition(loadMap.getPosition() + tool2FixPos);
	playMap.setPosition(resetMap.getPosition() + tool2FixPos);
}

void EditScene::ReleaseUiTool()
{
	for (int i = 0; i < uiTool.size(); i++)
	{
		for (int j = 0; j < uiTool[i].size(); j++)
		{
			if (uiTool[i][j].first != nullptr)
				delete uiTool[i][j].first;
		}
	}

	for (auto list : loadList)
	{
		if(list.first != nullptr)
			delete list.first;
		if(list.second != nullptr)
			delete list.second;		
	}
}

void EditScene::UpdateUiTool(float dt)
{
	if (isSaving || isLoading)
		return;

	uiToolCheckBox->SetActive(false);
	uiTool2CheckBox->SetActive(false);
	crossUiCheckBox->SetActive(false);
	mouseOnUi = false;

	Vector2f mousePos = InputMgr::GetMousePos();

	for (int i = 0; i < uiTool2List.size(); i++)
	{
		if (uiTool2List[i]->getGlobalBounds().contains(mousePos))
		{
			mouseOnUi = true;
			uiTool2CheckBox->SetActive(true);
			uiTool2CheckBox->SetPos(uiTool2List[i]->getPosition());
			if (InputMgr::GetMouseButtonDown(Mouse::Left))
			{
				switch (i)
				{
				case 0:
					isSaving = true;
					break;
				case 1:
					isLoading = true;	
					LoadMapList();
					break;
				case 2:
					Reset();
					break;
				case 3:
					//Play();
					break;
				}
			}
			return;
		}
	}

	if (uiMove.getGlobalBounds().contains(mousePos))
	{
		mouseOnUi = true;
		crossUiCheckBox->SetPos(uiMove.getPosition());
		crossUiCheckBox->SetActive(true);
	}

	if (uiOpenClose.getGlobalBounds().contains(mousePos))
	{
		mouseOnUi = true;
		crossUiCheckBox->SetPos(uiOpenClose.getPosition());
		crossUiCheckBox->SetActive(true);
	}

	if (uiOpenClose.getGlobalBounds().contains(mousePos) &&
		InputMgr::GetMouseButtonUp(Mouse::Left))
	{
		isUiOpen = !isUiOpen;
		if (!isUiOpen)
		{
			Vector2f pos{ -1000.f,-1000.f };
			uiOutLine.setPosition(pos);
			uiBackGround.setPosition(pos);	
			wiringModGui.setPosition(pos);
			
			for (int i = 0; i < uiTool.size(); i++)
			{
				for (int j = 0; j < uiTool[i].size(); j++)
				{					
					uiTool[i][j].second.setPosition(pos);
					if (uiTool[i][j].first != nullptr)
						uiTool[i][j].first->SetPos(uiTool[i][j].second.getPosition());
				}
			}			
		}
		SetUiToolPos(uiMove.getPosition());
	}


	if (!isUiOpen)
		return;

	bool mouseOnCrossBox =
	uiMove.getGlobalBounds().contains(mousePos) ||
	uiOpenClose.getGlobalBounds().contains(mousePos);

	for (int i = 0; i < uiTool.size(); i++)
	{
		for (int j = 0; j < uiTool[i].size(); j++)
		{
			if (uiTool[i][j].second.getGlobalBounds().contains(mousePos) &&
				!mouseOnCrossBox)
			{
				mouseOnUi = true;
				uiToolCheckBox->SetActive(true);
				uiToolCheckBox->SetPos(uiTool[i][j].second.getPosition());
				break;
			}
		}
	}

	//for (int i = 0; i < uiTool.size(); i++)
	//{
	//	for (int j = 0; j < uiTool[i].size(); j++)
	//	{
	//		if (uiTool[i][j].first != nullptr && uiTool[i][j].first->GetActive())
	//			uiTool[i][j].first->Update(dt);
	//	}
	//}
}

void EditScene::UpdateSaveString()
{
	if (!isSaving)
		return;	

	if (InputMgr::GetKeyDown(Keyboard::Enter))
	{
		isSaving = false;
		Save();
		return;
	}

	bool backSpace = false;

	if (InputMgr::GetKeyDown(Keyboard::Escape))
	{
		backSpace = true;
	}

	if (InputMgr::GetKeyUp(Keyboard::Escape))
	{
		isSaving = false;	
		saveString.clear();
		saveText.setString(saveString);
		return;
	}

	if (InputMgr::GetKeyDown(Keyboard::BackSpace))
	{
		backSpace = true;
		if (!saveString.isEmpty())
		{			
			saveString.erase(saveString.getSize() - 1);
		}
	}

	if (char temp = InputMgr::GetTextInput())
	{
		if (!backSpace &&
			saveString.getSize() < 13)
			saveString += temp;

		saveText.setString(saveString);
	}	
}

void EditScene::LoadMapList()
{
	for (auto list : loadList)
	{
		if (list.first != nullptr)
			delete list.first;
		if (list.second != nullptr)
			delete list.second;
	}
	loadList.clear();

	string path = "Map";
	for (const auto& entry : fs::directory_iterator(path)) 
	{
		Vector2f pos;
		if (loadList.empty())
			pos = { WINDOW_WIDTH * 0.5f, 100.f };
		else
			pos = loadList.back().first->getPosition() + Vector2f{0.f, 130.f};		

		string path_string = fs::path(entry).filename().string();
		for(int i = 0; i <4; i++)
			path_string.pop_back();
		
		loadList.push_back({ new RectangleShape, new Text });
		loadList.back().first->setFillColor(Color::White);
		loadList.back().first->setSize({ 400.f,75.f });
		loadList.back().first->setOutlineColor(Color::Black);
		loadList.back().first->setOutlineThickness(5);
		Utils::SetOrigin(*loadList.back().first, Origins::MC);
		loadList.back().first->setPosition(pos);

		loadList.back().second->setString(path_string);
		loadList.back().second->setFillColor(Color::Black);
		loadList.back().second->setFont(*RESOURCEMGR->GetFont("Fonts/D-DINCondensed-Bold.otf"));
		Utils::SetOrigin(*loadList.back().second, Origins::MC);		
		loadList.back().second->setCharacterSize(50);
		loadList.back().second->setPosition(pos + Vector2f{0.f,-20});
	}
}

void EditScene::DrawMapList(RenderWindow& window)
{
	if (!isLoading)
		return;

	if (InputMgr::GetKeyUp(Keyboard::Escape) ||
		InputMgr::GetMouseButtonDown(Mouse::Right))
	{
		isLoading = false;
		return;
	}

	if (InputMgr::GetMouseWheelState() == 1 &&
		loadView.getCenter().y > WINDOW_HEIGHT * 0.5f)
	{
		loadView.setCenter(loadView.getCenter() - Vector2f(0, 30.f));
	}
	if (InputMgr::GetMouseWheelState() == -1 &&
		loadView.getCenter().y < loadList.back().first->getPosition().y)
	{
		loadView.setCenter(loadView.getCenter() + Vector2f(0, 30.f));
	}

	window.setView(loadView);
		
	Vector2f mouse = window.mapPixelToCoords((Vector2i)InputMgr::GetMousePos(), loadView);

	for (auto lst : loadList)
	{
		if (lst.first->getGlobalBounds().contains(mouse))
		{
			lst.first->setOutlineColor(Color(248, 147, 30));
			if (InputMgr::GetMouseButtonDown(Mouse::Left))
			{
				loadString = lst.second->getString();
				isLoading = false;
				Load();
			}
		}
		else
			lst.first->setOutlineColor(Color::Black);

		window.draw(*lst.first);
		window.draw(*lst.second);
	}	
}

void EditScene::FillUiToolBox()
{
	uiTool[0][0].first = new Tile;
	uiTool[0][0].first->SetResourceTexture("Graphics/Ui/tile.png");

	uiTool[0][1].first = new Cube;
	uiTool[0][1].first->SetResourceTexture("Graphics/Ui/cube.png");

	uiTool[0][2].first = new Player;

	uiTool[0][3].first = new Button;	
	uiTool[0][3].first->SetResourceTexture("Graphics/Ui/button.png");
	Button::SetButtonNum(0);

	uiTool[1][0].first = new Goal;
	uiTool[1][0].first->SetResourceTexture("Graphics/Ui/goal.png");	
}

void EditScene::SetUiToolPos(Vector2f pos)
{
	uiMove.setPosition(pos);	

	if (!isUiOpen)
	{
		Vector2f fixpos{ 40.f,0.f };
		uiOpenClose.setPosition(pos + fixpos);
		return;
	}

	uiOutLine.setPosition(pos);
	uiBackGround.setPosition(pos);
	wiringModGui.setPosition(pos);

	FloatRect rect = uiOutLine.getLocalBounds();
	Vector2f uiEndPos{ pos.x + rect.width, pos.y + rect.height };
	uiOpenClose.setPosition(uiEndPos);	

	Vector2f fixPos = { 75.f * 0.5f, 75.f * 0.5f };
	for (int i = 0; i < uiTool.size(); i++)
	{
		for (int j = 0; j < uiTool[i].size(); j++)
		{
			Vector2f setPos = { 75.f * j, 75.f * i };
			uiTool[i][j].second.setPosition(setPos + pos);
			if (uiTool[i][j].first != nullptr)
				uiTool[i][j].first->SetPos(uiTool[i][j].second.getPosition() + fixPos);
		}
	}
}

void EditScene::MouseSpriteBoxUpdate()
{
	if (isWiring)
	{
		if (mouseBoxSprite != nullptr)
		{
			delete mouseBoxSprite;
			mouseBoxSprite = nullptr;
		}
		return;
	}

	Vector2f mousePos = InputMgr::GetMousePos();

	for (int i = 0; i < uiTool.size(); i++)
	{
		for (int j = 0; j < uiTool[i].size(); j++)
		{
			if (uiTool[i][j].first != nullptr &&
				uiTool[i][j].second.getGlobalBounds().contains(mousePos) &&
				InputMgr::GetMouseButtonDown(Mouse::Left)&&
				!uiMove.getGlobalBounds().contains(InputMgr::GetMousePos()))
			{
				if (mouseBoxSprite != nullptr)
				{								
					delete mouseBoxSprite;
				}			

				mouseBoxSprite = uiTool[i][j].first->NewThis();
				mouseBoxSprite->FitScale(50.f);				
			}
		}
	}

	if (InputMgr::GetMouseButtonUp(Mouse::Right) &&
		mouseBoxSprite != nullptr)
	{
		delete mouseBoxSprite;
		mouseBoxSprite = nullptr;
	}

	if (mouseBoxSprite != nullptr)
		mouseBoxSprite->SetPos(mousePos);
}

void EditScene::Reset()
{
	ReleaseMapTool();
	InitMapTool();
}

void EditScene::Save()
{		
	msgTime = 4.f;
	msgTrasp = 255;
	saveMsg.setFillColor(Color::Black);

	string mapName = "Map/";
	mapName += saveString.isEmpty() ? "temp" : saveString;
	mapName += ".txt";

	saveString.clear();
	saveText.setString(saveString);	

	int playerNum = 0;
	int goalNum = 0;

	for (int i = 0; i < colNum; i++)
	{
		for (int j = 0; j < rowNum; j++)
		{
			if (!mapTool[i][j].first.empty())
			{
				for (auto obj : mapTool[i][j].first)
				{
					if(obj->GetId() == 'p')
						playerNum++;
					if (obj->GetId() == '@')
						goalNum++;
				}
			}
		}
	}

	if (playerNum != 1 || goalNum != 1)
	{				
		saveMsg.setString("Save Failed : Player or Eixt must be one");
		Utils::SetOrigin(saveMsg, Origins::MC);		
		return;
	}

	ofstream map(mapName);
	
	for (int i = colNum - 1; i >= 0 ; i--)
	{
		for (int j = 0; j < rowNum; j++)
		{
			vector<string> strs;
			for (int k = 0; k < 4; k++)
			{
				strs.push_back(":0");
			}

			for (auto obj : mapTool[i][j].first)
			{
				for (int k = 0; k < strs.size(); k++)
				{
					if ((int)obj->GetRotation() == k)
					{
						strs[k].pop_back();
						strs[k] += obj->GetId();
						if (obj->GetObjType() == ObjectType::Trigger ||
							obj->GetObjType() == ObjectType::Catcher)
						{
							strs[k] += '(';
							WireableObject* wobj = (WireableObject*)obj;
							for (auto w : wobj->GetWireListFromMapTool())
							{
								strs[k] += to_string(w) + ' ';
							}
							strs[k] += ')';
						}
					}
				}
			}	

			map << '[';
			for (int k = 0; k < strs.size(); k++)
			{
				map << strs[k];
			}
			map << ']';
		}
		if (i == 0)
			continue;
		map << '\n';
	}	

	saveMsg.setString("Save succeed");
	Utils::SetOrigin(saveMsg, Origins::MC);	
}

void EditScene::Load()
{	
	Reset();

	string mapName;

	mapName += "Map/" + loadString + ".txt";
	ifstream map(mapName);
	string s;

	colNum = 0;
	rowNum = 0;

	Vector2f fixPos{ TILE_SIZE / 2,TILE_SIZE };

	//while (getline(txt, s))
	//{		
	//	int row = 0;
	//	for (int i = 0; i < s.size(); i++)
	//	{
	//		switch (s[i])
	//		{
	//		case '0':
	//			break;
	//		case '1':
	//			mapTool[colNum][row].first.push_front(new Tile);
	//			break;
	//		case '@':
	//			mapTool[colNum][row].first.push_front(new Goal);
	//			mapTool[colNum][row].first.front()->SetPos(mapTool[colNum][row].second->GetPos() + fixPos);
	//			i += 2;
	//			while (s[i] != ')')
	//			{
	//				WireableObject* tempObj = (WireableObject*)mapTool[colNum][row].first.front();					
	//				string tempStr;
	//				while (s[i] != ' ')
	//				{
	//					tempStr += s[i];
	//					i++;
	//				}
	//				tempObj->AddNumBox(stoi(tempStr));
	//				i++;
	//			}				
	//			break;
	//		case 'b':
	//			mapTool[colNum][row].first.push_front(new Button);
	//			mapTool[colNum][row].first.front()->SetPos(mapTool[colNum][row].second->GetPos() + fixPos);
	//			i += 5;
	//			while (s[i] != ')')
	//			{
	//				WireableObject* tempObj = (WireableObject*)mapTool[colNum][row].first.front();
	//				string tempStr;
	//				while (s[i] != ' ')
	//				{
	//					tempStr += s[i];
	//					i++;
	//				}
	//				tempObj->AddNumBox(stoi(tempStr));
	//				i++;
	//			}				
	//			break;
	//		}
	//		if (!mapTool[colNum][row].first.empty())
	//		{
	//			mapTool[colNum][row].first.front()->FitScale(TILE_SIZE);
	//			mapTool[colNum][row].first.front()->SetOrigin(Origins::BC);
	//			mapTool[colNum][row].first.front()->SetBoolInMapTool(true);
	//			mapTool[colNum][row].first.front()->SetPos(mapTool[colNum][row].second->GetPos() + fixPos);
	//			mapTool[colNum][row].first.front()->Init();
	//		}
	//		row++;
	//	}
	//	++colNum;
	//}
	
	bool noneObj = false;
	for (int i = 0; i < s.size(); i++)
	{
		++rowNum;
		if (s[i] == '(')
		{
			noneObj = true;
		}
		if (s[i] == ')')
		{
			noneObj = false;
			--rowNum;
		}
		if (s[i] == '\"')
		{
			noneObj = !noneObj;
			if (!noneObj)
				--rowNum;
		}			

		if (noneObj)
			--rowNum;
	}	

	cout << "세로: " << colNum << "가로: " << rowNum << endl;

	SetMapToolSize();
}
