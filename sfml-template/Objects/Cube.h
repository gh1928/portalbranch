#pragma once
#include "SpriteObj.h"
#include "box2d/box2d.h"

class Cube : public SpriteObj
{
public:
	Cube();
	virtual ~Cube();

	virtual SpriteObj* NewThis();
	bool GetGround() { return ground; }
	void SetGround(bool a) { ground = a; }
	FloatRect GethitboxGlobalBounds() { return hitbox->getGlobalBounds(); }
	void ChangeBodyTypeBetweenStaticAndDynamic(bool b);

	virtual void Update(float dt);
	void PhysicsUpdate();

	virtual void Draw(RenderWindow& window);
	void SetSide(bool s);
	bool GetSide() { return side; }
	void SetCubeBodyPos(Vector2f pos);
	void SetCubeBodyForce(b2Vec2 force);
	b2Vec2 GetCubeBodyForce() { return body->GetLinearVelocity(); }

	/////////////////////////////////////////////////////////////////////////
	Cube(b2World* world, const Vector2f& position, Vector2f dimensions);
	b2Body* GetBody() { return body; }
	b2Fixture* GetFixture() { return fixture; }	

	Vector2f GetRecentSpeed() { return maxspeed; }

protected:
	bool ground = false;
	RectangleShape* hitbox;
	float verticalspeed = 0.f;
	float addspeed = 50;
	bool side=false; //false=left true=right
	bool grab = false;

	float speedtX = 0.f;
	float speedtY = 0.f;
	float maximumspeed = 18.f;

	Vector2f maxspeed;
	///////////////////////////////////////////////////////////////////////
	b2Body* body = nullptr;
	b2Fixture* fixture = nullptr;


};

