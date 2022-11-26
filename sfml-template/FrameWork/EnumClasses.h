#pragma once

//enum class Origins
//{
//	TL,	// 0
//	TC,
//	TR,
//	ML,
//	MC,
//	MR,
//	BL,
//	BC,
//	BR,	// 8
//};

enum class Scenes
{
	GAMESTART,
	MAPEDITER,
	MAPSELECTER,
};

enum class AnimationType
{
	None = -1,
	Projectile,
	Actor,	
	Background,
};

enum class WirePos
{
	None = -1,
	Top,
	Left,
	Right,
	Bottom,
};
