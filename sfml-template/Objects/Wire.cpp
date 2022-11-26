#include "Wire.h"
#include "../FrameWork/EnumClasses.h"
#define TOP (int)WirePos::Top
#define LEFT (int)WirePos::Left
#define RIGHT (int)WirePos::Right
#define BOTTOM (int)WirePos::Bottom

Wire::Wire()
{
	SetResourceTexture("Graphics/wire.png");
	Utils::SetOrigin(sprite, Origins::MC);
	
	for (int i = 0; i < wires.size(); i++)
	{
		wires[i].second.setTexture(*sprite.getTexture());
		Utils::SetOrigin(wires[i].second, Origins::MC);
		wires[i].second.setColor(Color(37, 255, 254));
	}

	sprite.setColor(Color(37, 255, 254));
}

Wire::~Wire()
{
	
}

void Wire::Init()
{
	active = false;	
	for (int i = 0; i < wires.size(); i++)
	{
		wires[i].first = false;
	}	
}

void Wire::SetPos(const Vector2f& pos)
{
	SpriteObj::SetPos(pos);
}

void Wire::SetPos(const Vector2f& pos, const float tileSize)
{
	Vector2f middle{ pos.x + tileSize * 0.5f, pos.y + tileSize * 0.5f };
	SpriteObj::SetPos(middle);

	float offset = tileSize / 3;
	wires[TOP].second.setPosition(middle.x, middle.y - offset);
	wires[LEFT].second.setPosition(middle.x - offset, middle.y);
	wires[RIGHT].second.setPosition(middle.x + offset, middle.y);
	wires[BOTTOM].second.setPosition(middle.x, middle.y + offset);
}

void Wire::Update(float dt)
{
	
}

void Wire::Draw(RenderWindow& window)
{
	if (!active)
		return;

	SpriteObj::Draw(window);	
	for (int i = 0; i < wires.size(); i++)
	{
		if (wires[i].first)
			window.draw(wires[i].second);
	}
}

void Wire::SetSize(Vector2f size)
{
	Utils::SetSpriteSize(sprite, size);
	for (int i = 0; i < wires.size(); i++)
	{
		Utils::SetSpriteSize(wires[i].second, size);
	}
}

void Wire::SetOnOff(bool onOff)
{
	isOn = onOff;
	SetColor(isOn ? Color(248,147,30) : Color(37, 255, 254));
}

void Wire::SetColor(Color color)
{
	sprite.setColor(color);
	for (int i = 0; i < wires.size(); i++)
	{
		wires[i].second.setColor(color);
	}
}

void Wire::SetWire(WirePos wirepos, bool onoff)
{
	wires[(int)wirepos].first = onoff;
}
