#pragma once

#include"Render.hpp"
#include"Player.hpp"
#include"Map.hpp"
#include"KeyBoard.hpp"

class MainGame
{
public:
	MainGame(Render& render, KeyBoard& keyboard, Location loc, int room) :
		render{ render }, keyBoard{ keyboard }
	{
		map.LoadFromFile(loc, room);
	}

	void Run()
	{
		while (render.IsOpen() && !keyBoard.IsPause())
		{
			GetEvent();
			TicksUpdate();
			render.GameMapDraw(map, silant);
		}
	}


private:
	Render& render;
	KeyBoard& keyBoard;

	GameMap map;
	Player silant{ "Sprites\\Player.png", sf::IntRect{0, 0, 16, 34} };

	int ticks = 0;

	void GetEvent()
	{
		render.PollEvent();
		if (keyBoard.IsUp())
			if (!map.InBocks(silant.GetBarierBoxAspir() - sf::Vector2f{0.f, silant.GetSpeed()}))
				silant.Up();
		if (keyBoard.IsLeft())
			if (!map.InBocks(silant.GetBarierBoxAspir() - sf::Vector2f{silant.GetSpeed(), 0.f}))
				silant.Left();
		if (keyBoard.IsDown())
			if (!map.InBocks(silant.GetBarierBoxAspir() + sf::Vector2f{0.f, silant.GetSpeed()}))
				silant.Down();
		if (keyBoard.IsRight())
			if (!map.InBocks(silant.GetBarierBoxAspir() + sf::Vector2f{silant.GetSpeed(), 0.f}))
				silant.Right();
		if (keyBoard.IsPressed(sf::Keyboard::Space))
		{
			float x,  y;
			x = toFloat(rand() % 200);
			y = toFloat(rand() % 200);
			silant.SetFootCenterPosition(x, y);
			silant.SetAspirCenterFootPos(x, y);
		}
	}

	void TicksUpdate()
	{
		silant.UpdatePosition();
		if (ticks % 100 == 0)
		{
			silant.UpdateAnum();
		}
		if (ticks > 1000000)
			ticks = 0;
		ticks++;
	}

};

class Game
{
public:
	Game()
	{
		render.Zoom(0.25);
	}

	void Run()
	{
		//MainMenu() TODO
		MainGame game(render, keyboard, Location::Default, 4);
		game.Run();
	}


private:

	Render render;
	KeyBoard keyboard;


};
