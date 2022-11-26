#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Object
{
public:
	Object();
	virtual ~Object();

	virtual void SetActive(bool active);
	virtual bool GetActive() const;

	virtual void Init();
	virtual void Release();

	virtual void Reset();

	virtual void SetPos(const Vector2f& pos);
	virtual const Vector2f& GetPos() const;

	virtual void Translate(const Vector2f& delta);

	virtual void Update(float dt) = 0;
	virtual void Draw(RenderWindow& window) = 0;
protected:
	bool active;
	Vector2f position;
};

