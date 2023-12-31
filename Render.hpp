#pragma once

#include<SFML\Graphics.hpp>

#include"Map.hpp"
#include"Player.hpp"
#include"WindowDialog/DialogeMutiWindow.hpp"

class Render
{
public:
	Render() :
		window{ sf::VideoMode::getDesktopMode(), L"��������� ����� ������ �� ������" }, view{ window.getDefaultView() }
	{

	}

	#pragma region GetPos

	sf::Vector2f GetCenterPos() const
	{
		return view.getCenter();
	}
	sf::Vector2f GetNullPos() const
	{
		return window.mapPixelToCoords({0,0});
	}
	sf::Vector2u GetSize() const
	{
		return { toUInt(view.getSize().x), toUInt(view.getSize().y) };
	}

	float GetCenterX() const
	{
		return view.getCenter().x;
	}
	float GetCenterY()
	{
		return view.getCenter().y;
	}

	float GetWidth() const
	{
		return view.getSize().x;
	}
	float GetHeight() const
	{
		return view.getSize().y;
	}

	#pragma endregion

	void SaveToTexture(sf::Texture& texture)
	{
		texture.create(window.getSize().x / 0.25f , window.getSize().y / 0.25f);
		texture.update(window);
	}

	void SetWindowSize(unsigned width, unsigned height)
	{
		view.setSize(width, height);
		window.setSize({ width, height });
	}

	void GameMapDraw(const GameMap& map, const Player& player)
	{
		view.setCenter(player.GetCenterPosition());
		map.Draw(window);

		//auto pr = map.GetObjs();
		for (const auto& i : map.GetObjs())
			if (i.GetCenterBarierBoxPosition().y < player.GetAspirCenterFootPos().y)
				i.Draw(window);
		for(const auto& i : map.GetIObjs())
			if (i.GetCenterBarierBoxPosition().y < player.GetAspirCenterFootPos().y)
				i.Draw(window);

		player.Draw(window);

		for (const auto& i : map.GetObjs())
			if (i.GetCenterBarierBoxPosition().y > player.GetAspirCenterFootPos().y)
				i.Draw(window);
		for (const auto& i : map.GetIObjs())
			if (i.GetCenterBarierBoxPosition().y > player.GetAspirCenterFootPos().y)
				i.Draw(window);
	}
	void DrawDialoge(const DialogeWindow& dialog)
	{
		dialog.Draw(window);
	}
	void DrawInventory(const InventoryWindow& invent)
	{
		invent.Draw(window);
	}
	void DrawMultyWindow(const MultiDialogeWindow& dialoge)
	{
		dialoge.Draw(window);
	}

	void DrawSprite(const sf::Sprite& sprite)
	{
		window.draw(sprite);
	}
	void DrawText(const sf::Text& text)
	{
		window.draw(text);
	}

	void BlackWindow()
	{
		sf::Clock clock;

		sf::RectangleShape rect;
		rect.setSize({ toFloat(window.getSize().x),toFloat( window.getSize().y) });
		rect.setFillColor(sf::Color::Transparent);
		rect.setPosition(window.mapPixelToCoords({0,0}));

		while (clock.getElapsedTime().asSeconds() < 1)
		{
			PollEvent();
			rect.setFillColor({0,0,0,toUInt8(clock.getElapsedTime().asSeconds() * 51.f)});
			window.draw(rect);
			window.display();
		}
		
	}

	bool IsFullScreen() const
	{
		return fullScreen;
	}

	#pragma region Standart Window Methods

	void Clear(const sf::Color& color = sf::Color{0,0,0,255})
	{
		window.clear(color);
	}

	void FinalizeRender()
	{
		window.setView(view);
		window.display();
	}

	void CloseWindow()
	{
		window.close();
	}

	void Zoom(float val)
	{
		view.zoom(val);
	}

	bool IsOpen()
	{
		return window.isOpen();
	}

	void PollEvent()
	{
		if (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}

	void SetFullScreen(const bool vs)
	{
		if(vs)
			window.create(sf::VideoMode(window.getSize().x, window.getSize().y), L"��������� ����� ������ �� ������", sf::Style::Fullscreen);
		else
			window.create(sf::VideoMode(window.getSize().x, window.getSize().y), L"��������� ����� ������ �� ������", sf::Style::Default);
		fullScreen = vs;
	}

	void SetDefaultCenterPos()
	{
		view.setCenter(GetSize().x / 2.f, GetSize().y / 2.f);
	}

	#pragma endregion

private:
	sf::RenderWindow window;
	sf::View view;
	sf::Event event {};

	bool fullScreen = false;

};