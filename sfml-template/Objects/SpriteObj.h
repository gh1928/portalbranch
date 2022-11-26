#pragma once
#include "Object.h"
#include "../FrameWork/Utils.h"
#include <string>

enum class ObjectType
{
	None = -1,
	Trigger,
	Catcher,
	Tile,
	Object,
};

enum class ObjectSize
{
	None = -1,
	Carriable,
	Normal,
	Big,
};

enum class Rotate
{
	None = -1,
	Up,
	Right,
	Down,
	Left,
};

class SpriteObj : public Object
{
public:
	SpriteObj();
	virtual ~SpriteObj();

	void SetOrigin(Origins origin);
	virtual void SetPos(const Vector2f& pos);

	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);	
	virtual void Draw
	(RenderTexture& diffuse, Shader& nShader, RenderTexture& normal);
public:
	void SetResourceTexture(String id);
	
	Sprite* GetSprite();

	void SetScale(Vector2f scale);
	virtual void SetSize(Vector2f size);
	Vector2f GetSize();

	void FitScale(float size);

	void SetRotation(int val);
	void SetRotation(Rotate rotation);
	void SetRotation(float val) { sprite.setRotation(val); }
	void SetRotationInBox(int rotationNum, float boxLineHeight, Vector2f boxPos);

	Rotate GetRotation() { return attatchedPos; }

	void SetDir(Vector2f dir) { direction = dir; };
	char GetId();

	virtual SpriteObj* NewThis();

	FloatRect GetGlobalBounds();

	void NormalPass
	(RenderTexture& normalPass, Sprite& sprite, Texture* tex, Shader& nShader);
	static void OnOffWiringState() { isWiring = !isWiring; }
	static void OnOffWiringState(bool onoff) { isWiring = onoff; }
	
	virtual bool GetWiringState() { return isWiring; }		

	void SetBoolInMapTool(bool isIn) { isInMapTool = isIn; }
	
	void SetType(ObjectType type) { this->type = type; }
	ObjectType GetObjType() { return type; }
	ObjectSize GetObjSize() { return objSize; }
	void SetObjSize(ObjectSize size) { objSize = size; }
protected:
	Sprite sprite;
	Texture* normalMap;
	float speed=100.f;
	float velocity=0.f;
	Vector2f direction={0,0};

	char id;
	ObjectType type;
	ObjectSize objSize;
	Rotate attatchedPos;

	static bool isWiring;	
	bool isInMapTool;
};

