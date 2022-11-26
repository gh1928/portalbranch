#include <iostream>
#include "FrameWork/stdafx.h"
#include "FrameWork/Const.h"
#include "FrameWork/Framework.h"


int main()
{
	FRAMEWORK->Init(WINDOW_WIDTH, WINDOW_HEIGHT);
	FRAMEWORK->Do();

	return 0;

}