#pragma once

#include<SFML\Graphics.hpp>

#include"Render.hpp"
#include"KeyBoard.hpp"
#include"Player.hpp"

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
	InventoryWindow(PersonInfo& info, const ResourceMeneger& meneger) :
		info{ info }, sprite{meneger.GetInventorySprite()}
	{
		FillText(txt, meneger.GetMainFont(), sf::Color::Black, 0.25f, 0.25f);
		point.setRadius(1);
		point.setFillColor(sf::Color::Black);
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
	}
	void Stop()
	{
		isActive = false;
	}

	void GetEvent(const KeyBoard& keyboard)
	{
		if (keyboard.IsInventoryClick())
			Stop();
		bool update = false;

		if (!isUse)
		{
			if (keyboard.IsUpClick())
				--caret, (update = true);
			if (keyboard.IsDownClick())
				++caret, (update = true);
			if (keyboard.IsLeftClick())
				caret -= 5, (update = true);
			if (keyboard.IsRightClick())
				caret += 5, (update = true);

			if (keyboard.IsNextClick() && caret < info.inventory.GetSize())
				isUse = true;
		}
		else
		{
			if (keyboard.IsDownClick())
				++useCaret, (update = true);
			if (keyboard.IsUpClick())
				--useCaret, (update = true);
			if (keyboard.IsNextClick())
				Use();
			else if (keyboard.IsBackClick())
				isUse = false;
		}
		if (update)
			UpdateCaret();
	}

	void SetPositionAtWindow(const sf::Vector2f& posCenter)
	{
		sprite.setPosition(posCenter - sf::Vector2f{FloatRectFabrick::RectMenu().width / 2.f, FloatRectFabrick::RectMenu().height / 2.f});
		UpdateCaret();
	}

	void Draw(sf::RenderWindow& window) const
	{
		window.draw(sprite);
		window.draw(point);
		for (short i = 0; i < info.inventory.GetSize(); ++i)
			DrawTxt(window, FloatRectFabrick::GetTextPosAtCenter(CenterPos(), i), info.inventory.GetAt(toUInt8(i)).GenNane());

		DrawTxt(window, FloatRectFabrick::GetHPPosAtCenter(CenterPos()), std::to_string(info.hp));
		DrawTxt(window, FloatRectFabrick::GetBorodaPosAtCenter(CenterPos()), std::to_string(info.boroda));
		DrawTxt(window, FloatRectFabrick::GetMoneyPosAtCenter(CenterPos()), std::to_string(info.money));
		DrawTxt(window, FloatRectFabrick::GetArmorPosArCenter(CenterPos()), info.inventory.GetArmor().GenNane());
		DrawTxt(window, FloatRectFabrick::GetWeaponPosArCenter(CenterPos()), info.inventory.GetWeapon().GenNane());
	}

private:
	PersonInfo& info;

	sf::Sprite sprite;

	sf::CircleShape point;

	mutable sf::Text txt;

	sf::String waitStr;

	bool wait = false;
	bool isUse = false;
	bool isActive = false;
	
	short caret = 0;
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

	void Use()
	{
		if (useCaret == 0)
			waitStr = info.inventory.UseAt(toUInt8(caret), info.hp, info.boroda, info.defence, info.armor);
		else if (useCaret == 2)
			waitStr = info.inventory.SeeAt(toUInt8(caret));
		else
			waitStr = info.inventory.TrashAt(toUInt8(caret));
		wait = true;
		isUse = false;
	}

	void DrawTxt(sf::RenderWindow& window, const sf::Vector2f& pos, const sf::String& str) const
	{
		txt.setPosition(pos);
		txt.setString(str);
		window.draw(txt);
	}

};