#include "PlayScene.h"
#include <fstream>
#include "../FrameWork/stdafx.h"
#include "../FrameWork/InputMgr.h"
#include "SceneMgr.h"
#include "../FrameWork/Framework.h"
#include "../Manager/ResourceMgr.h"
#include "../Objects/Goal.h"

#pragma warning(disable:4996)

void PlayScene::Update(float dt)
{	
	ClearRenderBuffer();
	//////////////////////////////////////////////////////
	if (goal->GetGlobalBounds().intersects(player->GetGlobalBounds())) {
		if (goal->IsFinish()) {
			dark += dt * 500;
			ending.setFillColor(Color(0, 0, 0, dark));
		}
		if (dark >= 255) {
			SCENE_MGR->ChangeScene(Scenes::GAMESTART);
			return;
		}
	}
	else {
		if (dark > 0) {
			dark -= dt * 500;
			ending.setFillColor(Color(0, 0, 0, dark));

		}
	}



	player->Update(dt);
	goal->Update(dt);
	for (auto c : cube) {
		c->Update(dt);
	}
	for (auto w : wall)
		w->Update(dt);

	blue->Update(dt);
	orange->Update(dt);


	if (!isMovingViewCenter) {
		
		Vector2f currentcampos= worldView.getCenter();
		cout << currentcampos.x << " " << currentcampos.y << endl;
		cout << player->GetPos().x << " " << player->GetPos().y << endl<<endl;

		worldView.setCenter(Utils::Lerp(currentcampos.x,player->GetPos().x,0.1), Utils::Lerp(currentcampos.y, player->GetPos().y, 0.1));
		
	}

	//blue
	if (InputMgr::GetMouseButtonDown(Mouse::Left) && !grabitem) {
		blue->SetSize({ 20,20 });
		madeblue = false;
		blue->SetPos({ player->GetPos().x,player->GetPos().y - 25 });
		blue->SetDir(Utils::Normalize(ScreenToWorldPos((Vector2i)InputMgr::GetMousePos()) - player->GetPositions()));
	}

	//orange
	if (InputMgr::GetMouseButtonDown(Mouse::Right) && !grabitem) {
		orange->SetSize({ 20,20 });
		madeorange = false;
		orange->SetPos({ player->GetPos().x,player->GetPos().y - 25 });
		orange->SetDir(Utils::Normalize(ScreenToWorldPos((Vector2i)InputMgr::GetMousePos()) - player->GetPositions()));
	}

	MakePortal();
	MoveToPortal();
	
	PushButton();


	if (grabitem) {
		if (grabbedcube->GetSide())
			grabbedcube->SetCubeBodyPos({ player->GetPos().x + 40,player->GetPos().y - 40 });
		else
			grabbedcube->SetCubeBodyPos({ player->GetPos().x - 40,player->GetPos().y - 40 });

		if (InputMgr::GetKeyDown(Keyboard::A)) {
			grabbedcube->SetSide(false);
		}
		else if (InputMgr::GetKeyDown(Keyboard::D)) {
			grabbedcube->SetSide(true);
		}

		if (InputMgr::GetKeyDown(Keyboard::E)) {
			cout << "drop" << endl;
			grabitem = false;
			grabbedcube->ChangeBodyTypeBetweenStaticAndDynamic(grabitem);
			grabbedcube = nullptr;
		}

	}
	else if (!grabitem) {
		for (auto c : cube) {
			if (c->GethitboxGlobalBounds().intersects(player->GetGlobalBounds()) && InputMgr::GetKeyDown(Keyboard::E)) {
				if (!grabitem) {
					cout << "pickup" << endl;

					grabitem = true;
					grabbedcube = c;
					float cposX = c->GetGlobalBounds().left + (c->GetGlobalBounds().width / 2);
					if (player->GetPositions().x <= cposX)
					{
						c->SetSide(true);
						c->ChangeBodyTypeBetweenStaticAndDynamic(grabitem);
					}
					else if (player->GetPos().x > cposX) {
						c->SetSide(false);
						c->ChangeBodyTypeBetweenStaticAndDynamic(grabitem);
					}
					c->SetGround(false);
				}

			}
		}
	}

	if (particle.running())
		particle.update(dt);

	Input();
}

