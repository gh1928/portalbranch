#pragma once
#include "SpriteObj.h"
#include <array>

enum class WirePos;
class Wire : public SpriteObj
{
public:
	Wire();
	virtual ~Wire();

	virtual void Init();

	virtual void SetPos(const Vector2f& pos);
	virtual void SetPos(const Vector2f& pos, const float tileSize);

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);

	virtual void SetSize(Vector2f size);
public:
	void SetOnOff(bool onoff);
	void SetColor(Color color);
	void SetWire(WirePos wirepos, bool onoff);

protected:
	array<pair<bool,Sprite>, 4> wires;
	
	bool isOn;
};

