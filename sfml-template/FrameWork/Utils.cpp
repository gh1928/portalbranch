#include "Utils.h"
#include "const.h"
#include "box2d/box2d.h"
#include <math.h>
#include "../Objects/SpriteObj.h"

random_device Utils::rd;
mt19937 Utils::gen(Utils::rd());

void Utils::SetOrigin(Text& obj, Origins origin)
{
	SetOrigin(obj, origin, obj.getLocalBounds());
}

void Utils::SetOrigin(Sprite& obj, Origins origin)
{
	SetOrigin(obj, origin, obj.getLocalBounds());
}

void Utils::SetOrigin(Shape& obj, Origins origin)
{
	SetOrigin(obj, origin, obj.getLocalBounds());
}

void Utils::SetOrigin(Transformable& obj, Origins origin, FloatRect rect)
{
	Vector2f originPos(rect.width, rect.height);
	originPos.x *= ((int)origin % 3) * 0.5f;
	originPos.y *= ((int)origin / 3) * 0.5f;
	obj.setOrigin(originPos);
}

float Utils::Clamp(float v, float min, float max)
{
	if (v < min)
		return min;
	if (v > max)
		return max;
	return v;
}

int Utils::RandomRange(int min, int maxExclude)
{
	return (gen() % (maxExclude - min)) + min;
}

float Utils::RandomRange(float min, float max)
{
	uniform_real_distribution<> dist(min, max);
	return dist(gen);
}



float Utils::SqrMagnitude(const Vector2f& vec)
{
	return vec.x * vec.x + vec.y * vec.y;
}

float Utils::Magnitude(const Vector2f& vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

Vector2f Utils::Normalize(const Vector2f& vec)
{
	float mag = Magnitude(vec);
	if (mag == 0.f)
		return vec;
	return vec / mag;
}

float Utils::Distance(const Vector2f& vec1, const Vector2f& vec2)
{
	return Magnitude(vec1 - vec2);
}

float Utils::Dot(const Vector2f& a, const Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

Vector2f Utils::GetNormal(const Vector2f& vec)
{
	return Vector2f(-vec.y,vec.x);
}

float Utils::Angle(const Vector2f& start, const Vector2f& end)
{
	return Angle(Normalize(end - start));
}

float Utils::Angle(const Vector2f& dir)
{
	return atan2(dir.y , dir.x) * (180.f / M_PI);
}

float Utils::RandomZeroToOne()
{
	return RandomRange(0.f,1.f);
}

Vector2f Utils::RandomInCirclePoint()
{
	return RandomZeroToOne()* RandomOutCirclePoint();
}

Vector2f Utils::RandomOutCirclePoint()
{
	float radian = RandomRange(0.f, M_PI * 2);
	return Vector2f({ cos(radian),sin(radian) });
}

void Utils::SetSpriteSize(Sprite& sprite, Vector2f size)
{
	Vector2u texSize = sprite.getTexture()->getSize();
	float ratioX = texSize.x / size.x;
	float ratioY = texSize.y / size.y;

	sprite.setScale({ 1 / ratioX , 1 / ratioY });
}

Vector2f Utils::GetSpriteSize(Sprite& sprite)
{
	Vector2u size = sprite.getTexture()->getSize();
	Vector2f scale = sprite.getScale();

	return Vector2f{ size.x * scale.x, size.y * scale.y };	
}

#include <iostream>

void Utils::SetRotateSpriteInBox(Sprite& sprite, int rotationNum, float boxLineSize, Vector2f boxPos)
{	
	Utils::SetOrigin(sprite, Origins::BC);	

	switch (rotationNum)
	{
	case 0:
		sprite.setRotation(180.f);
		sprite.setPosition({ boxPos.x + boxLineSize * 0.5f, boxPos.y });
		break;
	case 1:
		sprite.setRotation(270.f);
		sprite.setPosition({ boxPos.x + boxLineSize, boxPos.y + boxLineSize * 0.5f });
		break;
	case 2:
		sprite.setRotation(0.f);
		sprite.setPosition({ boxPos.x + boxLineSize * 0.5f, boxPos.y + boxLineSize });
		break;
	case 3:
		sprite.setRotation(90.f);
		sprite.setPosition({ boxPos.x , boxPos.y + boxLineSize * 0.5f });
		break;
	}
}

void Utils::ChangeCoordSFMLToBox2d(Sprite& sprite, b2Body& body)
{
	//sprite.setPosition();
}

void Utils::ChangeBCSpriteSFMLPosToBox2dPos(SpriteObj& sprite, b2Body& body,float dt)
{
	b2Vec2 bodypos = body.GetPosition();
	Vector2f spritesize = Utils::GetSpriteSize(*sprite.GetSprite());

	float spritesetY = Utils::GetSpriteSize(*sprite.GetSprite()).y / 2;
	Vector2f temp(body.GetPosition().x * SCALE, (body.GetPosition().y * SCALE * -1)+Utils::GetSpriteSize(*sprite.GetSprite()).y/2);
	sprite.SetPos(temp);
}

void Utils::ChangePPM(b2Vec2 vec)
{
		
}

float Utils::Lerp(float A, float B, float Alpha)
{
	return A * (1 - Alpha) + B * Alpha;
}
