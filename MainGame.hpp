#pragma once

#include"Render.hpp"
#include"Player.hpp"
#include"Map.hpp"
#include"KeyBoard.hpp"
#include"WindowDialog/DialogeMutiWindow.hpp"

class MainGame
{
public:
	MainGame(Render& render, KeyBoard& keyboard, Location loc, int room) :
		render{ render }, keyBoard{ keyboard }, dielogeM{ silant.GetInfoLink() }
	{
		render.Zoom(0.25f);

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
		render.Zoom(4.f);
	}


private://параметры
	Render& render;
	KeyBoard& keyBoard;

	MultiDialogeWindow dielogeM;

	GameMap map;
	Player silant{ "Sprites\\Player.png", sf::IntRect{0, 0, 16, 34} };

	int ticks = 0;

private://методы

	void DrawAll()
	{
		render.Clear();

		render.GameMapDraw(map, silant);

		if (dielogeM.IsActive())
			render.DrawMultyWindow(dielogeM);

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

		if (dielogeM.IsActive())
		{
			dielogeM.GetEvent(keyBoard);
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
			if (keyBoard.IsNextClick())
			{
				sf::String txt;
				if (map.HaveIntersectionWithObjs(silant.GetViewPosition(), txt) && !txt.isEmpty())
					dielogeM.RunDialoge(txt);
				else if (map.HaveIntersectionWithIObjs(silant.GetViewPosition(), txt) && !txt.isEmpty())
					dielogeM.RunDialoge(txt);
			}
			else if (keyBoard.IsInventoryClick())
				dielogeM.RunInventory();
			if (sucs)
				ProvGoRoom();
		}

		if (keyBoard.IsPressed(sf::Keyboard::Space))
		{
			float x, y;
			x = toFloat(rand() % 500 - 100);
			y = toFloat(rand() % 500 - 100);
			silant.SetFootCenterPosition(x, y);
			silant.SetAspirCenterFootPos(x, y);
		}

		if (keyBoard.IsFullScreenClick())
			render.SetFullScreen(!render.IsFullScreen());

		if (keyBoard.IsPressed(sf::Keyboard::P))
			dielogeM.RunAsk(L"Взять зонтик!", L"Оставить зонтик");
	}

	void TicksUpdate()
	{
		silant.UpdatePosition();
		if (ticks % 100 == 0)
			silant.UpdateAnum();

		if (dielogeM.IsActive())
		{
			dielogeM.SetPosAtCenter(render.GetCenterPos(), render.GetSize());
			if (ticks % 10 == 0)
				dielogeM.Update();
		}
		if (ticks > 1000000)
			ticks = 0;
		ticks++;
	}
};