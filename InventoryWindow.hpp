#pragma once

#include<SFML\Graphics.hpp>

#include"Render.hpp"
#include"KeyBoard.hpp"
#include"Player.hpp"

#define INVENTORY_TEXTURE_PATH "Sprites\\Inventory.png"

class InventoryWindow
{
private:
	class FloatRectFabrick
	{
	public:
		static sf::FloatRect RectMenu()
		{
			return sf::FloatRect{0, 0, 150, 150};
		}
		static sf::Vector2f GetPosCaretAtCenter(sf::Vector2f centerPos, short caret)
		{
			centerPos.x += caret < 5 ? -70 : 70;

			centerPos.y -= 20;
			centerPos.y += caret % 5 * 10;

			return centerPos;
		}
		static sf::Vector2f GetTextPosAtCenter(sf::Vector2f centerPos, short caret)
		{
			centerPos.x += caret < 5 ? -70 : 20;

			centerPos.y -= 20;
			centerPos.y += caret % 5 * 10;

			return centerPos;
		}
	};
public:
	InventoryWindow(Player& player) : player{ player }
	{
		texture.loadFromFile(INVENTORY_TEXTURE_PATH);
		sprite.setTexture(texture);
		sprite.setScale(0.5f, 0.5f);

		point.setRadius(1);
		point.setFillColor(sf::Color::Yellow);

		font.loadFromFile(FONT_PATH);
		for (size_t i = 0; i < 10; ++i)
		{
			texts.push_back({});
			texts[i].setFont(font);
			texts[i].setFillColor(sf::Color::Black);
			texts[i].setScale(0.25,0.25);
		}
	}

	bool IsActive() const
	{
		return isActive;
	}

	void Run()
	{
		isActive = true;
		sf::String pr = "";
		for (size_t i = 0; i < player.GetInfo().invent.GetSize(); ++i)
		{
			texts[i].setString(player.GetInfo().invent.GetAt(i).GenNane());
		}
	}

	void Stop()
	{
		isActive = false;
	}

	void Update()
	{
	
	
	}

	void GetEvent(const KeyBoard& keyboard)
	{
		if (keyboard.IsInventory())
			Stop();
		bool update = false;

			if (keyboard.IsUp())
				--caret, (update = true);
			if (keyboard.IsDown())
				++caret, (update = true);
			if (keyboard.IsLeft())
				caret -= 5, (update = true);
			if (keyboard.IsRight())
				caret += 5, (update = true);

		if(update)
			UpdateCaret();
	}

	void SetPositionAtWindow(sf::Vector2f posCenter)
	{
		posCenter.x -= FloatRectFabrick::RectMenu().width / 2.f;
		posCenter.y -= FloatRectFabrick::RectMenu().height / 2.f;
		sprite.setPosition(posCenter);
		UpdateCaret();
	}

	void Draw(sf::RenderWindow& window) const
	{
		window.draw(sprite);
		window.draw(point);
		for (const auto& i : texts)
			window.draw(i);
	}

private:

	Player& player;

	sf::Texture texture;
	sf::Sprite sprite;

	sf::CircleShape point;

	sf::Font font;
	std::vector<sf::Text> texts;

	bool isActive = false;

	short caret = 0;
	bool isUse = false;

	void UpdateCaret()
	{
		while (caret < 0)
			caret += player.GetInfo().invent.MaxElement();
		while (caret >= player.GetInfo().invent.MaxElement())
			caret -= player.GetInfo().invent.MaxElement();
		std::cout << "caret = " << caret << '\n';
		point.setPosition(FloatRectFabrick::GetPosCaretAtCenter(CenterPos(), caret));
	}

	sf::Vector2f CenterPos() const
	{
		return sprite.getPosition() + sf::Vector2f{FloatRectFabrick::RectMenu().width / 2.f, FloatRectFabrick::RectMenu().height / 2.f};
	}

	void SetTextPos(const sf::Vector2f center)
	{
		for (size_t i = 0; i < texts.size(); ++i)
			texts[i].setPosition(FloatRectFabrick::GetTextPosAtCenter(center, i));
	}

};