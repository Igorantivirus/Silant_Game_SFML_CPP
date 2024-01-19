#pragma once

#include"Render.hpp"
#include"KeyBoard.hpp"
#include"MainMenu.hpp"
#include"MainGame.hpp"
#include"SettingsMenu.hpp"

class Game
{
public:
	Game() { }

	void Run()
	{
		while (render.IsOpen())
		{
			MainMenu menu{ render, keyboard };
			menu.Run();

			auto res = menu.GetResult();
			
			if (res == MainMenu::Exit)
				render.CloseWindow();
			else if (res == MainMenu::Game)
			{
				MainGame game(render, keyboard, Location::Default, 5);
				game.Run();
			}
			else if (res == MainMenu::Settings)
			{
				SettingsMenu settings(render, keyboard);
				settings.Run();
			}
		}
	}


private:
	Render render;
	KeyBoard keyboard;


};
