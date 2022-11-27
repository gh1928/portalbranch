#pragma once
#include <vector>
#include <nlohmann/json.hpp>

struct Map_Size_Struct
{
	int row;
	int col;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Map_Size_Struct, row, col);
};

/////

struct Object_struct
{
	int posX;
	int posY;	
};

/////

struct Player_struct : public Object_struct
{
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Player_struct, posX, posY);
};

struct Goal_struct : public Object_struct
{
	vector<int> buttonList;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Goal_struct, posX, posY, buttonList);
};

struct Tile_struct : public Object_struct
{
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Tile_struct, posX, posY);
};

struct Cube_struct : public Object_struct
{
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Cube_struct, posX, posY);
};

struct Button_sturct : public Object_struct
{
	int id;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Button_sturct, posX, posY, id);
};

struct Data_struct
{
	Map_Size_Struct size;
	Player_struct player;
	Goal_struct goal;
	vector<Tile_struct> tiles;
	vector<Cube_struct> cubes;
	vector<Button_sturct> buttons;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data_struct, size, player, goal, tiles, cubes, buttons);
};