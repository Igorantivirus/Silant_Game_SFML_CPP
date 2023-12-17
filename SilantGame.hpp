#pragma once

#include"Render.hpp"
#include"Player.hpp"
#include"Map.hpp"
#include"KeyBoard.hpp"
#include"DIalogeWindow.hpp"
#include"InventoryWindow.hpp"

class MainGame
{
public:
	MainGame(Render& render, KeyBoard& keyboard, Location loc, int room) :
		render{ render }, keyBoard{ keyboard }, invent{silant.GetInfoLink()}
	{
		map.LoadFromFile(loc, room);
		silant.SetFootCenterPosition(7 * PIXELS_IN_BLOCK, 7 * PIXELS_IN_BLOCK);
		silant.StopRun();
	}

	void Run()
	{
		while (render.IsOpen() && !keyBoard.IsPause())
		{
			DrawAll();
			GetEvent();
			TicksUpdate();
		}
	}


private://параметры
	Render& render;
	KeyBoard& keyBoard;

	DialogeWindow dialoge;
	InventoryWindow invent;

	GameMap map;
	Player silant{ "Sprites\\Player.png", sf::IntRect{0, 0, 16, 34} };

	int ticks = 0;
	sf::Clock clock;

private://методы

	void DrawAll()
	{
		render.Clear();

		render.GameMapDraw(map, silant);

		if (dialoge.IsAvtive())
			render.DrawDialoge(dialoge);
		if (invent.IsActive())
			render.DrawInventory(invent);

		render.FinalizeRender();
	}

	void ProvGoRoom()
	{
		size_t pos;
		if (map.InOpenDoor(silant.GetBarierBox(), pos))
		{
			render.BlackWindow();
			auto pr = map.GetDoorAt(pos);
			map.LoadFromFile(pr.GetNextLock(), pr.GetNextNomer());
			silant.SetFootCenterPosition(pr.GetNextPos());
			silant.StopRun();
		}
			
	}

	void GetEvent()
	{
		render.PollEvent();


		if (dialoge.IsAvtive())
		{
			if(GoodTime())
				dialoge.GetEvent(keyBoard);
		}
		else if (invent.IsActive())
		{
			if (GoodTime())
				invent.GetEvent(keyBoard);
		}
		else
		{
			#pragma region WASD
			bool sucs = false;
			if (keyBoard.IsUp())
			{
				silant.SetRotation(Rotation::Up);
				if (!map.InBocks(silant.GetBarierBoxAspir() - sf::Vector2f{0.f, silant.GetSpeed()}))
					(sucs = true), silant.Up();
			}
			if (keyBoard.IsLeft())
			{
				silant.SetRotation(Rotation::Left);
				if (!map.InBocks(silant.GetBarierBoxAspir() - sf::Vector2f{silant.GetSpeed(), 0.f}))
					(sucs = true), silant.Left();
			}
			if (keyBoard.IsDown())
			{
				silant.SetRotation(Rotation::Down);
				if (!map.InBocks(silant.GetBarierBoxAspir() + sf::Vector2f{0.f, silant.GetSpeed()}))
					(sucs = true), silant.Down();
			}
			if (keyBoard.IsRight())
			{
				silant.SetRotation(Rotation::Right);
				if (!map.InBocks(silant.GetBarierBoxAspir() + sf::Vector2f{silant.GetSpeed(), 0.f}))
					(sucs = true), silant.Right();
			}
			#pragma endregion
			if (keyBoard.IsNext() && GoodTime())
			{
				sf::String txt;
				if (map.HaveIntersectionWithObjs(silant.GetViewPosition(), txt) && !txt.isEmpty())
					dialoge.Run(txt);
			}
			else if (keyBoard.IsInventory() && GoodTime())
				invent.Run();
			if (sucs)
				ProvGoRoom();
		}
		if (keyBoard.IsPressed(sf::Keyboard::Space))
		{
			float x,  y;
			x = toFloat(rand() % 500 - 100);
			y = toFloat(rand() % 500 - 100);
			silant.SetFootCenterPosition(x, y);
			silant.SetAspirCenterFootPos(x, y);
		}
		if (keyBoard.IsPressed(sf::Keyboard::F11))
			render.SetFullScreen(!render.IsFullScreen());
	}

	void TicksUpdate()
	{
		silant.UpdatePosition();
		if (ticks % 100 == 0)
			silant.UpdateAnum();

		if (dialoge.IsAvtive())
		{
			dialoge.SetPositionAtWindow(sf::Vector2f{render.GetCenterX(), render.GetCenterY() + render.GetSize().y / 2.f});
			if(ticks % 10 == 0)
				dialoge.Update();
		}
		else if (invent.IsActive())
		{
			if (invent.IsWait())
			{
				dialoge.Run(invent.GetWaitStr());
				invent.Stop();
			}
			else if (ticks % 10 == 0)
				invent.Update();
			invent.SetPositionAtWindow(render.GetCenterPos());
		}

		if (ticks > 1000000)
			ticks = 0;
		ticks++;
	}

	bool GoodTime()
	{
		if (clock.getElapsedTime().asMilliseconds() > 100)
			return clock.restart(), true;
		return false;
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
		MainGame game(render, keyboard, Location::Default, 5);
		game.Run();
	}


private:

	Render render;
	KeyBoard keyboard;


};
