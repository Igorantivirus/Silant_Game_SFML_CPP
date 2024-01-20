#pragma once

#include<vector>

#include<SFML\Graphics.hpp>

#include"Reader.hpp"
#include"Render.hpp"
#include"KeyBoard.hpp"
#include"Helper.hpp"

#define BACKGROUND_TEXTURE_MAIN_MENU "Textures/MainMenu.png"
#define BACKGROUND_TEXTURE_SETTINGS_MENU "Textures/SettingsMenu.png"
#define BACKGROUND_TEXTURE_INFO_MENU "Textures/InfoMenu.png"

#define FONT_MENU "Settings/constan.ttf"
#define FONT_SETTINGS "Settings/consola.ttf"

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
	MainMenu(Render& render, KeyBoard& keyboard) : render{ render }, keyboard{ keyboard }
	{
		font.loadFromFile(FONT_MENU);

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

		texture.loadFromFile(BACKGROUND_TEXTURE_MAIN_MENU);
		background.setTexture(&texture);
		background.setSize({ render.GetWidth(), render.GetHeight() });

		background.setPosition(render.GetNullPos());

	}

	void Run()
	{
		res = Result::None;
		CentrateALl();
		while (res == Result::None && render.IsOpen())
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

class SettingsMenu
{
public:
	SettingsMenu(Render& render, KeyBoard& keyboard) : render{ render }, keyboard{ keyboard }
	{
		font.loadFromFile(FONT_SETTINGS);

		CentrateAll();

		texture.loadFromFile(BACKGROUND_TEXTURE_INFO_MENU);
		background.setTexture(&texture);
		background.setSize({ render.GetWidth(), render.GetHeight() });
		background.setPosition(render.GetNullPos());

	}

	void Run()
	{
		startKeyBoard = keyboard;
		isOpen = true;
		useButton = false;
		CentrateAll();
		while (isOpen && render.IsOpen())
		{
			Draw();
			GetEvents();
			Update();
		}
	}

private:
	Render& render;
	KeyBoard& keyboard;

	KeyBoard startKeyBoard;

	sf::Texture texture;
	sf::RectangleShape background;

	sf::Font font;
	std::vector<sf::Text> texts;
	int caret = 1;

	bool useButton{};
	bool isOpen{};

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
		if (useButton)
		{
			auto pr = render.PollEventKeysReleased();
			if(pr.size() == 1)
			{
				if (!keyboard.HaveKey(pr[0]))
				{
					keyboard[caret].SetKey(pr[0]);
					CentrateAll();
				}
				useButton = false;
			}
		}
		else
			render.PollEvent();

		if (keyboard.IsUpClick())
			caret--;
		else if (keyboard.IsDownClick())
			caret++;
		if (keyboard.IsNextClick())
			useButton = true;

		if (keyboard.IsBack())
			isOpen = false;

		if (keyboard.IsFullScreenClick())
			render.SetFullScreen(!render.IsFullScreen());
	}

	void Update()
	{
		if (texts.size() == 0)
			return;
		if (caret < 0)
			caret = toInt(texts.size()) - 1;
		else if (caret >= texts.size())
			caret = 0;

		for (auto& i : texts)
			i.setOutlineColor({});

		texts[caret].setOutlineColor(sf::Color::Blue);

		if (useButton)
		{
			if (caret == (texts.size() - 1))
			{
				keyboard.DefaultSettings();
				CentrateAll();
				useButton = false;
			}
			else if (caret == texts.size() - 2)
			{
				keyboard = startKeyBoard;
				isOpen = useButton = false;
			}
			else if (caret == texts.size() - 3)
			{
				SaveAll();
				isOpen = useButton = false;
			}
			else
				texts[caret].setOutlineColor(sf::Color::Red);
		}

	}

	void CentrateAll()
	{
		auto null = render.GetNullPos();
		std::vector<sf::String> names = keyboard.GetNames();
		texts = std::vector<sf::Text>(names.size() + 3, sf::Text{});

		for (size_t i = 0; i < names.size(); ++i)
			FillText(texts[i], font, names[i], sf::Color::Yellow, null + sf::Vector2f{ 20.f, i * 50.f + 50.f }, { 1,1 }, 50);

		null.y += render.GetHeight() - 100;
		null.x += 50;
		FillText(texts[texts.size() - 1], font, L"Стандартные настройки", sf::Color::Yellow, null);
		null.y -= texts[texts.size() - 1].getLocalBounds().height + 25;
		FillText(texts[texts.size() - 2], font, L"Отменить  и выйти", sf::Color::Yellow, null);
		null.y -= texts[texts.size() - 2].getLocalBounds().height + 25;
		FillText(texts[texts.size() - 3], font, L"Сохранить и выйти", sf::Color::Yellow, null);
		null.y -= texts[texts.size() - 3].getLocalBounds().height + 25;
	}

	void SaveAll()
	{

	}
};

class GameInfo
{
public:
	GameInfo(Render& render, KeyBoard& keyboard) : render{ render }, keyboard{ keyboard }
	{
		texture.loadFromFile(BACKGROUND_TEXTURE_SETTINGS_MENU);
		background.setTexture(&texture);
		background.setSize({ render.GetWidth(), render.GetHeight() });
		background.setPosition(render.GetNullPos());

		font.loadFromFile(FONT_MENU);
		text.setFont(font);
		text.setString(ReadWrite::ReadInfoGame());
		text.setPosition(render.GetNullPos());
	}

	void Run()
	{
		isOpen = true;
		while (isOpen && render.IsOpen())
		{
			Draw();
			GetEvents();
		}
	}

private:
	Render& render;
	KeyBoard& keyboard;

	sf::Texture texture;
	sf::RectangleShape background;

	sf::Font font;
	sf::Text text;

	bool isOpen{};

private:

	void Draw()
	{
		render.Clear();

		render.DrawRectangle(background);
		render.DrawText(text);

		render.FinalizeRender();
	}

	void GetEvents()
	{
		render.PollEvent();
		if (keyboard.IsBackClick())
			isOpen = false;
		if (keyboard.IsFullScreenClick())
			render.SetFullScreen(!render.IsFullScreen());
	}

};