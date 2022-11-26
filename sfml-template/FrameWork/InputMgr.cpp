#include "InputMgr.h"
#include <iostream>
#include <algorithm>
#include "Framework.h"

map<Axis, AxisInfo> InputMgr::axisInfoMap;

list<int> InputMgr::downList;
list<int> InputMgr::ingList;
list<int> InputMgr::upList;

Vector2f InputMgr::mousePos;

Vector2f InputMgr::prevMousePos;
Vector2f InputMgr::mousePosDisplacement;

int InputMgr::mouseWheel;
char InputMgr::textInput = 0;

void InputMgr::Init()
{
	// Horizontal
	AxisInfo infoH;
	infoH.axis = Axis::Horizontal;
	infoH.positives.push_back(Keyboard::Key::D);
	infoH.positives.push_back(Keyboard::Key::Right);

	infoH.negatives.push_back(Keyboard::Key::A);
	infoH.negatives.push_back(Keyboard::Key::Left);

	infoH.sensi = 5.f;
	infoH.value = 0.f;

	axisInfoMap.insert({ infoH.axis, infoH });
	
	// Vertical
	AxisInfo infoV;
	infoV.axis = Axis::Vertical;

	infoV.positives.push_back(Keyboard::Key::S);
	infoV.positives.push_back(Keyboard::Key::Down);

	infoV.negatives.push_back(Keyboard::Key::W);
	infoV.negatives.push_back(Keyboard::Key::Up);

	infoV.sensi = 5.f;
	infoV.value = 0.f;

	axisInfoMap.insert({ infoV.axis, infoV });
}

void InputMgr::Update(float dt)
{
	downList.clear();
	upList.clear();
	mouseWheel = 0;
	textInput = 0;

	for (auto& it : axisInfoMap)
	{
		AxisInfo& axis = it.second;
		float raw = GetAxisRaw(axis.axis);
		if (raw == 0.f && axis.value != 0.f)
			raw = axis.value > 0.f ? -1.f : 1.f;
		axis.value += raw * axis.sensi * dt;

		if (axis.value > 1.0)
			axis.value = 1.f;
		if (axis.value < -1.0)
			axis.value = -1.f;
		if (abs(axis.value) < 1.f / 60)
			axis.value = 0.f;
	}
		
	mousePos = (Vector2f)Mouse::getPosition(FRAMEWORK->GetWindow());

	mousePosDisplacement = prevMousePos - mousePos;
	prevMousePos = mousePos;
}

void InputMgr::ProcessInput(Event& ev)
{
	switch (ev.type)
	{
	case Event::EventType::MouseButtonPressed:
	{
		int code = ev.mouseButton.button + Keyboard::KeyCount;
		if (find(ingList.begin(), ingList.end(), code) == ingList.end())
		{
			downList.push_back(code);
			ingList.push_back(code);
		}
	}
		break;
	case Event::EventType::MouseButtonReleased:
	{
		int code = ev.mouseButton.button + Keyboard::KeyCount;

		ingList.remove(code);
		upList.push_back(code);
	}
		break;
	case Event::EventType::KeyPressed:
		if (find(ingList.begin(), ingList.end(), ev.key.code) == ingList.end())
		{
			downList.push_back(ev.key.code);
			ingList.push_back(ev.key.code);
		}
		break;
	case Event::EventType::KeyReleased:
		ingList.remove(ev.key.code);
		upList.push_back(ev.key.code);
		break;
	case Event::EventType::MouseWheelMoved:
		mouseWheel = ev.mouseWheel.delta;		
		break;
	case Event::TextEntered:
		textInput = ev.text.unicode;

	}	
}

bool InputMgr::GetKeyDown(Keyboard::Key key)
{
	return find(downList.begin(), downList.end(), key) != downList.end();
}

bool InputMgr::GetKey(Keyboard::Key key)
{
	return find(ingList.begin(), ingList.end(), key) != ingList.end();
}

bool InputMgr::GetKeyUp(Keyboard::Key key)
{
	return find(upList.begin(), upList.end(), key) != upList.end();
}

float InputMgr::GetAxis(Axis axis)
{
	return axisInfoMap[axis].value;
}

float InputMgr::GetAxisRaw(Axis axis)
{
	const AxisInfo& info = axisInfoMap[axis];
	auto it = ingList.rbegin();
	while (it != ingList.rend())
	{
		if (*it < Keyboard::KeyCount)
		{
			Keyboard::Key key = (Keyboard::Key)*it;
			if (find(info.negatives.begin(), info.negatives.end(), key) !=
				info.negatives.end())
			{
				return -1.f;
			}
			if (find(info.positives.begin(), info.positives.end(), key) !=
				info.positives.end())
			{
				return 1.f;
			}
		}
		++it;
	}
	return 0.f;

}

const Vector2f& InputMgr::GetMousePos()
{
	return mousePos;
}

bool InputMgr::GetMouseButtonDown(Mouse::Button key)
{
	int code = key + Keyboard::KeyCount;
	return find(downList.begin(), downList.end(), code) != downList.end();
}

bool InputMgr::GetMouseButton(Mouse::Button key)
{
	int code = key + Keyboard::KeyCount;
	return find(ingList.begin(), ingList.end(), code) != ingList.end();
}

bool InputMgr::GetMouseButtonUp(Mouse::Button key)
{
	int code = key + Keyboard::KeyCount;
	return find(upList.begin(), upList.end(), code) != upList.end();
}

int InputMgr::GetMouseWheelState()
{
	return mouseWheel;
}

char InputMgr::GetTextInput()
{
	return textInput;
}


Vector2f InputMgr::GetMousePosDisplacement()
{
	return mousePosDisplacement;
}