void PlayScene::PhysicsUpdate(float dt)
{
	dtstack += dt;

	if (dtstack >= 1 / 60.f) {
		world->Step(1 / 60.f, 8, 3);
		for (auto w : wall) {
			w->PhysicsUpdate();
		}

		for (auto c : cube) {
			c->PhysicsUpdate();
		}

		player->PhysicsUpdate(dt);

		dtstack -= 1 / 60.f;
	}

}

void PlayScene::Draw(RenderWindow& window)

{
	DrawBackGroundView(window);

	window.setView(worldView);

	for (auto v : wall) {
		v->Draw(window);
		v->Draw(pass_diffuse, normals_shader, pass_normals);
	}
	DrawRenderedBuffer(window);

	if (madeorange) {
		orange->Draw(window);
	}

	if (madeblue) {
		blue->Draw(window);
	}

	if (goal != nullptr) {
		goal->Draw(window);
	}
	if (player != nullptr)
		player->Draw(window);

	for (auto v : button) {
		v->Draw(window);
	}

	for (auto v : cube) {
		v->Draw(window);
	}



	if (particle.running())
		window.draw(particle);

	window.setView(endingView);
	window.draw(ending);
}



PlayScene::PlayScene(string path)
	:light(sf::Vector3f(255 / 255.0, 214 / 255.0, 170 / 255.0),
		sf::Vector3f(0, 0, 0.02),
		sf::Vector3f(0.5, 0.5, 0.5)),
	falloff(0.5, 0.5, 0.5)
{

	b2Vec2 g(0.0f, -10);
	world = make_unique<b2World>(g);

	/////////////////////////////////////////////////////////////////////////////
	blue = new Blue;
	orange = new Orange;

	blue->SetSize({ 10,10 });
	orange->SetSize({ 10,10 });

	ifstream fin;
	fin.open(path);

	string str;
	//each line
	while (getline(fin, str)) {
		//verifying each character
		for (int i = 0; i < str.size() + 1; i++) {
			switch (str[i]) {
			case '1':
				if (str[i + 1] == '1') {
					MakeWall(false);
					wallbunchwidth += GRIDSIZE;
				}
				else {
					MakeWall(true);
					//cout<<
					box2dposition.x += currgrid.x + GRIDSIZE;
					wallbunchwidth = GRIDSIZE;
				}
				break;
			case 'p':
			case 'P':
				MakePlayer();
				break;
			case 'C':
			case 'c':
				MakeCube();
				break;

			case 'b':
			case 'B':
				//b"2 4 4 "(1)
			{
				i += 2;//index of gridposition number 
				string poslist;
				int posnum = str.find('"', i);
				poslist = str.substr(i, posnum - i);

				i += 3; //index of buttonId
				string idlist;
				int idnum = str.find(')', i);
				idlist = str.substr(i, idnum - i);
				//	MakeButton(poslist, idlist);
				i++;

				MakeButton(poslist, idlist);

				/////////
				i++;
				/////////

				break;
			}
			case'@':
			{
				string list;
				int num = str.find(')', i);
				list = str.substr(i + 2, num - i - 2);
				MakeGoal(list);
				i = num;
				break;
			}
			default:
				currgrid.x += GRIDSIZE;
				box2dposition.x = currgrid.x;

				///////
				tempContainer.push_back(nullptr);
				///////
			}

		}
		width = currgrid.x;		

		currgrid = { GRIDSIZE / 2, currgrid.y + GRIDSIZE };
		box2dposition = { GRIDSIZE / 2, currgrid.y + GRIDSIZE };
		wallbunchwidth = GRIDSIZE;

		height = currgrid.y;

		///////////
		objInfos.push_back(tempContainer);
		tempContainer.clear();
		///////////
	}

	goal->SetButtonlist(button);

	fin.close();

	/*for (int i = 0; i < objInfos.size(); i++)
	{
		for (int j = 0; j < objInfos[i].size(); j++)
		{
			if (objInfos[i][j] &&
				objInfos[i][j]->GetId() == '1')
			{
				if (i - 1 >= 0 &&
					objInfos[i - 1][j] &&
					objInfos[i - 1][j]->GetId() == '1')
				{
					Tile* temp = (Tile*)objInfos[i][j];
					temp->SetActiveSideTiles(0, false);
				}

				if (j + 1 < objInfos[i].size() &&
					objInfos[i][j + 1] &&
					objInfos[i][j + 1]->GetId() == '1')
				{
					Tile* temp = (Tile*)objInfos[i][j];
					temp->SetActiveSideTiles(1, false);
				}

				if (i + 1 < objInfos.size() &&
					objInfos[i + 1][j] &&
					objInfos[i + 1][j]->GetId() == '1')
				{
					Tile* temp = (Tile*)objInfos[i][j];
					temp->SetActiveSideTiles(2, false);
				}

				if (j - 1 >= 0 &&
					objInfos[i][j - 1] &&
					objInfos[i][j - 1]->GetId() == '1')
				{
					Tile* temp = (Tile*)objInfos[i][j];
					temp->SetActiveSideTiles(3, false);
				}
			}
		}
	}*/

	particle.init(500);
	worldView.setCenter(player->GetPos());

}

