#include "SpriteObj.h"
#include "../Manager/ResourceMgr.h"
#pragma warning(disable:4996)

bool SpriteObj::isWiring = false;

SpriteObj::SpriteObj()
	:isInMapTool(false), type(ObjectType::Object), attatchedPos(Rotate::Down)
{
}

SpriteObj::~SpriteObj()
{
}

void SpriteObj::SetOrigin(Origins origin)
{
	Utils::SetOrigin(sprite, origin);
}

void SpriteObj::SetPos(const Vector2f& pos)
{
	Object::SetPos(pos);
	sprite.setPosition(pos);
}

void SpriteObj::Update(float dt)
{
	
}

void SpriteObj::Draw(RenderWindow& window)
{
	window.draw(sprite);
}

void SpriteObj::Draw(RenderTexture& diffuse, Shader& nShader, RenderTexture& normal)
{

}

void SpriteObj::SetResourceTexture(String id)
{	
	sprite.setTexture(*RESOURCEMGR->GetTexture(id), true);
}

Sprite* SpriteObj::GetSprite()
{
	return &sprite;
}

void SpriteObj::SetScale(Vector2f scale)
{
	sprite.setScale(scale);
}

void SpriteObj::SetSize(Vector2f size)
{
	if (sprite.getTexture() == nullptr)
		return;

	Utils::SetSpriteSize(sprite, size);
}

Vector2f SpriteObj::GetSize()
{
	return Utils::GetSpriteSize(sprite);
}

void SpriteObj::FitScale(float size)
{
	if (sprite.getTexture() == nullptr)
		return;

	Vector2u texSize = sprite.getTexture()->getSize();
	float scaleRatio = texSize.x > texSize.y ? texSize.x / size : texSize.y / size;
	sprite.setScale(sprite.getScale() / scaleRatio);
}


void SpriteObj::SetRotation(int val)
{
	if (val == 0) {
		sprite.setRotation(180.f);
	}
	else if (val == 1) {
		sprite.setRotation(270.f);
	}
	else if (val == 3) {
		sprite.setRotation(90.f);
	}
	else
		sprite.setRotation(0.f);
}

void SpriteObj::SetRotation(Rotate rotation)
{
	switch (rotation)
	{
	case Rotate::Up:
		sprite.setRotation(180.f);
		attatchedPos = Rotate::Up;
		break;
	case Rotate::Right:
		sprite.setRotation(270.f);
		attatchedPos = Rotate::Right;
		break;
	case Rotate::Down:
		sprite.setRotation(0.f);
		attatchedPos = Rotate::Down;
		break;
	case Rotate::Left:
		sprite.setRotation(90.f);
		attatchedPos = Rotate::Left;
		break;
	}
}

void SpriteObj::SetRotationInBox(int rotationNum, float boxHeight, Vector2f boxPos)
{
	if (objSize == ObjectSize::Big)
		return;
	Utils::SetRotateSpriteInBox(sprite, rotationNum, boxHeight, boxPos);
	attatchedPos = (Rotate)rotationNum;
}

char SpriteObj::GetId()
{
	return id;
}

SpriteObj* SpriteObj::NewThis()
{
	return new SpriteObj;
}

FloatRect SpriteObj::GetGlobalBounds()
{
	return sprite.getGlobalBounds();
}

void SpriteObj::NormalPass(RenderTexture& normalPass, Sprite& sprite, Texture* tex, Shader& nShader)
{
	nShader.setParameter("sampler_normal", *tex);
	normalPass.draw(sprite, &nShader);
}
