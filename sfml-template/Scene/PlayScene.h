#pragma once
#include "Scene.h"
#include "SFML/Graphics.hpp"
#include <vector>
#include "../FrameWork/Const.h"
#include "../FrameWork/ObjectHeaders.h"
#include "../Objects/Goal.h"
#include <box2d/box2d.h>
#include "../Particle/ParticleSystem.h"

//////////////////////////
#include <Candle/Candle.hpp>
#include <memory>
using namespace candle;
/////////////////////////

struct Light
{
	Light(sf::Vector3f col, sf::Vector3f pos, sf::Vector3f att) : color(col),
		position(pos),
		attenuation(att)
	{
	}
	sf::Vector3f color;
	sf::Vector3f position;
	sf::Vector3f attenuation;
};

class PlayScene : public Scene
{
public:
	//PlayScene() {};
	PlayScene(string path);
	virtual ~PlayScene();

	virtual void Init();	// ����
	virtual void Release();		// ����

	virtual void Enter();
	virtual void Exit();

	virtual void Update(float dt);
	virtual void PhysicsUpdate(float dt);

	virtual void Draw(RenderWindow& window);

	void MakeWall(bool isEnd);
	void MakeCube();
	void MakePlayer();
	void MakeButton(string dir, string id);
	void MakePortal();
	void MakeGoal(string list);
	void PushButton();
	Vector2f CameraMove(Vector2f currpos, Vector2f playerpos,float, float dt);

	void MoveToPortal();
	
public:
	void DrawBackGroundView(RenderWindow& window);
	void DrawRenderedBuffer(RenderWindow& window);
	void Input();
	void LightTestInputForDev();
	void ClearRenderBuffer();
	
private:
	Player* player;
	Goal* goal;
	vector<Tile*> wall;
	vector<Button*> button;
	vector<Cube*> cube;

	Cube* grabbedcube = nullptr;
	Vector2f currgrid = { GRIDSIZE / 2,GRIDSIZE / 2 };
	float wallbunchwidth = 50;
	Vector2f box2dposition={ GRIDSIZE / 2,GRIDSIZE / 2 };

	View backgroundView;
	View endingView;
	Sprite background;

	Blue* blue;
	Orange* orange;
	bool madeblue = false;
	bool madeorange = false;

	bool grabitem = false;

	RectangleShape ending;


	////////////////////////////////////////////////////
	unique_ptr<b2World> world;

	float dtstack = 0.f;
	int dark = 0;
	int zoomCount;
	bool isMovingViewCenter;


	//////
	vector<SpriteObj*> tempContainer;
	vector<vector<SpriteObj*>> objInfos;

	ParticleSystem particle;
	
	int width;
	int height;


	// Front and backbuffer as Pointer for std::swap
	unique_ptr<RenderTexture> front, back;
	RenderTexture pass_normals, pass_diffuse;
	//Texture normal_map, diffuse_map;

	Shader lights_shader;
	Shader normals_shader;

	Light light;

	float ambient_intensity = 0.7;
	Vector3f falloff;
	/////

};

