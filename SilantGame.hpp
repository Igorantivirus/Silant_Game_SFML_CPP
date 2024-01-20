#pragma once


#include"Converter.hpp"
#include<SFMLmyplus/out.hpp>
#include"Render.hpp"
#include"KeyBoard.hpp"
#include"MainGame.hpp"
#include"AllMenu.hpp"

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
			else
			{
				GameInfo infomenu(render, keyboard);
				infomenu.Run();
			}
		}
	}


private:
	Render render;
	KeyBoard keyboard;
};
