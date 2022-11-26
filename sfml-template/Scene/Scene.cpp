#include "Scene.h"
#include "../Framework/Framework.h"
#include "../FrameWork/InputMgr.h"
#include "../Manager/ResourceMgr.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	Release();

}

void Scene::Release()
{
	
}

void Scene::Update(float dt)
{
	
}

void Scene::PhysicsUpdate(float dt)
{
}

void Scene::Draw(RenderWindow& window)
{

}

void Scene::SetWorldView()
{
	auto size = (Vector2f)FRAMEWORK->GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter(size * 0.5f);
	FRAMEWORK->GetWindow().setView(worldView);
}

void Scene::SetUiView()
{
	auto size = (Vector2f)FRAMEWORK->GetWindowSize();
	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);
	FRAMEWORK->GetWindow().setView(uiView);
}

Vector2f Scene::ScreenToWorldPos(Vector2i screenPos)
{
	RenderWindow& window = FRAMEWORK->GetWindow();
	return window.mapPixelToCoords(screenPos, worldView);
}

Vector2f Scene::ScreenToUiPos(Vector2i screenPos)
{
	RenderWindow& window = FRAMEWORK->GetWindow();
	return window.mapPixelToCoords(screenPos, uiView);
}
Vector2i Scene::UiPosToScreen(Vector2f UiPos)
{
	RenderWindow& window = FRAMEWORK->GetWindow();
	return window.mapCoordsToPixel(UiPos, uiView);
}

Vector2f Scene::GetMouseWorldPos()
{
	return ScreenToWorldPos((Vector2i)InputMgr::GetMousePos());
}

void Scene::SetTex(Sprite& sprite, string id)
{
	sprite.setTexture(*RESOURCEMGR->GetTexture(id));
}

