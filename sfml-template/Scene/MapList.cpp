#include "MapList.h"
#include <iostream>
#include <filesystem>
#include"../FrameWork/Framework.h"
#include "../Manager/ResourceMgr.h"
#include "../FrameWork/Utils.h"
#include "../FrameWork/InputMgr.h"
#include "SceneMgr.h"

namespace fs = std::filesystem;

MapList::MapList()
{

}

MapList::~MapList()
{

}

void MapList::Init()
{

}

void MapList::Release()
{
	for (auto v : stagelist) {
		delete v;
	}
	stagelist.clear();
	delete mousepos;
}

void MapList::Enter()
{
	mousepos = new RectangleShape();
	Utils::SetOrigin(*mousepos, Origins::MC);
	mousepos->setSize({ 5,5 });
	selectbox.setFillColor(Color::White);

	//////////////get path string///////////////////////////////////////////
	string path = "Map";
	for (const auto& entry : fs::directory_iterator(path)) {
		string path_string = fs::path(entry).filename().string();
		Text* temp = new Text();
		temp->setString(path_string);
		
		stagelist.push_back(temp);
		stagelist.back()->setFillColor(Color::Blue);
		stagelist.back()->setFont(*RESOURCEMGR->GetFont("Fonts/NanumGothic.otf"));
		Utils::SetOrigin(*stagelist.back(), Origins::MC);

	}

	if (!stagelist.empty()) {
		y = FRAMEWORK->GetWindowSize().y / stagelist.size() / 2;

		for (int i = 0; i < stagelist.size(); i++) {
			stagelist[i]->setPosition(FRAMEWORK->GetInstance()->GetWindowSize().x / 2, y);
			y += 40;
		}
	}
	////////////////////////////////////////////////////////////////////////

}

void MapList::Update(float dt)
{
	if (InputMgr::GetKeyDown(Keyboard::Escape)) {
		SCENE_MGR->ChangeScene(Scenes::GAMESTART);
	}
	selected = false;
	mousepos->setPosition(InputMgr::GetMousePos());

	for (auto v : stagelist) {
		if (stagelist.empty()) {
			break;
		}
		if (mousepos->getGlobalBounds().intersects(v->getGlobalBounds())) {
			selected = true;
			Utils::SetOrigin(selectbox, Origins::MC);

			selectbox.setPosition(v->getPosition());
			selectbox.setSize({ (float)v->getGlobalBounds().width,(float)40 });
			if (InputMgr::GetMouseButtonDown(Mouse::Left)) {

				
				SCENE_MGR->AddScene("Map\\"+v->getString());
				SCENE_MGR->ChangeScene(Scenes::PLAY);
			}
		}
	}

	if (stagelist.empty()) {
		return;
	}
	///////////////////////wheel up ,down//////////////

	if (stagelist.back()->getPosition().y >= FRAMEWORK->GetWindowSize().y||
		stagelist.front()->getGlobalBounds().top<0) {
		//wheel up
		if (InputMgr::GetMouseWheelState() == 1&&
			stagelist.front()->getPosition().y < 0) {
			for (auto v : stagelist) {
				v->setPosition(v->getPosition().x,v->getPosition().y+40 );
			}
		}   //wheel down
		else if (InputMgr::GetMouseWheelState() == -1&&
			stagelist.back()->getGlobalBounds().top+ stagelist.back()->getGlobalBounds().height > FRAMEWORK->GetWindowSize().y) {
			for (auto v : stagelist) {
				v->setPosition(v->getPosition().x, v->getPosition().y - 40);
			}
		}
	}
}

void MapList::Exit()
{
	Release();
}

void MapList::Draw(RenderWindow& window)
{
	if (selected)
		window.draw(selectbox);

	for (auto v : stagelist) {
		window.draw(*v);
	}
}