void PlayScene::MakeWall(bool isEnd)
{
	Tile* temp = new Tile(world.get(), currgrid, Vector2f({ wallbunchwidth, GRIDSIZE }), box2dposition, isEnd);
	temp->SetOrigin(Origins::MC);
	wall.push_back(temp);
	cout << wall.back()->GetPos().x << " " << wall.back()->GetPos().y << endl;

	currgrid.x += GRIDSIZE;

	///////////////
	tempContainer.push_back(temp);
	///////////////
}

void PlayScene::MakeCube()
{
	Cube* newCube = new Cube(world.get(), Vector2f{ currgrid }, Vector2f({ GRIDSIZE, GRIDSIZE }));

	cube.push_back(newCube);

	///////////////
	tempContainer.push_back(newCube);
	//////////////

	currgrid.x += GRIDSIZE;
}

void PlayScene::MakePlayer()
{

	player = new Player(world.get(), Vector2f{ currgrid }, Vector2f({ 20, 50 }));

	currgrid.x += GRIDSIZE;

	/////////////////
	tempContainer.push_back(player);
	/////////////////
}

void PlayScene::MakeButton(string dir, string id)
{
	for (auto& b : dir) {
		if (b == ' ')
			continue;

		//seek id
		string idtemp;
		for (int i = 0; i < id.size(); i++) {
			if (id[i] == ' ') {
				id.erase(i, i + 1);
				break;
			}
			idtemp.push_back(id[i]);
			id.erase(i, i + 1);
		}

		Button* temp = new Button();

		button.push_back(temp);
		button.back()->SetOrigin(Origins::BC);
		button.back()->SetButtonId(stoi(idtemp));
		button.back()->SetPos(currgrid);
		button.back()->SetRotation(atoi(&b));

		if (b == '0') {			//top of gird
			button.back()->SetPos({ currgrid.x,currgrid.y - GRIDSIZE / 2 });
			button.back()->SetSize({ GRIDSIZE,GRIDSIZE / 4 });

		}
		else if (b == '1') {	//right of gird
			button.back()->SetPos({ currgrid.x + GRIDSIZE / 2,currgrid.y });
			button.back()->SetSize({ GRIDSIZE,GRIDSIZE / 4 });

		}
		else if (b == '2') {	//bottom of gird
			button.back()->SetPos({ currgrid.x,currgrid.y + GRIDSIZE / 2 });
			button.back()->SetSize({ GRIDSIZE,GRIDSIZE / 4 });

		}
		else if (b == '3') {	//left of gird
			button.back()->SetPos({ currgrid.x - GRIDSIZE / 2,currgrid.y });
			button.back()->SetSize({ GRIDSIZE ,GRIDSIZE / 4 });

		}

		////////////
		tempContainer.push_back(temp);
		////////////
	}
	currgrid.x += GRIDSIZE;


}

