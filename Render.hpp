#pragma once

#include<SFML\Graphics.hpp>

#include"Map.hpp"
#include"Player.hpp"

class Render
{
public:
	Render() :
		window{ sf::VideoMode::getDesktopMode(), L"Силант симулятор" }, view{ window.getDefaultView() }
	{}

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
		player.Draw(window);
		FinalizeRender();
	}

	void DrawSprite(const sf::Sprite& sprite)
	{
		window.draw(sprite);
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

private:
	sf::RenderWindow window;
	sf::View view;
	sf::Event event {};

	sf::Font gameFont;

	void FinalizeRender()
	{
		window.setView(view);
		window.display();
	}

};