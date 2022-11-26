#include "NumBox.h"
#include "../Manager/ResourceMgr.h"
#include "../FrameWork/Utils.h"
#include "../FrameWork/Const.h"

NumBox::NumBox()
{
	sprite.setTexture(*RESOURCEMGR->GetTexture("Graphics/Ui/numbox.png"));
	Utils::SetSpriteSize(sprite, { NUMBOX_SIZE, NUMBOX_SIZE });

	numTex.setFont(*RESOURCEMGR->GetFont("Fonts/times.ttf"));
	numTex.setString(to_string(num));
	numTex.setCharacterSize(NUMBOX_SIZE);
	numTex.setFillColor(Color::Black);
	SetOrigin(Origins::TL);
}

NumBox::~NumBox()
{
	
}

void NumBox::SetPos(Vector2f pos)
{
	sprite.setPosition(pos);
	numTex.setPosition(pos);
}

Vector2f NumBox::GetPos()
{
	return sprite.getPosition();
}

void NumBox::Draw(RenderWindow& window)
{
	window.draw(sprite);
	window.draw(numTex);
}

void NumBox::SetString()
{
	numTex.setString(to_string(num));
}

void NumBox::SetOrigin(Origins origin)
{
	Utils::SetOrigin(sprite, origin);
	Vector2f size = Utils::GetSpriteSize(sprite);
	numTex.setOrigin(-size.x * 0.3f, size.y * 0.15f);
}

void NumBox::SetNum(int n)
{
	num = n;
	numTex.setString(to_string(num));
}