void PlayScene::MakePortal()
{
	int bluecollidercount = 0;
	//wall=mc blue=mc

	float sety;
	float setx;

	bool intersect = false;
	RectangleShape blueTL;
	RectangleShape blueTR;
	RectangleShape blueBL;
	RectangleShape blueBR;

	Vector2f bluetlpos, bluetrpos, blueblpos, bluebrpos;
	bool bluetlhit = false;
	bool bluetrhit = false;
	bool blueblhit = false;
	bool bluebrhit = false;
	float bluey;
	float bluex;

	for (auto w : wall) {
		if (!madeblue && w->GetGlobalBounds().intersects(blue->GetGlobalBounds())) {
			blue->SetSize({ 50,50 });
			blue->SetDir({ 0,0 });

			bluetlpos = { blue->GetGlobalBounds().left,blue->GetGlobalBounds().top };
			blueTL.setSize({ 0.1f,0.1f });

			bluetrpos = { blue->GetGlobalBounds().left + blue->GetGlobalBounds().width,blue->GetGlobalBounds().top };
			blueTR.setSize({ 0.1f,0.1f });

			blueblpos = { blue->GetGlobalBounds().left ,blue->GetGlobalBounds().top + blue->GetGlobalBounds().height };
			blueBL.setSize({ 0.1f,0.1f });

			bluebrpos = { blue->GetGlobalBounds().left + blue->GetGlobalBounds().width,blue->GetGlobalBounds().top + blue->GetGlobalBounds().height };
			blueBR.setSize({ 0.1f,0.1f });

			blueBR.setPosition(bluebrpos);
			blueBL.setPosition(blueblpos);
			blueTR.setPosition(bluetrpos);
			blueTL.setPosition(bluetlpos);

			bluey = blue->GetPos().y;
			bluex = blue->GetPos().x;
			intersect = true;
			break;
		}
	}

	if (intersect)
	{
		for (auto w : wall) {
			if (!madeblue && w->GetGlobalBounds().contains(blueBL.getPosition())) {
				setx = w->GetGlobalBounds().left + w->GetGlobalBounds().width;
				sety = w->GetGlobalBounds().top;

				bluecollidercount++;
				blueblhit = true;

			}

			if (!madeblue && w->GetGlobalBounds().contains(blueBR.getPosition())) {

				setx = w->GetGlobalBounds().left;
				sety = w->GetGlobalBounds().top;
				bluecollidercount++;
				bluebrhit = true;

			}
			if (!madeblue && w->GetGlobalBounds().contains(blueTL.getPosition())) {

				setx = w->GetGlobalBounds().left + w->GetGlobalBounds().width;
				sety = w->GetGlobalBounds().top + w->GetGlobalBounds().height;
				bluecollidercount++;
				bluetlhit = true;

			}
			if (!madeblue && w->GetGlobalBounds().contains(blueTR.getPosition())) {

				setx = w->GetGlobalBounds().left;
				sety = w->GetGlobalBounds().top + w->GetGlobalBounds().height;
				bluecollidercount++;
				bluetrhit = true;

			}
		}

	}

	if (bluecollidercount == 2) {
		//bottom
		if (bluetlhit && bluetrhit) {
			blue->SetLightDir(90);
			blue->SetSize({ 50,20 });
			blue->SetPos({ blue->GetPos().x,sety });
			blue->SetPortalDir(2);
			cout << "low" << endl;
		}
		//left
		else if (bluetrhit && bluebrhit) {
			blue->SetLightDir(180);
			blue->SetSize({ 20,50 });
			blue->SetPos({ setx,blue->GetPos().y });
			blue->SetPortalDir(3);
			cout << "left" << endl;
		}
		//top
		else if (blueblhit && bluebrhit) {
			blue->SetLightDir(270);
			blue->SetSize({ 50,20 });
			blue->SetPos({ blue->GetPos().x,sety });
			blue->SetPortalDir(0);
			cout << "top" << endl;

		}
		//right
		else if (bluetlhit && blueblhit) {
			blue->SetLightDir(0);
			blue->SetSize({ 20,50 });
			blue->SetPos({ setx,bluey });
			blue->SetPortalDir(1);
			cout << "right" << endl;
		}
		madeblue = true;

		blue->SetDir({ 0,0 });
	}
	else if (bluecollidercount != 0) {
		particle.emitParticles(blue->GetPos(), false);
		blue->SetPos({ -1000,-1000 });
	}


	int orangecollidercount = 0;
	//wall=mc orange=mc

	float orangesety;
	float orangesetx;
	bool orangeintersect = false;
	RectangleShape orangeTL;
	RectangleShape orangeTR;
	RectangleShape orangeBL;
	RectangleShape orangeBR;

	Vector2f orangetlpos, orangetrpos, orangeblpos, orangebrpos;
	bool orangetlhit = false;
	bool orangetrhit = false;
	bool orangeblhit = false;
	bool orangebrhit = false;
	float orangey;
	float orangex;

	for (auto w : wall) {
		if (!madeorange && w->GetGlobalBounds().intersects(orange->GetGlobalBounds())) {
			orange->SetSize({ 50,50 });
			orange->SetDir({ 0,0 });


			orangetlpos = { orange->GetGlobalBounds().left,orange->GetGlobalBounds().top };
			orangeTL.setSize({ 0.1f,0.1f });

			orangetrpos = { orange->GetGlobalBounds().left + orange->GetGlobalBounds().width,orange->GetGlobalBounds().top };
			orangeTR.setSize({ 0.1f,0.1f });

			orangeblpos = { orange->GetGlobalBounds().left ,orange->GetGlobalBounds().top + orange->GetGlobalBounds().height };
			orangeBL.setSize({ 0.1f,0.1f });

			orangebrpos = { orange->GetGlobalBounds().left + orange->GetGlobalBounds().width,orange->GetGlobalBounds().top + orange->GetGlobalBounds().height };
			orangeBR.setSize({ 0.1f,0.1f });

			orangeBR.setPosition(orangebrpos);
			orangeBL.setPosition(orangeblpos);
			orangeTR.setPosition(orangetrpos);
			orangeTL.setPosition(orangetlpos);

			orangeintersect = true;
			orangex = orange->GetPos().x;
			orangey = orange->GetPos().y;
			break;
		}
	}

	if (orangeintersect)
	{
		for (auto w : wall) {
			if (!madeorange && w->GetGlobalBounds().contains(orangeBL.getPosition())) {
				orangesetx = w->GetGlobalBounds().left + w->GetGlobalBounds().width;
				orangesety = w->GetGlobalBounds().top;

				orangecollidercount++;
				orangeblhit = true;

			}

			if (!madeorange && w->GetGlobalBounds().contains(orangeBR.getPosition())) {

				orangesetx = w->GetGlobalBounds().left;
				orangesety = w->GetGlobalBounds().top;
				orangecollidercount++;
				orangebrhit = true;

			}
			if (!madeorange && w->GetGlobalBounds().contains(orangeTL.getPosition())) {

				orangesetx = w->GetGlobalBounds().left + w->GetGlobalBounds().width;
				orangesety = w->GetGlobalBounds().top + w->GetGlobalBounds().height;
				orangecollidercount++;
				orangetlhit = true;

			}
			if (!madeorange && w->GetGlobalBounds().contains(orangeTR.getPosition())) {

				orangesetx = w->GetGlobalBounds().left;
				orangesety = w->GetGlobalBounds().top + w->GetGlobalBounds().height;
				orangecollidercount++;
				orangetrhit = true;

			}
		}

	}

	if (orangecollidercount == 2) {

		//bottom
		if (orangetlhit && orangetrhit) {
			orange->SetLightDir(90);
			orange->SetSize({ 50,20 });
			orange->SetPos({ orange->GetPos().x,orangesety });
			orange->SetPortalDir(2);
			cout << "low" << endl;
		}
		//left
		else if (orangetrhit && orangebrhit) {
			orange->SetLightDir(180);
			orange->SetSize({ 20,50 });
			orange->SetPos({ orangesetx,orange->GetPos().y });
			orange->SetPortalDir(3);
			cout << "left" << endl;
		}
		//top
		else if (orangeblhit && orangebrhit) {
			orange->SetLightDir(270);
			orange->SetSize({ 50,20 });
			orange->SetPos({ orange->GetPos().x,orangesety });
			orange->SetPortalDir(0);
			cout << "top" << endl;

		}
		//right
		else if (orangetlhit && orangeblhit) {
			orange->SetLightDir(0);
			orange->SetSize({ 20,50 });
			orange->SetPos({ orangesetx,orangey });

			orange->SetPortalDir(1);
			cout << "right" << endl;
		}
		madeorange = true;
		orange->SetDir({ 0,0 });
	}
	else if (orangecollidercount != 0) {
		particle.emitParticles(orange->GetPos(), true);
		orange->SetPos({ -1000,-1000 });
	}

}

