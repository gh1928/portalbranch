#include "SceneMgr.h"
#include "StartMenu.h"
#include "EditScene.h"
#include "MapList.h"
#include "PlayScene.h"

bool SceneMgr::Init()
{
	Scene* startMenu = new StartMenu();

	sceneMap.insert({ Scenes::GAMESTART,startMenu });
	Scene* editScene = new EditScene();
	sceneMap.insert({ Scenes::MAPEDITER,editScene });
	Scene* mapList = new MapList();
	sceneMap.insert({ Scenes::MAPLIST,mapList });


	currScene = Scenes::GAMESTART;
	sceneMap[currScene]->Enter();

	return true;
}

Scene* SceneMgr::GetCurrScene()
{
	return sceneMap[currScene];
}

Scene* SceneMgr::GetScene(Scenes scene)
{
	return sceneMap[scene];
}

void SceneMgr::AddScene(string newscene)
{
	Scene* play = new PlayScene(newscene);
	sceneMap.insert({ Scenes::PLAY,play });
}

Scenes SceneMgr::GetCurrKey()
{
	return currScene;
}

void SceneMgr::ChangeScene(Scenes scene)
{
	if (currScene == Scenes::PLAY) {
		delete sceneMap[Scenes::PLAY];
		sceneMap.erase(Scenes::PLAY);
		
	}
	else
		sceneMap[currScene]->Exit();


	currScene = scene;
	sceneMap[currScene]->Enter();
}

void SceneMgr::Update(float dt)
{
	sceneMap[currScene]->Update(dt);
}

void SceneMgr::PhysicsUpdate(float dt)
{
	sceneMap[currScene]->PhysicsUpdate(dt);

}

void SceneMgr::Draw(RenderWindow& window)
{
	sceneMap[currScene]->Draw(window);
}
