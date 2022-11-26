#include "WirePointArrows.h"
#include "../Manager/ResourceMgr.h"
#include "../FrameWork/EnumClasses.h"
#include "../FrameWork/Utils.h"

WirePointArrows::WirePointArrows()	
{	
	for (int i = 0; i < arrowContainer.size(); i++)
	{
		arrowContainer[i].second.setTexture(*RESOURCEMGR->GetTexture("Graphics/Ui/wirearrow.png"));
		arrowContainer[i].second.setColor(Color(37, 255, 254));
		Utils::SetSpriteSize(arrowContainer[i].second, { 20,20 });
		Utils::SetOrigin(arrowContainer[i].second, Origins::ML);
	}
}

void WirePointArrows::Init()
{
	for (int i = 0; i < arrowContainer.size(); i++)
	{
		arrowContainer[i].first = false;
	}
}

void WirePointArrows::SetPosition(Vector2f pos, float tileSize)
{
	arrowContainer[(int)WirePos::Top].second.setRotation(90.f);
	arrowContainer[(int)WirePos::Top].second.setPosition(pos.x + tileSize * 0.5f, pos.y);

	arrowContainer[(int)WirePos::Left].second.setPosition(pos.x, pos.y + tileSize * 0.5f);

	arrowContainer[(int)WirePos::Right].second.setRotation(180.f);
	arrowContainer[(int)WirePos::Right].second.setPosition(pos.x + tileSize, pos.y + tileSize * 0.5f);

	arrowContainer[(int)WirePos::Bottom].second.setRotation(270.f);
	arrowContainer[(int)WirePos::Bottom].second.setPosition(pos.x + tileSize * 0.5f, pos.y + tileSize);
}

void WirePointArrows::Update(float dt)
{
}

void WirePointArrows::Draw(RenderWindow& window)
{
	for (int i = 0; i < arrowContainer.size(); i++)
	{
		if(arrowContainer[i].first)
			window.draw(arrowContainer[i].second);
	}
}

void WirePointArrows::SetSize(Vector2f size)
{
	for (int i = 0; i < arrowContainer.size(); i++)
	{
		Utils::SetSpriteSize(arrowContainer[i].second, size);
	}
}
