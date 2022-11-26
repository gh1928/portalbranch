#include "Player.h"
#include "../FrameWork/InputMgr.h"
#include "../FrameWork/Utils.h"
#include "../FrameWork/stdafx.h"
#include "../FrameWork/Const.h"

Player::Player()
{
	SetResourceTexture("Graphics/player.png");
	id = 'p';

	hitbox = new RectangleShape;
	hitbox->setFillColor(Color::Red);

	objSize = ObjectSize::Carriable;
	attatchedPos = Rotate::Down;
}

Player::Player(b2World* world, const Vector2f& position, Vector2f dimensions)
{
	SetResourceTexture("Graphics/player.png");
	id = 'p';

	hitbox = new RectangleShape;
	hitbox->setFillColor(Color::Red);
	Utils::SetSpriteSize(sprite, dimensions);
	Utils::SetOrigin(sprite, Origins::BC);

	////////////////////////////////////////////////////////////////////////
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x / SCALE, position.y / SCALE * -1);
	body = world->CreateBody(&bodyDef);

	FloatRect bound = sprite.getGlobalBounds();
	float radius = bound.width * 0.5f;

	b2PolygonShape boxShape;

	b2Vec2 center(0, 0);
	center.y = (bound.height - radius) / 2 - (bound.height / 2 - radius) - 5;
	center.y /= SCALE;


	boxShape.SetAsBox(
		dimensions.x / SCALE / 2.0f,
		dimensions.y / SCALE / 2.0f,
		center, 0.f);


	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.2f;
	fixture = body->CreateFixture(&fixtureDef);
	body->SetFixedRotation(true);


	b2CircleShape circleShape;
	circleShape.m_radius = radius / SCALE;
	circleShape.m_p.Set(0, -(bound.height / 2 - radius) / SCALE); //position, relative to body position

	b2FixtureDef circlefixtureDef;
	circlefixtureDef.shape = &circleShape;
	circlefixtureDef.density = 1.0f;
	circlefixtureDef.friction = 0.2f;
	//fixture = body->CreateFixture(&circlefixtureDef);
	body->SetFixedRotation(true);

	//body->SetGravityScale(0);
	Utils::ChangeBCSpriteSFMLPosToBox2dPos(*this, *body, 1 / 60.f);

	float tempx = position.x;
	float tempy = position.y;
	SetPos({ tempx,tempy });
	hitbox->setPosition(GetPos());

}

Player::~Player()
{
	delete hitbox;
}

SpriteObj* Player::NewThis()
{
	return new Player;
}

void Player::Update(float dt)
{
	SpriteObj::Update(dt);
	//cout<<GetPos().y << endl;

	dir.x = InputMgr::GetAxis(Axis::Horizontal);

	Utils::ChangeBCSpriteSFMLPosToBox2dPos(*this, *body, dt);

	Utils::SetOrigin(*hitbox, Origins::BC);
	hitbox->setSize({ GetSize() });
	hitbox->setPosition(GetPos());


	if (body->GetLinearVelocity().x != 0) {
		if (maxspeed.x > body->GetLinearVelocity().x&&abs(maxspeed.x)<= maximumspeed)
			maxspeed.x = body->GetLinearVelocity().x;
		speedtX = 0;
	}

	if (body->GetLinearVelocity().y != 0 ) {
		if (maxspeed.y > body->GetLinearVelocity().y && abs(maxspeed.y) <= maximumspeed)
			maxspeed.y = body->GetLinearVelocity().y;
		speedtY = 0;
	}

	speedtX += dt;
	speedtY += dt;
	if (speedtX >= 0.2f ) {
		maxspeed.x = 0;
		//cout << "x리셋" << endl;
		speedtX = 0;

	}
	if (speedtY >= 0.2f) {
		maxspeed.y = 0;
		//cout << "y리셋" << endl;
		speedtY = 0;
	}

	//cout << maxspeed.x << " " << maxspeed.y << endl;
}

void Player::PhysicsUpdate(float dt)
{
	if (dir.x != 0)
	{		//body->SetLinearVelocity({ dir.x * 10,GetPlayerBodyForce().y });
		if (body->GetLinearVelocity().x <= 2.5 && body->GetLinearVelocity().x >= -2.5) {
			body->ApplyForce(b2Vec2({ dir.x * 10 , 0 }), body->GetWorldCenter(), true);
		}
	}
	else
	{
		body->SetLinearVelocity({ 0,GetPlayerBodyForce().y });
	}

	if (InputMgr::GetKeyDown(Keyboard::Space) && (body->GetLinearVelocity().y > -0.1f && body->GetLinearVelocity().y < 0.1f)) {
		//body->SetTransform({body->GetPosition().x,body->GetPosition().y+0.1f},0);
		body->ApplyLinearImpulse({ 0,1.5f }, GetPlayerBodyForce(), 1);
	}
	/*if (body->GetLinearVelocity().y > -0.1 && body->GetLinearVelocity().y < 0.1) {
		body->SetGravityScale(0.f);
		cout << "set 0" << endl;
	}
	else
		body->SetGravityScale(10.f);
	cout << body->GetLinearVelocity().y << endl;*/


}

void Player::Draw(RenderWindow& window)
{
	SpriteObj::Draw(window);
	//window.draw(*hitbox);

}

void Player::SetPlayerBodyPos(Vector2f pos)
{
	b2Vec2 newPos({ pos.x / SCALE,pos.y / SCALE * -1 });
	body->SetTransform(newPos, 0);
}

void Player::SetPlayerBodyForce(b2Vec2 force)
{
	body->ApplyLinearImpulse(force, body->GetWorldCenter(), true);

}