void PlayScene::MakeGoal(string list)
{
	goal = new Goal(currgrid, GRIDSIZE, list);

	currgrid.x += GRIDSIZE;

	////////////
	tempContainer.push_back(goal);
	////////////
}

void PlayScene::PushButton()
{
	for (auto b : button) {
		if (b->GetHitbox()->getGlobalBounds().intersects(player->GethitboxGlobalBounds())) {
			b->SetPressed(true);
			cout << "test1" << endl;
			break;
		}
	}

	for (auto b : button) {
		if (b->GetPressed() && !b->GetHitbox()->getGlobalBounds().intersects(player->GetGlobalBounds())) {
			b->SetPressed(false);
			cout << "test2" << endl;
			break;
		}
	}

	for (auto b : button) {
		for (auto c : cube) {
			if (!b->GetPressed() && !c->GetGlobalBounds().intersects(b->GetHitbox()->getGlobalBounds())) {
				b->SetPressed(false);

				break;
			}
		}
	}

	for (auto b : button) {
		for (auto c : cube) {
			if (!b->GetPressed() && c->GetGlobalBounds().intersects(b->GetHitbox()->getGlobalBounds())) {
				b->SetPressed(true);
				break;
			}
		}

	}
}

Vector2f PlayScene::CameraMove(Vector2f currpos, Vector2f playerpos,float alpah, float dt)
{
	//return currpos * (1 - dt) + playerpos * dt;
	float x = (currpos.x + alpah * (playerpos.x - currpos.x))*dt;
	float y = (currpos.y + alpah * (playerpos.y - currpos.y))*dt;

	return Vector2f(x, y);
}

