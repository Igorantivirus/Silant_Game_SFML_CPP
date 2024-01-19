#pragma once

#include<vector>

#include<SFML/Graphics.hpp>

#include"Render.hpp"
#include"KeyBoard.hpp"

#define SETTING_MENU_TEXTURE "Textures/SettingsMenu.png"

class SettingsMenu
{
public:
	SettingsMenu(Render& render, KeyBoard& keyboard) : render{ render }, keyboard{ keyboard }
	{
		texture.loadFromFile(SETTING_MENU_TEXTURE);
		background.setTexture(&texture);
		background.setSize({ render.GetWidth(), render.GetHeight() });

		background.setPosition(render.GetNullPos());
	}

	void Run()
	{
		isOpen = true;
		while (isOpen)
		{
			Draw();
			GetEvents();
			Update();
		}
	}



private:
	Render& render;
	KeyBoard& keyboard;

	bool isOpen{};

	sf::Texture texture;
	sf::RectangleShape background;

	std::vector<sf::Text> texts;

private:

	void Draw()
	{
		render.Clear();

		render.DrawRectangle(background);
		for (const auto& i : texts)
			render.DrawText(i);

		render.FinalizeRender();
	}

	void GetEvents()
	{
		render.PollEvent();
		if (keyboard.IsFullScreenClick())
			render.SetFullScreen(!render.IsFullScreen());
		if (keyboard.IsBackClick())
			isOpen = false;
	}

	void Update()
	{

	}
};