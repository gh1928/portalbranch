#include "Object.h"

Object::Object()
{
	Init();
}

Object::~Object()
{
	Release();
}

void Object::SetActive(bool active)
{
	this->active = active;
}

bool Object::GetActive() const
{
	return active;
}

void Object::Init()
{
	active = true;	
}

void Object::Release()
{
}

void Object::Reset()
{
	active = true;
}

void Object::SetPos(const Vector2f& pos)
{
	position = pos;
}

const Vector2f& Object::GetPos() const
{
	return position;
}

void Object::Translate(const Vector2f& delta)
{
	SetPos(position + delta);
}