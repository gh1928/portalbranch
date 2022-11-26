#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

enum class AnimationType
{
	None = -1,
	Projectile,
	Actor,
	Background,
};

class SpriteObj;
class Animator
{
protected:
	SpriteObj* spriteObj;
	Sprite* sprite;

	AnimationType type;
	Texture* texture;
	IntRect rectSourceSprite;
	Vector2f origin;

	int rowNum;
	int columnNum;

	bool loop;
	bool stop;

	float timer;
	float playTime;

public:
	Animator();
	~Animator();
public:
	//무조건 셋 스프라이트 함수를 맨 위에놓을것
	void SetSpriteObj(SpriteObj* spriteObj, AnimationType type);
	void SetOrigin(Vector2f origin);
	void SetEndTime(float timer);
	void SetTexture(string id, int rowNum, int columnNum = 1, int posx = 0, int posy = 0);	
	void SetLoop(bool loop);
	void ResetAnimaion();	
	void UpdateAnimation(float dt);	
	bool FrameCheck(int n);
	void Stop();
	void Play();
};