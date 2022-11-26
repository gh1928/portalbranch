#include "Cube.h"
#include "../FrameWork/Const.h"
#include "../FrameWork/stdafx.h"

Cube::Cube()
{
	SetResourceTexture("Graphics/cube.png");
	id = 'c';
	
	hitbox = new RectangleShape;
	hitbox->setFillColor(Color::Red);

	objSize = ObjectSize::Carriable;
	attatchedPos = Rotate::Down;
}

Cube::Cube(b2World* world, const Vector2f& position, Vector2f dimensions)
{
	SetResourceTexture("Graphics/cube.png");
	id = 'c';

	Utils::SetSpriteSize(sprite, dimensions);
	Utils::SetOrigin(sprite, Origins::BC);
	Utils::SetSpriteSize(sprite, { 30,30 });
	hitbox = new RectangleShape;
	hitbox->setFillColor(Color::Red);
	hitbox->setSize({ sprite.getGlobalBounds().width + 30,sprite.getGlobalBounds().height });

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x/SCALE, position.y/SCALE* -1);
	body = world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(30.f/ SCALE / 2.0f, 30.f/ SCALE / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	fixture = body->CreateFixture(&fixtureDef);

	Utils::ChangeBCSpriteSFMLPosToBox2dPos(*this, *body,1/60.f);

	float tempx = position.x;
	float tempy = position.y;
	SetPos({ tempx,tempy });
	hitbox->setPosition(GetPos());

}

Cube::~Cube()
{
	//delete fixture;
	delete hitbox;
}

SpriteObj* Cube::NewThis()
{
	return new Cube;
}


void Cube::SetCubeBodyPos(Vector2f pos)
{
	b2Vec2 newPos({ pos.x/SCALE,pos.y/SCALE * -1 });
	body->SetTransform(newPos, body->GetAngle());
}

void Cube::ChangeBodyTypeBetweenStaticAndDynamic(bool b)
{
	if (b) {
		body->SetType(b2BodyType::b2_staticBody);
		grab = true;
	}else{
		body->SetType(b2BodyType::b2_dynamicBody);
		grab = false;
	}
}

void Cube::Update(float dt)
{
	Utils::SetOrigin(*hitbox, Origins::BC);

	Utils::ChangeBCSpriteSFMLPosToBox2dPos(*this, *body,dt);
	sprite.setRotation(body->GetAngle());

	hitbox->setSize({ GetSize().x+10,GetSize().y});
	hitbox->setPosition(GetPos() );
	hitbox->setRotation(body->GetAngle());


	if (body->GetLinearVelocity().x != 0) {
		if (maxspeed.x > body->GetLinearVelocity().x && abs(maxspeed.x) <= maximumspeed)
			maxspeed.x = body->GetLinearVelocity().x;
		speedtX = 0;
	}

	if (body->GetLinearVelocity().y != 0) {
		if (maxspeed.y > body->GetLinearVelocity().y && abs(maxspeed.y) <= maximumspeed)
			maxspeed.y = body->GetLinearVelocity().y;
		speedtY = 0;
	}

	speedtX += dt;
	speedtY += dt;
	if (speedtX >= 0.2f) {
		maxspeed.x = 0;
	//	cout << "x리셋" << endl;
		speedtX = 0;

	}
	if (speedtY >= 0.2f) {
		maxspeed.y = 0;
	//	cout << "y리셋" << endl;
		speedtY = 0;
	}

	//cout << body->GetLinearVelocity().y << endl;
}

void Cube::PhysicsUpdate()
{
	//body->SetTransform(b2Vec2(GetPos().x / SCALE,GetPos().y / SCALE),body->GetAngle());
}

void Cube::Draw(RenderWindow& window)
{

	SpriteObj::Draw(window);
	//window.draw(*hitbox);
}

void Cube::SetSide(bool s)
{
	side = s;
}

void Cube::SetCubeBodyForce(b2Vec2 force)
{
	body->ApplyLinearImpulse(force, body->GetWorldCenter(), true);

}