void PlayScene::DrawBackGroundView(RenderWindow& window)
{
	window.setView(backgroundView);
	window.draw(background);
}

void PlayScene::DrawRenderedBuffer(RenderWindow& window)
{
	pass_diffuse.display();
	pass_normals.display();

	lights_shader.setParameter("resolution", sf::Vector2f(width, height));
	lights_shader.setParameter("sampler_normal", pass_normals.getTexture());
	lights_shader.setParameter("ambient_intensity", ambient_intensity);
	lights_shader.setParameter("falloff", falloff);

	lights_shader.setParameter("sampler_light", front->getTexture());
	lights_shader.setParameter("light_pos", light.position);
	lights_shader.setParameter("light_color", light.color);
	back->draw(sf::Sprite(pass_diffuse.getTexture()), &lights_shader);
	back->display();
	std::swap(back, front);

	// Draw diffuse color
	window.draw(sf::Sprite(pass_diffuse.getTexture()));
	// Blend lighting over
	window.draw(sf::Sprite(front->getTexture()), sf::BlendMultiply);
}

void PlayScene::Input()
{
	LightTestInputForDev();
	if (InputMgr::GetKeyDown(Keyboard::Escape)) {
		SCENE_MGR->ChangeScene(Scenes::GAMESTART);
	}

	if (InputMgr::GetMouseWheelState() == 1)
	{
		if (zoomCount > 20)
			return;
		zoomCount++;
		worldView.zoom(0.94f);
	}
	if (InputMgr::GetMouseWheelState() == -1)
	{
		if (zoomCount < -10)
			return;
		zoomCount--;
		worldView.zoom(1.06f);
	}

	if (InputMgr::GetMouseButton(Mouse::Middle))
	{
		isMovingViewCenter = true;
		Vector2f pos = InputMgr::GetMousePosDisplacement();
		worldView.setCenter(worldView.getCenter() + pos);
	}

	if (InputMgr::GetMouseButtonUp(Mouse::Middle))
	{
		isMovingViewCenter = false;
	}

	if (InputMgr::GetKeyDown(Keyboard::Num8))
	{
		SpriteObj::OnOffWiringState();
	}

	light.position.x = GetMouseWorldPos().x;
	light.position.y = height - GetMouseWorldPos().y;
}

void PlayScene::LightTestInputForDev()
{
	if (InputMgr::GetKeyDown(Keyboard::Numpad8))
	{
		light.position.z += 0.01f;
		cout << "light.position.z : " << light.position.z << endl;
	}
	if (InputMgr::GetKeyDown(Keyboard::Numpad2))
	{
		light.position.z -= 0.01f;
		cout << "light.position.z : " << light.position.z << endl;
	}
	if (InputMgr::GetKeyDown(Keyboard::Add))
	{
		ambient_intensity += 0.05f;
		cout << "ambient_intensity : " << ambient_intensity << endl;
	}
	if (InputMgr::GetKeyDown(Keyboard::Subtract))
	{
		ambient_intensity -= 0.05f;
		cout << "ambient_intensity : " << ambient_intensity << endl;
	}
	if (InputMgr::GetKeyDown(Keyboard::Numpad4))
	{
		falloff /= 0.5f;
		cout
			<< "falloff : "
			<< falloff.x << ','
			<< falloff.y << ','
			<< falloff.z << ','
			<< endl;
	}
	if (InputMgr::GetKeyDown(Keyboard::Numpad6))
	{
		falloff *= 0.5f;
		cout
			<< "falloff : "
			<< falloff.x << ','
			<< falloff.y << ','
			<< falloff.z << ','
			<< endl;
	}
}

