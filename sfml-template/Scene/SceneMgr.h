#pragma once
#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <map>
#include "../3rd/Singleton.h"
#include "Scene.h"

using namespace sf;
using namespace std;

enum class Scenes
{
	GAMESTART,
	MAPEDITER,
	MAPLIST,
	PLAY,
};

class SceneMgr : public Singleton<SceneMgr>
{
private:
	map<Scenes, Scene*> sceneMap;
	Scenes currScene;

public:
	bool Init();

	Scene* GetCurrScene();
	Scene* GetScene(Scenes scene);
	void AddScene(string newscene);
	Scenes GetCurrKey();

	void ChangeScene(Scenes scene);

	void Update(float dt);
	void PhysicsUpdate(float dt);
	void Draw(RenderWindow& window);
};

#define SCENE_MGR (SceneMgr::GetInstance())