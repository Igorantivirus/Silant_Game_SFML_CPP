#pragma once

#include<SFML\Graphics.hpp>

#include"Map.hpp"
#include"Player.hpp"

class Render
{
public:
	Render() :
		window{ sf::VideoMode::getDesktopMode(), L"Бородатый мужик гуляет по театру" }, view{ window.getDefaultView() }
	{

	}

	sf::Vector2f GetCenterPos() const
	{
		return window.mapPixelToCoords({ toInt(window.getSize().x / 2u), toInt(window.getSize().y / 2u) });
	}
	sf::Vector2f GetNullPos() const
	{
		return window.mapPixelToCoords({0,0});
	}
	sf::Vector2u GetSize() const
	{
		return window.getSize();
	}

	void SaveToTexture(sf::Texture& texture)
	{
		texture.create(window.getSize().x / 0.25f , window.getSize().y / 0.25f);
		texture.update(window);
	}

	void Zoom(float val)
	{
		view.zoom(val);
	}
	void SetWindowSize(unsigned width, unsigned height)
	{
		view.setSize(width, height);
		window.setSize({ width, height });
	}

	void GameMapDraw(const GameMap& map, const Player& player)
	{
		window.clear();
		view.setCenter(player.GetCenterPosition());
		map.Draw(window);

		auto pr = map.GetObjs();
		for (const auto& i : pr)
			if (i.GetCenterBarierBoxPosition().y < player.GetAspirCenterFootPos().y)
				i.Draw(window);

		player.Draw(window);

		for (const auto& i : pr)
			if (i.GetCenterBarierBoxPosition().y > player.GetAspirCenterFootPos().y)
				i.Draw(window);

		FinalizeRender();
	}

	void DrawSprite(const sf::Sprite& sprite)
	{
		window.draw(sprite);
	}
	void DrawText(const sf::Text& text)
	{
		window.draw(text);
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

	void CloseWindow()
	{
		window.close();
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

#pragma region experement

	void DrawDialoge() {}



#pragma endregion


	void DrawDialoge(const sf::Sprite& oldWindow, const sf::Text& text, const sf::Sprite& sprite)
	{
		window.clear();

		window.draw(oldWindow);
		window.draw(sprite);
		window.draw(text);

		FinalizeRender();
	}

private:
	sf::RenderWindow window;
	sf::View view;
	sf::Event event {};

	void FinalizeRender()
	{
		window.setView(view);
		window.display();
	}

};