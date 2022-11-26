#include "Orange.h"

Orange::Orange()
{
	SetResourceTexture("Graphics/orange.png");

	Utils::SetOrigin(sprite, Origins::MC);
	SetPos({ -100, -100 });
	hitbox = new RectangleShape;
	hitbox->setFillColor(Color::Red);

	light.setRange(6);
	light.setColor(Color(248, 147, 30));
	light.setBeamAngle(90);
	light.setIntensity(1);
	light.rotate(0);
	light.setScale(Utils::GetSpriteSize(sprite));
	light.castLight(edges.begin(), edges.end());

	edges.emplace_back(sprite.getPosition(), sprite.getPosition());
}

Orange::~Orange()
{
	delete hitbox;
}

void Orange::Update(float dt)
{
	Utils::SetOrigin(*hitbox, Origins::BC);
	light.setPosition(sprite.getPosition());

	hitbox->setSize({ sprite.getGlobalBounds().width - 10,sprite.getGlobalBounds().height });
	hitbox->setPosition(sprite.getPosition());

	
	Translate(direction * dt * projectilespeed);

}

void Orange::Draw(RenderWindow& window)
{
	window.draw(light);

	SpriteObj::Draw(window);
	//window.draw(*hitbox);
}

void Orange::SetLightDir(int side)
{
	light.setRotation(side);
}
