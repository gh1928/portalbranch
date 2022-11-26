#include "Goal.h"
#include "../Manager/ResourceMgr.h"
#include "../FrameWork/Utils.h"
#include "Button.h"
#include "../FrameWork/InputMgr.h"
#include "../FrameWork/Const.h"

Goal::Goal()
{
	SetResourceTexture("Graphics/goal2.png");
	
	id = '@';	

	type = ObjectType::Catcher;
	objSize = ObjectSize::Big;
	attatchedPos = Rotate::Down;

	Utils::SetOrigin(sprite, Origins::BC);

}

Goal::Goal(Vector2f currgrid, float size, string buttonlist)
{
	SetResourceTexture("Graphics/goal.png");
	id = '@';	

	Utils::SetOrigin(sprite, Origins::MC);

	SetSize({ GRIDSIZE,GRIDSIZE });
	SetPos(currgrid);

	for (int i = 0; i < buttonlist.size(); i++) {
		char temp = buttonlist[i];
		buttonid.push_back(atoi(&temp));
	}

	door.setPrimitiveType(Quads);
	door.resize(8);
	doorTex = RESOURCEMGR->GetTexture("Graphics/door.png");

	Vector2f texSize = (Vector2f)doorTex->getSize();
	leftDoorTexCoords = 0.f;
	rightDoorTexCoords = texSize.x;

	Vector2f pos = sprite.getPosition();
	leftDoorPos = pos.x;
	rightDoorPos = pos.x;

	on = false;
}

Goal::~Goal()
{	
	/*for (auto b : button)
		delete b;
	button.clear();*/
}

SpriteObj* Goal::NewThis()
{
	return new Goal;
}

void Goal::Update(float dt)
{
	SetDoor(dt);
	for (auto b : button) {
		if (!b->GetPressed()) {
			on = false;
			return;
		}
	}

	//active door
	on = true;
}

void Goal::Draw(RenderWindow& window)
{
	window.draw(door, doorTex);
	WireableObject::Draw(window);
}

void Goal::SetDoor(float dt)
{
	Vector2f texSize = (Vector2f)doorTex->getSize();
	Vector2f pos = sprite.getPosition();
	float transSpeed = dt;

	if (on)
	{
		if (leftDoorTexCoords <= texSize.x * 0.35f)
			leftDoorTexCoords += texSize.x * transSpeed;
		if (leftDoorTexCoords > texSize.x * 0.35f)
			leftDoorTexCoords = texSize.x * 0.35f;

		if (leftDoorPos >= pos.x - GRIDSIZE * 0.35f)
			leftDoorPos -= GRIDSIZE * transSpeed;		
		if (leftDoorPos < pos.x - GRIDSIZE * 0.35f)
			leftDoorPos = pos.x - GRIDSIZE * 0.35f;

		if (rightDoorTexCoords >= texSize.x * 0.65f)
			rightDoorTexCoords -= texSize.x * transSpeed;
		if (rightDoorTexCoords < texSize.x * 0.65f)
			rightDoorTexCoords = texSize.x * 0.65f;

		if (rightDoorPos <= pos.x + GRIDSIZE * 0.35f)
			rightDoorPos += GRIDSIZE * transSpeed;
		if (rightDoorPos > pos.x + GRIDSIZE * 0.35f)
			rightDoorPos = pos.x + GRIDSIZE * 0.35f;
	}
	else
	{
		if (leftDoorTexCoords >= 0.f)
			leftDoorTexCoords -= texSize.x * transSpeed;
		if (leftDoorTexCoords < 0.f)
			leftDoorTexCoords = 0.f;

		if (leftDoorPos <= pos.x)
			leftDoorPos += GRIDSIZE * transSpeed;
		if (leftDoorPos > pos.x)
			leftDoorPos = pos.x;

		if (rightDoorTexCoords <= texSize.x)
			rightDoorTexCoords += texSize.x * transSpeed;
		if (rightDoorTexCoords > texSize.x)
			rightDoorTexCoords = texSize.x;

		if (rightDoorPos >= pos.x)
			rightDoorPos -= GRIDSIZE * transSpeed;
		if (rightDoorPos < pos.x)
			rightDoorPos = pos.x;
	}


	door[0].texCoords = { leftDoorTexCoords,0.f };
	door[1].texCoords = { texSize.x * 0.5f,0.f };
	door[2].texCoords = { texSize.x * 0.5f,texSize.y };
	door[3].texCoords = { leftDoorTexCoords,texSize.y };

	door[0].position = { pos.x - GRIDSIZE * 0.5f, pos.y - GRIDSIZE * 0.5f };
	door[1].position = { leftDoorPos, pos.y - GRIDSIZE * 0.5f };
	door[2].position = { leftDoorPos, pos.y + GRIDSIZE * 0.5f };
	door[3].position = { pos.x - GRIDSIZE * 0.5f, pos.y + GRIDSIZE * 0.5f };

	door[4].texCoords = { texSize.x * 0.5f,0.f };
	door[5].texCoords = { rightDoorTexCoords, 0.f };
	door[6].texCoords = { rightDoorTexCoords, texSize.y };
	door[7].texCoords = { texSize.x * 0.5f, texSize.y };

	door[4].position = { rightDoorPos, pos.y - GRIDSIZE * 0.5f };
	door[5].position = { pos.x + GRIDSIZE * 0.5f, pos.y - GRIDSIZE * 0.5f };
	door[6].position = { pos.x + GRIDSIZE * 0.5f, pos.y + GRIDSIZE * 0.5f };
	door[7].position = { rightDoorPos, pos.y + GRIDSIZE * 0.5f };
}

void Goal::SetButtonlist(vector<Button*>& button)
{
	for (auto b : button) {
		for (int i = 0; i < buttonid.size(); i++) {
			if (b->GetButtonId() == buttonid[i]) {
				this->button.push_back(b);
			}
		}
	}
}
