#pragma once

#include"Render.hpp"
#include"Player.hpp"
#include"Map.hpp"
#include"KeyBoard.hpp"
#include"DIalogeWindow.hpp"

class MainGame
{
public:
	MainGame(Render& render, KeyBoard& keyboard, Location loc, int room) :
		render{ render }, keyBoard{ keyboard }, dialoge{ render, keyboard }
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


private://параметры
	Render& render;
	KeyBoard& keyBoard;
	DialogeWindow dialoge;

	GameMap map;
	Player silant{ "Sprites\\Player.png", sf::IntRect{0, 0, 16, 34} };

	int ticks = 0;

private://методы

	void DialogeWindow(const sf::String& txt = "")
	{
		std::cout << "tut\n";
		sf::Sprite sprite;// {dialoge, StaticFabricRect::RectDialogeWindow()};
		//sprite.setTexture(dialoge);
		//sprite.setTextureRect(StaticFabricRect::RectDialogeWindow());


		auto pos = render.GetCenterPos();
		/*pos.x -= sprite.getTextureRect().width / 2.f;
		pos.y += render.GetSize().y / 2.f - sprite.getTextureRect().height;*/
		sprite.setPosition((float)pos.x, (float)pos.y);

		render.DrawSprite(sprite);

		std::cout << sprite.getPosition() << '\n';
		std::cout << silant.GetCenterPosition() << '\n';

		sf::Clock clock;
		while (clock.getElapsedTime().asSeconds() < 1)
		{
			render.DrawSprite(sprite);
			/*render.PollEvent();
			render.PrintText("negr", sprite);*/
		}


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
			dialoge.DrawString("negr");
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
		MainGame game(render, keyboard, Location::Default, 1);
		game.Run();
	}


private:

	Render render;
	KeyBoard keyboard;


};
