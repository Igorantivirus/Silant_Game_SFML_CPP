#pragma once

#include"ResourceMeneger.hpp"
#include"Render.hpp"
#include"Player.hpp"
#include"Map.hpp"
#include"KeyBoard.hpp"
#include"DialogeMutiWindow.hpp"

class MainGame
{
public:
	MainGame(Render& render, KeyBoard& keyboard, Location loc, int room) :
		meneger{}, render{ render }, keyBoard{ keyboard }, silant{meneger}, dielogeM { silant.GetInfoLink(), meneger }, map{meneger}
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
	ResourceMeneger meneger;

	MultiDialogeWindow dielogeM;

	GameMap map;
	Player silant;

	unsigned short ticks = 0;

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
			bool sucs = false;
			if (keyBoard.IsUp())
				silant.Up(), sucs = true;
			if (keyBoard.IsDown())
				silant.Down(), sucs = true;
			if (keyBoard.IsLeft())
				silant.Left(), sucs = true;
			if (keyBoard.IsRight())
				silant.Right(), sucs = true;
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
		}

		if (keyBoard.IsFullScreenClick())
			render.SetFullScreen(!render.IsFullScreen());

		if (keyBoard.IsPressed(sf::Keyboard::P))
			dielogeM.RunAsk(L"Взять зонтик!", L"Оставить зонтик");
	}

	void TicksUpdate()
	{

		if (map.InBocks(silant.GetBarierBox() + sf::Vector2f{0.f, silant.GetNextY()}))
			silant.SetAspirY(0.f);
		if (map.InBocks(silant.GetBarierBox() + sf::Vector2f{silant.GetNextX(), 0.f}))
			silant.SetAspirX(0.f);
		silant.UpdatePosition();

		if(ticks % 150 == 0)
			silant.UpdateAnum();

		if (dielogeM.IsActive())
		{
			dielogeM.SetPosAtCenter(render.GetCenterPos(), render.GetSize());
			if (ticks % 10 == 0)
				dielogeM.Update();
		}
		if (ticks > 10000)
			ticks = 0u;
		ticks++;
	}
};