#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

enum class Origins;
class NumBox
{
public:
	NumBox();
	~NumBox();

	void SetPos(Vector2f pos);
	Vector2f GetPos();
	void Draw(RenderWindow& window);
	void SetString();
	void SetOrigin(Origins origin);

	int GetNum() { return num; }
	void SetNum(int n);
private:
	Sprite sprite;
	Text numTex;

	int num;
};

