#include "Animator.h"
#include "ResourceMgr.h"

Animator::Animator()
	:loop(false), timer(0)
{
}

Animator::~Animator()
{
}

void Animator::SetOrigin(Vector2f origin)
{
	this->origin = origin;
}

void Animator::SetEndTime(float timer)
{
	playTime = timer;
	this->timer = playTime;
}

void Animator::SetTexture(string id, int rowNum, int columnNum, int posx, int posy)
{
	//texture = RESOURCE_MGR->GetTexture(id);
	Vector2u pos = texture->getSize();
	this->rowNum = rowNum;
	this->columnNum = columnNum;
	rectSourceSprite = { posx, posy, (int)(pos.x / rowNum), (int)(pos.y / columnNum) };
	//spriteObj->SetTexture(*texture);	

	//sprite = spriteObj->GetSprite();
	sprite->setTextureRect(rectSourceSprite);
	sprite->setOrigin(origin);
}

void Animator::SetSpriteObj(SpriteObj* spriteObj, AnimationType type)
{
	this->spriteObj = spriteObj;	
	this->type = type;
	this->loop = type == AnimationType::Projectile ? false : true;
}


void Animator::SetLoop(bool loop)
{
	this->loop = loop;
}

void Animator::ResetAnimaion()
{
	rectSourceSprite.left = 0;
}

void Animator::UpdateAnimation(float dt)
{	
	if (stop)
		return;

	timer -= dt;
	if (timer <= 0)
	{
		if (FrameCheck(rowNum - 1))
		{
			ResetAnimaion();
			if (!loop)
			{
				//spriteObj->SetActive(false);
			}				
		}
		rectSourceSprite.left += rectSourceSprite.width;
		sprite->setTextureRect(rectSourceSprite);
		sprite->setOrigin(origin);
		timer = playTime;
	}
}


bool Animator::FrameCheck(int n)
{
	return rectSourceSprite.left == rectSourceSprite.width * n;
}

void Animator::Stop()
{
	stop = true;
}

void Animator::Play()
{
	stop = false;
}
