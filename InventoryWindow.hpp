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
			centerPos.x += caret < 5 ? -65 : 20;

			centerPos.y -= 25;
			centerPos.y += caret % 5 * 10;

			return centerPos;
		}
		static sf::Vector2f GetHPPosAtCenter(const sf::Vector2f& centerPos)
		{
			return centerPos + sf::Vector2f{40.f, -67.f};
		}
		static sf::Vector2f GetBorodaPosAtCenter(const sf::Vector2f& centerPos)
		{
			return centerPos + sf::Vector2f{40.f, -60.f};
		}
		static sf::Vector2f GetMoneyPosAtCenter(const sf::Vector2f& centerPos)
		{
			return centerPos + sf::Vector2f{40.f, -52.f};
		}
		static sf::Vector2f GetPosCaretUseAtCenter(sf::Vector2f centerPos, short caret)
		{
			centerPos.x += 30;
			centerPos.y += 55 + caret * 7;
			return centerPos;
		}
		static sf::Vector2f GetArmorPosArCenter(sf::Vector2f centerPos)
		{
			return centerPos + sf::Vector2f{-15, 15};
		}
		static sf::Vector2f GetWeaponPosArCenter(const sf::Vector2f& centerPos)
		{
			return centerPos + sf::Vector2f{-15, -15};
		}
	};
public:
	InventoryWindow(PersonInfo& info) : info{ info }
	{
		texture.loadFromFile(INVENTORY_TEXTURE_PATH);
		sprite.setTexture(texture);
		sprite.setScale(0.5f, 0.5f);

		point.setRadius(1);
		point.setFillColor(sf::Color::Black);

		font.loadFromFile(FONT_PATH);
		for (size_t i = 0; i < 10; ++i)
		{
			texts.push_back({});
			texts[i].setFont(font);
			texts[i].setFillColor(sf::Color::Black);
			texts[i].setScale(0.25, 0.25);
		}

		FillText(hp, font, sf::Color::Black, 0.25, 0.25);
		FillText(boroda, font, sf::Color::Black, 0.25, 0.25);
		FillText(money, font, sf::Color::Black, 0.25, 0.25);
		FillText(armor, font, sf::Color::Black, 0.25, 0.25);
		FillText(weapon, font, sf::Color::Black, 0.25, 0.25);
	}

	bool IsActive() const
	{
		return isActive;
	}
	bool IsWait() const
	{
		return wait;
	}
	const sf::String& GetWaitStr() const
	{
		return waitStr;
	}

	void Run()
	{
		isActive = true;
		wait = false;
		useCaret = 2;
		Update();
	}

	void Stop()
	{
		isActive = false;
	}

	void Update()
	{
		sf::String pr = "";
		for (size_t i = 0; i < info.inventory.GetSize(); ++i)
			texts[i].setString(info.inventory.GetAt(i).GenNane());
		for (size_t i = info.inventory.GetSize(); i < 10; ++i)
			texts[i].setString("");
		hp.setString(std::to_string(info.hp));
		boroda.setString(std::to_string(info.boroda));
		money.setString(std::to_string(info.money));

		armor.setString(info.inventory.GetArmor().GenNane());
		weapon.setString(info.inventory.GetWeapon().GenNane());
	}

	void GetEvent(const KeyBoard& keyboard)
	{
		if (keyboard.IsInventory())
			Stop();
		bool update = false;

		if (!isUse)
		{
			if (keyboard.IsUp())
				--caret, (update = true);
			if (keyboard.IsDown())
				++caret, (update = true);
			if (keyboard.IsLeft())
				caret -= 5, (update = true);
			if (keyboard.IsRight())
				caret += 5, (update = true);

			if (keyboard.IsNext() && caret < info.inventory.GetSize())
				isUse = true;
		}
		else
		{
			if (keyboard.IsDown())
				++useCaret, (update = true);
			if (keyboard.IsUp())
				--useCaret, (update = true);
			if (keyboard.IsNext())
				Use();
			else if (keyboard.IsBack())
				isUse = false;
		}
		if (update)
			UpdateCaret();
	}

	void SetPositionAtWindow(sf::Vector2f posCenter)
	{
		SetTextPos(posCenter);
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
		window.draw(hp);
		window.draw(boroda);
		window.draw(money);
		window.draw(armor);
		window.draw(weapon);
	}

private:

	PersonInfo& info;

	sf::Texture texture;
	sf::Sprite sprite;

	sf::CircleShape point;

	sf::String waitStr;
	bool wait = false;

	#pragma region Texts

	sf::Font font;
	std::vector<sf::Text> texts;

	sf::Text hp;
	sf::Text boroda;
	sf::Text money;

	sf::Text armor;
	sf::Text weapon;

	#pragma endregion

	bool isActive = false;

	short caret = 0;
	bool isUse = false;
	short useCaret = 0;

	void UpdateCaret()
	{
		if (!isUse)
		{
			while (caret < 0)
				caret += info.inventory.MaxElement();
			while (caret >= info.inventory.MaxElement())
				caret -= info.inventory.MaxElement();
			point.setPosition(FloatRectFabrick::GetPosCaretAtCenter(CenterPos(), caret));
		}
		else
		{
			if (useCaret < 0)
				useCaret += 3;
			if (useCaret >= 3)
				useCaret -= 3;
			point.setPosition(FloatRectFabrick::GetPosCaretUseAtCenter(CenterPos(), useCaret));

		}
	}

	sf::Vector2f CenterPos() const
	{
		return sprite.getPosition() + sf::Vector2f{FloatRectFabrick::RectMenu().width / 2.f, FloatRectFabrick::RectMenu().height / 2.f};
	}

	void SetTextPos(const sf::Vector2f center)
	{
		for (size_t i = 0; i < texts.size(); ++i)
			texts[i].setPosition(FloatRectFabrick::GetTextPosAtCenter(center, i));
		hp.setPosition(FloatRectFabrick::GetHPPosAtCenter(center));
		boroda.setPosition(FloatRectFabrick::GetBorodaPosAtCenter(center));
		money.setPosition(FloatRectFabrick::GetMoneyPosAtCenter(center));

		armor.setPosition(FloatRectFabrick::GetArmorPosArCenter(center));
		weapon.setPosition(FloatRectFabrick::GetWeaponPosArCenter(center));
	}

	void Use()
	{
		if (useCaret == 0)
			waitStr = info.inventory.UseAt(caret, info.hp, info.boroda, info.defence, info.armor);
		else if (useCaret == 2)
			waitStr = info.inventory.SeeAt(caret);
		else
			waitStr = info.inventory.TrashAt(caret);
		wait = true;
		isUse = false;
	}

};