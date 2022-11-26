#pragma once
#include <SFML/Graphics.hpp>
#include <array>

using namespace sf;
using namespace std;

class WirePointArrows
{
public:
	WirePointArrows();
	void Init();
	void SetPosition(Vector2f pos, float tileSize);
	void Update(float dt);
	void Draw(RenderWindow& window);
	void SetSize(Vector2f size);

	void SetActive(bool act) { active = act; }
	bool GetAcitve() { return active; }
private:	
	array<pair<bool, Sprite>, 4> arrowContainer;

	bool active;

	Vector2f position;
};

