#include "StartMenu.h"
#include "../FrameWork/InputMgr.h"
#include "../FrameWork/stdafx.h"
#include "../Scene/SceneMgr.h"
#include "../FrameWork/Framework.h"
#include "../Manager/ResourceMgr.h"

StartMenu::StartMenu()
{
	Init();
}

StartMenu::~StartMenu()
{
	Release();
}

void StartMenu::Init()
{
	start.setFont(*RESOURCEMGR->GetFont("Fonts/NanumGothic.otf"));
	edit.setFont(*RESOURCEMGR->GetFont("Fonts/NanumGothic.otf"));

	mousePos = new RectangleShape();
	Utils::SetOrigin(*mousePos, Origins::MC);
	mousePos->setSize({ 100, 50 });
	mousePos->setFillColor(Color::Blue);
	mousePos->setPosition(InputMgr::GetMousePos());


	startbox = new RectangleShape();
	Utils::SetOrigin(*startbox, Origins::MC);
	startbox->setSize({ 100, 40 });
	startbox->setFillColor(Color::Blue);
	startbox->setPosition(FRAMEWORK->GetWindowSize().x / 2, FRAMEWORK->GetWindowSize().y / 2 - 40);
	start.setPosition(startbox->getPosition());
	start.setString("START");

	editbox = new RectangleShape();
	Utils::SetOrigin(*editbox, Origins::MC);
	editbox->setSize({ 100, 40 });
	editbox->setFillColor(Color::Blue);
	editbox->setPosition(startbox->getPosition().x, startbox->getPosition().y + 100);
	edit.setPosition(editbox->getPosition());
	edit.setString("EDIT");

}

void StartMenu::Release()
{
	delete editbox;
	delete startbox;
}

void StartMenu::Enter()
{

	Scene::SetWorldView();

}

void StartMenu::Update(float dt)
{

	Utils::SetOrigin(*startbox, Origins::MC);
	Utils::SetOrigin(*editbox, Origins::MC);
	Utils::SetOrigin(edit, Origins::MC);
	Utils::SetOrigin(start, Origins::MC);

	mousePos->setPosition(InputMgr::GetMousePos());
	cout << "this is start menu" << endl;

	if (mousePos->getGlobalBounds().intersects(startbox->getGlobalBounds()) && InputMgr::GetMouseButtonDown(Mouse::Left)) {
		SCENE_MGR->ChangeScene(Scenes::MAPLIST);
	}
	else if (mousePos->getGlobalBounds().intersects(editbox->getGlobalBounds()) && InputMgr::GetMouseButtonDown(Mouse::Left)) {
		SCENE_MGR->ChangeScene(Scenes::MAPEDITER);
	}
}

void StartMenu::Exit()
{
}

void StartMenu::Draw(RenderWindow& window)
{
	if (editbox != nullptr) {
		window.draw(*editbox);
		window.draw(edit);
	}

	if (startbox != nullptr) {
		window.draw(*startbox);
		window.draw(start);
	}
}