void PlayScene::ClearRenderBuffer()
{
	back->clear();
	front->clear();
	pass_diffuse.clear(Color::Transparent);
	// Set normals buffer to neutral color
	pass_normals.clear(Color(128, 128, 255));
}


void PlayScene::MoveToPortal()
{
	if (!madeblue || !madeorange) {
		return;
	}
	if (madeblue && blue->GetGlobalBounds().intersects(player->GetGlobalBounds())) {
		if (orange->GetPortalDir() == 0) {
			player->SetPlayerBodyPos({ orange->GetPos().x,orange->GetPos().y - player->GetGlobalBounds().height });

			player->GetBody()->SetLinearVelocity({ player->GetPlayerBodyForce().x,player->GetRecentSpeed().y*-1+1});
		}
		else if (orange->GetPortalDir() == 1) {
			player->SetPlayerBodyPos({ orange->GetPos().x + 30,orange->GetPos().y });
			player->GetBody()->SetLinearVelocity({ player->GetRecentSpeed().y*-1 ,player->GetRecentSpeed().y });

		}
		else if (orange->GetPortalDir() == 2) {
			player->SetPlayerBodyPos({ orange->GetPos().x ,orange->GetPos().y + player->GetGlobalBounds().height });
			//	player->SetPlayerBodyForce({ player->GetPlayerBodyForce().x,-10000000 });
			player->GetBody()->SetLinearVelocity({ player->GetPlayerBodyForce().x,player->GetRecentSpeed().y-1 });
		}
		else if (orange->GetPortalDir() == 3) {
			player->SetPlayerBodyPos({ orange->GetPos().x - 30 ,orange->GetPos().y });
			//player->SetPlayerBodyForce({ 10000 * -333,0 });
			player->GetBody()->SetLinearVelocity({ player->GetRecentSpeed().y,player->GetPlayerBodyForce().y });


		}

	}

	if (madeorange && orange->GetGlobalBounds().intersects(player->GetGlobalBounds())) {
		if (blue->GetPortalDir() == 0) {
			player->SetPlayerBodyPos({ blue->GetPos().x,blue->GetPos().y - player->GetGlobalBounds().height });
			player->GetBody()->SetLinearVelocity({ player->GetPlayerBodyForce().x,player->GetRecentSpeed().y * -1 +1 });

		}
		else if (blue->GetPortalDir() == 1) {
			player->SetPlayerBodyPos({ blue->GetPos().x + 30,blue->GetPos().y });
			player->GetBody()->SetLinearVelocity({ player->GetRecentSpeed().y * -1 ,player->GetRecentSpeed().y });
		}
		else if (blue->GetPortalDir() == 2) {			
			player->SetPlayerBodyPos({ blue->GetPos().x ,blue->GetPos().y + player->GetGlobalBounds().height });
			player->GetBody()->SetLinearVelocity({ player->GetPlayerBodyForce().x,player->GetRecentSpeed().y - 1 });

		}
		else if (blue->GetPortalDir() == 3) {
			player->SetPlayerBodyPos({ blue->GetPos().x - 30 ,blue->GetPos().y });			
			player->GetBody()->SetLinearVelocity({ player->GetRecentSpeed().y,player->GetPlayerBodyForce().y });

		}
	}


	///////////////////////////////////////CUBE////////////////////////////////////////////////////
	for (auto c : cube) {
		if (madeblue && blue->GetGlobalBounds().intersects(c->GetGlobalBounds())) {
			if (orange->GetPortalDir() == 0) {
				c->SetCubeBodyPos({ orange->GetPos().x,orange->GetPos().y - c->GetGlobalBounds().height });
				player->GetBody()->SetLinearVelocity({ c->GetCubeBodyForce().x,c->GetRecentSpeed().y * -1 });

			}
			else if (orange->GetPortalDir() == 1) {
				c->SetCubeBodyPos({ orange->GetPos().x + c->GetGlobalBounds().width,orange->GetPos().y });
				c->GetBody()->SetLinearVelocity({ c->GetRecentSpeed().y * -1 ,c->GetRecentSpeed().y });
			}
			else if (orange->GetPortalDir() == 2) {
				c->SetCubeBodyPos({ orange->GetPos().x ,orange->GetPos().y + c->GetGlobalBounds().height });
				player->GetBody()->SetLinearVelocity({ c->GetCubeBodyForce().x,c->GetRecentSpeed().y });
			}
			else if (orange->GetPortalDir() == 3) {
				c->SetCubeBodyPos({ orange->GetPos().x - c->GetGlobalBounds().width ,orange->GetPos().y });
				player->GetBody()->SetLinearVelocity({ c->GetRecentSpeed().y,c->GetCubeBodyForce().y });
			}
		}


		if (madeorange && orange->GetGlobalBounds().intersects(c->GetGlobalBounds())) {
			if (blue->GetPortalDir() == 0) {
				c->SetCubeBodyPos({ blue->GetPos().x,blue->GetPos().y - c->GetGlobalBounds().height });
				player->GetBody()->SetLinearVelocity({ c->GetCubeBodyForce().x,c->GetRecentSpeed().y * -1 });
			}
			else if (blue->GetPortalDir() == 1) {
				c->SetCubeBodyPos({ blue->GetPos().x + c->GetGlobalBounds().width,blue->GetPos().y });
				c->GetBody()->SetLinearVelocity({ c->GetRecentSpeed().y * -1 ,c->GetRecentSpeed().y });

			}
			else if (blue->GetPortalDir() == 2) {
				c->SetCubeBodyPos({ blue->GetPos().x ,blue->GetPos().y + c->GetGlobalBounds().height });
				player->GetBody()->SetLinearVelocity({ c->GetCubeBodyForce().x,player->GetRecentSpeed().y });
			}
			else if (blue->GetPortalDir() == 3) {
				c->SetCubeBodyPos({ blue->GetPos().x - c->GetGlobalBounds().width ,blue->GetPos().y });
				player->GetBody()->SetLinearVelocity({ c->GetRecentSpeed().y,c->GetCubeBodyForce().y });
			}
		}
	}
	
	cout << player->GetPlayerBodyForce().x << endl<<" "<< player->GetPlayerBodyForce().y << endl;
}




