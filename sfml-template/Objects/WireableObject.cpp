#include "WireableObject.h"
#include "../Manager/ResourceMgr.h"
#include "../FrameWork/InputMgr.h"
#include "../FrameWork/Utils.h"
#include "../Objects/NumBox.h"
#include "../FrameWork/Const.h"

WireableObject::WireableObject()
{
	
}

WireableObject::~WireableObject()
{
	if (!numbox.empty())
	{
		for (auto i : numbox)
		{
			delete i;
		}
	}
	numbox.clear();
}

void WireableObject::Init()
{
	Object::Init();
	SetWireCheckBox();
}

void WireableObject::Update(float dt)
{

}

void WireableObject::Update(float dt, Vector2f mouse)
{

}

void WireableObject::Draw(RenderWindow& window)
{
	SpriteObj::Draw(window);
	WireModDraw(window);
}

void WireableObject::SetWireCheckBox()
{
	wireCheckBox.setTexture(RESOURCEMGR->GetTexture("Graphics/Ui/wirebox.png"));
	wireCheckBox.setSize((Vector2f)sprite.getTexture()->getSize());
	Utils::SetOrigin(wireCheckBox, Origins::BC);
	Vector2f scale = sprite.getScale();
	wireCheckBox.setSize((Vector2f)sprite.getTexture()->getSize());
	wireCheckBox.setScale(scale);
	wireCheckBox.setPosition(sprite.getPosition());
	wireCheckBox.setRotation(sprite.getRotation());
}

void WireableObject::WireModDraw(RenderWindow& window)
{	
	isMouseIn = false;
	Vector2f mouseWorldPos = window.mapPixelToCoords((Vector2i)InputMgr::GetMousePos(), window.getView());
	isMouseIn = wireCheckBox.getGlobalBounds().contains(mouseWorldPos);

	wireCheckBox.setFillColor(isMouseIn ? Color(248, 147, 30) : Color(37, 255, 254));

	if (isWiring && isInMapTool)
	{		
		window.draw(wireCheckBox);
	}

	if (!numbox.empty() && isWiring)
	{
		for (auto i : numbox)
		{
			i->Draw(window);
		}
	}
}

void WireableObject::AddNumBox(NumBox* nb)
{	
	if (numbox.empty())
	{
		numbox.push_back(new NumBox);
		numbox.back()->SetNum(nb->GetNum());
		numbox.back()->SetPos(GetPos());
	}
	else
	{
		Vector2f pos = numbox.back()->GetPos();
		Vector2f fixPos{ NUMBOX_SIZE, 0 };
		numbox.push_back(new NumBox);
		numbox.back()->SetNum(nb->GetNum());
		numbox.back()->SetPos(pos + fixPos);
	}
}

void WireableObject::AddNumBox(int num)
{
	numbox.push_back(new NumBox);
	numbox.back()->SetNum(num);
	numbox.back()->SetPos(sprite.getPosition());
}

void WireableObject::RemoveNumBox()
{
	if (numbox.empty())
		return;

	delete numbox.back();
	numbox.pop_back();
}

list<int> WireableObject::GetWireListFromMapTool()
{
	list<int> temp;
	
	for (auto boxnum : numbox)
	{
		temp.push_back(boxnum->GetNum());
	}

	return temp;
}
