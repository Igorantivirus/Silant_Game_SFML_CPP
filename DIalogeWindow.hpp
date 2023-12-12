#pragma once

#include<SFML\Graphics.hpp>

#include"Render.hpp"
#include"KeyBoard.hpp"

#define DIALOGE_PATH "Sprites\\Text.png"
#define FONT_PATH "constan.ttf"

class DialogeWindow
{
public:
	DialogeWindow(Render& render, KeyBoard& keyboard) :
		render{ render }, keyboard{ keyboard }
	{
		dialogeTexture.loadFromFile(DIALOGE_PATH);
		sprite.setTexture(dialogeTexture);

		gameFont.loadFromFile(FONT_PATH);
		text.setFont(gameFont);
	}

	void DrawString(const sf::String& txt)
	{
		sf::Texture currentWindow;
		render.SaveToTexture(currentWindow);
		sf::Sprite curSprite{currentWindow};
		curSprite.setPosition(render.GetNullPos());



		SetPositionToSprite();

		int tick = 1;
		size_t i = 0;

		text.setString("");
		while (i < txt.getSize())
		{
			render.DrawDialoge(curSprite, text, sprite);
			render.PollEvent();

			if (tick % 100 == 0)
				text.setString(text.getString() + txt[i++]);

			++tick;
		}

		SleepWhileNotBreak(curSprite);

	}

private:
	sf::Texture dialogeTexture;
	sf::Sprite sprite;

	sf::Font gameFont;
	sf::Text text;

	Render& render;
	KeyBoard& keyboard;

	void SetPositionToSprite()
	{
		auto pr = render.GetCenterPos();
		pr.x -= sprite.getTextureRect().width / 2.f;
		sprite.setPosition(pr.x, pr.y);
	}

	void UpdateKeys()
	{

	}

	void SleepWhileNotBreak(const sf::Sprite& curSprite)
	{
		while (!keyboard.IsNext() && render.IsOpen())
		{
			render.DrawDialoge(curSprite, text, sprite);
			render.PollEvent();
		}
	}

};
