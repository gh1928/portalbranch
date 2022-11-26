#pragma once

#include "SFML/Graphics.hpp"
#include "Scene.h"
#include "../FrameWork/Utils.h"
class StartMenu : public Scene
{
public:
	StartMenu();
	~StartMenu();

	virtual void Init() ;	// ����
	virtual void Release();		// ����

	virtual void Enter();
	virtual void Update(float dt);
	virtual void Exit();
	virtual void Draw(RenderWindow& window);

private:
	RectangleShape* editbox;
	RectangleShape* startbox;
	RectangleShape* mousePos;

	Text start;
	Text edit;

};

