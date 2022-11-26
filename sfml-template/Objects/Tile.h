#pragma once
#include "SpriteObj.h"
#include <box2d/box2d.h>
#include <array>

class Tile : public SpriteObj
{
public:
	Tile();
	virtual ~Tile();

	void SetOrigin(Origins origin);

	virtual SpriteObj* NewThis();

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	virtual void Draw
	(RenderTexture& diffuse, Shader& nShader, RenderTexture& normal);
	void PhysicsUpdate();

	////////////////////////////////////////////////////////////////////////////
	Tile(b2World* world, const Vector2f& position, Vector2f dimensions/*bunch of wall size*/, Vector2f box2dposition, bool isEnd);
	b2Body* GetBody() { return body; }
	b2Fixture* GetFixture() { return fixture; }


	static void SetIsPlayingGame(bool play) { isPlayingGame = play; }
	string GetRandTileTex();

	void GetRandDiffuseAndNormal();
	void SetActiveSideTiles(int pos, bool active);
	void SetSideTilesPosition(RenderWindow& window);
	void DrawSideTiles(RenderWindow& window);
protected:
	b2Body* body = nullptr;
	b2Fixture* fixture = nullptr;

	RectangleShape* hitbox;

	static bool isPlayingGame;

	Texture* normal;
	RectangleShape backFace;
	array<pair<bool, VertexArray>, 4> sideTiles;
	array<Texture*, 4> tileTextures;
};

