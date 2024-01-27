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
	MainGame(Render& render, KeyBoard& keyboard, int room) :
		meneger{}, render{ render }, keyBoard{ keyboard }, silant{meneger}, dielogeM { silant.GetInfoLink(), meneger }, map{meneger}
	{
		render.Zoom(0.25f);

		map.LoadFromFile(room);
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
			map.LoadFromFile(pr.GetNextNomer());
			silant.SetFootCenterPosition(pr.GetNextPos());
			silant.StopRun();
		}

	}

	void GetEvent()
	{
		render.PollEvent();

		if (dielogeM.IsActive())
			dielogeM.GetEvent(keyBoard);
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
				int ind;
				if (map.HaveIntersectionWithObjs(silant.GetViewPosition(), txt) && !txt.isEmpty())
					dielogeM.RunDialoge(txt);
				else if (map.HaveIntersectionWithIObjs(silant.GetViewPosition(), txt, ind) && !txt.isEmpty())
				{
					map.SetWaitItemIndex(ind);
					dielogeM.RunAsk(txt);
				}
			}
			else if (keyBoard.IsInventoryClick())
				dielogeM.RunInventory();
			if (sucs)
				ProvGoRoom();
		}

		if (keyBoard.IsRTPClick())
			silant.SetFootCenterPosition(toFloat(rand() % 500 - 100), toFloat(rand() % 500 - 100));
		if (keyBoard.IsSPosClick())
			print(silant.GetCenterPosition(), enter);
		if (keyBoard.IsPressed(sf::Keyboard::Q))
			render.CloseWindow();

		if (keyBoard.IsFullScreenClick())
			render.SetFullScreen(!render.IsFullScreen());
	}

	void TicksUpdate()
	{

		if (map.InBocks(silant.GetBarierBox() + sf::Vector2f{0.f, silant.GetNextY()}))
			silant.SetAspirY(0.f);
		if (map.InBocks(silant.GetBarierBox() + sf::Vector2f{silant.GetNextX(), 0.f}))
			silant.SetAspirX(0.f);
		silant.UpdatePosition();

		if (map.GetWaitItemInd() > -1 && !dielogeM.IsActive() && dielogeM.GetCaretResult() == 1)
		{
			if (!silant.AddItem(map.GetIObjs()[map.GetWaitItemInd()].GetIDOfItem()))
			{
				dielogeM.RunDialoge(L"У заполнен инвентарь!");
				map.SetWaitItemIndex(-1);
			}
			else
				map.DeleteIObjAtInd();
		}

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