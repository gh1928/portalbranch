#include "Tile.h"
#include "../FrameWork/Const.h"
#include "../FrameWork/Framework.h"
#include "../Manager/ResourceMgr.h"
#include "../FrameWork/InputMgr.h"
#pragma warning(disable:4996)

bool Tile::isPlayingGame = false;

Tile::Tile()
{
	SetResourceTexture(GetRandTileTex());
	id = '1';

	type = ObjectType::Tile;	
	objSize = ObjectSize::Big;
	attatchedPos = Rotate::Down;
}

Tile::~Tile()
{
	//delete fixture;
	delete hitbox;

}

void Tile::SetOrigin(Origins origin)
{
	SpriteObj::SetOrigin(origin);
	Utils::SetOrigin(backFace, origin);
}

SpriteObj* Tile::NewThis()
{
	return new Tile;
}

void Tile::Update(float dt)
{
	//Utils::SetOrigin(*hitbox, Origins::MC);
	//if (body != nullptr)
		//sprite.setRotation(body->GetAngle());
	//sprite.setPosition({ body->GetPosition().x,body->GetPosition().y*-1 });
	//SetPos({ body->GetPosition().x * SCALE,body->GetPosition().y * SCALE * -1 });

	//hitbox->setSize(GetSize());
	//if (body != nullptr)
		//hitbox->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);

}

void Tile::Draw(RenderWindow& window)
{
	if (!isPlayingGame)
		window.draw(sprite);

	if(isPlayingGame)
		DrawSideTiles(window);	
	
	//window.draw(*hitbox);
}

void Tile::Draw(RenderTexture& diffuse, Shader& nShader, RenderTexture& normal)
{
	diffuse.draw(sprite);
	NormalPass(normal, sprite, normalMap, nShader);
}

void Tile::PhysicsUpdate()
{
}

Tile::Tile(b2World* world, const Vector2f& position, Vector2f dimensions/*size of bunch wall */, Vector2f box2dposition, bool isEnd)
{
	GetRandDiffuseAndNormal();
	id = '1';
	SetSize({ GRIDSIZE,GRIDSIZE });

	Utils::SetOrigin(sprite, Origins::MC);


	if (isEnd) {
		float tilewidth = dimensions.y;
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(((box2dposition.x + dimensions.x / 2)- tilewidth/2) / SCALE, (box2dposition.y - GRIDSIZE) / SCALE * -1);
		body = world->CreateBody(&bodyDef);

		b2PolygonShape boxShape;
		boxShape.SetAsBox(dimensions.x / SCALE / 2.0f, dimensions.y / SCALE / 2.0f);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &boxShape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 1.f;
		fixture = body->CreateFixture(&fixtureDef);

		hitbox = new RectangleShape;
		Utils::SetOrigin(*hitbox, Origins::MC);
		hitbox->setFillColor(Color::Red);
		hitbox->setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);


	}


	SetPos({ position.x,position.y });



	type = ObjectType::Tile;

	backFace.setSize({ GRIDSIZE * DEPTH, GRIDSIZE * DEPTH });
	Utils::SetOrigin(backFace, Origins::MC);

	Vector2u texSize = sprite.getTexture()->getSize();

	for (int i = 0; i < sideTiles.size(); i++)
	{
		tileTextures[i] = RESOURCEMGR->GetTexture(GetRandTileTex());
		sideTiles[i].first = true;
		sideTiles[i].second.setPrimitiveType(Quads);
		sideTiles[i].second.resize(4);
		sideTiles[i].second[0].texCoords = { 0.f, 0.f };
		sideTiles[i].second[1].texCoords = { (float)texSize.x, 0.f };
		sideTiles[i].second[2].texCoords = (Vector2f)texSize;
		sideTiles[i].second[3].texCoords = { 0.f, (float)texSize.y };
	}
	backFace.setFillColor(Color::Red);
}

string Tile::GetRandTileTex()
{
	String str;
	int rand = Utils::RandomRange(0, 3);
	switch (rand)
	{
	case 0:
		str = "Graphics/Tile/tile1.png";
		break;
	case 1:
		str = "Graphics/Tile/tile2.png";
		break;
	case 2:
		str = "Graphics/Tile/tile3.png";
		break;
	}

	return str;
}

void Tile::GetRandDiffuseAndNormal()
{
	int rand = Utils::RandomRange(0, 3);
	switch (rand)
	{
	case 0:
		SetResourceTexture("Graphics/Tile/tile1.png");
		normalMap = RESOURCEMGR->GetTexture("Graphics/Tile/tile1n.png");
		break;
	case 1:
		SetResourceTexture("Graphics/Tile/tile2.png");
		normalMap = RESOURCEMGR->GetTexture("Graphics/Tile/tile2n.png");
		break;
	case 2:
		SetResourceTexture("Graphics/Tile/tile3.png");
		normalMap = RESOURCEMGR->GetTexture("Graphics/Tile/tile3n.png");
		break;
	}
}

void Tile::SetActiveSideTiles(int pos, bool active)
{
	sideTiles[pos].first = active;
}

void Tile::SetSideTilesPosition(RenderWindow& window)
{

	Vector2f vanishingPoint = window.getView().getCenter();
	//Vector2f vanishingPoint = { WINDOW_WIDTH / 2,WINDOW_HEIGHT / 2 };

	backFace.setPosition(
		sprite.getPosition() + (vanishingPoint - sprite.getPosition()) * (1.f - DEPTH)
	);

	FloatRect backRect = backFace.getGlobalBounds();
	FloatRect frontRect = sprite.getGlobalBounds();

	if (sideTiles[0].first)
	{
		sideTiles[0].second[0].position = { backRect.left, backRect.top };
		sideTiles[0].second[1].position = { backRect.left + backRect.width, backRect.top };
		sideTiles[0].second[2].position = { frontRect.left + frontRect.width, frontRect.top };
		sideTiles[0].second[3].position = { frontRect.left, frontRect.top };
	}

	if (sideTiles[1].first)
	{
		sideTiles[1].second[0].position = { frontRect.left + frontRect.width, frontRect.top };
		sideTiles[1].second[1].position = { backRect.left + backRect.width, backRect.top };
		sideTiles[1].second[2].position = { backRect.left + backRect.width, backRect.top + backRect.height };
		sideTiles[1].second[3].position = { frontRect.left + frontRect.width, frontRect.top + frontRect.height };
	}

	if (sideTiles[2].first)
	{
		sideTiles[2].second[0].position = { backRect.left, backRect.top + backRect.height };
		sideTiles[2].second[1].position = { backRect.left + backRect.width, backRect.top + backRect.height };
		sideTiles[2].second[2].position = { frontRect.left + frontRect.width, frontRect.top + frontRect.height };
		sideTiles[2].second[3].position = { frontRect.left , frontRect.top + frontRect.height };
	}

	if (sideTiles[3].first)
	{
		sideTiles[3].second[0].position = { frontRect.left, frontRect.top };
		sideTiles[3].second[1].position = { backRect.left, backRect.top };
		sideTiles[3].second[2].position = { backRect.left, backRect.top + backRect.height };
		sideTiles[3].second[3].position = { frontRect.left , frontRect.top + frontRect.height };
	}
}

void Tile::DrawSideTiles(RenderWindow& window)
{
	SetSideTilesPosition(window);

	for (int i = 0; i < sideTiles.size(); i++)
	{
		if (sideTiles[i].first)
			window.draw(sideTiles[i].second, tileTextures[i]);
	}
}
