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
		silant.SetFootCenterPosition(7 * PIXELS_IN_BLOCK, 7 * PIXELS_IN_BLOCK);
		silant.StopRun();
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

	void ProvGoRoom()
	{
		size_t pos;
		if (map.InOpenDoor(silant.GetBarierBox(), pos))
		{
			render.BlackWindow();
			auto pr = map.GetDootAt(pos);
			map.LoadFromFile(pr.GetNextLock(), pr.GetNextNomer());
			silant.SetFootCenterPosition(pr.GetNextPos());
			silant.StopRun();
		}
			
	}

	void GetEvent()
	{
		render.PollEvent();
		bool sucs = false;
		if (keyBoard.IsUp())
			if (!map.InBocks(silant.GetBarierBoxAspir() - sf::Vector2f{0.f, silant.GetSpeed()}))
				(sucs = true), silant.Up();
		if (keyBoard.IsLeft())
			if (!map.InBocks(silant.GetBarierBoxAspir() - sf::Vector2f{silant.GetSpeed(), 0.f}))
				(sucs = true), silant.Left();
		if (keyBoard.IsDown())
			if (!map.InBocks(silant.GetBarierBoxAspir() + sf::Vector2f{0.f, silant.GetSpeed()}))
				(sucs = true), silant.Down();
		if (keyBoard.IsRight())
			if (!map.InBocks(silant.GetBarierBoxAspir() + sf::Vector2f{silant.GetSpeed(), 0.f}))
				(sucs = true), silant.Right();
		if (keyBoard.IsPressed(sf::Keyboard::Space))
		{
			float x,  y;
			x = toFloat(rand() % 200);
			y = toFloat(rand() % 200);
			silant.SetFootCenterPosition(x, y);
			silant.SetAspirCenterFootPos(x, y);
		}
		if (keyBoard.IsPressed(sf::Keyboard::K))
		{
			render.BlackWindow();
		}

		if (sucs)
			ProvGoRoom();

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