PlayScene::~PlayScene()
{
	Release();
}

void PlayScene::Init()
{
}

void PlayScene::Release()
{
	FRAMEWORK->GetWindow().setView(FRAMEWORK->GetWindow().getDefaultView());
	delete player;

	for (auto v : wall) {
		delete v;
	}
	wall.clear();

	for (auto v : button) {
		delete v;
	}
	button.clear();

	for (auto v : cube) {
		delete v;
	}
	cube.clear();

	delete orange;
	delete blue;
	delete goal;

}

void PlayScene::Enter()
{
	background.setTexture(*RESOURCEMGR->GetTexture("Graphics/backgrounds/ruin2.png"));
	Utils::SetSpriteSize(background, { WINDOW_WIDTH,WINDOW_HEIGHT });
	auto size = (Vector2f)FRAMEWORK->GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter(size / 2.f);
	FRAMEWORK->GetWindow().setView(worldView);

	backgroundView.setSize(size);
	backgroundView.setCenter(size / 2.f);
	ending.setSize(size);
	ending.setFillColor(Color(0, 0, 0, 0));

	endingView.setSize(size);
	endingView.setCenter(size / 2.f);

	Tile::SetIsPlayingGame(true);
	zoomCount = 0;
	isMovingViewCenter = false;

	front = unique_ptr<RenderTexture>(new RenderTexture());
	back = unique_ptr<RenderTexture>(new RenderTexture());

	front->create(width, height);
	back->create(width, height);

	pass_normals.create(width, height);
	pass_diffuse.create(width, height);

	lights_shader.loadFromFile("Shader/light.frag", Shader::Fragment);
	normals_shader.loadFromFile("Shader/normals.frag", Shader::Fragment);
}

void PlayScene::Exit()
{
	Tile::SetIsPlayingGame(false);
	zoomCount = 0;
}
