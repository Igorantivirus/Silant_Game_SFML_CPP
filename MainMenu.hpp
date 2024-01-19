#pragma once

#include<vector>

#include<SFML\Graphics.hpp>

#include"Render.hpp"
#include"KeyBoard.hpp"
#include"Helper.hpp"

#define MAIN_MENU_FONT "GameFonts/constan.ttf"
#define BACKGROUND_TEXTURE "Textures/MainMenu.png"

class MainMenu
{
public:
	enum Result : char
	{
		None,
		Game,
		Settings,
		Info,
		Exit
	};
public:
	MainMenu(Render& render, KeyBoard& keyboard) : render{ render }, keyboard{keyboard}
	{
		font.loadFromFile(MAIN_MENU_FONT);
		
		texts = std::vector<sf::Text>(7, sf::Text{});
		
		#pragma region InitTexts

		FillText(texts[0], font, L"Силантъ", sf::Color::Yellow, { 0,0 }, { 1,1 }, 150);
		FillText(texts[1], font, L"Играть", sf::Color::Yellow, { 0,0 }, { 1,1 }, 50);
		FillText(texts[2], font, L"Настройки", sf::Color::Yellow, { 0,0 }, { 1,1 }, 50);
		FillText(texts[3], font, L"Об игре", sf::Color::Yellow, { 0,0 }, { 1,1 }, 50);
		FillText(texts[4], font, L"Выйти", sf::Color::Yellow, { 0,0 }, { 1,1 }, 50);

		FillText(texts[5], font, L"Версия - 1.0.0", sf::Color::Yellow, { 0,0 }, { 1,1 }, 25);
		FillText(texts[6], font, L"Автор - Igorantivirus", sf::Color::Yellow, { 0,0 }, { 1,1 }, 25);

		#pragma endregion

		texture.loadFromFile(BACKGROUND_TEXTURE);
		background.setTexture(&texture);
		background.setSize({render.GetWidth(), render.GetHeight()});

		background.setPosition(render.GetNullPos());

	}

	void Run()
	{
		res = Result::None;
		CentrateALl();
		while (res == Result::None)
		{
			Draw();
			GetEvents();
			Update();
		}
	}

	Result GetResult() const
	{
		return res;
	}


private:
	Render& render;
	KeyBoard& keyboard;

	sf::Texture texture;
	sf::RectangleShape background;

	sf::Font font;
	std::vector<sf::Text> texts;
	int caret = 1;

	Result res = Result::None;

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

		if (keyboard.IsUpClick())
			caret--;
		else if (keyboard.IsDownClick())
			caret++;

		if (keyboard.IsNextClick())
		{
			res = static_cast<Result>(caret);
		}

		if (keyboard.IsFullScreenClick())
			render.SetFullScreen(!render.IsFullScreen());
	}

	void Update()
	{
		if (caret < 1)
			caret = 4;
		else if (caret > 4)
			caret = 1;
	
		for (size_t i = 1; i < 5; ++i)
			texts[i].setOutlineColor({});

		texts[caret].setOutlineColor(sf::Color::Blue);
	}

	void CentrateALl()
	{
		sf::Vector2f pr;
		auto center = pr = render.GetCenterPos();
		
		pr.x -= texts[0].getLocalBounds().width / 2.f;
		pr.y -= render.GetHeight() / 4.f + texts[0].getLocalBounds().height;

		texts[0].setPosition(pr);

		pr = center;
		for (size_t i = 1; i < 5; ++i)
		{
			pr.x = center.x - texts[i].getLocalBounds().width / 2.f;
			texts[i].setPosition(pr);
			pr.y += texts[i].getLocalBounds().height + 20;
		}

		pr = render.GetNullPos();
		pr.y += render.GetHeight() - texts[5].getLocalBounds().height * 2;
		texts[5].setPosition(pr);

		pr = render.GetNullPos();
		pr.x += render.GetWidth() - texts[6].getLocalBounds().width;// *2;
		pr.y += render.GetHeight() - texts[6].getLocalBounds().height * 2;
		texts[6].setPosition(pr);
	}
};