#include "Blue.h"

Blue::Blue()
{
	SetResourceTexture("Graphics/blue.png");
	Utils::SetOrigin(sprite, Origins::MC);

	SetPos({ -1000,-1000 });
	hitbox = new RectangleShape;
	hitbox->setFillColor(Color::Red);

	light.setRange(6);
	light.setColor(Color(37, 255, 254));
	light.setBeamAngle(90);
	light.setIntensity(1);
	light.rotate(0);
	light.setScale(Utils::GetSpriteSize(sprite));
	light.castLight(edges.begin(), edges.end());

	edges.emplace_back(sprite.getPosition(),sprite.getPosition());
}

Blue::~Blue()
{
	delete hitbox;

}

void Blue::Update(float dt)
{
	Utils::SetOrigin(*hitbox, Origins::BC);
	light.setPosition(sprite.getPosition());
	hitbox->setSize({ sprite.getGlobalBounds().width - 10,sprite.getGlobalBounds().height });
	hitbox->setPosition(sprite.getPosition());

	Translate(direction *dt* projectilespeed);

	hitbox->setPosition(sprite.getPosition());

}

void Blue::Draw(RenderWindow& window)
{
	window.draw(light);
	SpriteObj::Draw(window);
}

void Blue::SetLightDir(int side)
{
	light.setRotation(side);
}
