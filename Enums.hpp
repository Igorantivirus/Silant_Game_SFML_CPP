#pragma once

enum class Rotation : unsigned char
{
	Up,
	Down,
	Right,
	Left
};

enum class Location : unsigned char
{
	Default,
	First
};

sf::String NameOfLoc(Location loc)
{
	switch (loc)
	{
	case Location::Default:
		return "Rooms\\";
	case Location::First:
		return "Rooms\\";
	}
	return "error_room";
}
std::string NameOfLocSTD(Location loc)
{
	switch (loc)
	{
	case Location::Default:
		return "Rooms\\";
	case Location::First:
		return "Rooms\\";
	}
	return "error_room";
}